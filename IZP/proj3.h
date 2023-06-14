/**
 * Kostra hlavickoveho souboru 3. projekt IZP 2017/18
 * a pro dokumentaci Javadoc.
 * @author Tomas Tlapak xtlapa00
 * @ date 15.12. 2017
 */

/*
* @brief struktura definujici objekt
*/
struct obj_t {
    int id;
    float x;
    float y;
};

/*
* @brief struktura definujici cluster
*/
struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*
* @defgroup Clusters operace s clustery
* @{
*/

/*
* @pre c!= NULL
* @pre cap >= 0
*
* @post clusteru naalokujeme pamet
*
* @param c  ukazatel na cluster pro inicializaci
* @param cap kapacita clusteru
*/
void init_cluster(struct cluster_t *c, int cap);

/*
* @pre c!= NULL
*
* @post dealokujeme pamet clusteru c
*
* @param c ukazatel na cluster pro dealokaci
*/
void clear_cluster(struct cluster_t *c);

extern const int CLUSTER_CHUNK;

/*
* @pre c->size >= 0
* @pre new_cap > 0
*
* @post kapacita clusteru c bude zmenena na new_cap
*
* @param c ukazatel na cluster pro realokaci
* @param new_cap nova kapacita clusteru
* 
* @return realokovany cluster
*/
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/*
* @pre c->size >= 0
*
* @post Na posledni misto clusteru c pridame novy objekt
*
* @param c ukazatel na cluster, ke kteremu pridame objekt
* @param obj objeket, ktery se prida na konec clusteru
*/
void append_cluster(struct cluster_t *c, struct obj_t obj);

/*
* @pre c2->size >= 1
*
* @post cluster c1 bude rozsiren a budou mu pridany objekty z clusteru c2
*
* @param c1 ukazatel na cluster, kam pridame objekty z c2
* @param c2 ukazatel na cluster, z ktereho pridame objekty do c1
*/
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/*
* @}
*/

/*
* @defgroup pole_clusterov operace s polem clusteru
* @{
*/

/*
* @pre narr > 0
* @pre idx < narr
*
* @post odstranime cluster na pozici idx a clustery za pozici idx posuneme o 1 
*
* @param carr ukazatel na pole clusteru
* @param narr pocet clusteru v poli
* @param idx cluster, ktery chceme odstranit z pole clusteru
* 
* @return novy pocet clusteru v poli
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx);

/*
* @pre o1!= NULL && o2!= NULL
*
* @param o1 ukazatel na objekt 1
* @param o2 ukazatel na objekt 2
*
* @return vzdalenost mezi objekt 1 a objekt 2
*/
float obj_distance(struct obj_t *o1, struct obj_t *o2);

/*
* @pre c1 != NULL && c1->size > 0
* @pre c2 != NULL && c2->size > 0
*
* @param c1 ukazatel na cluster 1
* @param c2 ukazatel na cluster 2
* 
* @return vzdalenost mezi cluster 1 a cluster 2
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/*
* @pre narr > 0
* @pre carr != NULL
*
* @post do c1 a c2 priradime pozici clusteru v poli clusteru
*
* @param carr ukazatel na pole clusteru
* @param narr pocet clusteru v poli
* @param c1 ukazatel na pozici clusteru 1
* @param c2 ukazatel na pozici clusteru 2
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/*
* @pre c!= NULL
*
* @post seradime vzestupne objekty v clusteru podle id
*
* @param c ukazatel na cluster
*/
void sort_cluster(struct cluster_t *c);

/*
* @post tisk clusteru na stdout
*
* @param c ukazatel na cluster*/
void print_cluster(struct cluster_t *c);

/*
* @pre fopen (filename, "r") != NULL
*
* @post pro kazdy objekt ze souboru file name vytvorime cluster a vlozime do nej objekt
*
* @filename ukazatel na soubor, ze ktereho nacteme objekty
* @arr ukazatel na ukazatel na pole clusteru
* 
* @return pocet clusteru
*/
int load_clusters(char *filename, struct cluster_t **arr);

/*
* @post tisk vsech clusteru z pole clusteru na stdout
*
* @param carr polu clusteru
* @param narr pocet clusteru v poli
*/
void print_clusters(struct cluster_t *carr, int narr);

/*
* @}
*/
