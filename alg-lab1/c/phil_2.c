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
            // parent har bara 1 v�rde, s� vi kan stoppa in skiten d�r

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
                // det h�r ska helst inte h�nda... lol
            }

            // r �r �verfull
            // r ska splittas till r1 och r2
            // r1 ska inneh�lla l_v
            // r2 ska inneh�lla r_v
            // m_v skickas upp�t

            nodeT* p = splitInsert(m_v, r->p);

            printf("SHIT DIS SHIT\n");

        }
    }
    else {
        if (r->t != THREE_NODE) {
            printf("GOD DAMNIT 2\n");
            // det h�r ska helst inte h�nda... lol
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
             *   r �r ett l�v (inga barn), och har endast ett v�rde. I detta
             *   fall kan vi s�tta in det nya v�rdet direkt i l�vet. Vi f�r
             *   d�refter en l�vnod med tv� v�rden. Det enda vi beh�ver kolla �r
             *   om vi ska s�tta in det nya v�rdet till v�nster eller h�ger i
             *   l�vet.
             *
             */
            if (value < r->l_v) {
                // Vi s�tter det nya v�rdet till v�nster genom att f�rst flytta
                // nodens nuvarande v�rde �t h�ger, sedan s�tta in nya v�rdet
                // p� v�nster sida.
                r->r_v = r->l_v;
                r->l_v = value;
            }
            else {
                // Vi s�tter det nya v�rdet till h�ger.
                r->r_v = value;
            }

            // r har nu tv� v�rden och blir s�ledes en tre-nod (utan barn).
            r->t = THREE_NODE;
        }
        else {
            /*
             * Fall 2:
             *
             *   r �r en nod med ett v�rde, och har �ven tv� barn-noder. I detta
             *   fall propagerar vi v�rdet ned�t i tr�det, tills vi finner ett
             *   l�v. Vi beh�ver ta reda p� om vi ska propagera v�rdet �t
             *   v�nster eller h�ger genom att j�mf�ra med v�rdet som finns i r.
             *
             */
            if (value < r->l_v) treeInsert(value, r->l_c); // V�nster.
            else                treeInsert(value, r->m_c); // H�ger.
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
                 *   r �r en nod med tv� v�rden, har inga barn, och inte heller
                 *   n�gon f�r�lder. I detta fall skapar vi en f�r�lder p, som
                 *   f�r det mellersta v�rdet. r �terst�lls till en tv�-nod och
                 *   f�r det l�gsta v�rdet. Vi skapar �ven en ny nod r' som f�r
                 *   det h�gsta v�rdet. p f�r r och r' som v�nster- respektive
                 *   h�ger-barn.
                 *
                 *     p
                 *    / \
                 *   r   r'
                 *
                 *   p f�r v�rdet m_v
                 *   r f�r v�rdet l_v
                 *   r' f�r v�rdet r_v
                 *
                 *   Genom detta f�r vi det mellersta v�rdet i p, l�gsta v�rdet
                 *   i r, och h�gsta v�rdet i r'.
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
                 *   r �r en nod med tv� v�rden, har inga barn, men en f�r�lder
                 *   med ett v�rde. I detta fall flyttar vi upp det mellersta
                 *   v�rdet till f�r�ldern, och g�r f�r�ldern till en tre-nod.
                 *   Eftersom f�r�ldern bara har ett v�rde sedan tidigare, s�
                 *   vet vi att vi inte beh�ver propagera f�r�ndringen vidare
                 *   upp�t. Vi beh�ver d�remot kolla om vi s�tter det nya v�rdet
                 *   till v�nster eller h�ger i f�r�ldern. r �terst�lls till en
                 *   tv�-nod (som har ett v�rde), och det andra v�rdet flyttas
                 *   till mellan-barnet.
                 *
                 *      p
                 *    / | \
                 *   r  r' *
                 *
                 *   Obs! Illustrationen ovan blir spegelv�nd om vi flyttar
                 *   v�rdet upp�t v�nster ist�llet f�r upp�t h�ger.
                 *
                 */
                r->t    = TWO_NODE;
                r->p->t = THREE_NODE;

                // Kom ih�g: r har inga barn, s� vi beh�ver inte arrangera om
                //           dem h�r, �ven fast vi g�r om r till en tv�-nod.

                if (m_v < r->p->l_v) {
                    // Det mellersta v�rdet s�tts till v�nster i f�r�ldern. Det
                    // betyder att vi flyttat det mellersta v�rdet upp�t h�ger.
                    // F�r�ldern f�r ett nytt mellan-barn med v�rdet r_v.
                    r->l_v = l_v;

                    r->p->r_v = r->p->l_v;
                    r->p->l_v = m_v;

                    r->p->r_c = r->p->m_c;
                    r->p->m_c = newNode(r_v);
                    r->p->m_c->p = r->p;
                }
                else {
                    // Det mellersta v�rdet s�tts till h�ger i f�r�ldern. Det
                    // betyder att vi flyttat det mellersta v�rdet upp�t
                    // v�nster. F�r�ldern f�r ett nytt mellan-barn med v�rdet
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
                 *   r �r en tre-nod, har inga barn, och en f�r�lder som �r en
                 *   tre-nod (med tv� v�rden).
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
             *   r �r en tre-nod, och har tre barn-noder. I detta fall
             *   propagerar vi v�rdet ned�t i tr�det, tills vi finner ett l�v
             *   att s�tta in v�rdet i. Vi h�ller koll p� vilket h�ll v�rdet ska
             *   propageras �t, beroende p� hur det st�r i j�mf�relse med de tv�
             *   v�rden som finns i r.
             *
             */
                 if (value < r->l_v) treeInsert(value, r->l_c); // V�nster.
            else if (value < r->r_v) treeInsert(value, r->m_c); // Mitten.
            else                     treeInsert(value, r->r_c); // H�ger.
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
    // den h�r funktionen underl�ttar bara ins�ttning och utskrivning

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