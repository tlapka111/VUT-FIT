/******** psa_stack.c *********
* Implementace překladače imperativního jazyka IFJ18.
* FIT VUT, Tým 087, varianta I
* Autori:   xvlach18    Martin Vlach
*           xhekrd01    Lukas Hekrdla
*           xtlapa00    Tomas Tlapak
* Shrnutí: soubor pro práci nad zásobníky pro precedenční analýzu
* Zásobník využíváme k aplikování pravidel na redukci terminálů na neterminál 'E'
*/

#include "psa_stack.h"
#include "error_codes.h"


void  PSA_stack_init(TPSA_stack *stack)
{
    stack->top = NULL;
    stack->active = NULL;
}

void PSA_stack_dispose(TPSA_stack *stack) 
{
    while (stack->top != NULL) 
    {
        Tpsa_stack_Element tmp = stack->top;
        stack->top = tmp->prev;
        free(tmp);
    }
    stack->top = NULL;
    stack->active = NULL;
}


void PSA_stack_POP(TPSA_stack *stack) 
{
    Tpsa_stack_Element tmp =  malloc(sizeof(struct psa_stact_Element));
    if (stack->top != NULL) 
    {
        tmp = stack->top;
        stack->top = stack->top->prev;
    }
    free(tmp);
}

void PSA_stack_PUSH(TPSA_stack *stack, enum operator o) 
{
    if(stack->top == NULL) 
    {
        Tpsa_stack_Element tmp = malloc(sizeof(struct psa_stact_Element));
        tmp->prev = NULL;
        tmp->operator = o;
        stack->top = tmp;
    } 
    else 
    {
        Tpsa_stack_Element tmp = malloc(sizeof(struct psa_stact_Element));
        tmp->operator = o;
        tmp->prev = stack->top;
        stack->top = tmp;
    }
}

int PSA_stack_TOP(TPSA_stack *stack) 
{
    if (stack->top != NULL) 
    {
        PSA_active_top(stack);
        while(stack->active != NULL) 
        {
            if (stack->active->operator < 15) 
            {
                return stack->active->operator;
            } 
            else 
            {
                PSA_stack_ACT_PREV(stack);
            }
        }
    } 
    else 
    {
        return INTERNAL_ERR;
    }
    return -1;
}

void PSA_stack_ACT_PREV(TPSA_stack *stack) 
{
    if (stack->active != NULL)
    {    
        stack->active = stack->active->prev;
    }
}

void PSA_active_top(TPSA_stack *stack) 
{
    stack->active = stack->top;
}

int PSA_normal_TOP(TPSA_stack *stack) 
{
    PSA_active_top(stack);
    return stack->active->operator;
}

void PSA_handle(TPSA_stack *stack) 
{
    PSA_active_top(stack);
    if (stack->top != NULL) 
    {
        Tpsa_stack_Element tmp1 = NULL;
        tmp1 = stack->active->prev;
        Tpsa_stack_Element new = malloc(sizeof(struct psa_stact_Element));
        new->operator = HANDLE_START;
        new->prev = tmp1;
        stack->active->prev = new;
    }
}


char *vypis(enum operator operator)  // Ladící funkce pro ověření správnosti redukcí 
{
    switch(operator) {
    case 0:
        return "+";
    case 1:
        return "-";
    case 2:
        return "*";
    case 3:
        return "/";
    case 4:
        return "(";
    case 5:
        return ")";
    case 6:
        return "id";
    case 7:
        return "<";
    case 8:
        return ">";
    case 9:
        return "<=";
    case 10:
        return "<=";
    case 11:
        return "!=";
    case 12:
        return "==";
    case 13:
        return "IFSN";
    case 14:
        return "$";
    case 15:
        return "HANDLE";
    case 16:
        return "E";
    }
    return "ERROR";
}



TexpStack * expStackInit()
{
    TexpStack *stack;
    stack = malloc(sizeof(TexpStack));
    stack->elementsCount = 0;
    stack->data = malloc(sizeof(TexpElem)*70);
    return stack;
}

void expStackPUSH(TexpStack *expStack,TexpElem *prvek)
{
    if (expStack != NULL)
    {
        expStack->elementsCount++;
        expStack->data[expStack->elementsCount] = prvek;
    }
}

void expStackPOP(TexpStack *expStack)
{
    if (expStack != NULL){
       expStack->elementsCount--;
    }
}

TexpElem *expStackTOP(TexpStack *expStack)
{
    if (expStack != NULL)
    {
      return expStack->data[expStack->elementsCount];
    }
    return NULL;
}
