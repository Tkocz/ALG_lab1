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

  Insert new leaf in appropriate place.  Repeat until all nonleaf nodes have 2 or 3 children    * if there is a node with 4 children, split the parent into two parent      nodes, with 2 children each.    * if split the root, then add a new root.  Adjust search values along insertion path.



*/

#define ONEVALUE 1
#define TWOVALUE 2

typedef struct nodeT{
    struct nodeT * nodeRight, nodeLeft, nodeMid, nodeParent;
    int leftValue, rightValue;
    int type;
}nodeT;


void treeInsert(nodeT * node, int value){
    if (node->type == ONEVALUE) {
        if (value < node->leftValue) {
            node->rightValue = node->leftValue;
            node->leftValue = value;
        } else
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