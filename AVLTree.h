#ifndef AVL_TREE_H
#define AVL_TREE_H

//#define DEBUG

#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <queue>

using namespace std;

// AvlTree class
//
// CONSTRUCTION: zero parameter
//
// ******************PUBLIC OPERATIONS*********************
// void insert( x )       --> Insert x (with rebalancing)
// void remove( x )       --> Remove x (with rebalancing)
// bool contains( x )     --> Return true if x is present
// boolean isEmpty( )     --> Return true if empty; else false
// void makeEmpty( )      --> Remove all items
// void printTree( )      --> Print tree in sorted order
// ******************ERRORS********************************

template <typename Comparable>
class AvlTree
{
private:
    struct AvlNode
    {
        Comparable element; //actual data stored on the node
        AvlNode *left;
        AvlNode *right;
        int height;

        AvlNode(const Comparable &ele, AvlNode *lt, AvlNode *rt, int h = 0)
            : element{ele}, left{lt}, right{rt}, height{h} {}
    }; //constructor, takes a reference to a piece of data

    AvlNode *root; 
    unsigned int sz;    // added

public:
    /**
     * @brief Construct a new Avl Tree object
     */
    AvlTree() : root{nullptr}, sz{0} //default constructor. Initializar list
    {
    }

    /**
     * @brief Copy constructor
     */
    AvlTree(const AvlTree &rhs) : root{nullptr}, sz{ rhs.sz }
    {
        root = clone(rhs.root); //ducplicate of each node starting with the root. to be able to make a copy of a tree
    }
    //destructor
    ~AvlTree()
    {
        makeEmpty();
    }

    /**
     * Copy assignment operator
     */
    AvlTree &operator=(const AvlTree &rhs)
    {
        if (&rhs != this) { // check for self-assignment
            makeEmpty();
            root = clone(rhs.root);
            sz = rhs.sz;
        }
        return *this;
    }

    /**
     * Returns true if x is found in the tree.
     */
    bool contains(const Comparable &x) const
    {
        return contains(x, root);
    }

    /**
     * Test if the tree is logically empty.
     * Return true if empty, false otherwise.
     */
    bool isEmpty() const
    {
        return root == nullptr;
    }

    /**
     * Print the tree contents in sorted order.
     */
    void printTree() const
    {
        if (isEmpty())
            cout << "Empty tree" << endl;
        else
            printTree(root);
    }

    /**
     * Print the tree structure.
     */
    void printPrettyTree() const
    {
        printPrettyTree("", root, false);
    }

    /**
     * Make the tree empty.
     */
    void makeEmpty()
    {
        makeEmpty(root);
    }

    /**
     * Insert x into the tree; duplicates are ignored.
     */
    void insert(const Comparable &x)
    {
         if (insert(x, root)) {
            sz++;
        }
    }

    /**
     * Remove x from the tree. Nothing is done if x is not found.
     */
    void remove(const Comparable &x)
    {
            if (remove(x, root)) {
            sz--;
        }
    }

    /**
     * Call the z tree node.
     * making this const doesn't allow a return of find(z,root) hmmm?
     */
    const Comparable& operator[](const Comparable& z){
        return find(z, root);
    }

    unsigned int size() const { return sz; }

   void write(ostream& file) {
        write(file, root);
    }
    void read(istream& file, unsigned int count) {
        Comparable data;
        while (count > 0) {
            file >> data;
            insert(data);
            count--;
        }
    }

	void read(istream& file){
        Comparable data;
        while(file >> data){
            AvlNode node{data};
            insert(node);
        }
    }

private:

    void write(ostream& file, AvlNode*&t){
        if(t == nullptr){
            return;
        }

        queue<AvlNode*> q;

        q.push(t);
        while(q.empty() == false){
            AvlNode* tmp = q.front();
            file << tmp->element << ";";

            q.pop();
            if(tmp->left != nullptr){
                q.push(tmp->left);
            }
            if(tmp->right != nullptr){
                q.push(tmp->right);
            }
        }
    }

    /**
     * Internal method to find x.
     * x is the item to find.
     * t is the node that roots the subtree.
     */
    const Comparable& find(const Comparable &x, AvlNode *&t){
        if (t == nullptr)
        {
            static Comparable notFound;
            return notFound;
        }
        if (x < t->element)
            return find(x, t->left);
        else if (t->element < x)
            return find(x, t->right);
        else{
            return t->element;
        }
    }

    /**
     * Internal method to insert into a subtree.
     * x is the item to insert.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    bool insert(const Comparable &x, AvlNode *&t)
    {
        if (t == nullptr) {
            t = new AvlNode{x, nullptr, nullptr, 0};
            return true; // a single node is always balanced
        }

        if (x < t->element) {
            if (insert(x, t->left) == false)
                return false;
        }
        else if (t->element < x) {
            if (insert(x, t->right) == false)
                return false;
        }
        else {
            return false;
        } // Duplicate; do nothing

        // This will call balance on the way back up the tree. It will only balance 
        // once at node the where the tree got imbalanced (called node alpha in the textbook).
        balance(t);
        return true;
    }

    /**
     * Internal method to remove from a subtree.
     * x is the item to remove.
     * t is the node that roots the subtree.
     * Set the new root of the subtree.
     */
    bool remove(const Comparable &x, AvlNode *&t)
    {
        // same as in a binary search tree -- from textbook
        if (t == nullptr){
            return false; 
        }
        if(x < t->element){
            if (remove(x, t->left) == false) {
                return false;
            }
        }
        else if(t->element < x){
            if (remove(x, t->right) == false) {
                return false;
            }
        }
        else if(t->left != nullptr && t->right != nullptr){
             t->element = findMin(t->right)->element;
            if (remove(t->element, t->right)==false){
            return false;
            }
        }
        else{
            AvlNode *currentNode = t;
            t = (t->left != nullptr) ? t->left : t->right;
            delete currentNode;         
        }

        // don't forget to balance
        balance(t);
        return true;
    }

    /**
     * Internal method to find the smallest item in a subtree t.
     * Return node containing the smallest item.
     */
    AvlNode *findMin(AvlNode *t) const
    {
        if (t == nullptr)
            return nullptr;

        if (t->left == nullptr)
            return t;

        return findMin(t->left);
    }

    /**
     * Internal method to test if an item is in a subtree.
     * x is item to search for.
     * t is the node that roots the tree.
     */
    bool contains(const Comparable &x, AvlNode *t) const
    {
        if (t == nullptr)
            return false;

        else if (x < t->element)
            return contains(x, t->left);
        else if (t->element < x)
            return contains(x, t->right);
        else
            return true; // Match
    }
    /****** NONRECURSIVE VERSION*************************
        bool contains( const Comparable & x, AvlNode *t ) const
        {
            while( t != nullptr )
                if( x < t->element )
                    t = t->left;
                else if( t->element < x )
                    t = t->right;
                else
                    return true;    // Match

            return false;   // No match
        }
    *****************************************************/

    /**
     * Internal method to make subtree empty.
     */
    void makeEmpty(AvlNode *&t)
    {
        if (t == nullptr)
            return;

        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        t = nullptr;
    }

    /**
     * Internal method to clone subtree.
     */
    AvlNode *clone(AvlNode *t) const
    {
        if (t == nullptr)
            return nullptr;

        return new AvlNode{t->element, clone(t->left), clone(t->right), t->height};
    }

    // Modified from: https://stackoverflow.com/questions/36802354/print-binary-tree-in-a-pretty-way-using-c
    void printPrettyTree(const std::string &prefix, const AvlNode *node, bool isRight) const
    {
        if (node == nullptr)
            return;

        std::cout << prefix;
        std::cout << (isRight ? "├──" : "└──");
        // print the value of the node
        std::cout << node->element << std::endl;

        // enter the next tree level - left and right branch
        printPrettyTree(prefix + (isRight ? "│   " : "    "), node->right, true);
        printPrettyTree(prefix + (isRight ? "│   " : "    "), node->left, false);
    }

    // Avl manipulations

    /**
     * Return the height of node t or -1 if nullptr.
     */
    int height(AvlNode *t) const
    {
        return t == nullptr ? -1 : t->height;
    }

    static const int ALLOWED_IMBALANCE = 1; // 1 is the default; more will make balancing cheaper
                                            // but the search less efficient.

    // Assume t is balanced or within one of being balanced since we check this after each manipulation
    // t represents alpha in the textbook
    void balance(AvlNode *&t)
    {
        if (t == nullptr)
            return;

        if (height(t->left) - height(t->right) > ALLOWED_IMBALANCE)  // unbalancing insertion was left
        {
            if (height(t->left->left) >= height(t->left->right)) 
                rotateWithLeftChild(t); // case 1 (outside)
            else
                doubleWithLeftChild(t); // case 2 (inside)
        }
        else if (height(t->right) - height(t->left) > ALLOWED_IMBALANCE) // unbalancing insertion was right
        {
            if (height(t->right->right) >= height(t->right->left))
                rotateWithRightChild(t); // case 4 (outside)
            else
                doubleWithRightChild(t); // case 3 (inside)
        }

        // update height
        t->height = max(height(t->left), height(t->right)) + 1;
    }

    int max(int lhs, int rhs) const
    {
        return lhs > rhs ? lhs : rhs;
    }

    /**
     * Rotate binary tree node with left child.
     * For AVL trees, this is a single rotation for case 1.
     * Update heights, then set new root.
     */
    void rotateWithLeftChild(AvlNode *&k2)
    {
#ifdef DEBUG
        cout << "need to rotateWithLeftChild for node " << k2->element << endl;
        cout << "Before:" << endl;
        printPrettyTree();
#endif

        AvlNode *k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        k2->height = max(height(k2->left), height(k2->right)) + 1;
        k1->height = max(height(k1->left), k2->height) + 1;
        k2 = k1;
#ifdef DEBUG
        cout << "After:" << endl;
        printPrettyTree();
#endif
    }

    /**
     * Rotate binary tree node with right child.
     * For AVL trees, this is a single rotation for case 4.
     * Update heights, then set new root.
     */
    void rotateWithRightChild(AvlNode *&k1)
    {
#ifdef DEBUG
        cout << "need to rotateWithRightChild for node " << k1->element << endl;
        cout << "Before:" << endl;
        printPrettyTree();

#endif

        AvlNode *k2 = k1->right;
        k1->right = k2->left;
        k2->left = k1;
        k1->height = max(height(k1->left), height(k1->right)) + 1;
        k2->height = max(height(k2->right), k1->height) + 1;
        k1 = k2;
#ifdef DEBUG
        cout << "After:" << endl;
        printPrettyTree();
#endif
    }

    /**
     * Double rotate binary tree node: first left child.
     * with its right child; then node k3 with new left child.
     * For AVL trees, this is a double rotation for case 2.
     * Update heights, then set new root.
     */
    void doubleWithLeftChild(AvlNode *&k3)
    {
#ifdef DEBUG
        cout << "doubleWithLeftChild" << endl;
#endif
        rotateWithRightChild(k3->left);
        rotateWithLeftChild(k3);
    }

    /**
     * Double rotate binary tree node: first right child.
     * with its left child; then node k1 with new right child.
     * For AVL trees, this is a double rotation for case 3.
     * Update heights, then set new root.
     */
    void doubleWithRightChild(AvlNode *&k1)
    {
#ifdef DEBUG
        cout << "doubleWithRightChild" << endl;
#endif
        rotateWithLeftChild(k1->right);
        rotateWithRightChild(k1);
    }
};

#endif

