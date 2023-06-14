/******** psa_stack.h *********
* Implementace překladače imperativního jazyka IFJ18.
* FIT VUT, Tým 087, varianta I
* Autori:   xvlach18    Martin Vlach
*           xhekrd01    Lukas Hekrdla
*           xtlapa00    Tomas Tlapak
* Shrnutí: HEADERFILE - soubor pro práci nad zásobníky pro precedenční analýzu
* Zásobník využíváme k aplikování pravidel na redukci terminálů na neterminál 'E'
*/

#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

// Výčet terminálů PSA reprezentovaných pomocí enumů

enum operator {
    OPERATOR_PLUS,              // +
    OPERATOR_MINUS,             // - 
    OPERATOR_MULTIPLY,          // *
    OPERATOR_DIV,               // /
    OPERATOR_LEFT_BRACKET,      // (
    OPERATOR_RIGHT_BRACKET,     // )
    OPERATOR_ID,                // ID
    OPERATOR_LESS,              // <
    OPERATOR_MORE,              // >
    OPERATOR_LESS_EQUAL,        // <=
    OPERATOR_MORE_EQUAL,        // >=
    OPERATOR_NOT_EQUAL,         // !=
    OPERATOR_EQUAL,             // ==    
    OPERATOR_IFSN,              // int, float, string 
    OPERATOR_END,               // $
    HANDLE_START,               // < E.. 
    NONTERM,                    // E
};

// Výčet datových typů reprezentovaných pomocí enum

enum expElemType
{
    E_ID,
    E_INT,
    E_FLOAT,
    E_STRING,
    E_NIL,
};

// Struktura prvku pro zásobník vyčíslování jednoduchých výrazů

typedef struct expElem
{
    enum expElemType type;
        int int_value;
        double float_value;
        char *string_value;
} TexpElem;

// Struktura pro zásobník vyčíslování jednoduchých výrazů

typedef struct expStack
{
    int elementsCount;
    TexpElem **data;
} TexpStack;

// Struktura pro zásobník preceddenční analýzy
// užívaného k redukci terminálů na neterminál E

typedef struct psa_stact_Element 
{
    enum operator operator;
    struct psa_stact_Element *prev;
} *Tpsa_stack_Element;


// Struktura zásobníku 
typedef struct psa_stack {
    Tpsa_stack_Element top;
    Tpsa_stack_Element active;
} TPSA_stack;

// Inicializace zásobníku precedenční analýzy
void PSA_stack_init(TPSA_stack *stack);

// Uvolnění alokovaného místa zabíraného zásobníkem precedenční analýzy
void PSA_stack_dispose(TPSA_stack *stack);

// Funkce provede operaci POP vrcholu zásobníku
void PSA_stack_POP(TPSA_stack *stack);

// Funkce mi provede PUSH zadaného operandu na zásobník PSA
void PSA_stack_PUSH(TPSA_stack *stack, enum operator o);

// Funkce mi vrátí hodnotu nejvyššího terminálu  na zásobníku PSA
int PSA_stack_TOP(TPSA_stack *stack);

// Funkce mi posune aktivitu na předchozí prvek
void PSA_stack_ACT_PREV(TPSA_stack *stack);

// 
void PSA_active_top(TPSA_stack *stack);

// Funce mi vloží na patřičné místo v zásobníku tz."handle start " víz TABULKA PSA
void PSA_handle(TPSA_stack *stack);

// Funkce mi vrátí hodnotu z vrcholu zásobníku
int PSA_normal_TOP(TPSA_stack *stack);

// Ladící funkce pro výpis zásobníku
void PSA_print_stack(TPSA_stack *stack);

// Inicializace zásobníku pro výpočet jednoduchých výrazů
TexpStack * expStackInit();

//  Vložení hodnoty do zásobníku ( operace PUSH)
void expStackPUSH(TexpStack *expStack,TexpElem *prvek);

// Provede mi operaci POP vrcholu zásobníku 
void expStackPOP(TexpStack *expStack);

// Funkce mi vrátí hodnotu na vrcholu zásobníku 
TexpElem *expStackTOP(TexpStack *expStack);
