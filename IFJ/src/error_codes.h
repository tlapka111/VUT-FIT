/******** error_codes.h *********
* Implementace překladače imperativního jazyka IFJ18.
* FIT VUT, Tým 087, varianta I
* Autori:   xvlach18    Martin Vlach
*           xhekrd01    Lukas Hekrdla
*           xtlapa00    Tomas Tlapak
* Shrnutí: výčet návratových kódu
*/
// Při korektním provedení se vrací nula
#define OK  0
// Chyba během lexikální analýzy
#define LEX_ERR 1
// Chyba během syntaktické analýzy
#define SYNTAX_ERR 2
// Sématická chyba nedefinované funkce/proměnné 
#define SEMATIC_DEFINIG_ERR 3
// Běhová / sématická chyba typové nekompatibility při operacích 
#define SEMATIC_TYPE_ERR    4
// Sématická chyba špatného počtu parametrů
#define SEMATIC_PARAMETRES_ERR  5
// Sématická chyba kategorie ostatní
#define SEMATIC_OTHERS_ERR  6
// Běhová chyba dělení nulou
#define DIV_BY_ZERO_ERR 9
// Interní chyba programu (například chyná alokace paměťi)
#define INTERNAL_ERR    99
