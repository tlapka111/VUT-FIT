/******** codegen.c *********
* Implementace překladače imperativního jazyka IFJ18.
* FIT VUT, Tým 087, varianta I
* Autori:   xvlach18    Martin Vlach
*           xhekrd01    Lukas Hekrdla
*           xtlapa00    Tomas Tlapak
* Shrnutí: Soubor obsahující switchcase pro výpis generovaného kódu
*/

#include <stdio.h>
#include "instr.h"

void printCode(tListOfInstr *codeList)
{
    tListItem *actualInstruction = codeList->first;
    while (actualInstruction != NULL)
    {
        switch (actualInstruction->Instruction.iType)
        {
        case START:
            printf(".IFJcode18\n");
            printf("JUMP $mainbody\n");
            break;
        case LABEL:
            printf("LABEL %s\n", (char *)actualInstruction->Instruction.destInstr);
            break;
        case JUMP:
            printf("JUMP %s\n",(char *)actualInstruction->Instruction.destInstr);
              break;  
        case JUMPIFEQ:
            printf("JUMPIFEQ %s %s %s\n", (char *)actualInstruction->Instruction.destInstr, (char *)actualInstruction->Instruction.src1Addr, (char *)actualInstruction->Instruction.src2Addr);
              break; 
        case NOT:
            printf("NOT %s %s\n", (char *)actualInstruction->Instruction.destInstr, (char *)actualInstruction->Instruction.src1Addr);
              break; 
        case CREATEFRAME:
            printf("CREATEFRAME\n");
            break;
        case DEFVAR:
            printf("DEFVAR %s\n", (char *)actualInstruction->Instruction.destInstr);
            break;
        case MOVE:
            printf("MOVE %s %s\n",(char *)actualInstruction->Instruction.destInstr,(char *)actualInstruction->Instruction.src1Addr);
            break;
        case MAINBODY:
            printf("LABEL $mainbody\n");
            break;
        case RETURN:
            printf("RETURN\n");
            printf("\n");
            break;       
        case PUSHFRAME:
            printf("PUSHFRAME\n");
            break;
        case POPFRAME:
            printf("POPFRAME\n");
            break;            
        case I_CALL:
            printf("CALL %s\n",(char*)actualInstruction->Instruction.destInstr);        
            break;
        case BF_LENGTH: 	// Vestavěná funkce length()
             printf("\n");
             printf("LABEL length\n");
             printf("PUSHFRAME\n");   
             printf("DEFVAR LF@$result\n");
             printf("STRLEN LF@$result LF@$$1\n");
             printf("CREATEFRAME\n");
             printf("DEFVAR TF@$result\n");
             printf("MOVE TF@$result LF@$result\n");
             printf("POPFRAME\n");
             printf("RETURN\n");
             printf("\n");
             break;
        case BF_INPUTS:		// Vestavěná funkce inputs()
            printf("LABEL inputs\n");
            printf("PUSHFRAME\n");
            printf("DEFVAR LF@$result\n");
            printf("READ LF@$result string\n");
            printf("CREATEFRAME\n");
            printf("DEFVAR TF@$result\n");
            printf("MOVE TF@$result LF@$result\n");
            printf("POPFRAME\n");
            printf("RETURN\n");  
            printf("\n");
         break;
        case BF_INPUTI:		// Vestavěná funkce nputi()
            printf("LABEL inputi\n");
            printf("PUSHFRAME\n");
            printf("DEFVAR LF@$result\n");
            printf("READ LF@$result int\n");
            printf("CREATEFRAME\n");
            printf("DEFVAR TF@$result\n");
            printf("MOVE TF@$result LF@$result\n");
            printf("POPFRAME\n");
            printf("RETURN\n");  
            printf("\n");
         break;
        case BF_INPUTF:		// Vestavěná funkce inputf()
            printf("LABEL inputf\n");
            printf("PUSHFRAME\n");
            printf("DEFVAR LF@$result\n");
            printf("READ LF@$result float\n");
            printf("CREATEFRAME\n");
            printf("DEFVAR TF@$result\n");
            printf("MOVE TF@$result LF@$result\n");
            printf("POPFRAME\n");
            printf("RETURN\n");  
            printf("\n");
         break;  

        case BF_PRINT:		// Vestavěná funkce print()
            printf("LABEL print\n");
            printf("PUSHFRAME\n");
            printf("DEFVAR LF@$result\n");
            printf("MOVE LF@$result nil@nil\n");
            printf("DEFVAR LF@$vytiknuto\n");
            printf("DEFVAR LF@$tmp\n");
            printf("MOVE LF@$tmp bool@false\n");
            printf("MOVE LF@$vytiknuto int@0\n");        
            printf("WRITE LF@$$1\n");
            printf("ADD LF@$vytiknuto LF@$vytiknuto int@1\n");
            printf("EQ LF@$tmp LF@$count LF@$vytiknuto\n");
            printf("JUMPIFEQ $print_end LF@$tmp bool@true\n");
            printf("WRITE LF@$$2\n");
            printf("ADD LF@$vytiknuto LF@$vytiknuto int@1\n");
            printf("EQ LF@$tmp LF@$count LF@$vytiknuto\n");
            printf("JUMPIFEQ $print_end LF@$tmp bool@true\n");
            printf("WRITE LF@$$3\n");
            printf("ADD LF@$vytiknuto LF@$vytiknuto int@1\n");
            printf("EQ LF@$tmp LF@$count LF@$vytiknuto\n");
            printf("JUMPIFEQ $print_end LF@$tmp bool@true\n");
            printf("WRITE LF@$$4\n");
            printf("ADD LF@$vytiknuto LF@$vytiknuto int@1\n");
            printf("EQ LF@$tmp LF@$count LF@$vytiknuto\n");
            printf("JUMPIFEQ $print_end LF@$tmp bool@true\n");
            printf("WRITE LF@$$5\n");
            printf("ADD LF@$vytiknuto LF@$vytiknuto int@1\n");
            printf("EQ LF@$tmp LF@$count LF@$vytiknuto\n");
            printf("JUMPIFEQ $print_end LF@$tmp bool@true\n");
            printf("WRITE LF@$$6\n");
            printf("ADD LF@$vytiknuto LF@$vytiknuto int@1\n");
            printf("EQ LF@$tmp LF@$count LF@$vytiknuto\n");
            printf("JUMPIFEQ $print_end LF@$tmp bool@true\n");
            printf("WRITE LF@$$7\n");
            printf("ADD LF@$vytiknuto LF@$vytiknuto int@1\n");
            printf("EQ LF@$tmp LF@$count LF@$vytiknuto\n");
            printf("JUMPIFEQ $print_end LF@$tmp bool@true\n");
            printf("WRITE LF@$$8\n");
            printf("ADD LF@$vytiknuto LF@$vytiknuto int@1\n");
            printf("EQ LF@$tmp LF@$count LF@$vytiknuto\n");
            printf("JUMPIFEQ $print_end LF@$tmp bool@true\n");
            printf("WRITE LF@$$9\n");
            printf("ADD LF@$vytiknuto LF@$vytiknuto int@1\n");
            printf("EQ LF@$tmp LF@$count LF@$vytiknuto\n");
            printf("JUMPIFEQ $print_end LF@$tmp bool@true\n");
            printf("WRITE LF@$$10\n");
            printf("ADD LF@$vytiknuto LF@$vytiknuto int@1\n");
            printf("EQ LF@$tmp LF@$count LF@$vytiknuto\n");
            printf("JUMPIFEQ $print_end LF@$tmp bool@true\n");
            printf("WRITE LF@$$11\n");
            printf("ADD LF@$vytiknuto LF@$vytiknuto int@1\n");
            printf("EQ LF@$tmp LF@$count LF@$vytiknuto\n");
            printf("JUMPIFEQ $print_end LF@$tmp bool@true\n");
            printf("WRITE LF@$$12\n");
            printf("ADD LF@$vytiknuto LF@$vytiknuto int@1\n");
            printf("EQ LF@$tmp LF@$count LF@$vytiknuto\n");
            printf("JUMPIFEQ $print_end LF@$tmp bool@true\n");
            printf("WRITE LF@$$13\n");
            printf("ADD LF@$vytiknuto LF@$vytiknuto int@1\n");
            printf("EQ LF@$tmp LF@$count LF@$vytiknuto\n");
            printf("JUMPIFEQ $print_end LF@$tmp bool@true\n");
            printf("WRITE LF@$$14\n");
            printf("ADD LF@$vytiknuto LF@$vytiknuto int@1\n");
            printf("EQ LF@$tmp LF@$count LF@$vytiknuto\n");
            printf("JUMPIFEQ $print_end LF@$tmp bool@true\n");
            printf("WRITE LF@$$15\n");
            printf("ADD LF@$vytiknuto LF@$vytiknuto int@1\n");
            printf("EQ LF@$tmp LF@$count LF@$vytiknuto\n");
            printf("JUMPIFEQ $print_end LF@$tmp bool@true\n");
            printf("LABEL $print_end\n");
            printf("CREATEFRAME\n");
            printf("DEFVAR TF@$result\n");
            printf("MOVE TF@$result LF@$result\n");
            printf("POPFRAME\n");
            printf("RETURN\n");  
            printf("\n");       
         break;     

        case BF_ORD:	// Vestavěná funkce ord()
           printf("LABEL ord\n");
           printf("PUSHFRAME\n");
           printf("DEFVAR LF@$result\n");
           printf("MOVE LF@$result nil@nil\n");
           printf("DEFVAR LF@$vstup\n");
           printf("DEFVAR LF@$index\n");
           printf("DEFVAR LF@$tmp\n");   
           printf("MOVE LF@$vstup LF@$$1\n");
           printf("MOVE LF@$index LF@$$2\n");
           printf("DEFVAR LF@$delka_vstupu\n");
           printf("STRLEN LF@$delka_vstupu LF@$vstup\n");
           printf("SUB LF@$delka_vstupu LF@$delka_vstupu int@1\n");
           printf("LT LF@$tmp LF@$index int@0\n");
           printf("JUMPIFEQ $end_ord LF@$tmp bool@true\n");
           printf("GT LF@$tmp LF@$index LF@$delka_vstupu\n");
           printf("JUMPIFEQ $end_ord LF@$tmp bool@true\n");
           printf("STRI2INT LF@$tmp LF@$vstup LF@$index\n");
           printf("MOVE LF@$result LF@$tmp\n");
           printf("LABEL $end_ord\n");
           printf("CREATEFRAME\n");
           printf("DEFVAR TF@$result\n");
           printf("MOVE TF@$result LF@$result\n");
           printf("POPFRAME\n");
           printf("RETURN\n");
           printf("\n");         
          break;
        case BF_SUBSTR:	// Vestavěná funkce substr()
          printf("LABEL substr\n");
          printf("PUSHFRAME\n");
          printf("DEFVAR LF@$result\n");
          printf("MOVE LF@$result nil@nil\n");
          printf("DEFVAR LF@$vstup\n");
          printf("DEFVAR LF@$delka\n");
          printf("DEFVAR LF@$index\n");
          printf("DEFVAR LF@$tmp\n");
          printf("DEFVAR LF@$delka_vstupu\n");
          printf("MOVE LF@$vstup LF@$$1\n");
          printf("MOVE LF@$index LF@$$2\n");
          printf("MOVE LF@$delka LF@$$3\n");
          printf("MOVE LF@$result string@\n");
          printf("STRLEN LF@$delka_vstupu LF@$vstup\n");
          printf("SUB LF@$delka_vstupu LF@$delka_vstupu int@1\n");  
              //parametry
          printf("LT LF@$tmp LF@$index int@0\n");
          printf("JUMPIFEQ chyba_substr LF@$tmp bool@true\n");
          printf("GT LF@$tmp LF@$delka LF@$delka_vstupu\n");
          printf("JUMPIFEQ chyba_substr LF@$tmp bool@true\n");
          printf("LT LF@$tmp LF@$delka int@0\n");
          printf("JUMPIFEQ chyba_substr LF@$tmp bool@true\n");
            
          printf("LABEL substr_while\n");
          printf("GETCHAR LF@$tmp LF@$vstup LF@$index\n");
          printf("CONCAT LF@$result LF@$result LF@$tmp\n");
          printf("ADD LF@$index LF@$index int@1\n");
          printf("SUB LF@$delka LF@$delka int@1\n");
          printf("GT LF@$tmp LF@$delka int@0\n");
          printf("JUMPIFEQ substr_while LF@$tmp bool@true\n");
          printf("EQ LF@$tmp LF@$delka int@0\n");
          printf("JUMPIFEQ chyba_substr LF@$tmp bool@true\n");
            //konec
          printf("LABEL chyba_substr\n");
          printf("CREATEFRAME\n");
          printf("DEFVAR TF@$result\n");
          printf("MOVE TF@$result LF@$result\n");
          printf("POPFRAME\n");
          printf("RETURN\n");
          printf("\n");     
          break;
        case BF_CHR:	// Vestavěná funkce chr()
          printf("LABEL chr\n");
          printf("PUSHFRAME\n");
          printf("DEFVAR LF@$result\n");
          printf("MOVE LF@$result nil@nil\n");
          printf("DEFVAR LF@$vstup\n"); 
          printf("MOVE LF@$vstup LF@$$1\n");
          printf("INT2CHAR LF@$result LF@$vstup\n");
          printf("CREATEFRAME\n");
          printf("DEFVAR TF@$result\n");
          printf("MOVE TF@$result LF@$result\n");
          printf("POPFRAME\n");
          printf("RETURN\n");
          printf("\n");                                    
            break;
         case ADD:	// Volání funkce pro sčítání()
          printf("CREATEFRAME\n");
          printf("DEFVAR TF@$1\n");
          printf("DEFVAR TF@$2\n");
          printf("MOVE TF@$1 LF@$prec1\n");
          printf("MOVE TF@$2 LF@$prec2\n");
          printf("CALL $add\n");
          printf("MOVE LF@$prec1 TF@$result\n");     
          break;
         case N_ADD:	// Funkce funkce pro sčítání()
         printf("LABEL $add\n");
         printf("PUSHFRAME\n");
         printf("DEFVAR LF@$val\n");
         printf("DEFVAR LF@$1type\n");
         printf("DEFVAR LF@$2type\n");
         printf("TYPE LF@$1type LF@$1\n");
         printf("TYPE LF@$2type LF@$2\n");

         printf("JUMPIFEQ $str1 LF@$1type string@string\n"); // Prvni string
         printf("JUMPIFEQ $err LF@$1type string@nil\n"); // Jakykoliv je nil
         printf("JUMPIFEQ $err LF@$2type string@nil\n");
         printf("JUMPIFEQ $first_int LF@$1type string@int\n"); // Prvni int 
         printf("JUMPIFEQ $first_float LF@$1type string@float\n"); // Prvni float

         printf("LABEL $first_int\n");
         printf("JUMPIFEQ $OK LF@$2type string@int\n");
         printf("JUMPIFEQ $err LF@$2type string@string\n");
         printf("INT2FLOAT LF@$1 LF@$1\n");
         printf("JUMP $OK\n");

         printf("LABEL $first_float\n");
         printf("JUMPIFEQ $OK LF@$2type string@float\n");
         printf("JUMPIFEQ $err LF@$2type string@string\n");
         printf("INT2FLOAT LF@$2 LF@$2\n");
         printf("JUMP $OK\n");

         printf("LABEL $str1\n");
         printf("JUMPIFEQ $str1$str2 LF@$2type string@string\n");
         printf("LABEL $err\n");
         printf("EXIT int@4\n");
         printf("LABEL $OK\n");
         printf("ADD LF@$val LF@$1 LF@$2\n");
         printf("JUMP $end\n");

         printf("LABEL $str1$str2\n");
         printf("CONCAT LF@$val LF@$1 LF@$2\n");
         printf("LABEL $end\n");
         printf("DEFVAR LF@$result\n");
         printf("MOVE LF@$result LF@$val\n");
         printf("POPFRAME\n");
         printf("RETURN\n");     
         printf("\n");   
         break;

         case N_SUB: // Funkce pro odečítání
         printf("LABEL $sub\n");
         printf("PUSHFRAME\n");
         printf("DEFVAR LF@$val\n");
         printf("DEFVAR LF@$1type\n");
         printf("DEFVAR LF@$2type\n");
         printf("TYPE LF@$1type LF@$1\n");
         printf("TYPE LF@$2type LF@$2\n");

         printf("JUMPIFEQ $err_sub LF@$1type string@string\n");
         printf("JUMPIFEQ $err_sub LF@$2type string@string\n");
         printf("JUMPIFEQ $err_sub LF@$1type string@nil\n");
         printf("JUMPIFEQ $err_sub LF@$2type string@nil\n");
         printf("JUMPIFEQ $first_int_sub LF@$1type string@int\n");
         printf("JUMPIFEQ $first_float_sub LF@$1type string@float\n");


         printf("LABEL $first_int_sub\n");
         printf("JUMPIFEQ $OK_sub LF@$2type string@int\n");
         printf("INT2FLOAT LF@$1 LF@$1\n");
         printf("JUMP $OK_sub\n");

         printf("LABEL $first_float_sub\n");
         printf("JUMPIFEQ $OK_sub LF@$2type string@float\n");
         printf("INT2FLOAT LF@$2 LF@$2\n");
         printf("JUMP $OK_sub\n");
         printf("LABEL $err_sub\n");
         printf("EXIT int@4\n");
         printf("LABEL $OK_sub\n");        
         printf("SUB LF@$val LF@$1 LF@$2\n");
         printf("DEFVAR LF@$result\n");
         printf("MOVE LF@$result LF@$val\n");
         printf("POPFRAME\n");
         printf("RETURN\n");     
         printf("\n");   
         break;

        case SUB:	// Volání funkce pro odečítání
          printf("CREATEFRAME\n");
          printf("DEFVAR TF@$1\n");
          printf("DEFVAR TF@$2\n");
          printf("MOVE TF@$1 LF@$prec1\n");
          printf("MOVE TF@$2 LF@$prec2\n");
          printf("CALL $sub\n");
          printf("MOVE LF@$prec1 TF@$result\n");
          break;

         case N_MUL: // Funkce pro násobení 
         printf("LABEL $mul\n");
         printf("PUSHFRAME\n");
         printf("DEFVAR LF@$val\n");
         printf("DEFVAR LF@$1type\n");
         printf("DEFVAR LF@$2type\n");
         printf("TYPE LF@$1type LF@$1\n");
         printf("TYPE LF@$2type LF@$2\n");

         printf("JUMPIFEQ $err_mul LF@$1type string@string\n");
         printf("JUMPIFEQ $err_mul LF@$2type string@string\n");
         printf("JUMPIFEQ $err_mul LF@$1type string@nil\n");
         printf("JUMPIFEQ $err_mul LF@$2type string@nil\n");
         printf("JUMPIFEQ $first_int_mul LF@$1type string@int\n");
         printf("JUMPIFEQ $first_float_mul LF@$1type string@float\n");


         printf("LABEL $first_int_mul\n");
         printf("JUMPIFEQ $OK_mul LF@$2type string@int\n");
         printf("INT2FLOAT LF@$1 LF@$1\n");
         printf("JUMP $OK_mul\n");

         printf("LABEL $first_float_mul\n");
         printf("JUMPIFEQ $OK_mul LF@$2type string@float\n");
         printf("INT2FLOAT LF@$2 LF@$2\n");
         printf("JUMP $OK_mul\n");
         printf("LABEL $err_mul\n");
         printf("EXIT int@4\n");
         printf("LABEL $OK_mul\n");        
         printf("MUL LF@$val LF@$1 LF@$2\n");
         printf("DEFVAR LF@$result\n");
         printf("MOVE LF@$result LF@$val\n");
         printf("POPFRAME\n");
         printf("RETURN\n");     
         printf("\n");   
         break;  
        case MUL: // Volání funkce pro násobení 
          printf("CREATEFRAME\n");
          printf("DEFVAR TF@$1\n");
          printf("DEFVAR TF@$2\n");
          printf("MOVE TF@$1 LF@$prec1\n");
          printf("MOVE TF@$2 LF@$prec2\n");
          printf("CALL $mul\n");
          printf("MOVE LF@$prec1 TF@$result\n");
          break;
         case N_EQ: // Funkce pro porovnání rovnosti
         printf("LABEL $eq\n");
         printf("PUSHFRAME\n");
         printf("DEFVAR LF@$val\n");
         printf("DEFVAR LF@$1type\n");
         printf("DEFVAR LF@$2type\n");
         printf("TYPE LF@$1type LF@$1\n");
         printf("TYPE LF@$2type LF@$2\n");

         printf("JUMPIFEQ $first_string_eq LF@$1type string@string\n"); // prvni string
         printf("JUMPIFEQ $first_int_eq LF@$1type string@int\n"); // prvni int 
         printf("JUMPIFEQ $first_float_eq LF@$1type string@float\n"); // prvni float
         printf("JUMPIFEQ $first_bool_eq LF@$1type string@bool\n"); // prvni string
         printf("JUMPIFEQ $first_nil_eq LF@$1type string@nil\n"); // prvni bool 

         printf("LABEL $first_string_eq\n");
         printf("JUMPIFEQ $compare_eq LF@$2type string@string\n");
         printf("EXIT int@4\n");

         printf("LABEL $first_int_eq\n");
         printf("JUMPIFEQ $compare_eq LF@$2type string@int\n");
         printf("JUMPIFEQ $conv1_eq LF@$2type string@float\n");
         printf("EXIT int@4\n");

         printf("LABEL $conv1_eq\n");
         printf("INT2FLOAT LF@$1type LF@$1type\n");
         printf("JUMP $compare_eq\n");

         printf("LABEL $conv2_eq\n");
         printf("INT2FLOAT LF@$2type LF@$2type\n");
         printf("JUMP $compare_eq\n");


         printf("LABEL $first_float_eq\n");
         printf("JUMPIFEQ $compare_eq LF@$2type string@float\n");
         printf("JUMPIFEQ $conv2_eq LF@$2type string@int\n");
         printf("EXIT int@4\n");


         printf("LABEL $first_bool_eq\n");
         printf("JUMPIFEQ $compare_eq LF@$2type string@bool\n");
         printf("EXIT int@4\n");

         printf("LABEL $first_nil_eq\n");
         printf("JUMPIFEQ $compare_eq LF@$2type string@nil\n");
         printf("EXIT int@4\n");         

         printf("LABEL $compare_eq\n");
         printf("EQ LF@$val LF@$1 LF@$2\n");
         printf("DEFVAR LF@$result\n");
         printf("MOVE LF@$result LF@$val\n");
         printf("POPFRAME\n");
         printf("RETURN\n");     
         printf("\n");
         break;
          case EQ: // Volání funkce pro porovnání rovnosti
          printf("CREATEFRAME\n");
          printf("DEFVAR TF@$1\n");
          printf("DEFVAR TF@$2\n");
          printf("MOVE TF@$1 LF@$prec1\n");
          printf("MOVE TF@$2 LF@$prec2\n");
          printf("CALL $eq\n");
          printf("MOVE LF@$prec1 TF@$result\n");
          break;

          case NEQ_EQ:	// Volání funkce pro porovnání nerovnosti
          printf("CREATEFRAME\n");
          printf("DEFVAR TF@$1\n");
          printf("DEFVAR TF@$2\n");
          printf("MOVE TF@$1 LF@$prec1\n");
          printf("MOVE TF@$2 LF@$prec2\n");
          printf("CALL $neq_eq\n");
          printf("MOVE LF@$prec1 TF@$result\n");
          break;

         case N_NEQ_EQ:	// Funkce pro porovnání nerovnosti
         printf("LABEL $neq_eq\n");
         printf("PUSHFRAME\n");
         printf("DEFVAR LF@$val\n");
         printf("MOVE LF@$val bool@false\n");
         printf("DEFVAR LF@$1type\n");
         printf("DEFVAR LF@$2type\n");
         printf("TYPE LF@$1type LF@$1\n");
         printf("TYPE LF@$2type LF@$2\n");

         printf("JUMPIFEQ $first_string_neq_eq LF@$1type string@string\n"); // prvni string
         printf("JUMPIFEQ $first_int_neq_eq LF@$1type string@int\n"); // prvni int 
         printf("JUMPIFEQ $first_float_neq_eq LF@$1type string@float\n"); // prvni float
         printf("JUMPIFEQ $first_bool_neq_eq LF@$1type string@bool\n"); // prvni bool
         printf("JUMPIFEQ $first_nil_neq_eq LF@$1type string@nil\n"); // prvni nill 

         printf("LABEL $first_string_neq_eq\n");
         printf("JUMPIFEQ $compare_eq LF@$2type string@string\n");
         printf("JUMP $konec_neq\n");

         printf("LABEL $first_int_neq_eq\n");
         printf("JUMPIFEQ $compare_neq_eq LF@$2type string@int\n");
         printf("JUMPIFEQ $conv1_neq_eq LF@$2type string@float\n");
         printf("JUMP $konec_neq\n");

         printf("LABEL $conv1_neq_eq\n");
         printf("INT2FLOAT LF@$1type LF@$1type\n");
         printf("JUMP $compare_neq_eq\n");

         printf("LABEL $conv2_neq_eq\n");
         printf("INT2FLOAT LF@$2type LF@$2type\n");
         printf("JUMP $compare_neq_eq\n");


         printf("LABEL $first_float_neq_eq\n");
         printf("JUMPIFEQ $compare_neq_eq LF@$2type string@float\n");
         printf("JUMPIFEQ $conv2_neq_eq LF@$2type string@int\n");
         printf("JUMP $konec_neq\n");


         printf("LABEL $first_bool_neq_eq\n");
         printf("JUMPIFEQ $compare_neq_eq LF@$2type string@bool\n");
         printf("JUMP $konec_neq\n");

         printf("LABEL $first_nil_neq_eq\n");
         printf("JUMPIFEQ $compare_neq_eq LF@$2type string@nil\n");
         printf("JUMP $konec_neq\n");         

         printf("LABEL $compare_neq_eq\n");
         printf("EQ LF@$val LF@$1 LF@$2\n");
         printf("NOT LF@$val LF@$val\n");
         printf("DEFVAR LF@$result\n");
         printf("MOVE LF@$result LF@$val\n");
         printf("LABEL $konec_neq\n");
         printf("POPFRAME\n");
         printf("RETURN\n");     
         printf("\n");
         break;
        case N_DIV: // Funkce pro dělení
         printf("LABEL $div\n");
         printf("PUSHFRAME\n");
         printf("DEFVAR LF@$val\n");
         printf("DEFVAR LF@$1type\n");
         printf("DEFVAR LF@$2type\n");
         printf("TYPE LF@$1type LF@$1\n");
         printf("TYPE LF@$2type LF@$2\n");

         printf("JUMPIFEQ $err_div LF@$1type string@string\n");
         printf("JUMPIFEQ $err_div LF@$2type string@string\n");
         printf("JUMPIFEQ $err_div LF@$1type string@nil\n");
         printf("JUMPIFEQ $err_div LF@$2type string@nil\n");
         printf("JUMPIFEQ $first_int_div LF@$1type string@int\n");
         printf("JUMPIFEQ $first_float_div LF@$1type string@float\n");

         printf("LABEL $first_int_div\n");
         printf("JUMPIFEQ $div_check_ints LF@$2type string@int\n");
         printf("INT2FLOAT LF@$1 LF@$1\n");
         printf("JUMPIFEQ $zero LF@$2 float@0x0.0p+0\n");
         printf("JUMP $OK_div_floats\n");


         printf("LABEL $first_float_div\n");
         printf("JUMPIFEQ $div_check_floats LF@$2type string@float\n");
         printf("INT2FLOAT LF@$2 LF@$2\n");
         printf("JUMPIFEQ $zero LF@$2 float@0x0.0p+0\n");
         printf("JUMP $OK_div_floats\n");


         printf("LABEL $div_check_ints\n");
         printf("JUMPIFEQ $zero LF@$2 int@0\n");
         printf("JUMP $OK_div_ints\n");

         printf("LABEL $div_check_floats\n");
         printf("JUMPIFEQ $zero LF@$2 float@0x0.0p+0\n");
         printf("JUMP $OK_div_floats\n");

         printf("LABEL $zero\n");
         printf("EXIT int@9\n");
         printf("LABEL $err_div\n");
         printf("EXIT int@4\n");

         printf("LABEL $OK_div_floats\n");        
         printf("DIV LF@$val LF@$1 LF@$2\n");
         printf("JUMP $div_end\n");

         printf("LABEL $OK_div_ints\n");        
         printf("IDIV LF@$val LF@$1 LF@$2\n");

         printf("LABEL $div_end\n");
         printf("DEFVAR LF@$result\n");
         printf("MOVE LF@$result LF@$val\n");
         printf("POPFRAME\n");
         printf("RETURN\n");
         printf("\n"); 
          break;

        case DIV: // Volání funkce pro dělení
          printf("CREATEFRAME\n");
          printf("DEFVAR TF@$1\n");
          printf("DEFVAR TF@$2\n");
          printf("MOVE TF@$1 LF@$prec1\n");
          printf("MOVE TF@$2 LF@$prec2\n");
          printf("CALL $div\n");
          printf("MOVE LF@$prec1 TF@$result\n");
          break;

          case N_LESS: // Funkce pro porovnání menší než
         printf("LABEL $less\n");
         printf("PUSHFRAME\n");
         printf("DEFVAR LF@$val\n");
         printf("DEFVAR LF@$1type\n");
         printf("DEFVAR LF@$2type\n");
         printf("TYPE LF@$1type LF@$1\n");
         printf("TYPE LF@$2type LF@$2\n");
         printf("JUMPIFEQ $first_string_less LF@$1type string@string\n"); // prvni string
         printf("JUMPIFEQ $first_int_less LF@$1type string@int\n"); // prvni int 
         printf("JUMPIFEQ $first_float_less LF@$1type string@float\n"); // prvni float
         printf("JUMPIFEQ $first_bool_less LF@$1type string@bool\n"); // 
         printf("JUMPIFEQ $less_err LF@$1type string@nil\n"); // prvni nil ani druhy       
         printf("JUMPIFEQ $less_err LF@$2type string@nil\n"); // nemuze byt nil  

         printf("LABEL $first_string_less\n");
         printf("JUMPIFEQ $OK_less LF@$2type string@string\n"); // 
         printf("EXIT int@4\n");

         printf("LABEL $first_bool_less\n");
         printf("JUMPIFEQ $OK_less LF@$2type string@bool\n"); // 
         printf("EXIT int@4\n");

         printf("LABEL $first_float_less\n");
         printf("JUMPIFEQ $OK_less LF@$2type string@float\n");
         printf("JUMPIFEQ $conv2_less LF@$2type string@int\n");
         printf("EXIT int@4\n");

         printf("LABEL $conv2_less\n");
         printf("INT2FLOAT LF@$2 LF@$2\n"); 
         printf("JUMP $OK_less\n"); 

         printf("LABEL $first_int_less\n");
         printf("JUMPIFEQ $OK_less LF@$2type string@int\n");
         printf("JUMPIFEQ $conv1_less LF@$2type string@float\n");
         printf("EXIT int@4\n");     

         printf("LABEL $conv1_less\n");
         printf("INT2FLOAT LF@$1 LF@$1\n"); 
         printf("JUMP $OK_less\n");     

         printf("LABEL $OK_less\n");
         printf("LT LF@$val LF@$1 LF@$2\n");
         printf("DEFVAR LF@$result\n");
         printf("MOVE LF@$result LF@$val\n");
         printf("POPFRAME\n");
         printf("RETURN\n");    
         
         printf("LABEL $less_err\n");
         printf("EXIT int@4\n");
         printf("\n");

          break;

          case LESS: // Volání funkce pro porovnání menší než
          printf("CREATEFRAME\n");
          printf("DEFVAR TF@$1\n");
          printf("DEFVAR TF@$2\n");
          printf("MOVE TF@$1 LF@$prec1\n");
          printf("MOVE TF@$2 LF@$prec2\n");
          printf("CALL $less\n");
          printf("MOVE LF@$prec1 TF@$result\n");
              break;


          case N_MORE: // Funkce pro porovnání větší než
         printf("LABEL $more\n");
         printf("PUSHFRAME\n");
         printf("DEFVAR LF@$val\n");
         printf("DEFVAR LF@$1type\n");
         printf("DEFVAR LF@$2type\n");
         printf("TYPE LF@$1type LF@$1\n");
         printf("TYPE LF@$2type LF@$2\n");
         printf("JUMPIFEQ $first_string_more LF@$1type string@string\n"); // prvni string
         printf("JUMPIFEQ $first_int_more LF@$1type string@int\n"); // prvni int 
         printf("JUMPIFEQ $first_float_more LF@$1type string@float\n"); // prvni float
         printf("JUMPIFEQ $first_bool_more LF@$1type string@bool\n"); // 
         printf("JUMPIFEQ $more_err LF@$1type string@nil\n"); // prvni nil ani druhy       
         printf("JUMPIFEQ $more_err LF@$2type string@nil\n"); // nemuze byt nil  

         printf("LABEL $first_string_more\n");
         printf("JUMPIFEQ $OK_more LF@$2type string@string\n"); // 
         printf("EXIT int@4\n");

         printf("LABEL $first_bool_more\n");
         printf("JUMPIFEQ $OK_more LF@$2type string@bool\n"); // 
         printf("EXIT int@4\n");

         printf("LABEL $first_float_more\n");
         printf("JUMPIFEQ $OK_more LF@$2type string@float\n");
         printf("JUMPIFEQ $conv2_more LF@$2type string@int\n");
         printf("EXIT int@4\n");

         printf("LABEL $conv2_more\n");
         printf("INT2FLOAT LF@$2 LF@$2\n"); 
         printf("JUMP $OK_more\n"); 

         printf("LABEL $first_int_more\n");
         printf("JUMPIFEQ $OK_more LF@$2type string@int\n");
         printf("JUMPIFEQ $conv1_more LF@$2type string@float\n");
         printf("EXIT int@4\n");     

         printf("LABEL $conv1_more\n");
         printf("INT2FLOAT LF@$1 LF@$1\n"); 
         printf("JUMP $OK_more\n");     

         printf("LABEL $OK_more\n");
         printf("GT LF@$val LF@$1 LF@$2\n");
         printf("DEFVAR LF@$result\n");
         printf("MOVE LF@$result LF@$val\n");
         printf("POPFRAME\n");
         printf("RETURN\n");    
         
         printf("LABEL $more_err\n");
         printf("EXIT int@4\n");
         printf("\n");

          break;

          case MORE: // Volání funkce větší než
          printf("CREATEFRAME\n");
          printf("DEFVAR TF@$1\n");
          printf("DEFVAR TF@$2\n");
          printf("MOVE TF@$1 LF@$prec1\n");
          printf("MOVE TF@$2 LF@$prec2\n");
          printf("CALL $more\n");
          printf("MOVE LF@$prec1 TF@$result\n");
              break;

         case N_MORE_EQ: // Funkce větši než-rovno
         printf("LABEL $more_eq\n");
         printf("PUSHFRAME\n");
         printf("DEFVAR LF@$val\n");
         printf("DEFVAR LF@$1type\n");
         printf("DEFVAR LF@$2type\n");
         printf("TYPE LF@$1type LF@$1\n");
         printf("TYPE LF@$2type LF@$2\n");
         printf("JUMPIFEQ $first_string_more_eq LF@$1type string@string\n"); // prvni string
         printf("JUMPIFEQ $first_int_more_eq LF@$1type string@int\n"); // prvni int 
         printf("JUMPIFEQ $first_float_more_eq LF@$1type string@float\n"); // prvni float
         printf("JUMPIFEQ $first_bool_more_eq LF@$1type string@bool\n"); // 
         printf("JUMPIFEQ $more_eq_err LF@$1type string@nil\n"); // prvni nil ani druhy       
         printf("JUMPIFEQ $more_eq_err LF@$2type string@nil\n"); // nemuze byt nil  

         printf("LABEL $first_string_more_eq\n");
         printf("JUMPIFEQ $OK_more_eq LF@$2type string@string\n"); // 
         printf("EXIT int@4\n");

         printf("LABEL $first_bool_more_eq\n");
         printf("JUMPIFEQ $OK_more_eq LF@$2type string@bool\n"); // 
         printf("EXIT int@4\n");

         printf("LABEL $first_float_more_eq\n");
         printf("JUMPIFEQ $OK_more_eq LF@$2type string@float\n");
         printf("JUMPIFEQ $conv2_more_eq LF@$2type string@int\n");
         printf("EXIT int@4\n");

         printf("LABEL $conv2_more_eq\n");
         printf("INT2FLOAT LF@$2 LF@$2\n"); 
         printf("JUMP $OK_more_eq\n"); 

         printf("LABEL $first_int_more_eq\n");
         printf("JUMPIFEQ $OK_more_eq LF@$2type string@int\n");
         printf("JUMPIFEQ $conv1_more_eq LF@$2type string@float\n");
         printf("EXIT int@4\n");     

         printf("LABEL $conv1_more_eq\n");
         printf("INT2FLOAT LF@$1 LF@$1\n"); 
         printf("JUMP $OK_more_eq\n");     

         printf("LABEL $OK_more_eq\n");
         printf("LT LF@$val LF@$1 LF@$2\n");
         printf("NOT LF@$val LF@$val\n");
         printf("DEFVAR LF@$result\n");
         printf("MOVE LF@$result LF@$val\n");
         printf("POPFRAME\n");
         printf("RETURN\n");    
         
         printf("LABEL $more_eq_err\n");
         printf("EXIT int@4\n");
         printf("\n");

          break;

          case MORE_EQ:	//Volání funkce větši než-rovno
          printf("CREATEFRAME\n");
          printf("DEFVAR TF@$1\n");
          printf("DEFVAR TF@$2\n");
          printf("MOVE TF@$1 LF@$prec1\n");
          printf("MOVE TF@$2 LF@$prec2\n");
          printf("CALL $more_eq\n");
          printf("MOVE LF@$prec1 TF@$result\n");
              break;

         case N_LESS_EQ: // Funkce menší než rovno
         printf("LABEL $less_eq\n");
         printf("PUSHFRAME\n");
         printf("DEFVAR LF@$val\n");
         printf("DEFVAR LF@$1type\n");
         printf("DEFVAR LF@$2type\n");
         printf("TYPE LF@$1type LF@$1\n");
         printf("TYPE LF@$2type LF@$2\n");
         printf("JUMPIFEQ $first_string_less_eq LF@$1type string@string\n"); // prvni string
         printf("JUMPIFEQ $first_int_less_eq LF@$1type string@int\n"); // prvni int 
         printf("JUMPIFEQ $first_float_less_eq LF@$1type string@float\n"); // prvni float
         printf("JUMPIFEQ $first_bool_less_eq LF@$1type string@bool\n"); // 
         printf("JUMPIFEQ $less_eq_err LF@$1type string@nil\n"); // prvni nil ani druhy       
         printf("JUMPIFEQ $less_eq_err LF@$2type string@nil\n"); // nemuze byt nil  

         printf("LABEL $first_string_less_eq\n");
         printf("JUMPIFEQ $OK_less_eq LF@$2type string@string\n"); // 
         printf("EXIT int@4\n");

         printf("LABEL $first_bool_less_eq\n");
         printf("JUMPIFEQ $OK_less_eq LF@$2type string@bool\n"); // 
         printf("EXIT int@4\n");

         printf("LABEL $first_float_less_eq\n");
         printf("JUMPIFEQ $OK_less_eq LF@$2type string@float\n");
         printf("JUMPIFEQ $conv2_less_eq LF@$2type string@int\n");
         printf("EXIT int@4\n");

         printf("LABEL $conv2_less_eq\n");
         printf("INT2FLOAT LF@$2 LF@$2\n"); 
         printf("JUMP $OK_less_eq\n"); 

         printf("LABEL $first_int_less_eq\n");
         printf("JUMPIFEQ $OK_less_eq LF@$2type string@int\n");
         printf("JUMPIFEQ $conv1_less_eq LF@$2type string@float\n");
         printf("EXIT int@4\n");     

         printf("LABEL $conv1_less_eq\n");
         printf("INT2FLOAT LF@$1 LF@$1\n"); 
         printf("JUMP $OK_less_eq\n");     

         printf("LABEL $OK_less_eq\n");
         printf("GT LF@$val LF@$1 LF@$2\n");
         printf("NOT LF@$val LF@$val\n");
         printf("DEFVAR LF@$result\n");
         printf("MOVE LF@$result LF@$val\n");
         printf("POPFRAME\n");
         printf("RETURN\n");    
         
         printf("LABEL $less_eq_err\n");
         printf("EXIT int@4\n");
         printf("\n");

          break;

          case LESS_EQ: // Funkce menší než rovno
          printf("CREATEFRAME\n");
          printf("DEFVAR TF@$1\n");
          printf("DEFVAR TF@$2\n");
          printf("MOVE TF@$1 LF@$prec1\n");
          printf("MOVE TF@$2 LF@$prec2\n");
          printf("CALL $less_eq\n");
          printf("MOVE LF@$prec1 TF@$result\n");
              break;
         } 

        actualInstruction = actualInstruction->nextItem;
    }
}
