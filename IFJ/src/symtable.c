/******** symtable.c *********
* Implementace překladače imperativního jazyka IFJ18.
* FIT VUT, Tým 087, varianta I
* Autori:   xvlach18    Martin Vlach
*           xhekrd01    Lukas Hekrdla
*           xtlapa00    Tomas Tlapak
* Shrnutí: Soubor se funkcemi nad BST a Globální tabulkou symbolů
*/

#include"symtable.h"

/* ===========  BST FUNKCE ============= */

void BST_init(Ttree_pointer *tree) 
{
    tree->Root = NULL;
}

void BST_branches_destroy(TtreeNode tmp) 
{
    if (tmp->left_Children != NULL) 
    {
        BST_branches_destroy(tmp->left_Children);
    }
    if(tmp->right_Children != NULL) 
    {
        BST_branches_destroy(tmp->right_Children);
    }
    //free(tmp->data); // Ukazatel na paměť kteá je alokována pro datovou složku
    free(tmp);
}

void BST_tree_destroy(Ttree_pointer *tree) 
{
    if (tree->Root != NULL) 
    {
        BST_branches_destroy(tree->Root);
        tree->Root = NULL;
    }
}

Tree_Operation_Result BST_node_insert(Ttree_pointer *tree, char *Key, void *data)
{
    if (tree->Root == NULL)
    {
        tree->Root = malloc(sizeof(struct treeNode));
        if (tree->Root == NULL) 
        {
            return TREE_ERROR_NODE_ALLOCATION;
        }
        tree->Root->Key = malloc(strlen(Key)+1);
        strcpy(tree->Root->Key,Key);
        tree->Root->data = data;
        tree->Root->left_Children = NULL;
        tree->Root->right_Children = NULL;
        return OK;
    }
    else
    {
        int scomp;
        TtreeNode tmp = tree->Root;

        while (tmp != NULL)
        {
            scomp = strcmp(tmp->Key,Key);

            // Obsah obou stringu je stejny
            if(scomp == 0)
            {
                return TREE_ERROR_ALREADY_EXIST; // Funkce jiz je v GTS
            }
            //the first character that does not match has a greater value in ptr1 than in ptr2
            else if (scomp > 0)
            {
                if(tmp->right_Children != NULL)
                {
                    tmp = tmp->right_Children;
                }
                else
                {
                    tmp->right_Children = malloc(sizeof(struct treeNode));
                    if (tmp->right_Children == NULL)
                    {
                        return TREE_ERROR_NODE_ALLOCATION;
                    }
                    tmp->right_Children->left_Children = NULL;
                    tmp->right_Children->right_Children = NULL;
                    tmp->right_Children->Key = malloc(strlen(Key)+1);
                    strcpy(tmp->right_Children->Key,Key);
                    tmp->right_Children->data = data;
                    return OK;
                }
            }
            //the first character that does not match has a lower value in ptr1 than in ptr2
            else if (scomp < 0)
            {
                if (tmp->left_Children != NULL)
                {
                    tmp = tmp->left_Children;
                }
                else
                {
                    tmp->left_Children = malloc(sizeof(struct treeNode));
                    if (tmp->left_Children == NULL)
                    {
                        return TREE_ERROR_NODE_ALLOCATION;
                    }
                    tmp->left_Children->left_Children = NULL;
                    tmp->left_Children->right_Children = NULL;
                    tmp->left_Children->Key = malloc(strlen(Key)+1);
                    strcpy(tmp->left_Children->Key,Key);
                    tmp->left_Children->data = data;
                    return OK;
                }
            }
        }
    }
    return -1;
}

bool BST_is_id_function(Ttree_pointer *tree, char *Key)
{
    if (tree->Root == NULL)
    {
        return false;
    }
    TtreeNode tmp = tree->Root;

    while (tmp != NULL)
    {
        if ((strcmp(tmp->Key, Key)) == 0)
        {
            return true;
        }
        else if ((strcmp(tmp->Key, Key)) < 0)
        {
            tmp = tmp->left_Children;
        }
        else
        {
            tmp = tmp->right_Children;
        }
    }
    return false;
}

void * BSTreturnVALUE(Ttree_pointer *tree, char *Key)
{
    if (tree->Root == NULL)
    {
        return NULL;
    }
    TtreeNode tmp = tree->Root;

    while (tmp != NULL)
    {
        if ((strcmp(tmp->Key, Key)) == 0)
        {
            return tmp->data;
        }
        else if ((strcmp(tmp->Key, Key)) < 0)
        {
            tmp = tmp->left_Children;
        }
        else
        {
            tmp = tmp->right_Children;
        }
    }
    return NULL;
}

/* ===========  Global table  FUNKCE ============= */

void GT_init(TGsymTable *global_table)  
{
    if (global_table != NULL) 
    {
        BST_init(&(global_table->functions));
        global_table->acc_function = NULL;
        global_table->functionsCount = 0;
        global_table->variablesStack = stack_Init();
    }
}

Tree_Operation_Result GT_insertFunction(TGsymTable *global, char *name,int cParams,bool nope) 
{
    if(global != NULL) 
    {
        TFunction *function = malloc(sizeof(TFunction));
        if (function == NULL) 
        {
            return INTERNAL_ERR;
        }
        else 
        {
            // Tady budou polozky funce, respektive jejich init
            function->functionParams_count = cParams;
            function->defined = nope; // Defautně je funkce nedefinovaná (využití dvojprůchodu
            function->functionName = malloc(strlen(name)+1);
            function->variablesStack = stack_Init(); 
            strcpy(function->functionName,name);

            if ((BST_node_insert(&(global->functions),name,function))== TREE_ERROR_ALREADY_EXIST) 
            {
                return TREE_ERROR_ALREADY_EXIST;
            }

            global->functionsCount++;
            global->acc_function = function;
            return OK;
        }
    }
  else
    {
        return INTERNAL_ERR;
    }
}

bool GT_isFunction(TGsymTable *global, char *name) 
{
    bool result;
    result = BST_is_id_function(&(global->functions),name);
    return result;
}

void GT_dispose(TGsymTable *global) 
{
    BST_tree_destroy(&(global->functions));
    free(global->acc_function);
}

int GT_function_f_parametres_cout (TGsymTable *global, char *name) 
{
    TFunction *tmp = BSTreturnVALUE((&global->functions),name);
    if (tmp != NULL) {
        return tmp->functionParams_count;
    }
    return -1;
}
