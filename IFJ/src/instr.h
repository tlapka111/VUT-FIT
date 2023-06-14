/******** instr.h *********
* Implementace překladače imperativního jazyka IFJ18.
* FIT VUT, Tým 087, varianta I
* Autori:  z většiny převzato z referenčního interpretu
* Shrnutí: Soubor pro práci se seznamem instrukcí
*/

// ----- Instrukce -------
#define MOVE            0
#define CREATEFRAME     69
#define PUSHFRAME       1
#define POPFRAME        2
#define DEFVAR          3
#define I_CALL          4
#define RETURN          5
#define PUSHS           6
#define POPS            7
#define CLEARS          8
#define ADD             9
#define SUB             10
#define MUL             11
#define DIV             12
#define IDIV            13
#define ADDS            14
#define SUBS            15
#define MULS            16
#define DIVS            17
#define IDIVS           18
#define LT              19
#define GT              20
#define EQ              21
#define LTS             22
#define GTS             23
#define EQS             24
#define AND             25
#define OR              26
#define NOT             27
#define ANDS            28
#define ORS             29
#define NOTS            30
#define INT2FLOAT       31
#define FLOAT2INT       32
#define INT2CHAR        33
#define STRI2INT        34
#define INT2FLOATS      35
#define FLOAT2INTS      36
#define INT2CHARS       37
#define STRI2INTS       38
#define READ            39
#define WRITE           40
#define CONCAT          41
#define STRLEN          42
#define GETCHAR         43
#define SETCHAR         44
#define TYPE            45
#define LABEL           46
#define JUMP            47
#define JUMPIFEQ        48
#define JUMPIFNEQ       49
#define JUMPIFEQS       50
#define JUMPIFNEQS      51
#define EXIT            52
#define BREAK           53
#define DPRINT          54
//--- Startovací instrukce ---- // 
#define START           55
#define MAINBODY 56
//---- Vestavěné funkce ------//
#define BF_LENGTH	57
#define BF_INPUTS 58
#define BF_INPUTI	59
#define BF_INPUTF	60
#define BF_PRINT 	61
#define BF_ORD 62
#define BF_SUBSTR 63
#define BF_CHR 64
// ---Aritmetické/Relatní operace ----//
#define N_ADD 150
#define N_SUB 151
#define N_MUL 152 
#define N_DIV 153
#define N_EQ 154
#define NEQ_EQ 155
#define N_NEQ_EQ 156
#define LESS 157
#define N_LESS 158
#define MORE 159
#define N_MORE 160
#define LESS_EQ 161
#define N_LESS_EQ 162
#define MORE_EQ 163
#define N_MORE_EQ 164

// Struktura pro 3AC
// iType - typ instrukce
// destInstr - cílová adresa
// src1Addr - první operační adresa
// src2Addr - druhá operační adresa

typedef struct {
  int iType;
  void *destInstr;
  void *src1Addr;
  void *src2Addr;
} tInstr;

// Položka seznamu istrukcí
// Obsahuje datavou složku instrukce
// Nadále ukazatek na další položku

typedef struct listItem {
  tInstr Instruction;
  struct listItem *nextItem;
} tListItem;

// Datová struktura seznamu
// first - ukazatel na první položku
// last -	ukazatel na poslední položku 
// active - ukazatel na aktivní položku 

typedef struct {
  struct listItem *first;
  struct listItem *last;
  struct listItem *active;
} tListOfInstr;

// Inicializace seznamu instrukcí 
void instrListInit(tListOfInstr *L);

// Uvolnění místa zabíraného seznamem instukcí
void instrListFree(tListOfInstr *L);

// Vložení za poslední prvek seznamu instrukcí
void insertListInsertLast(tListOfInstr *L, tInstr I);

void instrListFirst(tListOfInstr *L);

void instrListNext(tListOfInstr *L);

void instrListGoto(tListOfInstr *L, void *gotoInstr);

void *instrListGetLast(tListOfInstr *L);

tInstr *instrListGetData(tListOfInstr *L);

// Přidání instrukce za poslední prvek
void addInstruction(int type, void * destination, void * adress1, void * adress2, tListOfInstr *InstructionsList);

// Vložení na aktivní instrukci
void instrInsertafterACT(tListOfInstr *L, tInstr I);

// Přidání instrukce za aktivní prvek
void addInstructionFUN(int type, void * destination, void * adress1, void * adress2, tListOfInstr *InstructionsList);