/******** scanner.h *********
* Implementace překladače imperativního jazyka IFJ18.
* FIT VUT, Tým 087, varianta I
* Autori:   xvlach18    Martin Vlach
*           xhekrd01    Lukas Hekrdla
*           xtlapa00    Tomas Tlapak
* Shrnutí:
*/

#include"str.h"
#include<stdio.h>

// Funkce pro inicializaci scanneru
int set_Scanner(); 

// Funkce pro korektní zavření scanneru a závislostí
int close_Scanner(); 

// Funkce která mi načte token ze stdin, v případě chyby se vrací LEX_ERROR v případě korektního tokenu se vrací OK
// Funkce přidění token type (viz níže), a popřípadně pro T_ID, T_INT, T_FLOAT, T_STRING mi taky do struktury uloží hodnotu 
// T_ID = token.string_value
// T_INT = token.int_value
// T_FLOAT = token_float_value 
// T_STRING = token.string_value
int next_Token(); 

// Pozice v řádku 
int position_X;

// Číslo řádku
int position_Y;

// Pomocný buffer pro ukládání během procesu načítání tokenu
string buffer_tmp;
string *buffer;

// Vstupní soubor
FILE *input;

// Pomocný buffer pro hexa_escape sekvenci ve stringu 
char slash_buffer[2];

// Typy tokenu
typedef enum {
 // Obecné
        T_ERROR, 
        T_EOL,
        T_EOF,
        T_ID,
 // Datové typy
        T_INT,
        T_FLOAT,
        T_STRING,
// Klíčová slova
        T_IF,
        T_ELSE,
        T_WHILE,
        T_THEN,
        T_DEF,
        T_DO,
        T_END,
        T_NIL,
        T_NOT,  
// Operátory
        T_MINUS,
        T_ADD,
        T_MULTIPLY,
        T_DIV,
        T_LESS,
        T_MORE,
        T_ASSIGMENT,  
// SLožené operátory
        T_EQUAL,
        T_NOT_EQUAL,
        T_LESS_EQUAL,
        T_MORE_EQUAL,
// Závorky
        T_RIGHT_BRACKET,
        T_LEFT_BRACKET,        
// Čárka
        T_COMMA
} Token_type;

// Struktura TOKENU
// Typ reprezentován pomocí enumu
// Datová složka reprezenzována pomocí unie
struct {
    Token_type type;
    union {
        int int_value;
        double float_value;
        char *string_value;
    };
} token;