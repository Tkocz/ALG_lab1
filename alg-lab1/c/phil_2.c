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

    return node;
}

int max_val(int a, int b, int c) {
    int t = a;

    if (b > t) t = b;
    if (c > t) t = c;

    return t;
}

int mid_val(int a, int b, int c) {
    int t0 = min_val(a, b, c);
    int t1 = max_val(a, b, c);

    if (a == t0) {
        if (b == t1)
            return c;
        else
            return b;
    }
    
    if (b == t0) {
        if (a == t1)
            return c;
        else
            return a;
    }

    if (c == t0) {
        if (a == t1)
            return b;
        else
            return a;
    }
}

int min_val(int a, int b, int c) {
    int t = a;

    if (b < t) t = b;
    if (c < t) t = c;

    return t;
}

nodeT* splitInsert(int value, nodeT* r) {
    int l_v = min_val(r->l_v, value, r->r_v);
    int m_v = mid_val(r->l_v, value, r->r_v);
    int r_v = max_val(r->l_v, value, r->r_v);

    if (r->p) {
        if (r->p->t == TWO_NODE) {
            // parent har bara 1 värde, så vi kan stoppa in skiten där

            r->p->t = THREE_NODE;

            if (m_v < r->p->l_v) {
                r->p->r_v = r->p->l_v;
                r->p->l_v = m_v;

                r->p->r_c = r->p->m_c;
                r->p->m_c = r;
                r->p->l_c = newNode(l_v);
                r->p->l_c->p = r->p;

                r->t = TWO_NODE;
                r->l_v = r_v;
            }
            else {
                r->p->r_v = m_v;

                r->p->r_c = newNode(r_v);
                r->p->r_c->p = r->p;
                r->p->m_c = r;
                r->t = TWO_NODE;
                r->l_v = l_v;
            }

            return r->p; //????
        }
        else /* if (r->p->t == THREE_NODE) */ {
            if (r->t != THREE_NODE) {
                printf("GOD DAMNIT\n");
                // det här ska helst inte hända... lol
            }

            // r är överfull
            // r ska splittas till r1 och r2
            // r1 ska innehålla l_v
            // r2 ska innehålla r_v
            // m_v skickas uppåt

            nodeT* p = splitInsert(m_v, r->p);

            printf("SHIT DIS SHIT\n");

        }
    }
    else {
        if (r->t != THREE_NODE) {
            printf("GOD DAMNIT 2\n");
            // det här ska helst inte hända... lol
        }

        nodeT* p = newNode(m_v);

        nodeT* a = newNode(l_v);
        nodeT* b = newNode(r_v);

        p->l_c = a;
        p->m_c = b;

        a->p = p;
        b->p = b;

        return p;
    }
}

void treeInsert(int value, nodeT* r) {
    if (r->t == TWO_NODE) {
        if (noChildren(r)) {
            /*
             * Fall 1:
             *
             *   r är ett löv (inga barn), och har endast ett värde. I detta
             *   fall kan vi sätta in det nya värdet direkt i lövet. Vi får
             *   därefter en lövnod med två värden. Det enda vi behöver kolla är
             *   om vi ska sätta in det nya värdet till vänster eller höger i
             *   lövet.
             *
             */
            if (value < r->l_v) {
                // Vi sätter det nya värdet till vänster genom att först flytta
                // nodens nuvarande värde åt höger, sedan sätta in nya värdet
                // på vänster sida.
                r->r_v = r->l_v;
                r->l_v = value;
            }
            else {
                // Vi sätter det nya värdet till höger.
                r->r_v = value;
            }

            // r har nu två värden och blir således en tre-nod (utan barn).
            r->t = THREE_NODE;
        }
        else {
            /*
             * Fall 2:
             *
             *   r är en nod med ett värde, och har även två barn-noder. I detta
             *   fall propagerar vi värdet nedåt i trädet, tills vi finner ett
             *   löv. Vi behöver ta reda på om vi ska propagera värdet åt
             *   vänster eller höger genom att jämföra med värdet som finns i r.
             *
             */
            if (value < r->l_v) treeInsert(value, r->l_c); // Vänster.
            else                treeInsert(value, r->m_c); // Höger.
        }
    }
    else /* if (r->t == THREE_NODE) */ {
        int l_v = min_val(r->l_v, value, r->r_v);
        int m_v = mid_val(r->l_v, value, r->r_v);
        int r_v = max_val(r->l_v, value, r->r_v);

        if (noChildren(r)) {
            if (r->p == NULL) {
                /*
                 * Fall 3:
                 *
                 *   r är en nod med två värden, har inga barn, och inte heller
                 *   någon förälder. I detta fall skapar vi en förälder p, som
                 *   får det mellersta värdet. r återställs till en två-nod och
                 *   får det lägsta värdet. Vi skapar även en ny nod r' som får
                 *   det högsta värdet. p får r och r' som vänster- respektive
                 *   höger-barn.
                 *
                 *     p
                 *    / \
                 *   r   r'
                 *
                 *   p får värdet m_v
                 *   r får värdet l_v
                 *   r' får värdet r_v
                 *
                 *   Genom detta får vi det mellersta värdet i p, lägsta värdet
                 *   i r, och högsta värdet i r'.
                 *
                 */
                r->t = TWO_NODE;
                r->l_v = l_v;
                r->p = newNode(m_v);
                r->p->l_c = r;
                r->p->m_c = newNode(r_v);
                r->p->m_c->p = r->p;
            }
            else if (r->p->t == TWO_NODE) {
                /*
                 * Fall 4:
                 *
                 *   r är en nod med två värden, har inga barn, men en förälder
                 *   med ett värde. I detta fall flyttar vi upp det mellersta
                 *   värdet till föräldern, och gör föräldern till en tre-nod.
                 *   Eftersom föräldern bara har ett värde sedan tidigare, så
                 *   vet vi att vi inte behöver propagera förändringen vidare
                 *   uppåt. Vi behöver däremot kolla om vi sätter det nya värdet
                 *   till vänster eller höger i föräldern. r återställs till en
                 *   två-nod (som har ett värde), och det andra värdet flyttas
                 *   till mellan-barnet.
                 *
                 *      p
                 *    / | \
                 *   r  r' *
                 *
                 *   Obs! Illustrationen ovan blir spegelvänd om vi flyttar
                 *   värdet uppåt vänster istället för uppåt höger.
                 *
                 */
                r->t    = TWO_NODE;
                r->p->t = THREE_NODE;

                // Kom ihåg: r har inga barn, så vi behöver inte arrangera om
                //           dem här, även fast vi gör om r till en två-nod.

                if (m_v < r->p->l_v) {
                    // Det mellersta värdet sätts till vänster i föräldern. Det
                    // betyder att vi flyttat det mellersta värdet uppåt höger.
                    // Föräldern får ett nytt mellan-barn med värdet r_v.
                    r->l_v = l_v;

                    r->p->r_v = r->p->l_v;
                    r->p->l_v = m_v;

                    r->p->r_c = r->p->m_c;
                    r->p->m_c = newNode(r_v);
                    r->p->m_c->p = r->p;
                }
                else {
                    // Det mellersta värdet sätts till höger i föräldern. Det
                    // betyder att vi flyttat det mellersta värdet uppåt
                    // vänster. Föräldern får ett nytt mellan-barn med värdet
                    // l_v.
                    r->l_v = r_v;

                    r->p->r_v = m_v;

                    r->p->r_c = r->p->m_c;
                    r->p->m_c = newNode(l_v);
                    r->p->m_c->p = r->p;
                }

            }
            else /* if (r->p->t == THREE_NODE) */ {
                /*
                 * Fall 5:
                 *
                 *   r är en tre-nod, har inga barn, och en förälder som är en
                 *   tre-nod (med två värden).
                 *
                 *     p
                 *  / / \ \
                 *  a b c d
                 *
                 */

                splitInsert(value, r);
            }
        }
        else {
            /*
             * Fall 6:
             *
             *   r är en tre-nod, och har tre barn-noder. I detta fall
             *   propagerar vi värdet nedåt i trädet, tills vi finner ett löv
             *   att sätta in värdet i. Vi håller koll på vilket håll värdet ska
             *   propageras åt, beroende på hur det står i jämförelse med de två
             *   värden som finns i r.
             *
             */
                 if (value < r->l_v) treeInsert(value, r->l_c); // Vänster.
            else if (value < r->r_v) treeInsert(value, r->m_c); // Mitten.
            else                     treeInsert(value, r->r_c); // Höger.
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


void treeInsert2(int v, nodeT* r) {
    // den här funktionen underlättar bara insättning och utskrivning

    while (TRUE) {
        if (r->p == NULL)
            break;

        r = r->p;
    }


    printf("\nInserting %d:\n", v);
    treeInsert(v, r);
    while (TRUE) {
        if (r->p == NULL)
            break;

        r = r->p;
    }
    printTree(r);
    printf("\n");

}


        
void main(void){
    nodeT *r = newNode(10);

    printTree(r);
    printf("\n");

    /*treeInsert2(7, r);
    treeInsert2(3, r);
    treeInsert2(9, r);
    treeInsert2(1, r);
    treeInsert2(2, r);
    treeInsert2(4, r);
    treeInsert2(0, r);
    treeInsert2(10, r);*/
    treeInsert2(40, r);
    treeInsert2(30, r);
    treeInsert2(37, r);
    treeInsert2(20, r);
    treeInsert2(39, r);
    treeInsert2(38, r);

    treeInsert2(36, r);






    while (TRUE) {
        if (r->p == NULL)
            break;

        r = r->p;
    }

    system("pause");
} 