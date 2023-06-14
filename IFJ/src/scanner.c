/******** scanner.c *********
* Implementace překladače imperativního jazyka IFJ18.
* FIT VUT, Tým 087, varianta I
* Autori:   xvlach18    Martin Vlach
*           xhekrd01    Lukas Hekrdla
*           xtlapa00    Tomas Tlapak
* Shrnutí:
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
#include"scanner.h"
#include"error_codes.h"

// Define stavů
#define S_INIT  0
#define S_LINE_COMMENT  1
#define S_LESS  2
#define S_MORE  3
#define S_NOT_EQUAL 4
#define S_EQUAL 5
#define S_ID    6
#define S_NUMBER  7
#define S_ZERO 8
#define S_BIN 9
#define S_BIN1 10
#define S_HEX 11
#define S_HEX1 12
#define S_OCT 13
#define S_POINT 14
#define S_POINT1 15
#define S_EXP 16
#define S_EXP1 17
#define S_FLOAT 18
#define S_STRING 19
#define S_ESCAPE 20
#define S_ASCII_HEX 21
#define S_ASCII_HEX1 22
#define S_BEGIN_B 23
#define S_BEGIN_E 24
#define S_BEGIN_G 25
#define S_BEGIN_I 26
#define S_BEGIN_N 27
#define S_END_E 28
#define S_END_N 29
#define S_END_D 30
#define S_BLOCK_COMMENT 31
#define S_BLOCK_COMMENT0 32
#define S_BLOCK_COMMENT1 33

// Počáteční inicializace scanneru
int set_Scanner() 
{
    input = stdin;
    position_X = 0; // Index pozice v řádku
    position_Y = 1; // Číslo řádku

    buffer = (&buffer_tmp);
    if (strInit(buffer) == 1) 
    {
        fprintf(stderr,"Error - allocation memory for scanner buffer unsuccessful \n");
        return INTERNAL_ERR;
    }
    return OK;
}

// Funkce pro korektní zavření scanneru a závislostí
int close_Scanner() 
{
    strFree(buffer); // Vyčištění zbytků v bufferu
    if (fclose(input) == EOF) 
    {
        fprintf(stderr,"Error - closing input stream fail \n");
        return INTERNAL_ERR;
    }
    else
    {    
        return OK;
    }
}

// Funkce pro načtení tokenu
int next_Token() 
{
    char c; // Proměnná pro načítaný znak
    int state = 0; // Proměnná pro stav kam se budu přesouvat
    slash_buffer[0] = '0'; // Vymažu si pole pro hexa escape sekvenci
    slash_buffer[1] = '0';

    const int read = 1;
    token.type = T_ERROR; // Pokud neskončím v cílovém stavu jedná se o chybu aktuálníně spracovávaného tokenu
    strClear(buffer); // Vyčístím buffer

    while(read) 
    {
        c = getc(input); // Načítám znak
        position_X++;

        switch(state) {

        // Počáteční stav
        case S_INIT:   

            if (c == EOF) 
            { 
                // Pokud dostanu EOF, jedná se o token typu T_EOF
                token.type = T_EOF;
                return OK;
            }
            else if (c == '\n') 
            {   
                // Pokud dostanu EOL, jedná se o koncový stav T_EOL
                position_Y++;   // Přičtu index řádku
                position_X = 0;
                token.type = T_EOL;
                return OK;
            }
            else if (isspace(c)) 
            { 
                // Ignoruju mezery
                state = S_INIT;
            }
            else if (c == '0') 
            { 
                // Pokud dostanu nulu, přidám si ji do bufferu, posouvám se do stavu S_ZERO
                strAddChar(buffer,c);
                state = S_ZERO;
            }
            else if (c == '+') 
            { 
                // Pokud dostanu + jedná se o token typu T_ADD
                token.type = T_ADD;
                return OK;
            }
            else if (isdigit(c) && c != '0') 
            { 
                // Pokud se jedná o číslo, které není nula posouvám se do stavu pro číslo S_NUMBER
                strAddChar(buffer,c);
                state = S_NUMBER;
            }
            else if (c == '-') 
            { 
                // Pokud dostanu - jedná se o token typu T_MINUS
                token.type = T_MINUS;
                return OK;
            }
            else if (c == '*') 
            { 
                // Pokud dostanu * jedná se o koncový stav T_MULTIPLY
                token.type = T_MULTIPLY;
                return OK;
            }
            else if (c == '=') 
            { 
                // Pokud dostanu rovná se, jdu do stavu S_EQUAL
                state = S_EQUAL;
            }
            else if (c == '/') 
            { 
                // Pokud dostanu / jedná se o token typu T_DIV
                token.type = T_DIV;
                return OK;
            }
            else if (c == '(') 
            { 
                // Pokud dostanu ( jedná se o token typu T_LEFT_BRACKET
                token.type = T_LEFT_BRACKET;
                return OK;
            }
            else if (c == ')') 
            { 
                // Pokud dostanu ) jedná se o token typu T_RIGHT_BRACKET
                token.type = T_RIGHT_BRACKET;
                return OK;
            }
            else if (c == ',') 
            { 
                // Pokud dostanu , jedná se o token typu T_COMMA
                token.type = T_COMMA;
                return OK;
            }
            else if (c == '#') 
            { 
                // Pokud dostanu # jedná se o řádkový komentář, budu cyklit ve stavu S_LINE_COMMENT
                state = S_LINE_COMMENT;
            }
            else if (c == '!') 
            { 
                // Pokud dostanu ! jdu do stavu S_NOT_EQUAL
                state = S_NOT_EQUAL;
            }
            else if (c == '<') 
            { 
                // Pokud dostanu < jdu do stavu S_LESS
                state = S_LESS;
            }
            else if (c == '"') 
            { 
                // Pokud dostanu " jendá se o začátek stringu
                state = S_STRING;
            }
            else if (c == '>') 
            { 
                // Pokud dostanu > jdu do stavu S_MORE
                state = S_MORE;
            }
            else if (( islower(c) || c == '_' ) && !isdigit(c)) 
            { 
                // Pokud se jedná o povolený počáteční prefix indetifikátoru, nebo klíčového slova jdu do stavu S_ID
                strAddChar(buffer,c);
                state = S_ID;
            }
            else    
            {  
                // Pokud jsem nepřešel do žádného výše zmíněného stavu, tak se jedná o nepovolený znak (neznámý)
                fprintf(stderr,"Unsupported character with ASCII value '%d'[line %d]\n",c,position_Y);
                return LEX_ERR;
            }
            break;
        
        // Dokud nedostanu \n nebo EOF jedna se o 1 řádkový komentář
        case S_LINE_COMMENT: 
            if (c == '\n' || c == EOF)
            {
                ungetc(c,input);
                state = S_INIT;
            }
            else
            {   
                state = S_LINE_COMMENT; 
            }
            break;

        // Pokud mi za méně následuje = jedná se o T_LESS_EQUAL, pokud cokoli jiného tak T_LESS
        case S_LESS: 
            if (c == '=') 
            {
                token.type = T_LESS_EQUAL;
                return OK;
            }
            else 
            {
                ungetc(c,input);
                token.type = T_LESS;
                return OK;
            }
            break;

        // Pokud za ! následuje = jedná se o T_NOT_EQUAL
        case S_NOT_EQUAL: 
            if (c == '=') 
            {
                token.type = T_NOT_EQUAL;
                return OK;
            }
            else 
            {
                ungetc(c,input);
                fprintf(stderr, "Unexpect character after '!'[line %d]\n",position_Y);
                return LEX_ERR;
            }
            break;

        // Pokud mi za méně následuje = jedná se o T_MORE_EQUAL, pokud cokoli jiného tak T_MORE
        case S_MORE:   
            if (c == '=') 
            {
                token.type = T_MORE_EQUAL;
                return OK;
            }
            else 
            {
                ungetc(c,input);
                token.type = T_MORE;
                return OK;
            }
            break;

        // Pokud za = následuje = jedná se o T_EQUAL, jinak se jedná o T_ASSIGMENT
        case S_EQUAL: 
            if (c == '=' && position_X != 2) 
            {
                token.type = T_EQUAL;
                return OK;
            }
            else if(c == 'b' && position_X == 2) 
            { 
                // Pokud jsem po = dostal b, a součastně toto rovná se bylo na začátku řádku, může se jednat o začátek blokového komentáře
                state = S_BEGIN_B;
            }
            else if (position_X != 2) 
            {
                ungetc(c,input);
                token.type = T_ASSIGMENT;
                return OK;
            }
            else 
            {
                ungetc(c,input);
                token.type = T_ERROR;
                fprintf(stderr,"Expecting start of block comment[line %d]\n",position_Y);
                return LEX_ERR;
            }
            break;

        // Blokový komentář
        case S_BEGIN_B:
            if (c == 'e') 
            {
                state = S_BEGIN_E;
            }
            else 
            {
                ungetc(c,input);
                token.type = T_ERROR;
                fprintf(stderr,"Expecting start of block comment [line %d]\n",position_Y);
                return LEX_ERR;
            }
            break;

        // Stavy blokového komentáře
        case S_BEGIN_E:
            if (c == 'g') 
            {
                state = S_BEGIN_G;
            }
            else 
            {
                ungetc(c,input);
                token.type = T_ERROR;
                fprintf(stderr,"Expecting start of block comment [line %d]\n",position_Y);
                return LEX_ERR;
            }
            break;

        case S_BEGIN_G:
            if (c == 'i') 
            {
                state = S_BEGIN_I;
            }
            else 
            {
                ungetc(c,input);
                token.type = T_ERROR;
                fprintf(stderr,"Expecting start of block comment[line %d]\n",position_Y);
                return LEX_ERR;
            }
            break;

        case S_BEGIN_I:
            if (c == 'n') 
            {
                state = S_BEGIN_N;
            }
            else 
            {
                ungetc(c,input);
                token.type = T_ERROR;
                fprintf(stderr,"Expecting start of block comment[line %d]\n",position_Y);
                return LEX_ERR;
            }
            break;

        case S_BEGIN_N:
            if (isspace(c)|| c == '\n') 
            {
                position_X = 0;
                position_Y++;
                state = S_BLOCK_COMMENT;
            }
            else if (c == EOF) 
            {
                ungetc(c,input);
                token.type = T_ERROR;
                fprintf(stderr,"Unended block comment [line %d]\n",position_Y);
                return LEX_ERR;
            }
            else 
            {
                ungetc(c,input);
                fprintf(stderr,"Expecting start of block comment[line %d]",position_Y);
                token.type = T_ERROR;
                return LEX_ERR;
            }
            break;

        case S_BLOCK_COMMENT:
            if (c == '=' && position_X == 1) 
            {
                state = S_END_E;
            }
            else if( c == EOF) 
            {
                ungetc(c,input);
                fprintf(stderr,"Expecting '=end' of block comment [line %d]\n",position_Y);
                token.type = T_ERROR;
                return LEX_ERR;
            }
            else 
            {
                state = S_BLOCK_COMMENT;
                if (c == '\n') 
                {
                    position_X = 0;
                    position_Y++;
                }
            }
            break;

        case S_END_E:
            if (c == 'e') 
            {
                state = S_END_N;
            }
            else if( c == EOF) 
            {
                ungetc(c,input);
                fprintf(stderr,"Expecting '=end' of block comment [line %d]\n",position_Y);
                token.type = T_ERROR;
                return LEX_ERR;
            }
            else
            { 
                state = S_BLOCK_COMMENT;
            }
            break;

        case S_END_N:
            if (c == 'n') 
            {
                state = S_END_D;
            }
            else if( c == EOF) 
            {
                ungetc(c,input);
                fprintf(stderr,"Expecting '=end' of block comment [line %d]\n",position_Y);
                token.type = LEX_ERR;
                return LEX_ERR;
            }
            else
            { 
                state = S_BLOCK_COMMENT;
            }
            break;

        case S_END_D:
            if (c == 'd') 
            {
                state = S_BLOCK_COMMENT0;
            }
            else if( c == EOF) 
            {
                ungetc(c,input);
                token.type = LEX_ERR;
                fprintf(stderr,"Expecting '=end' of block comment [line %d]\n",position_Y);
                return LEX_ERR;
            }
            else 
            {
                state = S_BLOCK_COMMENT;
            }
            break;

        case S_BLOCK_COMMENT0:
            if (c == '\n' || c == EOF || isspace(c)) 
            {
                ungetc(c,input);
                state = S_BLOCK_COMMENT1;
            }
            else 
            {
                token.type = T_ERROR;
                fprintf(stderr,"There must be at least 1 space or TAB between =end and comment [line %d]",position_Y);
                return LEX_ERR;
            }
            break;

        case S_BLOCK_COMMENT1:
            if (c == '\n' || c == EOF) 
            {
                position_Y++;
                position_X = 0;
                state = S_INIT;
                if (c == EOF) {
                    ungetc(c,input);
                }
            }
            else
            {
                state = S_BLOCK_COMMENT1;
            }
            break;

        // Stav poté co dostanu nulu
        case S_ZERO:
            if (c == 'b') 
            { 
                // Pokud za nulou dostanu b, jedná se o INT číslo které se zadává v binární podobě
                state = S_BIN;
            }
            else if (c >= '0' && c <= '7') 
            { 
                // Pokud za nulou dostanu 0-7, jedná se o INT číslo které se zadává v binární podobě
                strAddChar(buffer,c);
                state = S_OCT;
            }
            else if (c == 'x') 
            { 
                // Pokud za nulou dostanu x, jedná se o INT číslo které se zadává v hexadecimální podobě
                state = S_HEX;
                break;
            }
            else if (c == '.') 
            { 
                // Pokud dostanu po nule tečku jedná se o float číslo -> přesunu se do stavu S_POINT
                strAddChar(buffer,c);
                state = S_POINT;
            }
            else if (c == 'e' || c == 'E') 
            { 
                // Pokud po nule dostanu e/E bude se zadávat exponent -> přesudu se do stavu S_EXP
                strAddChar(buffer,c);
                state = S_EXP;
            }
            else if (c == '8' || c == '9') 
            {
                ungetc(c,input);
                fprintf(stderr,"After 0 character %c is unsupported [line %d]\n",c,position_Y);
                return LEX_ERR;
            }
            else 
            { 
                // Pokud nanastal nějaký jev výše jedná se o konec
                ungetc(c,input);
                token.type = T_INT;
                token.int_value = 0;
                return 0;
            }
            break;

        // binární zápis čísla
        case S_BIN: 
            if (c == '0' || c == '1') 
            {
                strAddChar(buffer,c);
                state = S_BIN1;
            }
            else  
            {
                ungetc(c,input);
                fprintf(stderr,"After 0b number prefix can be only 0 or 1 [line %d]\n",position_Y);
                return LEX_ERR;
            }
            break;

        // Koncový stav binárního čísla
        case S_BIN1: 
            if (c == '0' || c == '1') 
            {
                strAddChar(buffer,c);
                state = S_BIN1;
            }
            else 
            {
                ungetc (c,input);
                token.type = T_INT;
                token.int_value = strtol(strGetStr(buffer),NULL,2); // Převod binárního čísla na DEC-INT
                return OK;
            }
            break;

        // Octalový zápis čísla
        case S_OCT: 
            if (c >= '0' && c <= '7') 
            {
                strAddChar(buffer,c);
                state = S_OCT;
            }
            else 
            {
                ungetc (c,input);
                token.type = T_INT;
                token.int_value = strtol(strGetStr(buffer),NULL,8);
                return OK;
            }
            break;

        case S_HEX:
            if (isdigit(c) || (c>= 'A' && c<='F') || (c>= 'a' && c<='f')) 
            {
                strAddChar(buffer,c);
                state = S_HEX1;
            }
            else  
            {
                ungetc(c,input);
                fprintf(stderr,"After 0x number prefix can be only 0-9 number or A-F char[line %d]\n",position_Y);
                return LEX_ERR;
            }
            break;

        case S_HEX1:
            if (isdigit(c) || (c>= 'A' && c<='F') || (c>= 'a' && c<='f')) 
            {
                strAddChar(buffer,c);
                state = S_HEX1;
            }
            else 
            {
                ungetc (c,input);
                token.type = T_INT;
                token.int_value = strtol(strGetStr(buffer),NULL,16);
                return OK;
            }
            break;

        case S_EXP:
            if (c == '+' || c == '-') 
            {
                strAddChar(buffer,c);
                state = S_EXP1;
            }
            else if (isdigit(c)) 
            {
                strAddChar(buffer,c);
                state = S_FLOAT;
            }
            else 
            {
                ungetc(c,input);
                fprintf(stderr, "after exponent only digits or '+'/'-' can follow [line %d]\n",position_Y);
                return LEX_ERR;
            }
            break;

        case S_EXP1:
            if (isdigit(c)) 
            {
                strAddChar(buffer,c);
                state = S_FLOAT;
            }
            else 
            {
                ungetc(c,input);
                fprintf(stderr, "after '+' or '-' in exponent only a digits can follow [line %d]\n",position_Y);
                return LEX_ERR;
            }
            break;

        case S_FLOAT:
            if (isdigit(c)) 
            {
                strAddChar(buffer,c);
                state = S_FLOAT;
            }
            else 
            {
                ungetc(c,input);
                token.type = T_FLOAT;
                token.float_value = strtof(strGetStr(buffer),NULL);
                return OK;
            }
            break;

        case S_POINT:
            if (isdigit(c)) 
            {
                strAddChar(buffer,c);
                state = S_POINT1;
            }
            else    
            {
                ungetc(c,input);
                fprintf(stderr, "There must be at least one number after '.' [line %d]\n",position_Y);
                return LEX_ERR;
            }
            break;

        case S_POINT1:
            if (isdigit(c)) 
            {
                strAddChar(buffer,c);
                state = S_POINT1;
            }
            else if(c == 'e' || c == 'E') 
            {
                strAddChar(buffer,c);
                state = S_EXP;
            }
            else 
            {
                ungetc(c,input);
                token.type = T_FLOAT;
                token.float_value = strtof(strGetStr(buffer),NULL);
                return OK;
            }
            break;

        case S_NUMBER:
            if (isdigit(c)) 
            {
                strAddChar(buffer,c);
                state = S_NUMBER;
            }
            else if (c == 'e' || c == 'E') 
            {
                strAddChar(buffer,c);
                state = S_EXP;
            }
            else if (c == '.') 
            {
                strAddChar(buffer,c);
                state = S_POINT;
            }
            else 
            {
                ungetc(c,input);
                token.type = T_INT;
                token.int_value = atoi(strGetStr(buffer));
                return OK;
            }
            break;

        // Stav pro string
        case S_STRING: 
            if (c == '"') 
            { 
                // Pokud mám korektně ukončený string
                token.type = T_STRING;
                token.string_value = strGetStr(buffer);
                return OK;
            }
            else if (c == EOF) 
            { 
                // Pokud jsem dojel až na EOF, tak se jedná o chybu protože jsem neukončil string
                ungetc(c,input);
                fprintf(stderr,"Here is not end of string \" until end of file [line %d]\n",position_Y);
                return LEX_ERR;
            }
            else if(c < 32 && c != EOF) 
            { 
                // Pokud se jedná o netisknutelný znak, je to chyba
                ungetc(c,input);
                fprintf(stderr,"ASCII value in string of actual character is less then 32, write it with escape secvention[line %d]\n",position_Y);
                return LEX_ERR;
            }
            else if (c == '\\') 
            {
                 // Přecházím do escape sekvence
                state = S_ESCAPE;
            }
            else    
            { 
                // Jedná se o standartní znak, který přídám do bufferu aktuálního stringu
                strAddChar(buffer,c);
                state = S_STRING;
            }
            break;

        // Stav pro escape sekvenci
        case S_ESCAPE: 
            if(c == 't') 
            {
                strAddChar(buffer,'\t');
                state = S_STRING;
            }
            else if (c == 'n') {
                strAddChar(buffer,'\n');
                state = S_STRING;
            }
            else if (c == 's') {
                strAddChar(buffer,' ');
                state = S_STRING;
            }
            else if (c == '"') {
                strAddChar(buffer,'"');
                state = S_STRING;
            }
            else if (c == 'x') {
                state = S_ASCII_HEX;
            }
            else if (c =='\\')
            {
                strAddChar(buffer,'\\');
                state = S_STRING;
            }
            else    
            {   // Pokud za \ nenásledovala známá escape sekvence tak se toto ignoruje a znak se standartně vytiskne
                ungetc(c,input);
                state = S_STRING;
            }
            break;

        case S_ASCII_HEX:
            if (isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f') ) 
            {
                slash_buffer[0] = c;
                state = S_ASCII_HEX1;
            }
            else    
            {
                ungetc(c,input);
                fprintf(stderr, "Expecting HEX number [line %d]\n",position_Y);
                return LEX_ERR;
            }
            break;

        case S_ASCII_HEX1:
            if (isdigit(c) || (c>= 'A' && c<= 'F') || (c >= 'a' && c <= 'f')) 
            {
                slash_buffer[1] = c;
                int value = strtol(slash_buffer,NULL,16);
                strAddChar(buffer,value);
                state = S_STRING;
            }
            else  
            {
                ungetc(c,input);
                slash_buffer[1] = slash_buffer[0];
                slash_buffer[0] = '0';
                int value = strtol(slash_buffer,NULL,16);
                strAddChar(buffer,value);
                state = S_STRING;
            }

            break;
        case  S_ID:
            if (isupper(c) || islower(c) || isdigit(c) || c == '_') 
            {
                strAddChar(buffer,c);
                state = S_ID;
            }

            else if (c == '!' || c == '?') {
                strAddChar(buffer,c);
                token.type = T_ID;
                token.string_value = strGetStr(buffer);
                return OK;
            }
            else   
            {
                ungetc(c,input);
                if (strCmpConstStr(buffer, "if") == 0) 
                {
                    token.type = T_IF;
                    return OK;
                }
                else if (strCmpConstStr(buffer, "else") == 0) 
                {
                    token.type = T_ELSE;
                    return OK;
                }
                else if (strCmpConstStr(buffer, "while") == 0) 
                {
                    token.type = T_WHILE;
                    return OK;
                }
                else if (strCmpConstStr(buffer, "def") == 0) 
                {
                    token.type = T_DEF;
                    return OK;
                }
                else if (strCmpConstStr(buffer, "do") == 0) 
                {
                    token.type = T_DO;
                    return OK;
                }
                else if (strCmpConstStr(buffer, "end") == 0) 
                {
                    token.type = T_END;
                    return OK;
                }
                else if (strCmpConstStr(buffer, "nil") == 0) 
                {
                    token.type = T_NIL;
                    return OK;
                }
                else if (strCmpConstStr(buffer, "not") == 0) 
                {
                    token.type = T_NOT;
                    return OK;
                }
                else if (strCmpConstStr(buffer, "then") == 0) 
                {
                    token.type = T_THEN;
                    return OK;
                }
                else  
                {
                    token.type = T_ID;
                    token.string_value = strGetStr(buffer);
                    return OK;
                }
            }
            break;
        }
    }
    return LEX_ERR;
    fprintf(stderr,"Lexical error [line %d]",position_Y);
}