/******** parser.h *********
* Implementace překladače imperativního jazyka IFJ18.
* FIT VUT, Tým 087, varianta I
* Autori:   xvlach18    Martin Vlach
*           xhekrd01    Lukas Hekrdla
*           xtlapa00    Tomas Tlapak
* Shrnutí: headerfile pro parser.c jádro celého programu
*/







// Funkce parse() je jádrem celého programu, místem odkud se spouští rekurzivní sestup založený na LL gramatice
// a následně se do této funkce zpět vracíme, v případě chyby s návratovým kódem první chyby, popřípadně s hodnotou 0
// pokud se ověřila lexikální, syntaktická a sématická správnost programu, začne se následně generovat kód

int parse();

// Výčet netermilálů převedených co funkcí s následný rozklad programu pro ověření syntaktické správnosti
// Více viz LL - gramatika

int PROGRAM();
int PROGRAM_BODY();
int FUNCTION();
int IN_BLOCK();
int VALUE();
int PARAMS();
int PARAMS_NEXT();
int PRECENCE_PARSE();
int STATEMENT();
int FUNCTION_CALL_PARAMS_NEXT();
int FUNCTION_CALL_PARAMS();
int PARAMS_CALL();
int VLOZ_VESTAVENE();
int PRVNI_PRUCHOD();
