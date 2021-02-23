#ifndef GMAP_C
#define GMAP_C GMAP_C
#include "map.h"



/*
 *	ID and residue and all of delta is assigned to NULL
 * */
void Map_Init(Map *tree)
{
	tree->is_final=0;
	for(int i=0;i<256;++i)tree->delta[i] = NULL;
	tree->ID = NULL;
} 

void Map_Scour(Map *tree,void *str,size_t size,size_t *where,Map **final_node)
{
	for(
		 *where=0,*final_node=tree;
		*where<size && final_node[0]->delta[((unsigned char*)str)[ where[0] ]]!=NULL;
		++where[0]
	)
	{
		(*final_node) = (*final_node)->delta[((unsigned char*)str)[*where]];
	}
}



/*
 * tree must not be null	
 * */
void Map_Push(Map *tree,void *str,size_t size,void *id)
{
	size_t temp;
	Map_Scour(tree,str,size,&temp,&tree);	

	if(temp == size)
	{
		assert(tree->ID==NULL);
		tree->ID=id;
		tree->is_final=1;
		return;
	}
	for(temp;temp<size;++temp)
	{
		Map_Init(
			tree=tree->delta[((unsigned char*)str)[temp]]=malloc(sizeof(Map))
			);
	}

	tree->ID=id;
	tree->is_final=1;

}


/*
 * 	scours the tree and returns the id of the node that recognises the str
 * 	returns NULL if the string is not recognised
 * */
void* Map_Check(Map *tree, void *str,size_t size)
{
	size_t temp;
	Map_Scour(tree,str,size,&temp,&tree);

	if(temp<size)
	{
		return NULL;	
	}else
	{
		return tree->ID; //this has been set to be the last reached node
	}
}

void Map_Remove(Map *tree, void *str,size_t size)
{
	Stack stk;
	Stack_Init(&stk);
	size_t where;
	char what_to_null=((char*)str)[0];

	Stack_Push(&stk,tree);

	for(where=0;where<size-1 && tree->delta[((unsigned char*)str)[where]]!=NULL;++where)
	{
		tree = tree->delta[((unsigned char*)str)[where]];
		if(tree->is_final==1)
		{
			while(stk.size>0)Stack_Pop(&stk);
			what_to_null=((char*)str)[where+1];
		}
		Stack_Push(&stk,tree);
	}
	if(tree->delta[((unsigned char*)str)[where]] == NULL)return;
	free(tree->delta[((unsigned char*)str)[where]]);
	while(stk.size>1)free(Stack_Pop(&stk));
	tree=(Map*)Stack_Pop(&stk);
	tree->delta[(unsigned char)what_to_null]=NULL;
	
}
/*This function especially requires that the map has no loops*/
void Map_Map(Map *tree,void (*map)(void*))
{
	if(tree->is_final==1)map(tree->ID);
	for(int i=0;i<256;++i)
	{
		if(tree->delta[i]!=NULL)
		{
			Map_Map(tree->delta[i],map);
		}
	}
}



/*this does not destroy(free) any memory pointed to by a node in the Map. This does not free() the root (Map *tree) */
/*This function especially does not require that the map has no loop ( for example after grepification )*/
void Map_Destroy(Map *tree)
{
	Stack path;
	Stack nodes;
	Map *current_node;
	unsigned int i;
	 

	Stack_Init(&path);
	Stack_Init(&nodes);

	Stack_Push(&path,tree);
	Stack_Push(&nodes,tree);

	/*
	   using DFS we fill up the nodes stack with all the used
	   (accessible) nodes.
	 */
	while(path.size>0)
	{
		current_node=Stack_Pop(&path);
		current_node->ID=&(current_node->ID);/*mark the node*/
		for(i=0;i<256;++i)
		{
			if(current_node->delta[i]!=NULL && current_node->delta[i]->ID != &(current_node->delta[i]->ID) )
			{
				Stack_Push(&path,current_node->delta[i]);


				/*we mark the unmarked child of the current_node*/
				current_node->delta[i]->ID=&(current_node->delta[i]->ID);
				/*every node in nodes continues to be marked*/
				Stack_Push(&nodes,current_node->delta[i]);
			}
		}
		
	}
	/*
	   There should not be any duplicates in here
	 */
	while(nodes.size>1)
	{
		current_node=Stack_Pop(&nodes);
		/*Again the things that ID points to is not freed ( this structure is used to map the structure of data )
		  deletion of it is up to you.
		 */
		free(current_node);
	}


}

/*requres that cpy has no loops*/
Map* Map_Copy(Map *cpy)
{
	short i;
	Map *ret;

	if(cpy==NULL)
	{
		return NULL;
	}

	ret=malloc(sizeof(Map));
	ret->is_final=cpy->is_final;
	ret->ID=cpy->ID;

	for(i=0;i<256;++i)
	{
		ret->delta[i]=Map_Copy(cpy->delta[i]);
	}
	return ret;
}

struct Map* Map_Check_And_Get(Map *tree, void *str,size_t size)
{
	size_t temp;
	Map_Scour(tree,str,size,&temp,&tree);

	if(temp<size)
	{
		return NULL;	
	}else
	{
		return tree;
	}
}

struct Map* Map_Push_And_Get(struct Map* tree,void *str,size_t size,void *id)
{
	size_t temp;
	Map_Scour(tree,str,size,&temp,&tree);	

	if(temp == size)
	{
		if(tree->ID!=NULL)tree->ID=id;
		tree->is_final=1;
		return tree;
	}

	for(temp;temp<size;++temp)
	{
		Map_Init(
			tree=
			tree->delta[((unsigned char*)str)[temp]]=
			malloc(sizeof(Map))
			);
	}

	tree->ID=id;
	tree->is_final=1;
	return tree;
}

#endif //#ifndef GMAP
