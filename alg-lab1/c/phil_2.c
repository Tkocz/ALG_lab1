#include "genlib.h"
#include "simpio.h"

#define TWO_NODE 2
#define THREE_NODE 3

typedef struct nodeT {
	struct nodeT* l_c;
	struct nodeT* m_c;
	struct nodeT* r_c;
	struct nodeT* p;

	int l_v;
	int r_v;

	int t;

} nodeT;

bool noChildren(nodeT* r) {
	return (r->l_c == NULL) && (r->m_c == NULL) && (r->r_c == NULL);
}

nodeT* newNode(int value) {
	nodeT* node = (nodeT*)malloc(sizeof(nodeT));

	node->l_c = NULL;
	node->m_c = NULL;
	node->r_c = NULL;

	node->p = NULL;

	node->l_v = value;
	node->r_v = 0;

	node->t = TWO_NODE;
}

void treeInsert(int value, nodeT* r) {
	if (r->t == TWO_NODE && noChildren(r)) {
		if (value < r->l_v) {
			r->r_v = r->l_v;
			r->l_v = value;
		}
		else {
			r->r_v = value;
		}

		r->t = THREE_NODE;

		return;
	}

	if (r->t == TWO_NODE) {
		if (value < r->l_v)
			treeInsert(value, r->l_c);
		else
			treeInsert(value, r->m_c);

		return;
	}

	if (r->t == THREE_NODE) {
		if (value < r->l_v) {
			if (r->l_c == NULL) {
				nodeT* l_c = newNode(value);
				nodeT* m_c = newNode(r->r_v);

				l_c->p = r;
				m_c->p = r;

				r->t = TWO_NODE;

				r->l_c = l_c;
				r->m_c = m_c;
			}
			else {
				treeInsert(value, r->l_c);
			}
		}
		else if (value < r->r_v) {


			if (r->m_c == NULL) {
				r->t = TWO_NODE;

				if (r->p->t == TWO_NODE) {
					if (value < r->p->l_v) {
						r->p->r_c = r->p->m_c;
						r->p->r_v = r->p->l_v;

						r->p->l_v = value;

						r->p->t = THREE_NODE;

						r->p->m_c = newNode(r->r_v);
						r->p->m_c->p = r->p;
					}
				}
				else {
				}
			}
			else {
				treeInsert(value, r->m_c);
			}


		}
		else {
			if (r->m_c == NULL) {
				nodeT* l_c = newNode(r->l_v);
				nodeT* m_c = newNode(value);

				l_c->p = r;
				m_c->p = r;

				r->t = TWO_NODE;
				r->l_v = r->r_v;

				r->l_c = l_c;
				r->m_c = m_c;
			}
			else {
				treeInsert(value, r->r_c);
			}
		}
	}
}

//-----------------

void printTree(nodeT* node){
	if (node->t == TWO_NODE)
		printf("(Node %d ", node->l_v);
	else
		printf("(Node %d %d ", node->l_v, node->r_v);

	if (node->l_c)
		printTree(node->l_c);
	else
		printf("() ");

	if (node->m_c)
		printTree(node->m_c);
	else
		printf("() ");

	if (node->r_c)
		printTree(node->r_c);
	else
		printf("() ");

	printf(") ");
}



        
void main(void){
	nodeT *node = newNode(50);

	printf("\n\n");
	printTree(node);
	printf("\n\n");

	treeInsert(70, node);

	printf("\n\n");
	printTree(node);
	printf("\n\n");

	treeInsert(30, node);

	while (TRUE) {
		if (node->p == NULL)
			break;

		node = node->p;
	}

	printf("\n\n");
	printTree(node);
	printf("\n\n");

	treeInsert(90, node);

	while (TRUE) {
		if (node->p == NULL)
			break;

		node = node->p;
	}

	printf("\n\n");
	printTree(node);
	printf("\n\n");

	treeInsert(10, node);

	while (TRUE) {
		if (node->p == NULL)
			break;

		node = node->p;
	}

	printf("\n\n");
	printTree(node);
	printf("\n\n");

	treeInsert(20, node);

	while (TRUE) {
		if (node->p == NULL)
			break;

		node = node->p;
	}

	printf("\n\n");
	printTree(node);
	printf("\n\n");

	treeInsert(15, node);

	while (TRUE) {
		if (node->p == NULL)
			break;

		node = node->p;
	}

	printf("\n\n");
	printTree(node);
	printf("\n\n");

	system("pause");
} 