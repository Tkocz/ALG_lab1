#include "genlib.h"
#include "simpio.h"

#define ONEVALUE 1
#define TWOVALUE 2

typedef struct nodeT{
	struct nodeT * nodeRight, nodeLeft, nodeMid, nodeParent;
	int leftValue, rightValue;
	int type;
}nodeT;


void treeInsert(nodeT * node, int value){
	if (node->type == ONEVALUE) {
		if (value < node->leftValue){
			node->rightValue = node->leftValue;
			node->leftValue = value;
		}else
			node->rightValue = value;
		node->type = TWOVALUE;
	}else{
		if(value < node->leftValue){
			if(node->nodeLeft == NULL)
				node->nodeLeft = createNode(value, node);
			else
				treeInsert(node->nodeLeft, value);
		}else if(value > node->rightValue){
			if(node->nodeRight == NULL)
				node->nodeRight = createNode(value, node);
			else
				treeInsert(node->nodeRight, value);
		}else{
			if(node->nodeMid == NULL)
				node->nodeMid = createNode(value, node);
			else
				treeInsert(node->nodeMid, value);
		}
	}
}


void printTree(TREE tree){
		
}

nodeT* createNode(int value, nodeT * parent){
	nodeT* newNode = malloc(sizeof(nodeT));
	newNode->leftValue = value;
	newNode->rigthValue = NULL;
	newNode->nodeRight = NULL;
	newNode->nodeMid = NULL;
	newNode->nodeLeft = NULL;
	newNode->type = ONEVALUE;
	newNode->nodeParent = parent;
	return(newNode);
}
		
void main(void){

} 