#include <iostream>
#include <cassert>
using namespace std;

//---------------------------------------------------------
struct CNode
{
    CNode* leftChild;
    CNode* rightChild;
    int key;
};

class CTree
{
    private:
        CNode* root= nullptr;
        bool errorFlag=false;
        CNode* createNode(int key);
        CNode* insertNode(CNode*, int);
        CNode* deleteNode(CNode*, int);
        void deleteTree(CNode*);

        CNode* findMin(CNode*);
        CNode* findMax(CNode*);
        CNode* findNode(CNode*, int);


        void printNode(ostream&, CNode*);
        void showTree(ostream&, CNode*);
    public:
        bool addValue(int);
        void removeValue(int);
        int getMinimum();
        int getMaximum();
        friend std::ostream& operator<<(std::ostream&, CTree&);
        ~CTree();
};
//---------------------------------------------------------
bool CTree::addValue(int value)
{
    errorFlag = false;
    root = insertNode(root, value);
    return !errorFlag;
}
void CTree::removeValue(int value)
{
    root = deleteNode(root, value);
}

int CTree::getMinimum()
{
    CNode* minNode = findMin(root);
    if(minNode == nullptr) return -1;
    return minNode->key;
}
int CTree::getMaximum()
{
    CNode* minNode = findMax(root);
    if(minNode == nullptr) return -1;
    return minNode->key;
}

ostream& operator<<(ostream& os, CTree& tree)
{
    tree.showTree(os, tree.root);
    return os;
}

CTree::~CTree()
{
    deleteTree(root);
}
//---------------------------------------------------------

CNode* CTree::createNode(int key)
{

        CNode* tmpNode = new (nothrow)CNode;
        if(tmpNode == nullptr)
        {
            cerr << "Node not created" << endl;
            errorFlag = true;
            return nullptr;
        }

        tmpNode->key=key;
        tmpNode->leftChild=nullptr;
        tmpNode->rightChild=nullptr;
        return tmpNode;   
}
CNode* CTree::insertNode(CNode* root, int newVal)
{
    //start new tree
    if(root==NULL) {
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
    //return with added val or val existed and nothing changed
    return root;
}
CNode* CTree::deleteNode(CNode* root, int key)
{
    if(root== nullptr) return nullptr;

    if      (key < root->key) root->leftChild  = deleteNode(root->leftChild, key);
    else if (key > root->key) root->rightChild = deleteNode(root->rightChild, key);

    if(key == root->key) 
    {
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
        }
        CNode* rightestChild = findMin(root->rightChild);
        root->key = rightestChild->key;
        root->rightChild = deleteNode(root->rightChild,rightestChild->key);

    }
    return root;
}

void CTree::deleteTree(CNode* root)
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
CNode* CTree::findMin(CNode* root)
{
  if(root->leftChild == nullptr) return root;
  return findMin(root->leftChild);
}
CNode* CTree::findMax(CNode* root)
{
    if(root->rightChild == nullptr) return root;
    return findMax(root->rightChild);
}
//---------------------------------------------------------
void CTree::printNode(ostream& os, CNode* node)
{
    if(node == nullptr)
    {
        return;
    }
    os << node->key << ',';
}
void CTree::showTree(ostream& os, CNode* root)
{
    if(root == nullptr) 
    {
        return;
    }
    showTree(os, root->leftChild);
    printNode(os, root);
    showTree(os, root->rightChild);
}
//---------------------------------------------------------

CNode* CTree::findNode(CNode* root, int key)
{
    if(root==nullptr) return nullptr;
    if(key == root->key) return root;
    else if (key < root->key) return findNode(root->leftChild, key);
    else if (key > root->key) return findNode(root->rightChild, key);

    return nullptr;
}
//--------------------------------------
int main()
{
    CTree tree;
    tree.addValue(2);
    tree.addValue(3);
    tree.addValue(1);
    cout << tree <<endl;
    cout << "Min: " << tree.getMinimum() <<endl;
    cout << "Max: " << tree.getMaximum() <<endl;
    tree.removeValue(2);
    cout << tree << endl;
    tree.removeValue(3);
    cout << tree << endl;
    tree.removeValue(1);
    cout << tree << endl;
    tree.removeValue(1);
    
    tree.addValue(4);
    tree.addValue(6);
    tree.addValue(1);
    tree.addValue(5);
    tree.addValue(7);
    cout << tree << endl;

    return 0;
}