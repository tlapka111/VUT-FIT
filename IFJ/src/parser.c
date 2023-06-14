/******** parser.c *********
* Implementace překladače imperativního jazyka IFJ18.
* FIT VUT, Tým 087, varianta I
* Autori:   xvlach18    Martin Vlach
*           xhekrd01    Lukas Hekrdla
*           xtlapa00    Tomas Tlapak
* Shrnutí: hlavní soubor programu, kombinující syntaktickou, sématickou a precedenční analýzu
* Dále první prvky do tabulky symbolů a tvoří instrukce pro generování kodu
*/

#include"scanner.h"
#include<stdio.h>
#include<stdlib.h>
#include"error_codes.h"
#include"parser.h"
#include "symtable.h"
#include "psa_stack.h"
#include <stdbool.h>
#include "codegen.h"

// "Slovní" vyjádření tokeny,  pro případ výpisu na stderr, pokud je očekáván jiný
static const char *convert_num_Token[] = {
    [T_EOF] = "EOF",
    [T_EOL] = "EOL",
    [T_ID] = "ID",
    [T_ADD] = "+",
    [T_ASSIGMENT] = "=",
    [T_EQUAL] = "==",
    [T_ERROR] = "[LEX_ERROR]",
    [T_INT] = "INT value",
    [T_FLOAT] = "FLOAT value",
    [T_STRING] = "STRING value",
    [T_IF] = "IF",
    [T_ELSE] = "ELSE",
    [T_WHILE] = "WHILE",
    [T_THEN] = "THEN",
    [T_DEF] = "DEF",
    [T_DO] = "DO",
    [T_END] = "END",
    [T_NIL] = "NIL",
    [T_NOT] = "NOT",
    [T_MULTIPLY] = "*",
    [T_DIV] = "/",
    [T_LESS] = "<",
    [T_MORE] = ">",
    [T_MINUS] = "-",
    [T_NOT_EQUAL] = "!=",
    [T_LESS_EQUAL] = "<=",
    [T_MORE_EQUAL]= ">=",
    [T_RIGHT_BRACKET] = ")",
    [T_LEFT_BRACKET] = "(",
    [T_COMMA] = ",",
};

/* --------------------------------------- Global variables ------------------------------------------- */
bool restoreFlag = 0; // Proměnná pro informaci jestli načítat nový token či nikoli
TGsymTable Global_Table; // Globální tabulka symbolů (FUNKCE)
bool isFunction = 0; // Proměnná pro informaci jestli je daný identifikátor jménem funkce nebo proměnné
bool useTMP = 0; // Proměnná pro informaci jestli se má použít pomocný token
bool shouldFree = false; // Proměnná pro informaci jestli se použit pomocný token, abych kdyžtak uvolnil místo pro klíč
int accParamsCounter = 0; // Počítadlo aktuálně načtených parametrů u volání funkce
char *accFunName; // Jméno aktuální funkce
TFunction *InsideFunction = NULL; // Ukazatel do struktury aktuální funkce
tListOfInstr InstructionsList; // List instrukcí 
int genFPcounter = 1; // Index parametru u definice funkce
int labelGenCounter = 0; // Pomocná promenná pro integritu jména generovaného návěští
int labelGenCounterWhile = 0;  // Pomocná promenná pro integritu jména generovaného návěští
int retVal = 0; // Pomocná proměnná která mi pomůže určit návratovou hodnotu funkce
// Struktura pomocného tokenu
struct {
    Token_type type;
    union {
        int int_value;
        double float_value;
        char *string_value;
    };
} TMPtoken;

/* -------------------------------------------------- Macros ------------------------------------------- */
/* ---------------------------------------- Umožnují nám psát kratší kód --------------------------------*/

#define RECOVERY_TOKEN \
          restoreFlag = 1;

#define CALL(F) 						\
	if ((result = F())!=OK) { \
		return result;					\
}

#define UPDATE_TOKEN \
          if (restoreFlag == 1) { \
                    restoreFlag = 0; \
          }          \
          else {    \
                 next_Token();          \
                 if (token.type == T_ERROR) {     \
                           return LEX_ERR;        \
                 }  \
          }

#define check_Type(Type){ \
    if (token.type != Type){ \
        fprintf(stderr, "Expecting token of type %s, but parser got it token type %s [line %d]\n",convert_num_Token[Type],convert_num_Token[token.type],position_Y); \
        return SYNTAX_ERR; \
    } \
}

#define checkFUNsymbolsONLY \
     if (strchr(token.string_value,'?') != NULL){  \
        fprintf(stderr,"Only identificator of FUNCTION can end by character '?', expected ID of variable [line %d]\n",position_Y); \
        return SEMATIC_OTHERS_ERR; \
     } \
     if (strchr(token.string_value,'!') != NULL){ \
        fprintf(stderr,"Only identificator of FUNCTION can end by character '!', expected ID of variable [line %d]\n",position_Y); \
        return SEMATIC_OTHERS_ERR; \
    }

#define checkFUNsymbolsONLY_TMPtoken \
     if (strchr(TMPtoken.string_value,'?') != NULL){  \
        fprintf(stderr,"Only identificator of FUNCTION can end by character '?', expected ID of variable [line %d]\n",position_Y); \
        return SEMATIC_OTHERS_ERR; \
     } \
     if (strchr(TMPtoken.string_value,'!') != NULL){ \
        fprintf(stderr,"Only identificator of FUNCTION can end by character '!', expected ID of variable [line %d]\n",position_Y); \
        return SEMATIC_OTHERS_ERR; \
    }

#define varBODY \
if (InsideFunction == NULL){ \
     if(stack_alreadyIN((&Global_Table)->variablesStack,token.string_value) == false){ \
         TVariable *tmp = malloc(sizeof(TVariable)); \
          tmp->variableName = malloc(strlen(token.string_value)+1); \
          strcpy(tmp->variableName,token.string_value); \
          tmp->defined = false; \
          stack_Push((&Global_Table)->variablesStack,(TVariable *)tmp); \
           }\
     }

#define varFUN \
if (InsideFunction != NULL){ \
     if(stack_alreadyIN(InsideFunction->variablesStack,token.string_value) == false){ \
         TVariable *tmp = malloc(sizeof(TVariable)); \
          tmp->variableName = malloc(strlen(token.string_value)+1); \
          strcpy(tmp->variableName,token.string_value); \
          tmp->defined = false; \
          stack_Push(InsideFunction->variablesStack,(TVariable *)tmp); \
           }\
     }

/* ---------------------------------------------------- FUNCTION_CALL_PARAMS_NEXT()-------------------------------------------------------*/
int FUNCTION_CALL_PARAMS_NEXT() 
{
    int result;
    UPDATE_TOKEN
    if (token.type == T_COMMA) 
    {
        UPDATE_TOKEN
        if(!(token.type == T_ID || token.type == T_INT || token.type == T_FLOAT || token.type == T_STRING || token.type == T_NIL)) // Povolené typy argumentu při volání funce
        {
            fprintf(stderr,"There must be next argument after comma [line %d] \n",position_Y);
            return SYNTAX_ERR;
        }
        else 
        {
            accParamsCounter++;
            char *tmp5 = malloc(sizeof(char)*10);
            sprintf(tmp5,"TF@$$%d",accParamsCounter);
            if (InsideFunction == NULL) 
            {
                addInstruction(DEFVAR,(char *)tmp5,NULL,NULL,&InstructionsList);
            }
            else // Generování instrucí 
            {
                addInstructionFUN(DEFVAR,(char *)tmp5,NULL,NULL,&InstructionsList);
            }
            char *data = NULL;
            if (token.type == T_ID || token.type == T_STRING)
            {
                data = malloc(sizeof(char)*strlen(token.string_value));
            }
            else 
            {
                data = malloc(sizeof(char)*30);
            }
            switch(token.type) {
            case T_ID:
                sprintf(data,"LF@%s",token.string_value);
                break;
            case T_INT:
                sprintf(data,"int@%d",token.int_value);
                break;
            case T_FLOAT:
                sprintf(data,"float@%a",token.float_value);
                break;
            case T_STRING:
                sprintf(data,"string@%s",token.string_value);
                break;
            case T_NIL:
                sprintf(data,"nil@nil");
            default :
                break;
            }
            if (InsideFunction == NULL) 
            {
                addInstruction(MOVE,(char *)tmp5,(char *)data,NULL,&InstructionsList);
            }
            if (InsideFunction != NULL)
                 {
            addInstructionFUN(MOVE,(char *)tmp5,(char *)data,NULL,&InstructionsList);
                 }   
            if (token.type == T_ID) 
            {
                if((GT_isFunction(&Global_Table, token.string_value)) == true) 
                {
                    fprintf(stderr,"Function can't be argument of another function without FUNEXP extenstion [line %d]!\n",position_Y);
                    return SEMATIC_OTHERS_ERR;
                }
                if (InsideFunction == NULL)
                {
                    if(isVarDefined((&Global_Table)->variablesStack,token.string_value)!= true) 
                    {
                        fprintf(stderr,"Undefined paramater  '%s' used to function call, in main body [line %d]!\n",token.string_value,position_Y);
                        return SEMATIC_DEFINIG_ERR;
                    }
                }
                if (InsideFunction != NULL) 
                {
                    if(isVarDefined(InsideFunction->variablesStack,token.string_value)!= true) 
                    {
                        fprintf(stderr,"Undefined paramater '%s'  used to function call, in function '%s'[line %d]!\n",token.string_value,InsideFunction->functionName,position_Y);
                        return SEMATIC_DEFINIG_ERR;
                    }
                }
            }
            CALL(FUNCTION_CALL_PARAMS_NEXT)
            return result;
        }
    }
    else 
    {
        RECOVERY_TOKEN
        return OK;
    }
}

/* ------------------------------------------------------------- FUNCTION_CALL_PARAMS()--------------------------------------------------------------*/
int FUNCTION_CALL_PARAMS() 
{
    int result;
    UPDATE_TOKEN
    if(token.type == T_ID || token.type == T_INT || token.type == T_FLOAT || token.type == T_STRING || token.type == T_NIL) 
    {
        accParamsCounter++;
        char *tmp5 = malloc(sizeof(char)*10);
        sprintf(tmp5,"TF@$$%d",accParamsCounter);
        if (InsideFunction == NULL) 
        {
            addInstruction(DEFVAR,(char *)tmp5,NULL,NULL,&InstructionsList);
        }
        else 
        {
            addInstructionFUN(DEFVAR,(char *)tmp5,NULL,NULL,&InstructionsList);
        }
        char *data = NULL;
        if (token.type == T_ID || token.type == T_STRING)
        {
            data = malloc(sizeof(char)*150);
        }
        else 
        {
            data = malloc(sizeof(char)*30);
        }
        switch(token.type) {
        case T_ID:
            sprintf(data,"LF@%s",token.string_value);
            break;
        case T_INT:
            sprintf(data,"int@%d",token.int_value);
            break;
        case T_FLOAT:
            sprintf(data,"float@%a",token.float_value);
            break;
        case T_STRING:	 
            sprintf(data,"string@%s",token.string_value);
            break;
        case T_NIL:
            sprintf(data,"nil@nil");
        default :
            break;
        }
        if (InsideFunction == NULL) 
        {
            addInstruction(MOVE,(char *)tmp5,(char *)data,NULL,&InstructionsList);
        }
        if (InsideFunction != NULL)
        {
            addInstructionFUN(MOVE,(char *)tmp5,(char *)data,NULL,&InstructionsList);
        }   

        if (token.type == T_ID) 
        {
            if((GT_isFunction(&Global_Table, token.string_value)) == true) 
            {
                fprintf(stderr,"Function can't be argument of another function without FUNEXP extenstion [line %d]!\n",position_Y);
                return SEMATIC_OTHERS_ERR;
            }
            if (InsideFunction == NULL) 
            {
                if(isVarDefined((&Global_Table)->variablesStack,token.string_value)!= true) 
                {
                    fprintf(stderr,"Undefined paramater '%s' used to function call, in main body [line %d]!\n",token.string_value,position_Y);
                    return SEMATIC_DEFINIG_ERR;
                }
            }
            if (InsideFunction != NULL) 
            {
                if(isVarDefined(InsideFunction->variablesStack,token.string_value)!= true) 
                {
                    fprintf(stderr,"Undefined paramater '%s' used to function call, in function '%s'[line %d]!\n",token.string_value,InsideFunction->functionName,position_Y);
                    return SEMATIC_DEFINIG_ERR;
                }
            }
        }
        CALL(FUNCTION_CALL_PARAMS_NEXT)
        return result;
    }
    else 
    {
        RECOVERY_TOKEN
        return OK;
    }
}
/* ------------------------------------------------------------------- PARAMS_CALL()---------------------------------------------------------------*/
int PARAMS_CALL()
{
    int result = OK;
    accParamsCounter = 0;
    UPDATE_TOKEN
    int curCount = GT_function_f_parametres_cout(&Global_Table,accFunName);

    if(token.type == T_LEFT_BRACKET) 
    {
        CALL(FUNCTION_CALL_PARAMS)
        UPDATE_TOKEN
        check_Type(T_RIGHT_BRACKET)
    }
    else 
    {
        RECOVERY_TOKEN
        CALL(FUNCTION_CALL_PARAMS)
    }
    if ((accParamsCounter != curCount) && (strcmp(accFunName,"print")!= 0)) 
    {
        fprintf(stderr,"Invalid parametres count in function '%s' expected %d but given %d [line %d]!\n",accFunName,curCount,accParamsCounter,position_Y);
        free(accFunName);
        return SEMATIC_PARAMETRES_ERR;
    }
    if ((accParamsCounter < 1) && (strcmp(accFunName,"print") == 0)) 
    {
        fprintf(stderr,"Invalid parametres count in function '%s' expected more then 1 but given %d [line %d]!\n",accFunName,accParamsCounter,position_Y);
        free(accFunName);
        return SEMATIC_PARAMETRES_ERR;
    }
    if(strcmp(accFunName,"print") == 0)
    {
    	if (InsideFunction == NULL)
    	{
    		addInstruction(DEFVAR,"TF@$count",NULL,NULL,&InstructionsList);
    		char *pc = malloc(sizeof(char)*8);
    		sprintf(pc,"int@%d",accParamsCounter);
    		addInstruction(MOVE,"TF@$count",(char*)pc,NULL,&InstructionsList);

    	}
    	else 
    	{
    		addInstructionFUN(DEFVAR,"TF@$count",NULL,NULL,&InstructionsList);
    		char *pc = malloc(sizeof(char)*8);
    		sprintf(pc,"int@%d",accParamsCounter);
    		addInstructionFUN(MOVE,"TF@$count",(char*)pc,NULL,&InstructionsList);
    	}
    } 

    free(accFunName);
    retVal = 2;
    return result;
}
/* -------------------------------------------------------- VALUE()------------------------------------------------------------------*/
int VALUE() 
{
    int result = OK;
    if (useTMP == false) 
    {
        UPDATE_TOKEN
        switch(token.type) {
        case T_INT:
        case T_FLOAT:
        case T_STRING:
        case T_NIL:
        case T_ID:
        case T_LEFT_BRACKET:
            result = PRECENCE_PARSE(); // Volání precedenční syntaktické anal
            break;
        default:
            result = SYNTAX_ERR;
            break;
        }
    }
    else
    {
        CALL(PRECENCE_PARSE)
    }
    retVal = 1;
    return result;
}

/* ----------------------------------------------------------------- IN_BLOCK()-------------------------------------------------------------*/
int IN_BLOCK() 
{
    int result;
    UPDATE_TOKEN
    switch(token.type) {
    case T_IF:
    case T_WHILE:
    case T_EOL:
    case T_NIL:
    case T_STRING:
    case T_ID:
    case T_FLOAT:
    case T_INT:
    case T_LEFT_BRACKET:
        CALL(STATEMENT)
        CALL(IN_BLOCK)
        return OK;
        break;
    default:
        RECOVERY_TOKEN
        return OK;
    }
    retVal = 0;
    return result;
}

/* -------------------------------------------------------------STATEMENT()-----------------------------------------------------------*/
int STATEMENT() 
{
    int result;
    switch(token.type) {

    case T_IF:
        UPDATE_TOKEN
        CALL(PRECENCE_PARSE) //  vysl v LF@prec1

        if(InsideFunction == NULL)
        {
            char *nameIE = malloc(sizeof(char)*10);
            sprintf(nameIE,"$else%d",labelGenCounter);
            addInstruction(NOT,"LF@$prec1", "LF@$prec1", NULL, &InstructionsList);
            addInstruction(JUMPIFEQ, (char *)nameIE, "LF@$prec1", "bool@true", &InstructionsList);
        }
        else
        {
            char *nameIE = malloc(sizeof(char)*10);
            sprintf(nameIE,"$else%d",labelGenCounter);
            addInstructionFUN(NOT,"LF@$prec1", "LF@$prec1", NULL, &InstructionsList);
            addInstructionFUN(JUMPIFEQ, (char *)nameIE, "LF@$prec1", "bool@true", &InstructionsList);
        }

            UPDATE_TOKEN
            check_Type(T_THEN)
            UPDATE_TOKEN
            check_Type(T_EOL)
            CALL(IN_BLOCK) //Vetev true

        if(InsideFunction == NULL)
        {
            char *nameIE2 = malloc(sizeof(char)*10);
            sprintf(nameIE2,"$endif%d",labelGenCounter);
            addInstruction(JUMP, (char *)nameIE2, NULL, NULL, &InstructionsList);
        }
        else
        {
            char *nameIE2 = malloc(sizeof(char)*10);
            sprintf(nameIE2,"$endif%d",labelGenCounter);
            addInstructionFUN(JUMP, (char *)nameIE2, NULL, NULL, &InstructionsList);
        }



        if(InsideFunction == NULL)
        {
            char *nameIE = malloc(sizeof(char)*10);
            sprintf(nameIE,"$else%d",labelGenCounter);
            addInstruction(LABEL,(char *)nameIE, NULL, NULL, &InstructionsList);
        }
        else
        {
            char *nameIE = malloc(sizeof(char)*10);
            sprintf(nameIE,"$else%d",labelGenCounter);
            addInstructionFUN(LABEL,(char *)nameIE, NULL, NULL, &InstructionsList);
        }
        UPDATE_TOKEN
        check_Type(T_ELSE)
        UPDATE_TOKEN
        check_Type(T_EOL)
        CALL(IN_BLOCK) //Vetev else

        if(InsideFunction == NULL)
        {
            char *nameIE2 = malloc(sizeof(char)*10);
            sprintf(nameIE2,"$endif%d",labelGenCounter);
            addInstruction(LABEL,(char *)nameIE2, NULL, NULL, &InstructionsList);
        }
        else
        {
            char *nameIE2 = malloc(sizeof(char)*10);
            sprintf(nameIE2,"$endif%d",labelGenCounter);
            addInstructionFUN(LABEL,(char *)nameIE2, NULL, NULL, &InstructionsList);
        }
        UPDATE_TOKEN
        check_Type(T_END)
        labelGenCounter++;
        return OK;
        break;

    case T_WHILE:
        UPDATE_TOKEN
        if(InsideFunction == NULL)
        {
            char *nameW = malloc(sizeof(char)*10);
            sprintf(nameW,"$while%d",labelGenCounterWhile);
            addInstruction(LABEL,(char *)nameW, NULL, NULL, &InstructionsList);
        }
        else
        {
            char *nameW = malloc(sizeof(char)*10);
            sprintf(nameW,"$while%d",labelGenCounterWhile);
            addInstructionFUN(LABEL,(char *)nameW, NULL, NULL, &InstructionsList);
        }

        CALL(PRECENCE_PARSE)

        if(InsideFunction == NULL)
        {
            char *nameWF = malloc(sizeof(char)*10);
            sprintf(nameWF,"$whileF%d",labelGenCounterWhile);
            addInstruction(NOT,"LF@$prec1", "LF@$prec1", NULL, &InstructionsList);
            addInstruction(JUMPIFEQ, (char *)nameWF, "LF@$prec1", "bool@true", &InstructionsList);
        }
        else
        {
            char *nameWF = malloc(sizeof(char)*10);
            sprintf(nameWF,"$whileF%d",labelGenCounterWhile);
            addInstructionFUN(NOT,"LF@$prec1", "LF@$prec1", NULL, &InstructionsList);
            addInstructionFUN(JUMPIFEQ, (char *)nameWF, "LF@$prec1", "bool@true", &InstructionsList);
        }

        UPDATE_TOKEN
        check_Type(T_DO)
        UPDATE_TOKEN
        check_Type(T_EOL)
        CALL(IN_BLOCK)

        if(InsideFunction == NULL)
        {
            char *nameW = malloc(sizeof(char)*10);
            sprintf(nameW,"$while%d",labelGenCounterWhile);
            addInstruction(JUMP, (char *)nameW, NULL, NULL, &InstructionsList);
        }
        else
        {
            char *nameW = malloc(sizeof(char)*10);
            sprintf(nameW,"$while%d",labelGenCounterWhile);
            addInstructionFUN(JUMP, (char *)nameW, NULL, NULL, &InstructionsList);
        }

        if(InsideFunction == NULL)
        {
            char *nameWF = malloc(sizeof(char)*10);
            sprintf(nameWF,"$whileF%d",labelGenCounterWhile);
            addInstruction(LABEL,(char *)nameWF, NULL, NULL, &InstructionsList);
        }
        else
        {
            char *nameWF = malloc(sizeof(char)*10);
            sprintf(nameWF,"$whileF%d",labelGenCounterWhile);
            addInstructionFUN(LABEL,(char *)nameWF, NULL, NULL, &InstructionsList);
        }
        labelGenCounterWhile++;
        UPDATE_TOKEN
        check_Type(T_END)
        return OK;
        break;

    case T_EOL:
        return OK;
        break;
    case T_ID:
        isFunction = GT_isFunction(&Global_Table, token.string_value);
        TMPtoken.string_value = malloc(strlen(token.string_value)+1);
        strcpy(TMPtoken.string_value,token.string_value);
        char *varName = NULL;
        if(isFunction == false) 
        {
        	    varName = malloc(sizeof(char)*30);  

		if (InsideFunction == NULL){ 
		     if(stack_alreadyIN((&Global_Table)->variablesStack,token.string_value) == false){ 
		         TVariable *tmp = malloc(sizeof(TVariable)); 
		          tmp->variableName = malloc(strlen(token.string_value)+1); 
		          strcpy(tmp->variableName,token.string_value); 
		          tmp->defined = false; 
		          stack_Push((&Global_Table)->variablesStack,(TVariable *)tmp); 
		          char *name = returnVarName((&Global_Table)->variablesStack,token.string_value); 
          		  sprintf(varName,"LF@%s",name);
          		  addInstruction(DEFVAR,(char *)varName,NULL,NULL,&InstructionsList);
		   		  addInstruction(MOVE,(char *)varName,"nil@nil",NULL,&InstructionsList);
		           }
		     else {
		     	  char *name = returnVarName((&Global_Table)->variablesStack,token.string_value); 
          		  sprintf(varName,"LF@%s",name);
		     }      
		    }

        if (InsideFunction != NULL){ 
             if(stack_alreadyIN(InsideFunction->variablesStack,token.string_value) == false)
             { 
                 TVariable *tmp = malloc(sizeof(TVariable)); 
                  tmp->variableName = malloc(strlen(token.string_value)+1); 
                  strcpy(tmp->variableName,token.string_value); 
                  tmp->defined = false; 
                  stack_Push(InsideFunction->variablesStack,(TVariable *)tmp); 
                  char *name = returnVarName(InsideFunction->variablesStack,token.string_value); 
                  sprintf(varName,"LF@%s",name);
                  addInstructionFUN(DEFVAR,(char *)varName,NULL,NULL,&InstructionsList);
                  addInstructionFUN(MOVE,(char *)varName,"nil@nil",NULL,&InstructionsList);
             }
             else
             {
                  char *name = returnVarName(InsideFunction->variablesStack,token.string_value); 
                  sprintf(varName,"LF@%s",name);
             }      
            }            
            varFUN 
            checkFUNsymbolsONLY
        }

        UPDATE_TOKEN

        // Pokud je to prostě čistě jenom výraz který nikam nepřiřazuju
        if ((token.type != T_ASSIGMENT) && (isFunction == false)) 
        { 
            RECOVERY_TOKEN
            useTMP = true;
            shouldFree = true;
            CALL(VALUE)
            retVal = 1;
            UPDATE_TOKEN
            check_Type(T_EOL)
        }
        // Volání funkce, které nikam nepřiřazuju
        else if ((token.type != T_ASSIGMENT) && (isFunction == true)) 
        { 
            accFunName = malloc(strlen(TMPtoken.string_value)+1);
            strcpy(accFunName,TMPtoken.string_value);
            TFunction *pomocna  =(TFunction *)(BSTreturnVALUE(&((&Global_Table)->functions),accFunName));
            
            if(pomocna->defined == false && (InsideFunction == NULL)) 
            {
                fprintf(stderr,"Undefined function '%s' at line [line %d]\n",accFunName,position_Y);
                return SEMATIC_DEFINIG_ERR;
            }
            RECOVERY_TOKEN
            if(InsideFunction == NULL) 
            {
                addInstruction(CREATEFRAME,NULL,NULL,NULL,&InstructionsList);
            }
            else 
            {
                addInstructionFUN(CREATEFRAME,NULL,NULL,NULL,&InstructionsList);
            }
            
            CALL(PARAMS_CALL)
            UPDATE_TOKEN

            if (InsideFunction == NULL) 
            {
                addInstruction(I_CALL,(char *)pomocna->functionName,NULL,NULL,&InstructionsList);
            }
            else 
            {
                addInstructionFUN(I_CALL,(char *)pomocna->functionName,NULL,NULL,&InstructionsList);
            }
            check_Type(T_EOL)
            retVal = 2;
        }
        else
        { 
            // Mám id =
            if(isFunction == true) 
            { 
                // Pokud bych chtěl přiřazovat do funkce
                fprintf(stderr,"Can't assignment value into function! [line %d]\n",position_Y);
                return SEMATIC_DEFINIG_ERR; // Nevím jaká přesně sématická chyba to má být, zkontrolovat !!
            }
            if (InsideFunction == NULL) 
            {
                variableDefinedTrue((&Global_Table)->variablesStack,TMPtoken.string_value);
            }
            if (InsideFunction != NULL) 
            {
                variableDefinedTrue(InsideFunction->variablesStack,TMPtoken.string_value);
            }

            UPDATE_TOKEN

            if (token.type == T_ID) 
            { 
                // Podívám se do je dále pokud je to ID musím se rozhodnout, jestli budu volat funkci, nebo precedenčku
                isFunction = GT_isFunction(&Global_Table, token.string_value);
            }
            if (isFunction == false) 
            { 
                // Volám precedenčku kterou následně přiřadím do id
                RECOVERY_TOKEN
                CALL(VALUE)
                if (InsideFunction == NULL)
                {
                    addInstruction(MOVE,varName,"LF@$prec1",NULL,&InstructionsList);
                }
                else
                {
                    addInstructionFUN(MOVE,varName,"LF@$prec1",NULL,&InstructionsList);
                }                
                UPDATE_TOKEN
                check_Type(T_EOL)
                retVal = 1;
            }
            else 
            { 
                // Volám funkci kterou přiřadím do id
		      	  if(InsideFunction == NULL) 
		           {
		               addInstruction(CREATEFRAME,NULL,NULL,NULL,&InstructionsList);
		           }
		           else 
		          {
		             addInstructionFUN(CREATEFRAME,NULL,NULL,NULL,&InstructionsList);
		          }

                accFunName = malloc(strlen(token.string_value)+1);
                strcpy(accFunName,token.string_value);
                TFunction *pomocna  = (TFunction *)(BSTreturnVALUE(&((&Global_Table)->functions),accFunName));
                if(pomocna->defined == false && (InsideFunction == NULL)) 
                {
                    fprintf(stderr,"Undefined function '%s' at line [line %d]\n",accFunName,position_Y);
                    return SEMATIC_DEFINIG_ERR;
                }
                CALL(PARAMS_CALL)
                if (InsideFunction == NULL)
                {
                  addInstruction(I_CALL,(char *)pomocna->functionName,NULL,NULL,&InstructionsList);
                  addInstruction(MOVE,(char *)varName,"TF@$result",NULL,&InstructionsList);
                }
                else
                {
                	addInstructionFUN(I_CALL,(char *)pomocna->functionName,NULL,NULL,&InstructionsList);
                }
                UPDATE_TOKEN
                check_Type(T_EOL)
            }
        }
        return OK;
        break;
    case T_INT:
    case T_NIL:
    case T_STRING:
    case T_LEFT_BRACKET:
    case T_FLOAT:
        RECOVERY_TOKEN
        CALL(VALUE)
        UPDATE_TOKEN
        check_Type(T_EOL)
        return OK;
        break;
    default:
        return SYNTAX_ERR;
        break;
    }
}
/* ------------------------------------------------------------------------------------PARAMS_NEXT()-----------------------------------------------------------------------------------*/
int PARAMS_NEXT() 
{
    int result;
    UPDATE_TOKEN
    if (token.type == T_COMMA)
    {
        UPDATE_TOKEN
        check_Type(T_ID)

        varFUN
        char *tmp = malloc(sizeof(char)*30);
        char *tmp2 = malloc(sizeof(char)*10);
        sprintf(tmp,"LF@%s",returnVarName(InsideFunction->variablesStack,token.string_value));
        sprintf(tmp2,"LF@$$%d",genFPcounter);
        addInstructionFUN(DEFVAR,(char *)tmp,NULL,NULL,&InstructionsList);
        addInstructionFUN(MOVE,(char *)tmp,(char *)tmp2,NULL,&InstructionsList);
        genFPcounter++;
        
        if((GT_isFunction(&Global_Table, token.string_value)) == true) 
        {
            fprintf(stderr,"Function can't be argument of another function without FUNEXP extenstion [line %d]!\n",position_Y);
            return SEMATIC_OTHERS_ERR;
        }
        checkFUNsymbolsONLY
        CALL(PARAMS_NEXT)
        return result;
    }
    else 
    {
        RECOVERY_TOKEN
        return OK;
    }
}
/* -------------------------------------------------------------------PARAMS()------------------------------------------------------------*/
int PARAMS() 
{
    int result;
    UPDATE_TOKEN

    if (token.type == T_ID) 
    {
        varFUN
        char *tmp = malloc(sizeof(char)*30);
        char *tmp2 = malloc(sizeof(char)*10);
        sprintf(tmp,"LF@%s",returnVarName(InsideFunction->variablesStack,token.string_value));
        sprintf(tmp2,"LF@$$%d",genFPcounter);
        addInstructionFUN(DEFVAR,(char *)tmp,NULL,NULL,&InstructionsList);
        addInstructionFUN(MOVE,(char *)tmp,(char *)tmp2,NULL,&InstructionsList);
        genFPcounter++;

        if((GT_isFunction(&Global_Table, token.string_value)) == true) 
        {
            fprintf(stderr,"Function can't be argument of another function without FUNEXP extenstion [line %d]!\n",position_Y);
            return SEMATIC_OTHERS_ERR;
        }
        checkFUNsymbolsONLY
        CALL(PARAMS_NEXT)
        return result;
    }
    else 
    {
        RECOVERY_TOKEN
        return OK;
    }
}
/* -------------------------------------------------------FUNCTION()------------------------------------------------------------------*/
int FUNCTION() 
{
    int result;
    UPDATE_TOKEN
    check_Type(T_ID)
    InsideFunction = (TFunction *)(BSTreturnVALUE(&((&Global_Table)->functions),token.string_value));
    InsideFunction->defined = true;
    addInstructionFUN(LABEL,(char*)InsideFunction->functionName,NULL,NULL,&InstructionsList);
    addInstructionFUN(PUSHFRAME,NULL,NULL,NULL,&InstructionsList);
    addInstructionFUN(DEFVAR,"LF@$result",NULL,NULL,&InstructionsList);
    addInstructionFUN(MOVE,"LF@$result","nil@nil",NULL,&InstructionsList);
    addInstructionFUN(DEFVAR,"LF@$prec1",NULL,NULL,&InstructionsList);
    addInstructionFUN(DEFVAR,"LF@$prec2",NULL,NULL,&InstructionsList);
    UPDATE_TOKEN
    check_Type(T_LEFT_BRACKET)
    CALL(PARAMS)
    UPDATE_TOKEN
    check_Type(T_RIGHT_BRACKET)
    UPDATE_TOKEN
    check_Type(T_EOL)
    genFPcounter = 1;
    CALL(IN_BLOCK)
    UPDATE_TOKEN
    check_Type(T_END)

    if (retVal == 1) // Poslední hodnota byla hodnota PA
    {
        if (InsideFunction == NULL)
         {
            addInstruction(MOVE,"LF@$result","LF@$prec1",NULL,&InstructionsList);
         } 
         else
         {
            addInstructionFUN(MOVE,"LF@$result","LF@$prec1",NULL,&InstructionsList);
         }  
    }	// Návratová hodnota funkce
    else if (retVal == 2) // Poslední hodnota byla zavolána funkce 
        {
            if (InsideFunction == NULL)
            {
                addInstruction(MOVE,"LF@$result","TF@$result",NULL,&InstructionsList);
            }
            else
            {
                addInstructionFUN(MOVE,"LF@$result","TF@$result",NULL,&InstructionsList);
            }
        }
    else // nil
    {
        if(InsideFunction == NULL)
        {
             addInstruction(MOVE,"LF@$result","nil@nil",NULL,&InstructionsList);
        }
        else
        {
            addInstructionFUN(MOVE,"LF@$result","nil@nil",NULL,&InstructionsList);  
        }

    }
    addInstructionFUN(POPFRAME,NULL,NULL,NULL,&InstructionsList);
    addInstructionFUN(RETURN,NULL,NULL,NULL,&InstructionsList);
    InsideFunction = NULL;
    return OK;
}
/* ------------------------------------------------------------------------------------FUNCTION()-----------------------------------------------------------------------------------*/
int PROGRAM_BODY() 
{
    int result;
    UPDATE_TOKEN

    switch(token.type) {
    case T_EOF:
        return OK;
        break;
    case  T_DEF:
        CALL(FUNCTION)
        CALL(PROGRAM_BODY)
        return OK;
        break;
    case T_IF:
    case T_WHILE:
    case T_EOL:
    case T_NIL:
    case T_STRING:
    case T_ID:
    case T_LEFT_BRACKET:
    case T_FLOAT:
    case T_INT:
        CALL(STATEMENT)
        CALL(PROGRAM_BODY)
        return OK;
        break;
    default:
        return OK;
        break;
    }
    return SYNTAX_ERR;
}

/* ----------------------------------------------------------------------------   PROGRAM()    ----------------------------------------------------------------------------*/
int PROGRAM() 
{
    int result;
    result = PROGRAM_BODY();
    if(result != OK) 
    {
        return result;
    }
    if(token.type == T_EOF) 
    {
        return OK;
    }
    check_Type(T_ERROR)
    return result;
}
/* ------------------------------------------------------------------------------Vlozeni vestavenych funkci-----------------------------------------------------------------------------------*/
int VLOZ_VESTAVENE() 
{
    if (GT_insertFunction(&Global_Table,"print",0,true) == TREE_ERROR_ALREADY_EXIST)
    {    
        return SEMATIC_OTHERS_ERR;
    }
    if (GT_insertFunction(&Global_Table,"inputs",0,true) == TREE_ERROR_ALREADY_EXIST)
    {    
        return SEMATIC_OTHERS_ERR;
    }
    if (GT_insertFunction(&Global_Table,"inputi",0,true) == TREE_ERROR_ALREADY_EXIST)
    {    
        return SEMATIC_OTHERS_ERR;
    }
    if (GT_insertFunction(&Global_Table,"inputf",0,true) == TREE_ERROR_ALREADY_EXIST)
    {    
        return SEMATIC_OTHERS_ERR;
    }
    if (GT_insertFunction(&Global_Table,"length",1,true) == TREE_ERROR_ALREADY_EXIST)
    {    
        return SEMATIC_OTHERS_ERR;
    }
    if (GT_insertFunction(&Global_Table,"substr",3,true) == TREE_ERROR_ALREADY_EXIST)
    {    
        return SEMATIC_OTHERS_ERR;
    }
    if (GT_insertFunction(&Global_Table,"ord",2,true) == TREE_ERROR_ALREADY_EXIST)
    { 
        return SEMATIC_OTHERS_ERR;
    }
    if (GT_insertFunction(&Global_Table,"chr",1,true) == TREE_ERROR_ALREADY_EXIST) 
    {
        return SEMATIC_DEFINIG_ERR;
    }
    return OK;
}
/* ------------------------------------------------------------------------------PRVNI_PRUCHOD-----------------------------------------------------------------------------------*/
int PRVNI_PRUCHOD() 
{
    int result = OK;
    UPDATE_TOKEN
    
    while (1) 
    {
        if(token.type == T_EOF)
        {    
            break;
        }
        if (token.type != T_DEF) 
        {
            UPDATE_TOKEN
        }
        else 
        {
            UPDATE_TOKEN
            check_Type(T_ID)
            if (GT_insertFunction(&Global_Table,(token.string_value),0,false) == TREE_ERROR_ALREADY_EXIST) 
            {
                fprintf(stderr,"Redefiniton of function! [line %d]\n",position_Y);
                return SEMATIC_DEFINIG_ERR;
            }
            else 
            {
                UPDATE_TOKEN
                check_Type(T_LEFT_BRACKET)
                int count = 0;
                
                while(1) 
                {
                    UPDATE_TOKEN
                    
                    if (token.type == T_ID) 
                    {
                        if (stack_alreadyIN((&Global_Table)->acc_function->variablesStack,token.string_value) == true) 
                        {
                            fprintf(stderr,"Duplicit declaration of parametres in function '%s' [line %d]!\n",(&Global_Table)->acc_function->functionName,position_Y);
                            return SEMATIC_DEFINIG_ERR;
                        }
                        if(GT_isFunction((&Global_Table),token.string_value)== true) 
                        {
                            fprintf(stderr, "Can't use same name for parameter as ID of declared function '%s' [line %d]!\n",token.string_value,position_Y);
                            return SEMATIC_OTHERS_ERR;
                        }

                        TVariable *tmp = malloc(sizeof(TVariable));
                        tmp->variableName = malloc(strlen(token.string_value)+1);
                        strcpy(tmp->variableName,token.string_value);
                        tmp->defined = true;
                        stack_Push((&Global_Table)->acc_function->variablesStack,(TVariable *)tmp);
                        count++;
                    }
                    if (token.type == T_RIGHT_BRACKET || token.type == T_EOF) 
                    {
                        break;
                    }
                    (&Global_Table)->acc_function->functionParams_count = count;
                }
            }
        }
    }
    rewind(input);
    position_X = 0;
    position_Y = 1;
    return result;
}

int DEFINES_CHECK() 
{
    int result = OK;

    char *ErrPrint = VariablesAreYouDefined((&Global_Table)->variablesStack);
    if (ErrPrint != NULL) 
    {
        fprintf(stderr,"Undefined variable '%s' in main body !\n",ErrPrint);
        return SEMATIC_DEFINIG_ERR;
    }
    return result;
}
void startingGen() // Generování počátečních instrukcí
{
    addInstruction(START,NULL,NULL,NULL,&InstructionsList);
    instrListFirst(&InstructionsList); // Nastaveni aktivity
    addInstruction(MAINBODY,NULL,NULL,NULL,&InstructionsList);
    addInstruction(CREATEFRAME,NULL,NULL,NULL,&InstructionsList);
    addInstruction(PUSHFRAME,NULL,NULL,NULL,&InstructionsList);
    addInstructionFUN(BF_LENGTH,NULL,NULL,NULL,&InstructionsList);
    addInstructionFUN(BF_INPUTS,NULL,NULL,NULL,&InstructionsList);
    addInstructionFUN(BF_INPUTI,NULL,NULL,NULL,&InstructionsList);
    addInstructionFUN(BF_INPUTF,NULL,NULL,NULL,&InstructionsList);
    addInstructionFUN(BF_PRINT,NULL,NULL,NULL,&InstructionsList);
    addInstructionFUN(BF_ORD,NULL,NULL,NULL,&InstructionsList);
    addInstructionFUN(BF_SUBSTR,NULL,NULL,NULL,&InstructionsList);
    addInstructionFUN(BF_CHR,NULL,NULL,NULL,&InstructionsList);
    addInstruction(DEFVAR,"LF@$prec1",NULL,NULL,&InstructionsList);
    addInstruction(DEFVAR,"LF@$prec2",NULL,NULL,&InstructionsList);
    addInstructionFUN(N_ADD,NULL,NULL,NULL,&InstructionsList);
    addInstructionFUN(N_SUB,NULL,NULL,NULL,&InstructionsList);
    addInstructionFUN(N_MUL,NULL,NULL,NULL,&InstructionsList);
    addInstructionFUN(N_EQ,NULL,NULL,NULL,&InstructionsList);
    addInstructionFUN(N_DIV,NULL,NULL,NULL,&InstructionsList);
    addInstructionFUN(N_NEQ_EQ,NULL,NULL,NULL,&InstructionsList);
    addInstructionFUN(N_LESS,NULL,NULL,NULL,&InstructionsList);
    addInstructionFUN(N_MORE,NULL,NULL,NULL,&InstructionsList);
    addInstructionFUN(N_LESS_EQ,NULL,NULL,NULL,&InstructionsList);
    addInstructionFUN(N_MORE_EQ,NULL,NULL,NULL,&InstructionsList);
}
/* ----------------------------------------------------------------------------------parse()-----------------------------------------------------------------------------------*/
int parse() 
{
    int result; // návratová hodnota chyby
    GT_init(&Global_Table);	// Inicializace globální tabulky symbolů
    instrListInit(&InstructionsList); // Inicializace listů instrukcí 
    startingGen();
    CALL(VLOZ_VESTAVENE)	// Vložení vestavěných funkcí do GT
    CALL(PRVNI_PRUCHOD)   // První průchod, těžení dat
    UPDATE_TOKEN
    RECOVERY_TOKEN
    CALL(PROGRAM)  // Syntaxe a část sémetiky
    addInstruction(POPFRAME,NULL,NULL,NULL,&InstructionsList);
    CALL(DEFINES_CHECK)

    if (result == OK ) // Pokud je program lexikálně,syntakticky a sématicky správně, bude se generovat kod 
    {
        printCode(&InstructionsList); // Vytisknuti kodu 
    }
    GT_dispose(&Global_Table);	
    instrListFree(&InstructionsList);
    
    if (shouldFree == true) 
    { 
        // Pokud by se to neprovedlo, tak bych chtěl free NULL což by asi byl segfault, nebo by vznikaly memoryLeaky
        free(TMPtoken.string_value);
    }
    return result;
}
/* -----------------------------------------------------------------------    PRECEDENCE ANALYZE   -------------------------------------------------------------------*/
 // Tabulka precedenční analýzy
static int prec_table[15][15] = {
    //  +    -    *    /    (    )   ID    <    >   <=   >=   !=   ==   INM   $
    {'>', '>', '<', '<', '<', '>', '<', '>', '>', '>', '>', '>', '>', '<', '>'},          // +
    {'>', '>', '<', '<', '<', '>', '<', '>', '>', '>', '>', '>', '>', '<', '>'},          // -
    {'>', '>', '>', '>', '<', '>', '<', '>', '>', '>', '>', '>', '>', '<', '>'},          // *
    {'>', '>', '>', '>', '<', '>', '<', '>', '>', '>', '>', '>', '>', '<', '>'},          // /
    {'<', '<', '<', '<', '<', '=', '<', '<', '<', '<', '<', '<', '<', '<', 'e'},          // (
    {'>', '>', '>', '>', 'e', '>', 'e', '>', '>', '>', '>', '>', '>', 'e', '>'},          // )
    {'>', '>', '>', '>', 'e', '>', 'e', '>', '>', '>', '>', '>', '>', 'e', '>'},          // ID
    {'<', '<', '<', '<', '<', '>', '<', 'e', 'e', 'e', 'e', '>', '>', '<', '>'},          // <
    {'<', '<', '<', '<', '<', '>', '<', 'e', 'e', 'e', 'e', '>', '>', '<', '>'},          // >
    {'<', '<', '<', '<', '<', '>', '<', 'e', 'e', 'e', 'e', '>', '>', '<', '>'},          // <=
    {'<', '<', '<', '<', '<', '>', '<', 'e', 'e', 'e', 'e', '>', '>', '<', '>'},          // >=
    {'<', '<', '<', '<', '<', '>', '<', '<', '<', '<', '<', 'e', 'e', '<', '>'},          // !=
    {'<', '<', '<', '<', '<', '>', '<', '<', '<', '<', '<', 'e', 'e', '<', '>'},          // ==
    {'>', '>', '>', '>', 'e', '>', 'e', '>', '>', '>', '>', '>', '>', 'e', '>'},          // INM
    {'<', '<', '<', '<', '<', 'e', '<', '<', '<', '<', '<', '<', '<', '<', 'e'}           // $
};

int token_to_Enum() {
    switch(token.type) {
    case T_ADD: // 0
        return OPERATOR_PLUS;
        break;
    case T_MINUS: // 1
        return OPERATOR_MINUS;
        break;
    case T_MULTIPLY: //2
        return OPERATOR_MULTIPLY;
        break;
    case   T_DIV: //3
        return OPERATOR_DIV;
        break;
    case T_LESS: //4
        return OPERATOR_LESS;
        break;
    case T_MORE: // 5
        return OPERATOR_MORE;
        break;
    case T_LESS_EQUAL: //6
        return OPERATOR_LESS_EQUAL;
        break;
    case T_MORE_EQUAL: // 7
        return OPERATOR_MORE_EQUAL;
        break;
    case T_LEFT_BRACKET: //8
        return OPERATOR_LEFT_BRACKET;
        break;
    case T_RIGHT_BRACKET: //9
        return OPERATOR_RIGHT_BRACKET;
        break;
    case  T_EQUAL: //10
        return OPERATOR_EQUAL;
        break;
    case T_NOT_EQUAL: //11
        return OPERATOR_NOT_EQUAL;
        break;
    case T_ID: //12
        return OPERATOR_ID;
        break;
    case T_INT:
    case T_FLOAT:
    case T_STRING:
    case T_NIL:
        return OPERATOR_IFSN; //13
        break;
    default :
        return OPERATOR_END;
        break;
    }
}

int PRECENCE_PARSE() 
{
    int acctualToken;
    if (useTMP == false) 
    {
        acctualToken = token_to_Enum();
    }
    else 
    {
        acctualToken = OPERATOR_ID;
    }

    TPSA_stack termsStack;
    TPSA_stack tmp;
    TexpStack *valuesStack;
    valuesStack = expStackInit();
    int pomocny_Element;
    int Einxdex = 1;
    int poped = 0;
    int PRAVA = 0;
    int LEVA = 0;
    bool err = false;
    PSA_stack_init(&termsStack);     // DISPOSE ZASOBNIKU!!!
    PSA_stack_init(&tmp);
    PSA_stack_PUSH(&termsStack,OPERATOR_END);

        while(1) 
    {
        if (useTMP == true) 
        {
            if ((GT_isFunction(&Global_Table, TMPtoken.string_value) == true)) 
            {
                fprintf(stderr,"Function call in expresion is supported only in FUNEXP extension [line %d]\n",position_Y-1);
                return SEMATIC_OTHERS_ERR;
            }
            if (InsideFunction == NULL) 
            {
                if(isVarDefined((&Global_Table)->variablesStack,TMPtoken.string_value)!= true) 
                {
                    fprintf(stderr,"Undefined variable '%s' in main body [line %d]!\n",TMPtoken.string_value,position_Y);
                    return SEMATIC_DEFINIG_ERR;
                }
            }
            if (InsideFunction != NULL) 
            {
                if(isVarDefined(InsideFunction->variablesStack,TMPtoken.string_value)!= true) 
                {
                    fprintf(stderr,"Undefined variable '%s' in function '%s'[line %d]!\n",TMPtoken.string_value,InsideFunction->functionName,position_Y);
                    return SEMATIC_DEFINIG_ERR;
                }
            }
            checkFUNsymbolsONLY_TMPtoken
        }
        if (token.type == T_ID && useTMP == false) 
        {
            if ((GT_isFunction(&Global_Table, token.string_value) == true)) 
            {
                fprintf(stderr,"Function call in expresion is supported only in FUNEXP extension [line %d]\n",position_Y);
                return SEMATIC_OTHERS_ERR;
            }
            if (InsideFunction == NULL) 
            {
                if(isVarDefined((&Global_Table)->variablesStack,token.string_value)!= true) 
                {
                    fprintf(stderr,"Undefined variable '%s' in main body [line %d]!\n",token.string_value,position_Y);
                    return SEMATIC_DEFINIG_ERR;
                }
            }
            if (InsideFunction != NULL) 
            {
                if(isVarDefined(InsideFunction->variablesStack,token.string_value)!= true) 
                {
                    fprintf(stderr,"Undefined variable '%s' in function '%s'[line %d]!\n",token.string_value,InsideFunction->functionName,position_Y);
                    return SEMATIC_DEFINIG_ERR;
                }
            }
            checkFUNsymbolsONLY
        }

// -----------------------------------------------------------------------------------------------//
        switch(prec_table[PSA_stack_TOP(&termsStack)][acctualToken]) {
// -----------------------------------------------------------------------------------------------//            
        case 61:
            if (acctualToken == OPERATOR_LEFT_BRACKET || acctualToken == OPERATOR_RIGHT_BRACKET) 
            {
                if (acctualToken == OPERATOR_LEFT_BRACKET)
                {    
                    LEVA++;
                }
                else
                {
                    PRAVA++;
                }
            }

            PSA_stack_PUSH(&termsStack, acctualToken);
            if (token.type == T_INT || token.type == T_FLOAT || token.type == T_STRING || token.type == T_NIL || token.type == T_ID)
                {
                   TexpElem *tmpXX2 = malloc(sizeof(TexpElem)); 
                   switch(token.type)
                   {
                    case T_ID:
                        tmpXX2->string_value = malloc(strlen(token.string_value)+1);
                        strcpy(tmpXX2->string_value,token.string_value);
                        tmpXX2->type = E_ID;
                        expStackPUSH(valuesStack,tmpXX2);
                            break;
                    case T_INT:
                        tmpXX2->int_value = token.int_value;  
                        tmpXX2->type = E_INT;
                        expStackPUSH(valuesStack,tmpXX2);  
                            break;

                    case T_FLOAT:
                        tmpXX2->float_value = token.float_value;  
                        tmpXX2->type = E_FLOAT;
                        expStackPUSH(valuesStack,tmpXX2);  
                            break;

                    case T_STRING:
                         tmpXX2->string_value = malloc(strlen(token.string_value)+1);
                        strcpy(tmpXX2->string_value,token.string_value);
                        tmpXX2->type = E_STRING;
                        expStackPUSH(valuesStack,tmpXX2);                   
                            break;
                    case T_NIL:
                            tmpXX2->type = E_NIL;
                         expStackPUSH(valuesStack,tmpXX2);         
                    default:
                        break;    
                   }
            }                  

            UPDATE_TOKEN
            acctualToken = token_to_Enum();
            break;
// -----------------------------------------------------------------------------------------------//
        case 60:
            while(PSA_normal_TOP(&termsStack) > 14) 
            {
                pomocny_Element = PSA_normal_TOP(&termsStack);
                PSA_stack_POP(&termsStack);
                poped++;
                PSA_stack_PUSH(&tmp,pomocny_Element);
            }
            PSA_stack_PUSH(&termsStack,HANDLE_START);
            
            while (poped != 0) 
            {
                pomocny_Element = PSA_normal_TOP(&tmp);
                PSA_stack_POP(&tmp);
                poped--;
                PSA_stack_PUSH(&termsStack,pomocny_Element);
            }
            if (acctualToken == OPERATOR_LEFT_BRACKET || acctualToken == OPERATOR_RIGHT_BRACKET) 
            {
                if (acctualToken == OPERATOR_LEFT_BRACKET)
                {    
                    LEVA++;
                }
                else
                {    
                    PRAVA++;
                }
            }
            
            PSA_stack_PUSH(&termsStack,acctualToken);
            
            if (useTMP == false) 
            {
                if (token.type == T_INT || token.type == T_FLOAT || token.type == T_STRING || token.type == T_NIL || token.type == T_ID)
                {
                   TexpElem *tmpXX3 = malloc(sizeof(TexpElem));
                   switch(token.type)
                   {
                    case T_ID:
                        tmpXX3->string_value = malloc(strlen(token.string_value)+1);
                        strcpy(tmpXX3->string_value,token.string_value);
                        tmpXX3->type = E_ID;
                        expStackPUSH(valuesStack,tmpXX3);
                            break;
                    case T_INT:
                        tmpXX3->int_value = token.int_value;  
                        tmpXX3->type = E_INT;
                        expStackPUSH(valuesStack,tmpXX3);  
                            break;

                    case T_FLOAT:
                        tmpXX3->float_value = token.float_value;  
                        tmpXX3->type = E_FLOAT;
                        expStackPUSH(valuesStack,tmpXX3);  
                            break;

                    case T_STRING:
                         tmpXX3->string_value = malloc(strlen(token.string_value)+1);
                        strcpy(tmpXX3->string_value,token.string_value);
                        tmpXX3->type = E_STRING;
                        expStackPUSH(valuesStack,tmpXX3);                   
                            break;
                     case T_NIL:
                            tmpXX3->type = E_NIL;
                         expStackPUSH(valuesStack,tmpXX3);   
                         break;   
                    default:
                        break;    
                   }
                }      

                UPDATE_TOKEN
            }
            else 
            {
                UPDATE_TOKEN
                if (token.type == T_INT || token.type == T_FLOAT || token.type == T_STRING || token.type == T_NIL || token.type == T_ID)
                {
                   TexpElem *tmpXX1 = malloc(sizeof(TexpElem)); 
                   switch(TMPtoken.type)
                   {
                    case T_ID:
                        tmpXX1->string_value = malloc(strlen(TMPtoken.string_value)+1);
                        strcpy(tmpXX1->string_value,TMPtoken.string_value);
                        tmpXX1->type = E_ID;
                        expStackPUSH(valuesStack,tmpXX1);
                            break;
                    case T_INT:
                        tmpXX1->int_value = TMPtoken.int_value;  
                        tmpXX1->type = E_INT;
                        expStackPUSH(valuesStack,tmpXX1);  
                            break;

                    case T_FLOAT:
                        tmpXX1->float_value = TMPtoken.float_value;  
                        tmpXX1->type = E_FLOAT;
                        expStackPUSH(valuesStack,tmpXX1);  
                            break;

                    case T_STRING:
                         tmpXX1->string_value = malloc(strlen(TMPtoken.string_value)+1);
                        strcpy(tmpXX1->string_value,TMPtoken.string_value);
                        tmpXX1->type = E_STRING;
                        expStackPUSH(valuesStack,tmpXX1);                   
                            break;
                 case T_NIL:
                            tmpXX1->type = E_NIL;
                         expStackPUSH(valuesStack,tmpXX1);   
                    default:
                        break;    
                   }
                }      

                useTMP = false;
            }
            acctualToken = token_to_Enum();
            break;

// -----------------------------------------------------------------------------------------------//
        case 62:
            if(PSA_stack_TOP(&termsStack) == OPERATOR_IFSN || PSA_stack_TOP(&termsStack) == OPERATOR_ID) 
            {
                PSA_stack_POP(&termsStack);
                PSA_stack_POP(&termsStack);
                PSA_stack_PUSH(&termsStack, NONTERM);
                if (expStackTOP(valuesStack)!= NULL)
                    {
                    TexpElem *new = expStackTOP(valuesStack);
                    switch (new->type)
                    {
                        case E_INT:
                            if(InsideFunction == NULL)
                            {
                                char *echo = malloc(sizeof(char)*20);
                                sprintf(echo,"int@%d",new->int_value);
                                char *echo2 = malloc(sizeof(char)*10);
                                sprintf(echo2,"LF@$prec%d",Einxdex);
                                addInstruction(MOVE,echo2,echo,NULL,&InstructionsList);
                            }
                            else
                            {
                                char *echo = malloc(sizeof(char)*20);
                                sprintf(echo,"int@%d",new->int_value);
                                char *echo2 = malloc(sizeof(char)*10);
                                sprintf(echo2,"LF@$prec%d",Einxdex);
                                addInstructionFUN(MOVE,echo2,echo,NULL,&InstructionsList);
                            }
                        break;

                        case E_FLOAT:
                            if(InsideFunction == NULL)
                              {
                                char *echo = malloc(sizeof(char)*30);
                                sprintf(echo,"float@%a",new->float_value);
                                char *echo2 = malloc(sizeof(char)*10);
                                sprintf(echo2,"LF@$prec%d",Einxdex);
                                addInstruction(MOVE,echo2,echo,NULL,&InstructionsList);
                              }
                             else
                             {
                                char *echo = malloc(sizeof(char)*30);
                                sprintf(echo,"float@%a",new->float_value);
                                char *echo2 = malloc(sizeof(char)*10);
                                sprintf(echo2,"LF@$prec%d",Einxdex);
                                addInstructionFUN(MOVE,echo2,echo,NULL,&InstructionsList);                         
                             }   
                        break;
                        case E_STRING:
                            if(InsideFunction == NULL)
                              {
                                char *echo = malloc(sizeof(char)*80);
                                sprintf(echo,"string@%s",new->string_value);
                                char *echo2 = malloc(sizeof(char)*10);
                                sprintf(echo2,"LF@$prec%d",Einxdex);
                                addInstruction(MOVE,echo2,echo,NULL,&InstructionsList);
                              }
                             else
                             {
                                char *echo = malloc(sizeof(char)*80);
                                sprintf(echo,"string@%s",new->string_value);
                                char *echo2 = malloc(sizeof(char)*10);
                                sprintf(echo2,"LF@$prec%d",Einxdex);
                                addInstructionFUN(MOVE,echo2,echo,NULL,&InstructionsList);                              
                             }                         
                        break;
                        case E_ID:
                            if(InsideFunction == NULL)
                              {
                                char *echo = malloc(sizeof(char)*40);
                                sprintf(echo,"LF@%s",new->string_value);
                                char *echo2 = malloc(sizeof(char)*10);
                                sprintf(echo2,"LF@$prec%d",Einxdex);
                                addInstruction(MOVE,echo2,echo,NULL,&InstructionsList);
                              }
                             else
                             {
                                char *echo = malloc(sizeof(char)*40);
                                sprintf(echo,"LF@%s",new->string_value);
                                char *echo2 = malloc(sizeof(char)*10);
                                sprintf(echo2,"LF@$prec%d",Einxdex);
                                addInstructionFUN(MOVE,echo2,echo,NULL,&InstructionsList);                              
                             }                          
                        break;
                        case E_NIL:
                            if(InsideFunction == NULL)
                              {
                                char *echo2 = malloc(sizeof(char)*10);
                                sprintf(echo2,"LF@$prec%d",Einxdex);
                                addInstruction(MOVE,echo2,"nil@nil",NULL,&InstructionsList);
                              }
                             else
                             {
                                char *echo2 = malloc(sizeof(char)*10);
                                sprintf(echo2,"LF@$prec%d",Einxdex);
                                addInstructionFUN(MOVE,echo2,"nil@nil",NULL,&InstructionsList);                               
                             }                           
                        break;
                        default:
                            break;
                }
             }
           if(Einxdex != 2)
           {
              Einxdex = 2;   
           }
        } 

            else
            {
                if(PSA_stack_TOP(&termsStack) == OPERATOR_RIGHT_BRACKET) 
                {
                    PSA_stack_POP(&termsStack);
                    if(PSA_normal_TOP(&termsStack) == NONTERM) 
                    {
                        PSA_stack_POP(&termsStack);
                        if(PSA_stack_TOP(&termsStack) == OPERATOR_LEFT_BRACKET) 
                        {
                            PSA_stack_POP(&termsStack);
                            PSA_stack_POP(&termsStack);
                            PSA_stack_PUSH(&termsStack, NONTERM);
                        } 
                        else 
                        {
                            err = true;
                        }
                    } 
                    else 
                    {
                        err = true;
                    }
                }
                else 
                {
                    if(PSA_normal_TOP(&termsStack) == NONTERM) 
                    {

                        PSA_stack_POP(&termsStack);
                        if((PSA_stack_TOP(&termsStack) >= 0 && PSA_stack_TOP(&termsStack) <= 3) || (PSA_stack_TOP(&termsStack) >=7 && PSA_stack_TOP(&termsStack) <= 12)) 
                        {
                            switch(PSA_stack_TOP(&termsStack))
                            {
                                case 0:
                                    if (InsideFunction == NULL)
                                    {
                                        addInstruction(ADD,NULL,NULL,NULL,&InstructionsList);
                                    }
                                    else
                                    {
                                        addInstructionFUN(ADD,NULL,NULL,NULL,&InstructionsList);
                                    }  
                                         expStackPOP(valuesStack);        
                                         expStackPOP(valuesStack);   
                                    break;                    
                                case 1:
                                    if (InsideFunction == NULL)
                                    {
                                        addInstruction(SUB,NULL,NULL,NULL,&InstructionsList);
                                    }
                                    else
                                    {
                                        addInstructionFUN(SUB,NULL,NULL,NULL,&InstructionsList);
                                    }
                                         expStackPOP(valuesStack);        
                                         expStackPOP(valuesStack);                                    
                                    break;

                                case 2:
                                    if (InsideFunction == NULL)
                                    {
                                        addInstruction(MUL,NULL,NULL,NULL,&InstructionsList);
                                    }
                                    else
                                    {
                                        addInstructionFUN(MUL,NULL,NULL,NULL,&InstructionsList);
                                    }                                
                                         expStackPOP(valuesStack);        
                                         expStackPOP(valuesStack);
                                    break;    
                                case 3:
                                    if (InsideFunction == NULL)
                                    {
                                        addInstruction(DIV,NULL,NULL,NULL,&InstructionsList);
                                    }
                                    else
                                    {
                                        addInstructionFUN(DIV,NULL,NULL,NULL,&InstructionsList);
                                    }                                
                                         expStackPOP(valuesStack);        
                                         expStackPOP(valuesStack);
                                    break;                                       
                                 case 12:
                                        if (InsideFunction == NULL)
                                        {
                                            addInstruction(EQ,NULL,NULL,NULL,&InstructionsList);
                                        }
                                        else
                                        {
                                            addInstructionFUN(EQ,NULL,NULL,NULL,&InstructionsList);
                                        }
                                         expStackPOP(valuesStack);        
                                         expStackPOP(valuesStack);
                                        break;
                                  case 11:
                                        if (InsideFunction == NULL)
                                        {
                                            addInstruction(NEQ_EQ,NULL,NULL,NULL,&InstructionsList);
                                        }
                                        else
                                        {
                                            addInstructionFUN(NEQ_EQ,NULL,NULL,NULL,&InstructionsList);
                                        }
                                         expStackPOP(valuesStack);        
                                         expStackPOP(valuesStack);
                                        break;   
                                 case 7:
                                        if (InsideFunction == NULL)
                                        {
                                            addInstruction(LESS,NULL,NULL,NULL,&InstructionsList);
                                        }
                                        else
                                        {
                                            addInstructionFUN(LESS,NULL,NULL,NULL,&InstructionsList);
                                        }
                                         expStackPOP(valuesStack);        
                                         expStackPOP(valuesStack);
                                        break;
                                 case 8:
                                        if (InsideFunction == NULL)
                                        {
                                            addInstruction(MORE,NULL,NULL,NULL,&InstructionsList);
                                        }
                                        else
                                        {
                                            addInstructionFUN(MORE,NULL,NULL,NULL,&InstructionsList);
                                        }
                                         expStackPOP(valuesStack);        
                                         expStackPOP(valuesStack);
                                        break;
                                 case 10:
                                        if (InsideFunction == NULL)
                                        {
                                            addInstruction(MORE_EQ,NULL,NULL,NULL,&InstructionsList);
                                        }
                                        else
                                        {
                                            addInstructionFUN(MORE_EQ,NULL,NULL,NULL,&InstructionsList);
                                        }
                                         expStackPOP(valuesStack);        
                                         expStackPOP(valuesStack);
                                        break;
                                 case 9:
                                        if (InsideFunction == NULL)
                                        {
                                            addInstruction(LESS_EQ,NULL,NULL,NULL,&InstructionsList);
                                        }
                                        else
                                        {
                                            addInstructionFUN(LESS_EQ,NULL,NULL,NULL,&InstructionsList);
                                        }
                                         expStackPOP(valuesStack);        
                                         expStackPOP(valuesStack);
                                        break;                                                                                                                                                
                            }
                            PSA_stack_POP(&termsStack);
                            if(PSA_normal_TOP(&termsStack) == NONTERM) 
                            {
                                PSA_stack_POP(&termsStack);
                                PSA_stack_POP(&termsStack);
                                PSA_stack_PUSH(&termsStack, NONTERM);
                            } 
                            else 
                            {
                                err = true;
                            }
                        } 
                        else 
                        {
                            err = true;
                        }
                    } 
                    else 
                    {
                        err = true;
                    }
                }
            }
            break;
// -----------------------------------------------------------------------------------------------//
        case 101:
            err = true;
            break;
        }
        if(err == true) 
        {
            PSA_stack_dispose(&tmp);
            PSA_stack_dispose(&termsStack);
            return SYNTAX_ERR;
        }
// -----------------------------------------------------------------------------------------------//

        if (acctualToken == OPERATOR_END && PSA_normal_TOP(&termsStack) == NONTERM && PSA_stack_TOP(&termsStack) == OPERATOR_END) 
        {
            PSA_stack_dispose(&tmp);
            PSA_stack_dispose(&termsStack);
            break;
        }
    }
// -----------------------------------------------------------------------------------------------//    
    if (PRAVA != LEVA) 
    {
        fprintf(stderr,"Bad brackets count --- LEFT %d ,RIGHT- %d [line %d]\n",LEVA,PRAVA,position_Y);
        return SYNTAX_ERR;
    }
    RECOVERY_TOKEN
    return OK;
}
