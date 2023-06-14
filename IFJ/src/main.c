/******** main.c *********
* Implementace překladače imperativního jazyka IFJ18.
* FIT VUT, Tým 087, varianta I
* Autori:   xvlach18    Martin Vlach
*           xhekrd01    Lukas Hekrdla
*           xtlapa00    Tomas Tlapak
* Shrnutí: Výchozí soubor pro zavolání hlavní funkce parse(), otevření a zavření scanneru a návrat chybového kódu
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"parser.h"
#include"scanner.h"
#include"codegen.h"
#include"error_codes.h"

int main()
{
    set_Scanner(); // Otevření (inicializace) scanneru
    unsigned to_Print = parse(); // Proběhne lexikální, syntaktická a sématická kontrola, v případě chyby se vrací návratový kód první chyby, v případě správnosti se generuje kód

    if (to_Print == OK)
    {
        //  Tiskne se v Parseru pokud to bylo OK 
    }
    else if (to_Print == LEX_ERR)
    {
        fprintf(stderr,"Lexical ERROR\n");
    }
    else if (to_Print == SEMATIC_DEFINIG_ERR || to_Print == SEMATIC_OTHERS_ERR || to_Print == SEMATIC_PARAMETRES_ERR || to_Print == SEMATIC_TYPE_ERR)
    {
   		 fprintf(stderr,"Sematics ERROR\n");
    }
   else if (to_Print == INTERNAL_ERR)
    {
        printf("Internal ERROR\n");
    }
    else 
    {
        printf("Syntax ERROR\n");
    }   
    close_Scanner(); // Uvolnení zdrojů pro scanner
    return to_Print; // Navrácení chybového kódu
}
