#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#include "hashmap.h"


typedef struct HashMap HashMap;
int enlarge_called=0;

char * eliminarEspacios(char*);

struct HashMap {
    Pair ** buckets;
    long size; //cantidad de datos/pairs en la tabla
    long capacity; //capacidad de la tabla
    long current; //indice del ultimo dato accedido
};

Pair * createPair( char * key,  void * value) {
    key = eliminarEspacios(key);
    Pair * new = (Pair *)malloc(sizeof(Pair));
    new->key = key;
    new->value = value;
    return new;
}

long hash( char * key, long capacity) {
    key = eliminarEspacios(key);
    unsigned long hash = 0;
     char * ptr;
    for (ptr = key; *ptr != '\0'; ptr++) {
        hash += hash*32 + tolower(*ptr);
    }
    return hash%capacity;
}

int is_equal(void* key1, void* key2){
    if(key1==NULL || key2==NULL) return 0;
    if(strcmp((char*)key1,(char*)key2) == 0) return 1;
    return 0;
}


void insertMap(HashMap * map, char * key, void * value) {
    key = eliminarEspacios(key);
    long posicion = hash(key,map->capacity);

    Pair *aux = createPair(key,value);

    while (map->buckets[posicion] != NULL && map->buckets[posicion]->key != NULL){
        if (is_equal(key,eliminarEspacios(map->buckets[posicion]->key)) == 1) return;
        posicion = (posicion + 1)%(map->capacity);
    }
    map->buckets[posicion] = aux;
    map->size++;

    if ((double)map->size/(double)map->capacity > 0.75) enlarge(map);

}

void enlarge(HashMap * map) {
    enlarge_called = 1; //no borrar (testing purposes)

    Pair **Aux = map->buckets;
    long capacidad = map->capacity;
    map->capacity *= 2;
    map->buckets = (Pair**)malloc(sizeof(Pair*) * map->capacity);
    memset(map->buckets,0,map->capacity * sizeof(Pair*));
    map->size = 0;

    for (int i = 0 ; i < capacidad ; i++){
        if (Aux[i] != NULL && Aux[i]->value != NULL){
            insertMap(map,Aux[i]->key,Aux[i]->value);
        }
        else{
            free(Aux[i]);
        }
    }
    free(Aux);

}


HashMap * createMap(long capacity) {
    HashMap *Mapa = (HashMap*)malloc(sizeof(HashMap));
    Pair **Par = (Pair**)calloc(sizeof(Pair*), capacity);
    Mapa->buckets = Par;
    Mapa->capacity = capacity;
    Mapa->current = -1;
    Mapa->size = 0;
    return Mapa;
}

void eraseMap(HashMap * map,  char * key) {
    Pair *aux = searchMap(map,key);
    if (aux != NULL){
       aux->key = NULL;
       map->size--;
    }
}

Pair * searchMap(HashMap * map,  char * key) {
    key = eliminarEspacios(key);
    long posicion = hash(key,map->capacity);
    while(map->buckets[posicion] != NULL && is_equal((map->buckets[posicion]->key), key) == 0){
        posicion = (posicion + 1) % map->capacity;
    }
    if ((map->buckets[posicion] == NULL) || (map->buckets[posicion]->value) == NULL) return NULL;

    map->current = posicion;

    return map->buckets[posicion];
}

Pair * firstMap(HashMap * map) {

    int posicion = 0;

    while (map->buckets[posicion] == NULL || map->buckets[posicion]->key == NULL){
        posicion = (posicion+1)%(map->capacity);
    }
    map->current = posicion;
    return map->buckets[posicion];
}

Pair * nextMap(HashMap * map) {
    long posicion = (map->current) + 1;

    while (map->buckets[posicion] == NULL || map->buckets[posicion]->key == NULL){
        posicion = (posicion+1)%(map->capacity);
    }
    if (posicion == 0)return NULL;

    map->current = posicion;
    return map->buckets[posicion];

}

//hacer más estable la funcion searchMap
char * eliminarEspacios(char  *cadena){
    char *aux = (char *) calloc(31,sizeof(char));
    int k = 0;
    int h = 0;
    while (*(cadena + k) != '\0'){
        if(*(cadena + k) != ' '){
            *(aux + h) = (char) tolower(*(cadena + k));
            h++;
        }
        k++;
    }
    return aux;
}
