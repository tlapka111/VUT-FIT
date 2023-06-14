/******** stack.h *********
* Implementace překladače imperativního jazyka IFJ18.
* FIT VUT, Tým 087, varianta I
* Autori:   xvlach18    Martin Vlach
*           xhekrd01    Lukas Hekrdla
*           xtlapa00    Tomas Tlapak
* Shrnutí: headerfile pro práci ze zásobníkem (využívaný jako lokální tabulka symbolů)
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Pro optimalizaci nebudu přealokovávat po 1 prvku, ale po nějaké konstantě např 100
#define STACK_REALLOCSIZE 100

// Návratová hodnota výsledku operace se zásobníkem
// 0 - OK
// 1 - Chyba při (re)alokaci paměti pro data zásobníku
// 2 - Předán neplatný ukazatel na zásobník
// 3 - Pop prázného zásobníku

typedef enum stack_Operations_Result {
    STACK_OK,
    ERROR_ALLOCATION,
    ERROR_NULL_POINTER,
    ERROR_POP_EMPTY,
} Tstack_Operations_Result;

// Struktura zásobníku
// stack_Alloc_Size - pro kolik prvků mám alokované místo
// stack_Elements_Count - kolik prvků mám momentálně na zásovníku
// **data - ukazatel na data zásovníku

typedef struct tstack {
    int stack_Alloc_Size;
    int elements_Count;
    void **data;
} Tstack;


// Struktura promenne
// variableName -> jmeno promenne
// defined -> pravdivostni hodnota jestli je promenna definovana

typedef struct _variable {
    char *variableName;
    bool defined;
} TVariable;

// Funkce mi alokuje místo pro nový zásobník a vrátí mi ukazatel na něj
Tstack *stack_Init();

// Funkce mi vrátí počet položek v zásobníku
int stack_Elements_Count(Tstack *stack);

// Funkce mi vrátí 0 pokud je zásobník prázný, pokud není prázný vrátí mi jedna
bool stack_is_Empty(Tstack *stack);

// Funkce popne ze zásobníku, vrací 0 při ok, 3 pokud je zásobník prázný
Tstack_Operations_Result stack_Pop(Tstack *stack);

// Fuknce mi vrátí ukazatel na hodnotu na vrcholu zásobníku pokud zásobník není prázný, jinak mi vrátí ukazatel NULL
void *stack_Top(Tstack *stack);

// Funkce pushne prvek na zásovník, vrátí OK pokud se povede, nebo chybu pokud se nepovede alokace místa pro nový prvek, nebo ukazazel na zásobník bude nulový
Tstack_Operations_Result stack_Push(Tstack *stack, void *element);

// Funkce korektně uvolní místo zabírané zásobníkem
void stack_Free(Tstack *stack);

// Vrátí mi true pokud je vkládaná položka již v zásobníku
bool stack_alreadyIN(Tstack *stack, char *name);

// Nastaví danné proměnné definovanost na true
void variableDefinedTrue(Tstack *stack, char *name);

//Vrátí mi jméno nedefinované proměnné 
char *VariablesAreYouDefined(Tstack *stack);

// Vrátí mi pravdivostní hodnotu jestli je hledaná proměnná definována
bool isVarDefined(Tstack *stack, char *name);

// Překopírování zásobníků, nakonec nepoužito (původní nepochopení rozsahu platnosti proměnné)
void stackInherid(Tstack *parent, Tstack *child);

// Vrátí mi ukazatel na jméno proměnné 
char *returnVarName(Tstack *stack, char *name);
