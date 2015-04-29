#include "genlib.h"
#include "simpio.h"

/*

http://en.wikipedia.org/wiki/2%E2%80%933_tree

Insertion works by searching for the proper location of the key and adds it
there. If the node becomes a 4-node then the node is split from two 2-nodes and
the middle key is moved up to the parent. The diagram illustrates the process.

http://upload.wikimedia.org/wikipedia/commons/thumb/4/44/2-3_insertion.svg/581px-2-3_insertion.svg.png


---


http://db.cs.duke.edu/courses/cps100e/spring99/lects/sect1623treeH.pdf

Insertions in 2-3 trees

  Insert new leaf in appropriate place.

  Repeat until all nonleaf nodes have 2 or 3 children
    * if there is a node with 4 children, split the parent into two parent
      nodes, with 2 children each.
    * if split the root, then add a new root.

  Adjust search values along insertion path.



http://penguin.ewu.edu/cscd320/Topic/B-Tree/2-3_Build.html
pseudocode for insertion in a 2-3 tree.

*/

#define ONEVALUE 1
#define TWOVALUE 2

typedef struct nodeT {
    int type;

    int leftValue,
		rightValue;

    struct nodeT *leftChild,
		         *midChild,
				 *rightChild,
				 *parent;
} nodeT;

bool checkParent(nodeT *node){
	return(node->parent != NULL);
}

nodeT* createNode(int value, nodeT * leftChild, nodeT * rightChild) {
    nodeT* newNode = (nodeT*)malloc(sizeof(nodeT));

    newNode->leftValue  = value;
    newNode->rightValue = NULL;
    newNode->rightChild = rightChild;
    newNode->midChild   = NULL;
    newNode->leftChild  = leftChild;
    newNode->type       = ONEVALUE;
    newNode->parent     = NULL;

    return (newNode);
}

bool hasNoChildren(nodeT* node) {
	return ((node->leftChild == NULL) && (node->midChild == NULL) && (node->rightChild == NULL));

}

void treeInsert(nodeT * node, int value){
	int temp;
	nodeT* newNode;

    if (node->type == ONEVALUE && hasNoChildren(node)) {
        if (value < node->leftValue) {
            node->rightValue = node->leftValue;
            node->leftValue = value;
        } else
            node->rightValue = value;
        node->type = TWOVALUE;
    }else{
        if(value < node->leftValue){
            if(node->leftChild == NULL){
				temp = node->leftValue;
				node->leftValue = value;
				if(checkParent(node))
					treeInsert(node->parent, temp);
				else{
					newNode = createNode(node->leftValue, node->leftChild, node->midChild);
					node->leftChild = NULL;
					node->leftValue = node->rightValue;
					node->type = ONEVALUE;
					node->parent = createNode(temp , node , newNode);
					newNode->parent = node->parent;
					printf("a");
				}
			}else
				treeInsert(node->leftChild, value);
        }else if((node->type == ONEVALUE && node->type > node->leftValue) || value > node->rightValue){
			if(node->rightChild == NULL){
				temp = node->rightValue;
				node->rightValue = value;
				if(checkParent(node))
					treeInsert(node->parent, temp);
				else{
					newNode = createNode(node->rightValue, node->rightChild, node->midChild);
					node->rightChild = NULL;
					node->rightValue = node->leftValue;
					node->type = ONEVALUE;
					node->parent = createNode(temp , node , newNode);
					newNode->parent = node->parent;
					printf("b");
				}
			}else
				treeInsert(node->rightChild, value);
        }else{
			//KOLLA PÅ DETTA NERANFÖR FÖR HAR INGEN ANING OM DET ÄR RÄTT ÄNNU!!!!
            if(node->midChild == NULL) {
				node->midChild = createNode(value, NULL, NULL);
				node->midChild->parent = node;
			}
			else
				treeInsert(node->midChild, value);
			//TYP HIT!!!!
        }
    }
}


void printTree(nodeT* node){
	if (node->type == ONEVALUE)
		printf("(Node %d ", node->leftValue);
	else
		printf("(Node %d %d ", node->leftValue, node->rightValue);

	if (node->leftChild)
		printTree(node->leftChild);
	else
		printf("() ");

	if (node->midChild)
		printTree(node->midChild);
	else
		printf("() ");

	if (node->rightChild)
		printTree(node->rightChild);
	else
		printf("() ");

	printf(") ");
}



        
void main(void){
	nodeT *node = createNode(5, NULL, NULL);

	printf("\n\n");
	printTree(node);
	printf("\n\n");

	treeInsert(node, 7);

	printf("\n\n");
	printTree(node);
	printf("\n\n");

	treeInsert(node, 9);

	while (TRUE) {
		if (node->parent == NULL)
			break;

		node = node->parent;
	}

	printf("\n\n");
	printTree(node);
	printf("\n\n");

	treeInsert(node, 10);

	while (TRUE) {
		if (node->parent == NULL)
			break;

		node = node->parent;
	}

	printf("\n\n");
	printTree(node);
	printf("\n\n");

	treeInsert(node, 11);

	while (TRUE) {
		if (node->parent == NULL)
			break;

		node = node->parent;
	}

	printf("\n\n");
	printTree(node);
	printf("\n\n");

	system("pause");
} 