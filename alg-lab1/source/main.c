//---------------------------------------------------------
// INCLUDES
//---------------------------------------------------------

#include "genlib.h"
#include "simpio.h"
#include "strlib.h"

//---------------------------------------------------------
// CONSTANTS
//---------------------------------------------------------

/*--------------------------------------
 * Constant: TwoNode
 *
 * Description:
 *   Indikerar att en nod är en 2-nod, dvs har ett värde och två barn.
 *------------------------------------*/
#define TwoNode 2

/*--------------------------------------
 * Constant: ThreeNode
 *
 * Description:
 *   Indikerar att en nod är en 3-nod, dvs har två värden och tre barn.
 *------------------------------------*/
#define ThreeNode 3

//---------------------------------------------------------
// TYPES
//---------------------------------------------------------

/*--------------------------------------
 * Type: nodeT
 *
 * Description:
 *   Representerar en nod i ett 2-3-träd.
 *------------------------------------*/
typedef struct nodeT {
    struct nodeT *parent,
                 *leftChild,
                 *midChild,
                 *rightChild;

    int leftVal,
        rightVal;

    // Indikerar om noden är en 2-nod eller 3-nod.
    int type;
} nodeT;

//---------------------------------------------------------
// FUNCTIONS
//---------------------------------------------------------

/*--------------------------------------
 * Function: isLeaf()
 * Parameters: node  En nod i ett 2-3-träd.
 *
 * Description:
 *   Returnerar sant om den specificerade noden inte har några barn.
 *------------------------------------*/
bool isLeaf(nodeT *node) {
    if (node->type == TwoNode) {
        return (node->leftChild == NULL)
            && (node->midChild  == NULL);
    }

    return (node->leftChild  == NULL)
        && (node->midChild   == NULL)
        && (node->rightChild == NULL);
}

/*--------------------------------------
 * Function: createNode()
 * Parameters: value  Det värde som noden ska innehålla.
 *
 * Description:
 *   Skapar en ny, ensamstående två-nod med det givna värdet.
 *------------------------------------*/
nodeT *createNode(int value) {
    nodeT *node = (nodeT *)malloc(sizeof(nodeT));

    node->parent = NULL;

    node->leftChild  = NULL;
    node->midChild   = NULL;
    node->rightChild = NULL;

    node->leftVal  = value;
    node->rightVal = 0xdead;

    node->type = TwoNode;

    return node;
}

/*--------------------------------------
 * Function: findLeaf()
 * Parameters: value  Det värde som vars närmaste löv ska hittas.
 *             root   Den nod i trädet varifrån vi påbörjar sökningen.
 * Returns: Den lövnod som ligger närmast det givna värdet, dvs den nod vari vi
 *          skulle sätta in det givna värdet i trädet.
 *
 * Description:
 *   Letar upp den lövnod i trädet som ligger närmast det givna värdet. Vid
 *   insättning används denna lövnod för insättning.
 *------------------------------------*/
nodeT *findLeaf(int value, nodeT *root) {
    // Om vi hamnat i ett löv så kan vi inte söka vidare. Mest troligt har vi
    // hamnat i detta lövet för att vi kommit rätt, så vi returnerar noden.
    if (isLeaf(root))
        return root;

    // Om noden har två barn så söker vi vidare i ett av dem (beroende på
    // värdet) rekursivt.
    if (root->type == TwoNode) {
        if (value < root->leftVal) return findLeaf(value, root->leftChild);
        else                       return findLeaf(value, root->midChild );
    }

    // Om noden har tre barn så söker vi vidare i ett av dem (beroende på
    // värdet) rekursivt.

         if (value < root->leftVal ) return findLeaf(value, root->leftChild );
    else if (value < root->rightVal) return findLeaf(value, root->midChild  );
    else                             return findLeaf(value, root->rightChild);
}

/*--------------------------------------
 * Function: findRoot()
 * Parameters: node  Den node vars rot-nod ska letas upp.
 * Returns: Den absoluta roten för den givna noden.
 *
 * Description:
 *   Letar upp och returnerar den givna nodens absoluta rot.
 *------------------------------------*/
nodeT *findRoot(nodeT *node) {
    while (TRUE) {
        if (!node->parent)
            return node;

        node = node->parent;
    }
}

/*--------------------------------------
 * Function: treeSplit()
 * Parameters: value     Det värde som ska sättas in i noden, dvs det värde som
 *                       följer med upp från nivån nedanför, eller det värde som
 *                       ska sättas in i ett löv.
 *             node      Den nod i trädet som ska splittas.
 *             children  De två syskonparen från nivån nedanför, eller null om
 *                       vi är i ett löv.
 *
 * Description:
 *   Sätter in ett värde och splittar den specificerade noden.
 *------------------------------------*/
void treeSplit(int value, nodeT *node, nodeT *children[]) {
    if (node->type != ThreeNode)
        Error("Attempted to split a 2-node. This should not happen.");

    int midVal = (value < node->leftVal) ? min(node->leftVal, node->rightVal)
                                         : min(value        , node->rightVal);
    int minVal = min(value, min(node->leftVal, node->rightVal));
    int maxVal = max(value, max(node->leftVal, node->rightVal));

    // När vi splittar en nod behöver vi skapa en ny nod, vilken blir ett
    // nytt syskon i syskonskaran. newNode blir den vänstra, med lägst värde.
    // node återanvänder vi som den högra, med högst värde. Mittenvärdet
    // propagerar vi uppåt i trädet längre ner i koden.
    nodeT *newNode = createNode(minVal);
    nodeT *parent  = node->parent;

    // Eftersom vi splittar noden så vet vi att den går från tre-nod till två-
    // nod.
    node->leftVal = maxVal;
    node->type    = TwoNode;

    if (!parent) {
        //----------------------------------------------------------------------
        // 1. Ingen förälder finns (vi är i roten).
        //
        // Vi har (tyvärr) splittat trädets rot, vilket kräver en ny rot. Detta,
        // i sin tur, ger en till nivå i trädet, vilket egentligen är det sämsta
        // som kan hända.
        //----------------------------------------------------------------------

        nodeT *newRoot = createNode(midVal);

        newRoot->leftChild = newNode;
        newRoot->midChild  = node;

        newNode->parent = newRoot;
        node   ->parent = newRoot;
    }
    else if (parent->type == TwoNode) {
        //----------------------------------------------------------------------
        // 2. Föräldern är en två-nod.
        //
        // Föräldern till denna nod har bara ett värde, så vi kan bubbla upp
        // mittenvärdet dit, sedan är vi klara.
        //----------------------------------------------------------------------

        if (midVal < parent->leftVal) {
            // Värdet är mindre än nuvarande förälders enda värde.

            parent->rightVal = parent->leftVal;
            parent->leftVal  = midVal;
                
            parent->rightChild = parent->midChild;
            parent->midChild   = node;
            parent->leftChild  = newNode;
        }
        else {
            // Värdet är större än nuvarande förälders enda värde.

            parent->rightVal = midVal;

            parent->rightChild = node;
            parent->midChild   = newNode;
        }

        newNode->parent = parent;

        // Föräldern har nu två värden och blir således en tre-nod.
        parent->type = ThreeNode;
    }
    else {
        //----------------------------------------------------------------------
        // 3. Föräldern är en tre-nod.
        //
        // Vi får fyra syskon. Detta innebär att föräldern måste bli två stycken
        // två-noder, med två barn vardera. Detta löser vi genom att skicka
        // med de två syskonparen upp till nästa nivå.
        //----------------------------------------------------------------------

        nodeT *siblings[4];

        // Den nya noden blir vänster-syskon till nuvarande nod 'node'.
        // Detta garanterar att de hamnar i storleksordning, då de tre
        // barnen redan är sorterade. Vi behöver alltså bara sätta in det
        // nya syskonet på rätt plats i syskonskaran.
        if (node == node->parent->leftChild) {
            // Vi är i vänster barn. newNode blir det första syskonet (till
            // vänster om vänsterbarnet).
            siblings[0] = newNode;
            siblings[1] = parent->leftChild;
            siblings[2] = parent->midChild;
            siblings[3] = parent->rightChild;
        }
        else if (node == parent->midChild) {
            // Vi är i mittenbarnet. newNode blir det andra syskonet (till
            // vänster om mittenbarnet).
            siblings[0] = parent->leftChild;
            siblings[1] = newNode;
            siblings[2] = parent->midChild;
            siblings[3] = parent->rightChild;
        }
        else {
            // Vi är i höger barn. newNode blir det tredje syskonet (till
            // vänster om högerbarnet).
            siblings[0] = parent->leftChild;
            siblings[1] = parent->midChild;
            siblings[2] = newNode;
            siblings[3] = parent->rightChild;
        }

        // Rekursiooooon!
        treeSplit(midVal, parent, siblings);
    }

    if (children) {
        // Vi har fått med oss barn från nivån nedanför, så vi hakar på dem här.
        // Det vänstra syskonparet blir barn till newNode (som ligger till
        // vänster), och det högra syskonparet blir barn till node (som nu
        // ligger till höger).

        newNode->leftChild = children[0];
        newNode->midChild  = children[1];
        node   ->leftChild = children[2];
        node   ->midChild  = children[3];

        newNode->leftChild->parent = newNode;
        newNode->midChild ->parent = newNode;
        node   ->leftChild->parent = node;
        node   ->midChild ->parent = node;
    }
}

/*--------------------------------------
 * Function: treeInsert()
 * Parameters: value  Det värde som ska sättas in i trädet.
 *             root   En nod i det träd i vilket insättningen ska göras.
 *
 * Description:
 *   Sätter in ett värde och splittar den specificerade noden.
 *------------------------------------*/
void treeInsert(int value, nodeT *root) {
    //--------------------------------------------------------------------------
    // 1. Först letar vi upp det löv som värdet ska sättas in i.
    //--------------------------------------------------------------------------

    nodeT *leaf = findLeaf(value, findRoot(root));

    //--------------------------------------------------------------------------
    // 2. Sedan sätter vi in värdet. Om lövet är en två-nod kan vi sätta in
    //    värdet direkt, och lövet blir en tre-nod. Annars, om lövet är en tre-
    //    nod, så måste vi splitta den och propagera splitten uppåt rekursivt.
    //    Det görs i treeSplit()-funktionen.
    //--------------------------------------------------------------------------

    if (leaf->type == TwoNode) {
        // Lövet vi hittade har bara ett enda värde, så vi kan sätta in det nya
        // värdet direkt i det.

        if (value < leaf->leftVal) {
            // Nytt vänstervärde.

            leaf->rightVal = leaf->leftVal;
            leaf->leftVal  = value;
        }
        else {
            // Nytt högervärde.

            leaf->rightVal = value;
        }

        // Lövet har nu två värden och blir således en tre-nod.
        leaf->type = ThreeNode;
    }
    else {
        // Lövet har redan två värden, så vi splittar det rekursivt.
        treeSplit(value, leaf, NULL);
    }
}

/*--------------------------------------
 * Function: printIntroMsg()
 * Parameters:
 *
 * Description:
 *   Skriver lite info om programmet.
 *------------------------------------*/
void printIntroMsg() {
    printf("ALG-Lab1 v2.0\n\n");
    printf("  Philip Arvidsson\n");
    printf("  Martin Bergqvist\n");
    printf("  Rasmus Lundquist\n");
    printf("  Thires Nilsson\n");
    printf("  Henrik Stridsman\n");
    printf("  Tobias \x8fnhed\n");
    printf("\n\n");
}

/*--------------------------------------
 * Function: printIntroMsg()
 * Parameters:
 *
 * Description:
 *   Programmets huvudfunktion.
 *------------------------------------*/
void main(void){
    printIntroMsg();

    nodeT *root = NULL;

    while (TRUE) {
        string s   = GetLine(),
               cmd = ConvertToUpperCase(s);
        FreeBlock(s);

        if (StringCompare(cmd, "QUIT")==0)
            break;

        if (StringCompare(cmd, "HELP")==0) {
            printf("Commands:\n");
            printf("quit - Exits the program.");
            printf("i blallala\n");
            printf("\n");
        }

        if (cmd[0] == 'I') {

        }

        FreeBlock(cmd);
    }

    // Vänta på enter för avslut.
    printf("Press ENTER to exit . . .\n");
    FreeBlock(GetLine());
}
