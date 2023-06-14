/******** symtable.h *********
* Implementace překladače imperativního jazyka IFJ18.
* FIT VUT, Tým 087, varianta I
* Autori:   xvlach18    Martin Vlach
*           xhekrd01    Lukas Hekrdla
*           xtlapa00    Tomas Tlapak
* Shrnutí: header file pro BST a tabulky symbolů
*/

#include <stdlib.h>
#include <stdio.h>
#include "error_codes.h"
#include <string.h>
#include <stdbool.h>
#include "stack.h"

// Struktura pro uzel BST
// char *Key -- Klíč
// void *data -- Ukazatel na datovou složku
// struct treeNode *right_Children -- Ukazatel na pravého syna
// struct treeNode *left_Children -- Ukazatel na levého syna
typedef struct treeNode {
    char *Key;
    void *data;
    struct treeNode *right_Children;
    struct treeNode *left_Children;
} *TtreeNode;

// Struktura BinarySearchTree
// Ukládám si ukazazel na kořen stromu
typedef struct _binaryTree {
    TtreeNode Root;
} Ttree_pointer;

// Struktura pro jednotlivé funkce
// functionName -> jméno konkrétní funkce
// defined -> pravdivostní hodnota, je funkce již 'lexikálně' definování
// functionParams_cout -> počet formálních parametrů
// *variablesStack -> odkaz na zásobník lokálních proměnných
// (Lokální tabulka symbolů LTS)

typedef struct _function {
    char *functionName;
    bool defined;
    int functionParams_count;
    Tstack *variablesStack;
} TFunction;

// Struktura pro globální tabulku symbolů
// functionsCount -> počet funkcí v FTS
// acc_function -> odkaz na aktuální funkci 
// variablesStack -> Odkaz na zásobník lokálních proměnných pro hlavní tělo funkce

typedef struct _symTable {
    Ttree_pointer functions;
    int functionsCount;
    TFunction *acc_function;
    Tstack *variablesStack;
} TGsymTable;

// Emum chyb při práci s BST
typedef enum tree_Operation_Result {
    TREE_OK,
    TREE_ERROR_ALREADY_EXIST,
    TREE_ERROR_NODE_ALLOCATION,
    TREE_ELEMENT_DOESNT_EXIST,
} Tree_Operation_Result;

// BST Funkce
//Inicializace binarniho vyhledavaciho stromu
void BST_init(Ttree_pointer *tree);
//Pomocna funkce, ktera zrusi vsechny podstromy daneho uzlu
void BST_branches_destroy(TtreeNode tmp);
//Zrusi BST
void BST_tree_destroy(Ttree_pointer *tree);
//Funkce vracejici true, pokud je zadany parametr funkce
bool BST_is_id_function(Ttree_pointer *tree, char *Key);
//Vlozi novy uzel do BST
Tree_Operation_Result BST_node_insert(Ttree_pointer *tree,char *Key,void *data);
//Vrati ukazatel na strukturu funkce
void * BSTreturnVALUE(Ttree_pointer *tree, char *Key);

// Global table funkce
//Inicializace globalni tabulky symbolu
void GT_init(TGsymTable *global_table);
//Vldoži funkci do globalni tabulky symbolu, vrací chyby pokud se již daná položka v GTS nachází, nebo dojde k intertní chybě 
Tree_Operation_Result GT_insertFunction(TGsymTable *global, char *name,int cParams,bool nope);
//Funkce která mi vrátí true, pokud je zadaný identifikátor identifikátorem funkce
bool GT_isFunction(TGsymTable *global, char *name);
// Uvolnění místa zabírané GTS
void GT_dispose(TGsymTable *global);
// Vrátí počet parametrů ve hledané funkci pomocí GTS
int GT_function_f_parametres_cout (TGsymTable *global, char *name);