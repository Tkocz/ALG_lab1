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
 *   Indikerar att en nod �r en 2-nod, dvs har ett v�rde och tv� barn.
 *------------------------------------*/
#define TwoNode 2

/*--------------------------------------
 * Constant: ThreeNode
 *
 * Description:
 *   Indikerar att en nod �r en 3-nod, dvs har tv� v�rden och tre barn.
 *------------------------------------*/
#define ThreeNode 3

//---------------------------------------------------------
// TYPES
//---------------------------------------------------------

/*--------------------------------------
 * Type: nodeT
 *
 * Description:
 *   Representerar en nod i ett 2-3-tr�d.
 *------------------------------------*/
typedef struct nodeT {
    struct nodeT *parent,
                 *leftChild,
                 *midChild,
                 *rightChild;

    int leftVal,
        rightVal;

    // Indikerar om noden �r en 2-nod eller 3-nod.
    int type;
} nodeT;

//---------------------------------------------------------
// FUNCTIONS
//---------------------------------------------------------

/*--------------------------------------
 * Function: isLeaf()
 * Parameters: node  En nod i ett 2-3-tr�d.
 *
 * Description:
 *   Returnerar sant om den specificerade noden inte har n�gra barn.
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
 * Parameters: value  Det v�rde som noden ska inneh�lla.
 *
 * Description:
 *   Skapar en ny, ensamst�ende tv�-nod med det givna v�rdet.
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
 * Parameters: value  Det v�rde som vars n�rmaste l�v ska hittas.
 *             root   Den nod i tr�det varifr�n vi p�b�rjar s�kningen.
 * Returns: Den l�vnod som ligger n�rmast det givna v�rdet, dvs den nod vari vi
 *          skulle s�tta in det givna v�rdet i tr�det.
 *
 * Description:
 *   Letar upp den l�vnod i tr�det som ligger n�rmast det givna v�rdet. Vid
 *   ins�ttning anv�nds denna l�vnod f�r ins�ttning.
 *------------------------------------*/
nodeT *findLeaf(int value, nodeT *root) {
    // Om vi hamnat i ett l�v s� kan vi inte s�ka vidare. Mest troligt har vi
    // hamnat i detta l�vet f�r att vi kommit r�tt, s� vi returnerar noden.
    if (isLeaf(root))
        return root;

    // Om noden har tv� barn s� s�ker vi vidare i ett av dem (beroende p�
    // v�rdet) rekursivt.
    if (root->type == TwoNode) {
        if (value < root->leftVal) return findLeaf(value, root->leftChild);
        else                       return findLeaf(value, root->midChild );
    }

    // Om noden har tre barn s� s�ker vi vidare i ett av dem (beroende p�
    // v�rdet) rekursivt.

         if (value < root->leftVal ) return findLeaf(value, root->leftChild );
    else if (value < root->rightVal) return findLeaf(value, root->midChild  );
    else                             return findLeaf(value, root->rightChild);
}

/*--------------------------------------
 * Function: findRoot()
 * Parameters: node  Den node vars rot-nod ska letas upp.
 * Returns: Den absoluta roten f�r den givna noden.
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
 * Parameters: value     Det v�rde som ska s�ttas in i noden, dvs det v�rde som
 *                       f�ljer med upp fr�n niv�n nedanf�r, eller det v�rde som
 *                       ska s�ttas in i ett l�v.
 *             node      Den nod i tr�det som ska splittas.
 *             children  De tv� syskonparen fr�n niv�n nedanf�r, eller null om
 *                       vi �r i ett l�v.
 *
 * Description:
 *   S�tter in ett v�rde och splittar den specificerade noden.
 *------------------------------------*/
void treeSplit(int value, nodeT *node, nodeT *children[]) {
    if (node->type != ThreeNode)
        Error("Attempted to split a 2-node. This should not happen.");

    int midVal = (value < node->leftVal) ? min(node->leftVal, node->rightVal)
                                         : min(value        , node->rightVal);
    int minVal = min(value, min(node->leftVal, node->rightVal));
    int maxVal = max(value, max(node->leftVal, node->rightVal));

    // N�r vi splittar en nod beh�ver vi skapa en ny nod, vilken blir ett
    // nytt syskon i syskonskaran. newNode blir den v�nstra, med l�gst v�rde.
    // node �teranv�nder vi som den h�gra, med h�gst v�rde. Mittenv�rdet
    // propagerar vi upp�t i tr�det l�ngre ner i koden.
    nodeT *newNode = createNode(minVal);
    nodeT *parent  = node->parent;

    // Eftersom vi splittar noden s� vet vi att den g�r fr�n tre-nod till tv�-
    // nod.
    node->leftVal = maxVal;
    node->type    = TwoNode;

    if (!parent) {
        //----------------------------------------------------------------------
        // 1. Ingen f�r�lder finns (vi �r i roten).
        //
        // Vi har (tyv�rr) splittat tr�dets rot, vilket kr�ver en ny rot. Detta,
        // i sin tur, ger en till niv� i tr�det, vilket egentligen �r det s�msta
        // som kan h�nda.
        //----------------------------------------------------------------------

        nodeT *newRoot = createNode(midVal);

        newRoot->leftChild = newNode;
        newRoot->midChild  = node;

        newNode->parent = newRoot;
        node   ->parent = newRoot;
    }
    else if (parent->type == TwoNode) {
        //----------------------------------------------------------------------
        // 2. F�r�ldern �r en tv�-nod.
        //
        // F�r�ldern till denna nod har bara ett v�rde, s� vi kan bubbla upp
        // mittenv�rdet dit, sedan �r vi klara.
        //----------------------------------------------------------------------

        if (midVal < parent->leftVal) {
            // V�rdet �r mindre �n nuvarande f�r�lders enda v�rde.

            parent->rightVal = parent->leftVal;
            parent->leftVal  = midVal;
                
            parent->rightChild = parent->midChild;
            parent->midChild   = node;
            parent->leftChild  = newNode;
        }
        else {
            // V�rdet �r st�rre �n nuvarande f�r�lders enda v�rde.

            parent->rightVal = midVal;

            parent->rightChild = node;
            parent->midChild   = newNode;
        }

        newNode->parent = parent;

        // F�r�ldern har nu tv� v�rden och blir s�ledes en tre-nod.
        parent->type = ThreeNode;
    }
    else {
        //----------------------------------------------------------------------
        // 3. F�r�ldern �r en tre-nod.
        //
        // Vi f�r fyra syskon. Detta inneb�r att f�r�ldern m�ste bli tv� stycken
        // tv�-noder, med tv� barn vardera. Detta l�ser vi genom att skicka
        // med de tv� syskonparen upp till n�sta niv�.
        //----------------------------------------------------------------------

        nodeT *siblings[4];

        // Den nya noden blir v�nster-syskon till nuvarande nod 'node'.
        // Detta garanterar att de hamnar i storleksordning, d� de tre
        // barnen redan �r sorterade. Vi beh�ver allts� bara s�tta in det
        // nya syskonet p� r�tt plats i syskonskaran.
        if (node == node->parent->leftChild) {
            // Vi �r i v�nster barn. newNode blir det f�rsta syskonet (till
            // v�nster om v�nsterbarnet).
            siblings[0] = newNode;
            siblings[1] = parent->leftChild;
            siblings[2] = parent->midChild;
            siblings[3] = parent->rightChild;
        }
        else if (node == parent->midChild) {
            // Vi �r i mittenbarnet. newNode blir det andra syskonet (till
            // v�nster om mittenbarnet).
            siblings[0] = parent->leftChild;
            siblings[1] = newNode;
            siblings[2] = parent->midChild;
            siblings[3] = parent->rightChild;
        }
        else {
            // Vi �r i h�ger barn. newNode blir det tredje syskonet (till
            // v�nster om h�gerbarnet).
            siblings[0] = parent->leftChild;
            siblings[1] = parent->midChild;
            siblings[2] = newNode;
            siblings[3] = parent->rightChild;
        }

        // Rekursiooooon!
        treeSplit(midVal, parent, siblings);
    }

    if (children) {
        // Vi har f�tt med oss barn fr�n niv�n nedanf�r, s� vi hakar p� dem h�r.
        // Det v�nstra syskonparet blir barn till newNode (som ligger till
        // v�nster), och det h�gra syskonparet blir barn till node (som nu
        // ligger till h�ger).

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
 * Parameters: value  Det v�rde som ska s�ttas in i tr�det.
 *             root   En nod i det tr�d i vilket ins�ttningen ska g�ras.
 *
 * Description:
 *   S�tter in ett v�rde och splittar den specificerade noden.
 *------------------------------------*/
void treeInsert(int value, nodeT *root) {
    //--------------------------------------------------------------------------
    // 1. F�rst letar vi upp det l�v som v�rdet ska s�ttas in i.
    //--------------------------------------------------------------------------

    nodeT *leaf = findLeaf(value, findRoot(root));

    //--------------------------------------------------------------------------
    // 2. Sedan s�tter vi in v�rdet. Om l�vet �r en tv�-nod kan vi s�tta in
    //    v�rdet direkt, och l�vet blir en tre-nod. Annars, om l�vet �r en tre-
    //    nod, s� m�ste vi splitta den och propagera splitten upp�t rekursivt.
    //    Det g�rs i treeSplit()-funktionen.
    //--------------------------------------------------------------------------

    if (leaf->type == TwoNode) {
        // L�vet vi hittade har bara ett enda v�rde, s� vi kan s�tta in det nya
        // v�rdet direkt i det.

        if (value < leaf->leftVal) {
            // Nytt v�nsterv�rde.

            leaf->rightVal = leaf->leftVal;
            leaf->leftVal  = value;
        }
        else {
            // Nytt h�gerv�rde.

            leaf->rightVal = value;
        }

        // L�vet har nu tv� v�rden och blir s�ledes en tre-nod.
        leaf->type = ThreeNode;
    }
    else {
        // L�vet har redan tv� v�rden, s� vi splittar det rekursivt.
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

    // V�nta p� enter f�r avslut.
    printf("Press ENTER to exit . . .\n");
    FreeBlock(GetLine());
}
