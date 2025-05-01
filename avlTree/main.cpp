#include <iostream>
using namespace std;
struct CNode
{
    CNode* leftChild = nullptr;
    CNode* rightChild= nullptr;
    unsigned int key;
    int depth;
};

class Tree
{
public:
    void TreeAdd(unsigned int key);
    void TreeDel(unsigned int key);
    friend std::ostream& operator<<(std::ostream&, Tree&);
    ~Tree();

private:
    int nodesCount=0;
    CNode* root= nullptr;
    
    static CNode* createNode(unsigned int);
    CNode* insertNode(CNode*, unsigned int);
    CNode* deleteNode(CNode*, unsigned int);
    void deleteTree(CNode* root);
    
    CNode* changeKey(CNode*, CNode*, unsigned int);
    
    int getSideDepth(CNode*);
    int getNodeSign(CNode*);
    
    CNode* rightRotation(CNode*);
    CNode* leftRotation(CNode*);
    
    CNode* findMin(CNode*);
    CNode* findMax(CNode*);
    CNode* findPred(CNode*, CNode*);
    CNode* findNode(CNode*, unsigned int);
    
    void showTree(ostream&, CNode*);
    void printNode(ostream&, CNode*);
    
    friend CNode* ARRtoAVL(int[], int, int);
    friend void AVLtoARR(CNode*, int[], int*);

    friend Tree* TreeMerge(Tree*, Tree*);
    friend CNode* mergeTrees(CNode*, CNode*, int, int);
};
//---------------------------------------------------------
void Tree::TreeAdd(unsigned int key)
{
    root = insertNode(root,key);
    ++nodesCount;
}
void Tree::TreeDel(unsigned int key)
{
    root = deleteNode(root, key);
    --nodesCount;
}
ostream& operator<<(ostream& os, Tree& tree)
{
    tree.showTree(os, tree.root);
    return os;
}
Tree::~Tree()
{
    deleteTree(root);
}
//---------------------------------------------------------
CNode* Tree::createNode(unsigned int key)
{
    CNode* tmpNode = new CNode;
    tmpNode->key=key;
    tmpNode->depth=1;
    return tmpNode;
}
CNode* Tree::insertNode(CNode* root, unsigned int newVal)
{
    //start new tree
    if(root== nullptr) {
        //new node with key x
        root = createNode(newVal);
        //end and return root
        return root;
    }

    //go left
    if (newVal < root->key)
    {
        root->leftChild = insertNode(root->leftChild, newVal);
    }
        //go right
    else if (newVal > root->key)
    {
        root->rightChild = insertNode(root->rightChild, newVal);
    }

    root->depth = max(getSideDepth(root->leftChild),getSideDepth(root->rightChild)) +1; //depth of children + root
    int newSign = getNodeSign(root);
    //checking balance
    //left
    if (newSign > 1){                          //right side is deeper                   //RIGHT
        if(newVal > root->rightChild->key){                                                 //RIGHT
            return leftRotation(root);                     //left roation
        }
        else if(newVal < root->rightChild->key){                                         //LEFT
            root->rightChild = rightRotation(root->rightChild);
            return leftRotation(root);                      //right left rot
        }

    }
    // adding on right
    else if (newSign < -1){                   //left side is deeper                 //LEFT
        if(newVal < root->leftChild->key){                                             //LEFT
            return rightRotation(root);                      //right rotation
        }
        else if (newVal > root->leftChild->key){                                      //RIGHT
            root->leftChild = leftRotation(root->leftChild);
            return rightRotation(root);                      //left right roation
        }
    }

    //value was in tree, or new tree is balanced
    return root;
}
CNode* Tree::deleteNode(CNode* root, unsigned int key)
{
    //end
    if(root== nullptr) return nullptr;
    //looking for a child
    if      (key < root->key) root->leftChild  = deleteNode(root->leftChild, key);
    else if (key > root->key) root->rightChild = deleteNode(root->rightChild, key);
    //node is found
    if(key == root->key) {
        if((root->leftChild== nullptr) && (root->rightChild== nullptr)) 
        {
            delete root;
            return nullptr;
        }
        if(root->leftChild== nullptr) 
        {
            CNode* retNode = root->rightChild;
            delete root;
            return retNode;
        }
        if(root->rightChild== nullptr)
        {
            CNode* retNode = root->leftChild;
            delete root;
            return retNode;
        }                             //no RC LC will be attached

        //if node has both children
        CNode* rightestChild = findMin(root->rightChild);                                   //find the leftest right child
        root->key = rightestChild->key;                                                     //swap values
        root->rightChild = deleteNode(root->rightChild,rightestChild->key);                 //move right subtree

    }
    root->depth = max(getSideDepth(root->leftChild),getSideDepth(root->rightChild)) -1; //depth of children + root
    int rootSign = getNodeSign(root);

    if(rootSign > 1){
        int rightSign= getNodeSign(root->rightChild);
        if(rightSign >= 0){
            return leftRotation(root);
        }
        else if(rightSign < 0){
            root->rightChild = rightRotation(root->rightChild);
            return leftRotation(root);
        }

    }
    else if(rootSign < -1){
        int leftSign = getNodeSign(root->leftChild);
        if(leftSign >= 0){
            return rightRotation(root);
        }
        else if(leftSign < 0){
            root->leftChild = leftRotation(root->leftChild);
            return rightRotation(root);
        }

    }
    return root;
}
void Tree::deleteTree(CNode* root)
{
    if(root == nullptr) 
    {
        return;
    }
    deleteTree(root->leftChild);
    deleteTree(root->rightChild);
    delete root;
}
//---------------------------------------------------------
CNode* Tree::changeKey(CNode* root, CNode* node, unsigned int newKey)
{
    CNode* tmpRoot;
    tmpRoot = deleteNode(root,node->key);
    return    insertNode(tmpRoot,newKey);
}
//---------------------------------------------------------
int Tree::getSideDepth(CNode* node)
{
    return (node == nullptr) ? 0 : node->depth;
}
int Tree::getNodeSign(CNode* node)
{
    return ( getSideDepth(node->rightChild) - getSideDepth(node->leftChild) );
}
//---------------------------------------------------------
CNode* Tree::rightRotation(CNode* root)
{
    CNode* newRoot = root->leftChild;
    root->leftChild = root->leftChild->rightChild;
    newRoot->rightChild = root;

    root->depth = max(getSideDepth(root->leftChild),getSideDepth(root->rightChild)) +1;
    newRoot->depth = max(getSideDepth(newRoot->leftChild),getSideDepth(newRoot->rightChild)) +1;
    return newRoot;
}
CNode* Tree::leftRotation(CNode* root)
{
    CNode* newRoot = root->rightChild;
    root->rightChild = root->rightChild->leftChild;
    newRoot->leftChild = root;

    root->depth = max(getSideDepth(root->leftChild),getSideDepth(root->rightChild)) +1;
    newRoot->depth = max(getSideDepth(newRoot->leftChild),getSideDepth(newRoot->rightChild)) +1;
    return newRoot;
}
//---------------------------------------------------------
CNode* Tree::findMin(CNode* root)
{
    //go as far as can left
  if(root->leftChild == nullptr) return root;
  return findMin(root->leftChild);
}
CNode* Tree::findMax(CNode* root)
{
    //go as far as can right
    if(root->rightChild == nullptr) return root;
    return findMax(root->rightChild);
}
CNode* Tree::findPred(CNode* root, CNode* orphan)
{
    //looking for rightest in the left
    if(orphan->leftChild != nullptr) return findMax(orphan->leftChild);
    //node doesnt have left child
    //move down right, till have space or left child
    CNode* current = root;
    CNode* prevNode = nullptr;
    while( (current != orphan)&&(current!= nullptr) ){
        if(orphan->key > current->key){
            prevNode = current;
            current=current->rightChild;
        }
        else{
            current=current->leftChild;
        }
    }
    if(current != orphan) prevNode= nullptr;
    return prevNode;
}
CNode* Tree::findNode(CNode* root, unsigned int key)
{
    //end
    if(root==nullptr) return nullptr;
    //x=k(r)
    if(key == root->key) return root;
    //brach left or right and try there
    else if (key < root->key) return findNode(root->leftChild, key);
    else if (key > root->key) return findNode(root->rightChild, key);

    return nullptr;
}
//---------------------------------------------------------
void Tree::showTree(ostream& os, CNode* root)
{
    if(root == nullptr) 
    {
        return;
    }
    showTree(os, root->leftChild);
    printNode(os, root);
    showTree(os, root->rightChild);
}
void Tree::printNode(ostream& os, CNode* node)
{
    if(node == nullptr)
    {
        return;
    }
    os << node->key << ',';
}
//---------------------------------------------------------
CNode* ARRtoAVL(int arr[], int lo, int hi)
{
    if(lo > hi) return nullptr;
    int mid = (lo+hi)/2;

    CNode* node;
    node = Tree::createNode(arr[mid]);

    node->leftChild = ARRtoAVL(arr, lo, mid - 1);
    node->rightChild = ARRtoAVL(arr, mid + 1, hi);
    return node;
}
void AVLtoARR(CNode* root,int arr[], int* pos)
{
    if(root == nullptr) return;
    AVLtoARR(root->leftChild,arr,pos);

    arr[*pos]=root->key;
    ++(*pos);
    AVLtoARR(root->rightChild,arr,pos);
}
//---------------------------------------------------------
CNode* mergeTrees(CNode* rootA, CNode* rootB, int sizeA, int sizeB)
{
    CNode* holdPtr;
    int* arrA = new int[sizeA];
    int* arrB = new int[sizeB];
    int* arrMerged = new int[(sizeA+sizeB)];

    int iterA, iterB, iterMerged=0;
    int holdZero=0;
    AVLtoARR(rootA,arrA,&holdZero);
    holdZero=0;
    AVLtoARR(rootB,arrB,&holdZero);

    for(iterA=0, iterB=0, iterMerged=0 ; (iterA < sizeA) && (iterB<sizeB); ++iterMerged){
        if(arrA[iterA] < arrB[iterB]){

            arrMerged[iterMerged] = arrA[iterA];
            ++iterA;
        } else{

            arrMerged[iterMerged] = arrB[iterB];
            ++iterB;
        }

    }
    for(; iterA < sizeA; ++iterA, ++iterMerged)
    {
        arrMerged[iterMerged]=arrA[iterA];
    }
    for(; iterB < sizeB; ++iterB, ++iterMerged)
    {
        arrMerged[iterMerged]=arrB[iterB];
    }

    delete[] arrA;
    delete[] arrB;
    holdPtr =ARRtoAVL(arrMerged,0,(iterMerged)-1);
    delete[] arrMerged;
    return holdPtr;
}

Tree* TreeMerge(Tree* first, Tree* second)
{
    Tree* mergeTree = new Tree;
    CNode* mergeRoot = mergeTrees(first->root,second->root,first->nodesCount,second->nodesCount);
    mergeTree->root = mergeRoot;
    mergeTree->nodesCount=first->nodesCount+second->nodesCount;
    return mergeTree;
}

int main()
{
    Tree A, B;
    A.TreeAdd(10);
    A.TreeAdd(20);

    B.TreeAdd(40);
    B.TreeAdd(50);
    B.TreeAdd(10);

    //cout << A << endl;
    //cout << B << endl;
    Tree* Merged = TreeMerge(&A,&B);
    //cout << A << endl;
    //cout << B << endl;
    //cout << *Merged << endl;
    delete Merged;


    Tree C;
    C.TreeAdd(1);
    C.TreeAdd(2);
    C.TreeAdd(3);
    C.TreeAdd(4);
    C.TreeAdd(5);
    C.TreeAdd(6);
    C.TreeAdd(7);

    //cout<<C<<endl;

    C.TreeDel(7);
    C.TreeDel(6);
    C.TreeDel(5);
    C.TreeDel(4);
    C.TreeDel(3);
    C.TreeDel(2);
    C.TreeDel(1);

    //cout<<C<<endl;

    Tree D;
    D.TreeAdd(1);
    D.TreeAdd(2);
    D.TreeAdd(3);

    D.TreeDel(2);
    //cout<<D<<endl;

    return 0;
}