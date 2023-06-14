/*
 *      AUTOR: Tomas Tlapak (xtlapa00)
 *      
 *      
 * Program slouzi k vyhledavani konkretnich adres zadanych na vstupu (stdin).
 * Uzivatel pri spousteni programu urcuje argument, podle ktereho se budou
 * prohledavat mesta. Bude-li argumentu odpovidat pouze 1 mesto, vypise 
 * na vystup. Pokud se s hodnotou argumentu shoduje vice mest, vypise se 
 * napoveda obsahujici znaky pro dalsi hodontu argumentu pro upresneni hledani.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define RADKY 42
#define SLOUPCE 101

//prevedeni adres na vstupu na velka pismena a odstraneni \n po fgets
void prevod_pismen(int pocet_mest, char adresy[RADKY][SLOUPCE])
{
    for (int i = 0; i <= pocet_mest; i++)
    {
        for (int j = 0; j < SLOUPCE; j++)
        {
            adresy[i][j] = toupper(adresy[i][j]);
        }
    }
    
    for (int i = 0; i < pocet_mest; ++i) 
    {
        for (int j = 0; j < SLOUPCE; ++j) 
        {
            if (adresy[i][j]=='\n')
            {
                adresy[i][j]='\0';
            } 
        }
    }
}

// funkce na prevedeni argumentu na velka pismena
void prevod_argumentu(char* argument, int delka_argumentu)
{
    for (int i = 0; i < delka_argumentu; i++)
    {
        argument[i] = toupper(argument[i]);
    }
}

//funkce na porovnani argumentu se zaznamem v poli
void argument_porovnani(char* nalezene_znaky, char* argument, int pocet_mest, char adresy[RADKY][SLOUPCE], int delka_argumentu, int *pocet_shod, int* current)
{
    for (int i = 0; i <= pocet_mest; i++)
    {   
        if(strncmp(argument, adresy[i], delka_argumentu) == 0)
        {
            nalezene_znaky[*pocet_shod] = adresy[i][delka_argumentu]; 
            (*pocet_shod)++; 
            *current = i;
        }
    }
}

// funkce pro nacteni prvnich pismen vsech mest na vstupu
void nacteni_1pismen(int pocet_mest, char *nalezene_znaky, char adresy[RADKY][SLOUPCE])
{   
    for (int i = 0; i <= pocet_mest; i++)
    {   
        nalezene_znaky[i] = adresy[i][0];
    }
}

// funkce pro serazeni pismen v poli abecedne
void serazeni_pismen(char *nalezene_znaky, int pocet_znaku)
{
    char schranka; // pomocna promenna pro prehozeni hodnot 2 jinych promennych
    for (int i = 0; i < pocet_znaku; i++) 
    {
        for (int j = i + 1; j < pocet_znaku; j++) 
        {
            if (nalezene_znaky[i] > nalezene_znaky[j]) 
            {
                schranka = nalezene_znaky[i];
                nalezene_znaky[i] = nalezene_znaky[j];
                nalezene_znaky[j] = schranka;
            }
        }
    }
}

// funkce na smazani duplicitnich znaku v poli
void duplicitni_znaky(char *nalezene_znaky, int pocet_znaku)
{
    int current = 1;
    int i;
    for (int k = 1; k < pocet_znaku; k++)
    {
        for (i = k-1; i < k; i++)
        {
            if(nalezene_znaky[k] == nalezene_znaky[i])
            {
                break;
            }     
        }
             
        if (i == k)
        {
            nalezene_znaky[current] = nalezene_znaky[i];
            current++;
        }
    }
    nalezene_znaky[current] = 0; //poslednim znakem retezce bude \0
}

int main(int argc, char* argv[])
{
    int pocet_mest = 0;
    char adresy [RADKY][SLOUPCE];  
    while((fgets(adresy[pocet_mest], SLOUPCE, stdin)!= NULL))
    {
        pocet_mest++;
    }
    
    // pokud neni na vstupu zadny text
    if(pocet_mest == 0)
    {
        fprintf(stderr, "Soubour adresy.txt je prázný.\n");
        return -1;        
    }
    
    //pokud bude pocet mest > 42, program se ukonci s chybovou hlaskou
    if (pocet_mest > 42)
    {
        fprintf(stderr, "Prilist mnoho zaznamu v souboru."
               " Maximalni pocet zaznamu je 42.\n");
        return -1;
    }
   
    //pokud zadame vice nez 2 argumenty, program se ukonci s chybovou hlaskou
     if (argc > 2)
    {
        fprintf(stderr, "Neplatny argument.\n");
        return -1;
    }
    
    //zavolani funke na prevedni znaku a smazani \n
    prevod_pismen(pocet_mest, adresy);
    
    /*pokud adresy.txt obsahuje pouze 1 zaznam a program spoustime bez 
      argumentu, tak se tento zaznam vytiskne a program ukonci*/
    if (argc == 1 && pocet_mest == 1)
    {
        fprintf(stdout, "Found: %s\n", adresy[0]);
        return 0;
    }
   
    //pokud spustime program s argumentem, ale na vstupu je pouze 1 adresa
    if (argc == 2 && pocet_mest == 1)
    { 
        char* argument = argv[1];
        int delka_argumentu = strlen(argument);
        prevod_argumentu(argument, delka_argumentu);
        //pokud se zadany argument shoduje s adresou
        if (strncmp(argument, adresy[0], delka_argumentu) == 0)
        {
            fprintf(stdout, "Fonud: %s\n", adresy[0]);
        }
        else
        {
            fprintf(stdout, "Not found\n");
        }
        return 0;
    }
    
    //kdyz program spoustime bez zadaneho argumentu s vice zaznamy
    if (argc == 1 && pocet_mest > 1)
    {   
        char nalezene_znaky[pocet_mest];
        nacteni_1pismen(pocet_mest, nalezene_znaky, adresy);
        
        int pocet_znaku = strlen(nalezene_znaky);
        serazeni_pismen(nalezene_znaky, pocet_znaku);
        duplicitni_znaky(nalezene_znaky, pocet_znaku);
        printf("Enable: %s\n", nalezene_znaky);
        return(0);
    }
    
    //kdyz program spustime s argumentem a pocet mest je > 1
    if (argc == 2 && pocet_mest > 1)
    {
        char* argument = argv[1];  
        int delka_argumentu = strlen(argument);
        prevod_argumentu(argument, delka_argumentu);
        
        //hledani shody v adresach se zadanym argumentem
        int pocet_shod = 0;
        int current = 0;
        char nalezene_znaky[pocet_shod];
        argument_porovnani(nalezene_znaky, argument, pocet_mest, adresy, delka_argumentu, &pocet_shod, &current);
       
        if (pocet_shod == 0)
        {
            fprintf(stdout, "Not found\n");
            return 0;
        }
        
        if (pocet_shod == 1)
        {
            fprintf(stdout, "Found: %s\n", adresy[current]);
            return 0;
        }
        
        //pokud najdeme vice shod s argumentem, vypiseme je abecedne
        //serazene na vystup
        if (pocet_shod > 1)
        {
            int pocet_znaku = strlen(nalezene_znaky);
            serazeni_pismen(nalezene_znaky, pocet_znaku);
            duplicitni_znaky(nalezene_znaky, pocet_znaku);  
            fprintf(stdout, "Enable: %s\n", nalezene_znaky);
            return 0;
        }    
    }
    
    return 0;
}
