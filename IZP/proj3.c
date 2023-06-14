/**
 * 
 *    Autor: Tomas Tlapak
 *    login: xtlapa00
 *    
 * 
 * 
 * Kostra programu pro 3. projekt IZP 2017/18
 *
 * Jednoducha shlukova analyza
 * Unweighted pair-group average
 * https://is.muni.cz/th/172767/fi_b/5739129/web/web/usrov.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);
    
    c -> size = 0;
    c -> capacity = cap;
    c -> obj = malloc(sizeof(struct obj_t) * cap);
    
    //Pokud se ukazatel obj == NULL, tak capacity = 0;
    if(c -> obj == NULL)
    {
        c -> capacity = 0;
        fprintf(stderr, "Nedostatek paměti pro alokaci místa.\n");
    }
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    free(c -> obj);
    c ->obj = NULL;
    c -> capacity = 0;
    c -> size = 0;
}

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    if (c -> size >= c -> capacity)
    {
        //funkce vraci ukazatel o velikosti alok. mista
        resize_cluster(c, c -> capacity + CLUSTER_CHUNK);
        //pokud ukazatel nema alok. velikost, tak je NULL
        if (c == NULL)
        {
            fprintf(stderr, "Shluk se nepodaril rozsirit.\n");
        }   
    }
    
    c -> obj[c -> size].id = obj.id;
    c -> obj[c -> size].x = obj.x;
    c -> obj[c -> size].y = obj.y;
    c -> size++;     
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    for(int i = 0; i < c2 -> size; i++)
    {
        append_cluster(c1, c2 -> obj[i]);
    }
    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);
  
    clear_cluster(&carr[idx]);
    for (int i = idx; i < narr - 1; i++)
    {
        carr[i] = carr[i + 1];
    }
    return narr - 1;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);
    
    float obj_distance = sqrt((o1 -> x - o2 -> x) * (o1 ->x - o2 -> x) + (o1 -> y - o2 ->y) * (o1 -> y - o2 -> y));
    return obj_distance;
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c2 != NULL);
    assert(c2->size > 0);

    /*************Metoda nejbližšího souseda (Single linkage)****************/
    //vychozi vzdalenost pro porovnavani
    
/*
    float nejblizsi = obj_distance(&c1 -> obj[0], &c2 -> obj[0]);
    float tmp;
    
    for (int i = 0; i < c1 -> size; i++)
    {
        for (int j = 0; j < c2 -> size; j++)
        {
            if (i != j)
            {
                tmp = obj_distance(&c1 -> obj[i], &c2 -> obj[j]);
                if (tmp < nejblizsi)
                {
                    nejblizsi = tmp;
                }
            }
        }
    }
    return nejblizsi;
*/
   
    /*********************Unweighted pair-group average*********************/

    float soucet = 0;
    int velikost = c1 -> size * c2 -> size;

    for (int i = 0; i < c1->size; i++)
    {
        for (int j = 0; j < c2->size; ++j)
        {   
            float vzdalenost = obj_distance(&c1->obj[i], &c2->obj[j]);
            soucet = soucet + vzdalenost;
        }
    }
    return soucet / velikost;


    /**********Metoda nejvzdálenějšího souseda (Complete linkage)************/

/*
    float nejvzdalenejsi = 0;
    float tmp;
    
    for (int i = 0; i < c1 -> size; i++)
    {
        for (int j = 0; j < c2 -> size; j++)
        {
            tmp = obj_distance(&c1 -> obj[i], &c2 -> obj[j]);
            if (tmp > nejvzdalenejsi)
            {
                nejvzdalenejsi = tmp;
            }
        }
    }
    return nejvzdalenejsi;
*/
}
/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);
    float nejblizsi = cluster_distance(&carr[0], &carr [1]);
    float tmp;
    *c1 = 0;
    *c2 = 1;
    
    for (int i = 0; i < narr; i++)
    {
        for (int j = 0; j < narr; j++)
        {
            if (i != j)
            {
                tmp = cluster_distance(&carr[i], &carr[j]);
                
                if (tmp < nejblizsi)
                {
                    nejblizsi = tmp;
                    *c1 = i;
                    *c2 = j;
                }
            }
        }
    }
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */

void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukalazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);
    
    char line[20];
    int shluky = 0;

    FILE *file;
    if ((file = fopen(filename, "r")) == NULL) 
    {
        fprintf(stderr, "Soubor %s se nepodarilo otevrit!\n", filename);
        shluky = -1;
    }
    
    else
    {
        if(fgets(line, 20, file)== NULL) //nacte prvni radek
        {
            fprintf(stderr, "Soubor %s neobsahuje zadna data.\n ", filename);
            shluky = -1;
        }
        
        else 
        {
            if (sscanf(line, "count=%d", &shluky) < 1)
            {
                fprintf(stderr, "Spatny format dat v souboru %s.\n ", filename);
                shluky = -1;
            }
            
            else
            {   
                struct obj_t obj;
                //cyklus na otestovani souradnic objektu
                for (int i = 0; i < shluky; i++)  //nacteni zbyvajicich radku
                {
                    if(fgets(line, 20, file) != NULL)
                    {
                        sscanf(line, "%d %f %f", &obj.id, &obj.x, &obj.y);
                        if (obj.x >= 0 && obj.x <= 1000 && obj.y >= 0 && obj.y <= 1000)
                        ;
                        else
                        {
                            fprintf(stderr, "Souradnice objektu jsou mimo rozsah.\n ");
                            fclose(file);
                            return -1;
                        }
                    }
                }
                
                *arr = malloc(sizeof(struct cluster_t) * shluky);
                if (arr == NULL) //pokud se pamet nepodari naalokovat vraci -1
                {
                    fprintf(stderr, "Pamet se nepodarilo naalokovat!\n");
                    shluky = -1;
                }
                  
                else
                {   
                    rewind(file); //reset fgets
                    for (int i = 0; i < shluky; i++)  //nacteni zbyvajicich radku
                    { 
                        if (fgets(line, 20, file) != NULL)
                        {
                            sscanf(line, "%d %f %f", &obj.id, &obj.x, &obj.y);
                            init_cluster(&(*arr)[i], 1); //inicializace clusteru
                            append_cluster(&(*arr)[i], obj); //pridani objektu do clusteru 
                        }
                    }
                }   
            }
        }         
    }
    fclose(file);
    return shluky;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    struct cluster_t *clusters;
   
    if (argc == 1 || argc > 3)
    {
        fprintf(stderr, "Neplatny pocet argumentu.\n");
    }
    
    else
    {
        int pocet_shluku;
        if (argc == 2)
        {
            pocet_shluku = 1;
        }
        if (argc == 3)
        {
            pocet_shluku = atof(argv[2]);
            if (pocet_shluku <= 0)
            {
                fprintf(stderr, "Neplatny pocet shluku.\n");
                return -1;
            }
        }
        int shluky = load_clusters(argv[1], &clusters);
        if (shluky != -1)
        {
            int c1, c2;
            while (shluky > pocet_shluku)
            {  
                find_neighbours(clusters, shluky, &c1, &c2);
                merge_clusters(&clusters[c1], &clusters[c2]);
                shluky = remove_cluster(clusters, shluky, c2);
            }
            print_clusters(clusters, shluky);
            for (int i = 0; i < shluky; i++)
            { 
                clear_cluster(&clusters[i]);
            }
            free(clusters);
        }   
    }
    return 0;
}
