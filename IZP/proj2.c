/*
*      autor: Tomas Tlapak (xtlapa00)      
*      program: Iteracni vypocty
*      rok: 2017
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

const char *HELP = "\n***************************************ITERACNI VYPOCTY***************************************"
                   "\n*****************************************Tomas Tlapak*****************************************"
                   "\n*********************************************2017*********************************************\n\n"
                   " Program slousi k porovnavani hodnoty tangens z matematicke\n"
                   " knihovny s hodnotami tanges spocitanymi pomoci Taylorova polynomu\n"
                   " a zretezenych zlomku nebo prpo spocitani vysky a vzdalenosti objektu\n"
                   " vuci mericimu zarizeni.\n\n"
                   " Zpusoby zadani argumentu pro spusteni programu:\n\n"
                   " --help          //vypise napovedu programu\n"
                   " --tan A N M     //program vypocita hodnotu tangens pro uhel alfa a porovna hodnotu s\n"
                   "                 //Taylorovym polynomem a s zretezenymi zlomky po urcitem poctu provedeni cyklu.\n\n"
                   "                    A - uhel alfa zadany v radianech v intervalu (0, 1.4>\n"
                   "                    N - cislo od jake iterace chceme vypisovat vysledky\n"
                   "                    M - cislo kde chceme s iteracemi skoncit\n"
                   "                      - pro N a M plati: 1 <= N <= M <= 13\n\n"
                   " [-c X] -m A [B] //program vypocita delku, pripadne i vysku objektu vuci mericimu zarizeni\n"
                   "                 //hodnoty uvedene v [] jsou volitelne argumenty\n\n"
                   "                 -c X - prepinac nastavujici vysku zarizeni, pro x plati (0 < X <= 100)\n"
                   "                      - bez nastaveni se bere jako vychozi hodnota vyska 1.5\n"
                   "                    A - uhel alfa zadany v radianech, platne hodnoty (0 < A <= 1.4)\n"
                   "                      - hodnota slouzi k vypoctu vzdalenosti objektu vuci zarizeni pod uhlem alfa\n"
                   "                    B - uhel beta zadany v radianech, platne hodnoty (0 < B <= 1.4)\n"
                   "                      - hodnota slouzi k vypoctu vysky objektu vuci zarizeni pod uhlem beta\n";

//Tangens pomoci Taylorova polynomu v okoli 0
double taylor_tan(double x, unsigned int n)
{
    double citatel[13] = {1, 1, 2, 17, 62, 1382, 21844, 929569, 6404582, 443861162, 
                          18888466084, 113927491862, 58870668456604};
    double jmenovatel[13] = {1, 3, 15, 315, 2835, 155925, 6081075, 638512875, 10854718875, 
                             1856156927625, 194896477400625, 49308808782358125, 3698160658676859375};
    double vysledek = x;
    double exp = x * x; 
    double clen = x;
    unsigned int i;
    
    for (i = 1 ; i < n; i++)
    {   
        x = exp * x;
        clen = x * citatel[i]/jmenovatel[i];
        vysledek = vysledek + clen;
    }
   
    return vysledek; 
}

//Tangens pomoci zretezenych zlomku
double cfrac_tan(double x, unsigned int n)
{
    double vysledek = 0;
    double exp = x * x;
    double clen = 2 * n -1;
    unsigned int i;
    
    for (i = n; i > 1; i--)
    {
        vysledek = exp / (clen - vysledek);
        clen = clen - 2;
    }
    return x / (1 - vysledek);
}

// porovnani hodnot tanges v urcitem intervalu
void tangens_vypocet(double *alfa, unsigned int *N, unsigned int *M)
{
    double tangens = tan(*alfa); // tangens z math knihovny
            
    for (unsigned int i = 0; i <= *M; i++)
    {
        if (i >= *N)
        {
            printf("%d %e %e %e %e %e\n", i, tangens, taylor_tan(*alfa, i), tangens - taylor_tan(*alfa, i), cfrac_tan(*alfa, i), tangens - cfrac_tan(*alfa, i));
        }
    }
}

//vypocet vzdalenosti obektu pomoci goniometrickych funkci
void vzdalenost_vypocet(double *alfa, double *vpristroj)
{
    double vzdalenost = *vpristroj / cfrac_tan(*alfa, 10);
    printf("%.10e\n", vzdalenost);
}

//vypocet vysky obektu pomoci goniometrickych funkci
void vyska_vypocet(double *alfa, double *beta, double *vpristroj)
{
    double vzdalenost = *vpristroj / cfrac_tan(*alfa, 10);
    double vyska = (cfrac_tan(*beta, 10) * vzdalenost) + *vpristroj;
    printf("%.10e\n", vyska);
}

//funkce na overeni prevodu argumentu na double
int test_double(char *x)
{
    char *endptr;
    if (((strtod(x, &endptr) == 0 && *endptr != '\0') && (!(strtod(x, &endptr) == 0 && *endptr == '\0'))) ||
        (strtod(x, &endptr) != 0 && *endptr != '\0'))
        return 0; // nelze prevest
    else
        return 1; // lze prevest
}

//funkce na overeni prevodu argumentu na int
int test_int(char *x)
{
    char *endptr;
    if (((strtol(x, &endptr, 0) == 0 && *endptr != '\0') && (!(strtol(x, &endptr, 0) == 0 && *endptr == '\0'))) ||
        (strtol(x, &endptr, 0) != 0 && *endptr != '\0'))
        return 0; // nelze prevest
    else
        return 1; // lze prevest
}

// funkce na urceni stavu programu
void stav_programu(unsigned int *stav,  unsigned int *N, unsigned int *M, double *alfa, double *beta, double *vpristroj, int argc, char *argv[])
{
    //stav 1 - spusteni bez argumentu
    if (argc == 1)
    {
        *stav = 1;
    }
    
    //stav 2 - spusteni s argumentem --help
    else if (argc == 2 && strcmp(argv[1], "--help") == 0)
    {
        *stav = 2;
    }
    
    //stav 3 - spusteni pro vypocet hodnot tangens
    else if (argc == 5 && strcmp(argv[1], "--tan") == 0)
    {
        if(test_double(argv[2]) == 1)
        {
            *alfa = atof(argv[2]);
             //interval stpunu od 0 rad do 1.4 radianu
            if (*alfa <= 0  || *alfa > 1.4)
            {
                fprintf(stderr, "Zadany uhel pro tangens je mimo interval (0, 1.4>\n");
            }
            else
            {
                // hodnoty argumentu 3 a 4 jsou cisla
                if (test_int(argv[3]) == 1 && test_int(argv[4]) == 1)
                {
                    *N = atoi(argv[3]);
                    *M = atoi(argv[4]);
                    
                    if (*N>*M || *N<1 || *M>13)
                    {
                        fprintf(stderr, "Spatne hodnoty pro N nebo M. Musi platit 1 <= N <= M <= 13\n");
                    }
                    else
                    {
                        *stav = 3;
                    }    
                }
                else
                {
                    if (test_int(argv[3]) == 0)
                    {
                        fprintf(stderr, "Argument N neni cele cislo.\n");
                        
                    }
                    if (test_int(argv[4]) == 0)
                    {
                        fprintf(stderr, "Argument M neni cele cislo.\n");
                    }
                }
            }
        }
        else
        {
            fprintf(stderr, "Uhel nelze prevest na cislo.\n");
        }     
    }
    
    //stav 4 - spusteni pro vypocet delky
    else if (argc == 3 && strcmp(argv[1], "-m") == 0)
    { 
        if (test_double(argv[2]) == 1)
        {
            *alfa = atof(argv[2]);
            
            if (*alfa > 0 && *alfa <= 1.4)
            {
                *stav = 4;
            }
            
            else
            {
                fprintf(stderr, "Uhel A neni v intervalu (0 < A <= 1.4).\n");
            }
        }
        
        else
        {
            fprintf(stderr, "Uhel nelze prevest na ciso.\n");
        }
    }
    
    //stav 5 - spusteni pro vypocet delky a vysky zaroven
    else  if (argc == 4 && strcmp(argv[1], "-m") == 0)
    {  
        if (test_double(argv[2]) == 1 && test_double(argv[3]) == 1)
        {
            *alfa = atof(argv[2]);
            *beta = atof(argv[3]);
            
            if (*alfa > 0 && *alfa <= 1.4 && *beta > 0 && *beta <= 1.4)
            {
                *stav = 5;
            }
            
            else
            {
                fprintf(stderr, "Uhel A nebo B neni v intervalu (0 < A(B) <= 1.4).\n");
            }
        }
        
        else
        {
            if (test_double(argv[3]) == 0)
            {
                 fprintf(stderr, "Uhel beta nelze prevest na cislo.\n");
            }
            
            if (test_double(argv[2]) == 0)
            {
                fprintf(stderr, "Uhel alfa nelze prevest na cislo.\n");
            }
        }
    }
    
    //stav 6 - spusteni pro vypocet vysky s vlastni vyskou mericiho pristroje
    else  if (argc == 5 && strcmp(argv[1], "-c") == 0 &&  strcmp(argv[3], "-m") == 0)
    {
        if (test_double(argv[2]) == 1 && test_double(argv[4]) == 1)
        {
            *vpristroj = atof(argv[2]);
            *alfa = atof(argv[4]);
            if (*alfa > 0 && *alfa <= 1.4 && *vpristroj > 0 && *vpristroj <= 100)
            {
                *stav = 6;
            }
            
            else
            {
                if (*alfa <=0 || *alfa > 1.4)
                {
                    fprintf(stderr, "Uhel A nebo  neni v intervalu (0 < A <= 1.4).\n");
                }
              
                if (*vpristroj <=0 || *vpristroj > 100)
                {
                    fprintf(stderr, "Vyska pristroje neni v intervalu (0 < V <= 100).\n");
                }
            }
        }
        else
        {
            if (test_double(argv[2]) == 0)
            {
                 fprintf(stderr, "Vysku pristroje nelze prevest na cislo.\n");
            }
            
            if (test_double(argv[4]) == 0)
            {
                fprintf(stderr, "Uhel alfa nelze prevest na cislo.\n");
            }
        }
    }
    
    //stav 7 - spusteni pro vypocet vysky a delky s vlastni vyskou mericiho pristroje
    else  if (argc == 6 && strcmp(argv[1], "-c") == 0 &&  strcmp(argv[3], "-m") == 0)
    {
        if (test_double(argv[2]) == 1 && test_double(argv[4]) == 1 && test_double(argv[5]) == 1)
        {
            *vpristroj = atof(argv[2]);
            *alfa = atof(argv[4]);
            *beta = atof(argv[5]);
            
            if (*alfa > 0 && *alfa <= 1.4 && *beta > 0 && *beta <= 1.4 && *vpristroj > 0 && *vpristroj <= 100)
            {
                *stav = 7;
            }
            
            else
            { 
                if (*alfa <=0 || *alfa > 1.4 || *beta <=0 || *beta > 1.4)
                {
                    fprintf(stderr, "Uhel A nebo B neni v intervalu (0 < A(B) <= 1.4).\n");
                }
              
                if (*vpristroj <=0 || *vpristroj > 100)
                {  
                    fprintf(stderr, "Vyska pristroje  neni v intervalu (0 < V <= 100).\n");
                }
            }
        }
        else
        {
            if (test_double(argv[2]) == 0)
            {
                fprintf(stderr, "Vysku nelze prevest na cislo.\n");
            }
            
            if (test_double(argv[4]) == 0)
            {
                fprintf(stderr, "Uhel alfa nelze prevest na cislo.\n");
            }
             
            if (test_double(argv[5]) == 0)
            {
                fprintf(stderr, "Uhel beta nelze prevest na cislo.\n");
            }
        }
    }
    
    //- spusteni s nevyhovujicimi argumenty
    else
    {
        fprintf(stderr, "Spatne zadane argumenty, pouzijte napovedu --help\n");
    }
}

int main(int argc, char *argv[])
{
    double alfa, beta; 
    double vpristroj = 1.5; //vychozi vyska pristroje
    unsigned int N , M, stav;
    //vyhodnoceni stavu programu z funkce stav_programu a predani hodnot do promenych
    stav_programu(&stav, &N, &M, &alfa, &beta, &vpristroj, argc, argv); 
    
    if (stav == 1)  
    {
        printf("Program byl spusten bez argumnetu. Pro napovedu spuste program s argumentem --help\n");
    }
        
    if (stav == 2)
    {
        printf("%s", HELP);
    }   
    
    if (stav == 3)
    {
        tangens_vypocet(&alfa, &N, &M);
    }   
    
    //vypocet vzdalenosti s defaultni vyskou 1.5m || s vlastni vyskou
    if (stav == 4 || stav == 6)
    {
        vzdalenost_vypocet(&alfa, &vpristroj);
    }       
    
    //vypocet vzdalenosti a vysky s defaultni vyskou 1.5m || s vlastni vyskou
    if (stav == 5 || stav == 7)
    {
        vzdalenost_vypocet(&alfa, &vpristroj);
        vyska_vypocet(&alfa, &beta, &vpristroj);
    }
    return 0;    
}
