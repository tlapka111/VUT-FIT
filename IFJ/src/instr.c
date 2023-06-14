/******** instr.c *********
* Implementace překladače imperativního jazyka IFJ18.
* FIT VUT, Tým 087, varianta I
* Autori:  z většiny převzato z referenčního interpretu
* Shrnutí: Soubor pro práci se seznamem instrukcí
*/

#include <stdio.h>
#include<stdlib.h>
#include "instr.h"


void instrListInit(tListOfInstr *L) {
  L->first = NULL;
  L->last = NULL;
  L->active = NULL;
}
  
void instrListFree(tListOfInstr *L) {
  tListItem *ptr;
  while (L->first != NULL)
  {
    ptr = L->first;
    L->first = L->first->nextItem;
    // uvolnime celou polozku
    free(ptr);
  }
}

void instrInsertLast(tListOfInstr *L, tInstr I) {
  tListItem *new;
  new = malloc(sizeof (tListItem));
  new->Instruction = I;
  new->nextItem = NULL;
  if (!(L->first)) {
     L->first = new;
  } else {
     L->last->nextItem = new;
  }
  L->last = new;
}

void instrListFirst(tListOfInstr *L){
// zaktivuje prvni instrukci
  L->active = L->first;
}

void instrListNext(tListOfInstr *L){
// aktivni instrukci se stane nasledujici instrukce
  if (L->active)
  L->active = L->active->nextItem;
}

void instrListGoto(tListOfInstr *L, void *gotoInstr){
// nastavime aktivni instrukci podle zadaneho ukazatele
// POZOR, z hlediska predmetu IAL tato funkce narusuje strukturu
// abstraktniho datoveho typu
  L->active = (tListItem*) gotoInstr;
}

void *instrListGetLast(tListOfInstr *L){
// vrati ukazatel na posledni instrukci
// POZOR, z hlediska predmetu IAL tato funkce narusuje strukturu
// abstraktniho datoveho typu
  return (void*) L->last;
}

// vrati aktivni instrukci
tInstr *instrListGetData(tListOfInstr *L){
  if (!(L->active)) {
    printf("Chyba, zadna instrukce neni aktivni");
    return NULL;
  } else {
      return &(L->active->Instruction);
  }
}

void addInstruction(int type, void * destination, void * adress1, void * adress2, tListOfInstr *InstructionsList){
    tInstr instruct;
    instruct.iType = type;
    instruct.destInstr = destination;
    instruct.src1Addr = adress1;
    instruct.src2Addr = adress2;
    instrInsertLast(InstructionsList,instruct);
}

void addInstructionFUN(int type, void * destination, void * adress1, void * adress2, tListOfInstr *InstructionsList){
    tInstr instruct;
    instruct.iType = type;
    instruct.destInstr = destination;
    instruct.src1Addr = adress1;
    instruct.src2Addr = adress2;
   instrInsertafterACT(InstructionsList,instruct);
}

void instrInsertafterACT(tListOfInstr *L, tInstr I) {
  tListItem *new;
  new = malloc(sizeof (tListItem));
  new->Instruction = I;
  new->nextItem = L->active->nextItem;
  L->active->nextItem = new;
 L->active = L->active->nextItem;
}