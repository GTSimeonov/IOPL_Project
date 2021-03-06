#ifndef GMAP_H
#define GMAP_H GMAP_H
#include <stdlib.h>
#include <stdio.h>
#include <stack.h>
#include <queue.h>
//#include"../Queue/unique_queue.c"

typedef struct Map Map;



/*
 * 	A looples automata with things attached to the nodes
 * */
struct Map
{
	char is_final;
	Map *delta[256];
	/*ID cannot point to itself ( this is used in the deletion of the map ) */
	void *ID; 
};

void Map_Init(Map *tree);
void Map_Scour(Map *tree,void *str,size_t size,size_t *where,Map **final_node);
void Map_Push(Map *tree,void *str,size_t size,void *id);
void* Map_Check(Map *tree, void *str,size_t size);
struct Map* Map_Check_And_Get(Map *tree, void *str,size_t size);
void Map_Remove(Map *tree, void *str,size_t size);
void Map_Map(Map *tree,void (*map)(void*));
void Map_Map_Extended(Map *tree,void (*map)(void*,void*),void* pass_data);

void Map_Destroy(Map *tree);
void Map_Delete_Map(struct Map *tree);
struct Condensed_Map* Map_Condense(Map* tree);

struct Map* Map_Push_And_Get(struct Map* tree,void *str,size_t size,void *id);
/*returns NULL if id is not taken , returns pointer to taken id otherwise*/
void* Map_Check_And_Push(struct Map *tree,void *str,size_t size,void *id);

#endif
