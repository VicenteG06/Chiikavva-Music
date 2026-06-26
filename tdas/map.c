#include "map.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

#define HASH_CAPACITY 857
//Numero primo para evitar al máximo las colisiones
//se obtiene el número 857 con la fórmula: x*0,7=600 (600=canciones en el archivo //csv) (Qué numero, al tener una capacidad del 70%, puede almacenar 600 canciones?)
#define DJB2 5381



typedef struct Node {
    void *data;
    struct Node *next;
} Node;

struct List {
    Node *head;
    Node *tail;
    Node *current;
    int size;
};

struct Map {
    int (*lower_than)(void *key1, void *key2);
    int (*is_equal)(void *key1, void *key2);

    List **ls; //Vector de listas (para que sea un mapa encadenado)
    int current_bucket; //INDICE DEL BUCKET ACTUAL
};

typedef Map Map;
typedef List List;

/*Algoritmo hash usado:  DJB2
Es un algoritmo clasico y rápido, usado especialmente para strings, distribuye muy bien los strings a lo largo de la tabla para evitar las colisiones 
*/
// DJB2 5381
// HASH_CAPACITY 101
unsigned long hash_djb2(char* str){
    unsigned long hash= DJB2;
    int c;

    while( (c= *str++) ){
        hash = ( (hash << 5) + hash) +c;
    } 

    return hash % HASH_CAPACITY;
}



// Variable global para almacenar la función de comparación actual
int (*current_lt)(void *, void *) = NULL;

int pair_lt(void *pair1, void *pair2) {
  return (current_lt(((MapPair *)pair1)->key, ((MapPair *)pair2)->key));
}

//MOVI LIST_SORTED INSERT A LIST.C

Map *sorted_map_create(int (*lower_than)(void *key1, void *key2)) {
  Map *newMap = (Map *)malloc(sizeof(Map));
  newMap->lower_than = lower_than; 
  newMap->is_equal = NULL;
    newMap->ls = (List**)malloc(sizeof(List*) * HASH_CAPACITY);
    for(int i = 0; i< HASH_CAPACITY; i++) {
        newMap->ls[i] = list_create();
    }
    newMap->current_bucket = 0;
    return newMap;
}

Map *map_create(int (*is_equal)(void *key1, void *key2)) {
  Map *newMap = (Map *)malloc(sizeof(Map));
  newMap->lower_than = NULL; 
  newMap->is_equal = is_equal;

    newMap->ls = (List**)malloc(sizeof(List*) * HASH_CAPACITY);
    for(int i = 0; i< HASH_CAPACITY; i++) {
        newMap->ls[i] = list_create();
    }
    newMap->current_bucket = 0;
    return newMap;
}


void multimap_insert(Map *map, void *key, void *value) {
  MapPair *pair = (MapPair *)malloc(sizeof(MapPair));
  pair->key = key;
  pair->value = value;

    int index = hash_djb2((char *)key);


  if (map->lower_than) {
    current_lt = map->lower_than;
    list_sortedInsert(map->ls[index], pair, pair_lt);
  } else
    list_pushBack(map->ls[index], pair);
}

void map_insert(Map *map, void *key, void *value) {
  if (map_search(map, key) != NULL) return;
  multimap_insert(map, key, value);
}

int _is_equal(Map *map, MapPair *pair, void *key) {
  return ((map->is_equal && map->is_equal(pair->key, key)) ||
          (map->lower_than && !map->lower_than(pair->key, key) &&
           !map->lower_than(key, pair->key)));
}

MapPair *map_remove(Map *map, void *key) {

    int index = hash_djb2((char*)key);

  for (MapPair *pair = list_first(map->ls[index]); pair != NULL;
       pair = list_next(map->ls[index]))
    if (_is_equal(map, pair, key)) {
      list_popCurrent(map->ls[index]);
      return pair;
    }
  return NULL;
}

MapPair *map_search(Map *map, void *key) {

    int index =hash_djb2((char*)key);

  for (MapPair *pair = list_first(map->ls[index]); pair != NULL;
       pair = list_next(map->ls[index])) {
    if (_is_equal(map, pair, key))
      return pair;
  }
  return NULL;
}

MapPair *map_first(Map *map) {
    for( int i = 0;i<HASH_CAPACITY;i++){
        MapPair *pair= list_first(map->ls[i]);
        if(pair!= NULL){
            map->current_bucket = i;
            return pair;
        }
    }
    return NULL;
}

MapPair *map_next(Map *map) {
    MapPair *pair = list_next(map->ls[map->current_bucket]);

    if(pair!= NULL){
        return pair;
    }

    for(int i=map->current_bucket + 1; i< HASH_CAPACITY;i++){
        pair = list_first(map->ls[i]);
        if(pair!=NULL){
            map->current_bucket = i;
            return pair;
        }

    }
    return NULL;
}

void map_clean(Map *map) {
    for(int i=0; i<HASH_CAPACITY; i++){
        list_clean(map->ls[i]);
    }
    free(map->ls);
}
