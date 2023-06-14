/******** stack.c *********
* Implementace překladače imperativního jazyka IFJ18.
* FIT VUT, Tým 087, varianta I
* Autori:   xvlach18    Martin Vlach
*           xhekrd01    Lukas Hekrdla
*           xtlapa00    Tomas Tlapak
* Shrnutí: Soubor s funkcemi pro práci nad zásobníkem využíván jako lokální tabulka symbolů
*/

#include "stack.h"
#include <string.h>

Tstack *stack_Init()
{
    Tstack *stack;
    stack = malloc(sizeof(Tstack));

    if (stack == NULL)
    {
        fprintf(stderr, "Internal error - stack init error\n");
        return NULL;
    }
    stack->stack_Alloc_Size = 0;
    stack->elements_Count = 0;
    stack->data = NULL;
    //stack->data = realloc(stack->data, (sizeof(void *) * (stack->elements_Count + STACK_REALLOCSIZE)));
    return stack;
}

int Stack_Elements_Count(Tstack *stack)
{
    return stack->elements_Count;
}

bool stack_is_Empty(Tstack *stack)
{
    if (stack->elements_Count == 0)
    {    
        return true;
    }    
    else
    {    
        return false;
    }
}

Tstack_Operations_Result stack_Pop(Tstack *stack)
{
    if (stack->elements_Count != 0)
    {
        stack->elements_Count--;
        return STACK_OK;
    }
    else
    {
        fprintf(stderr, "INTERNAL ERROR - Pop of EMPTY stack\n");
        return ERROR_POP_EMPTY;
    }
}

void *stack_Top(Tstack *stack)
{
    if (stack->elements_Count != 0)
    {
        unsigned index = (stack->elements_Count);
        return stack->data[index];
    }
    else
    {
        return NULL;
    }
}

Tstack_Operations_Result stack_Push(Tstack *stack, void *element)
{
    if (stack == NULL)
    {
        return ERROR_NULL_POINTER;
    }

    if (stack->stack_Alloc_Size < (stack->elements_Count + 1))
    {
        stack->data = realloc(stack->data, (sizeof(void *) * (stack->elements_Count + STACK_REALLOCSIZE)));

        if (stack->data == NULL)
        {
            fprintf(stderr, "Realloc error in stack \n");
            free(stack->data);
            free(stack);
            return ERROR_ALLOCATION;
        }
        stack->stack_Alloc_Size = stack->stack_Alloc_Size + STACK_REALLOCSIZE;
    }
    stack->elements_Count++;
    stack->data[stack->elements_Count] = element;
    return STACK_OK;
}

bool stack_alreadyIN(Tstack *stack, char *name)
{
    for (int i = 0; i <= stack->elements_Count; i++)
    {
        if (i != 0)
        {
            if ((strcmp(name, ((TVariable *)(stack->data[i]))->variableName)) == 0)
            {
                return true;
            }
        }
    }
    return false;
}

void variableDefinedTrue(Tstack *stack, char *name)
{
    for (int i = 0; i <= stack->elements_Count; i++)
    {
        if (i != 0)
        {
            if ((strcmp(name, ((TVariable *)(stack->data[i]))->variableName)) == 0)
            {
                ((TVariable *)(stack->data[i]))->defined = true;
                break;
            }
        }
    }
}

char *VariablesAreYouDefined(Tstack *stack)
{
    for (int i = 0; i <= stack->elements_Count; i++)
    {
        if (i != 0)
        {
            if (((TVariable *)(stack->data[i]))->defined == false)
            {
                return ((TVariable *)(stack->data[i]))->variableName;
                break;
            }
        }
    }
    return NULL;
}

bool isVarDefined(Tstack *stack, char *name)
{
    for (int i = 0; i <= stack->elements_Count; i++)
    {
        if (i != 0)
        {
            if ((strcmp(name, ((TVariable *)(stack->data[i]))->variableName)) == 0)
            {
                if (((TVariable *)(stack->data[i]))->defined == true)
                {    
                    return true;
                }
                else
                {    
                    return false;
                }
            }
        }
    }
    return false;
}

void stack_Free(Tstack *stack)
{
    if (stack != NULL)
    {
        free(stack->data);
        free(stack);
    }
}

void stackInherid(Tstack *parent, Tstack *child)
{
    int counter = parent->elements_Count;
    TVariable *tmp = malloc(sizeof(TVariable));
    for (int i = 1; i <= counter; i++)
    {
        tmp->variableName = malloc(strlen(((TVariable *)(parent->data[i]))->variableName) + 1);
        strcpy(tmp->variableName, (((TVariable *)(parent->data[i]))->variableName));
        tmp->defined = true;
        stack_Push(child, (TVariable *)tmp);
    }
}

char *returnVarName(Tstack *stack, char *name)
{
    for (int i = 0; i <= stack->elements_Count; i++)
    {
        if (i != 0)
        {
            if ((strcmp(name, ((TVariable *)(stack->data[i]))->variableName)) == 0)
            {
                return ((TVariable *)(stack->data[i]))->variableName;
                break;
            }
        }
    }
    return NULL;
}