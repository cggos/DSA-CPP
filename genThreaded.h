//**************************  genThreaded.h  **********************
//                 Generic binary search threaded tree

#ifndef THREADED_TREE
#define THREADED_TREE

template<class T>
class ThreadedNode {
public:
    ThreadedNode() { 
        left = right = 0; 
    }
    ThreadedNode(const T& el, ThreadedNode *l = 0, ThreadedNode *r = 0) {
        key = el; left = l; right = r; successor = 0; 
    }
    T key;
    ThreadedNode *left, *right;
    unsigned int successor : 1;
};

template<class T>
class ThreadedTree {
public:
    ThreadedTree()  {
        root = 0;
    }
    void clear() {
        clear(root); root = 0;
    }
    ~ThreadedTree() {
        clear();
    }
    void virtual visit(ThreadedNode<T>* p) { 
        cout << p->key << " ";
    } 
    void preorder();
    void inorder();
    void postorder();
    void insert(const T&);
protected:
    ThreadedNode<T>* root;
    void clear(ThreadedNode<T>*);
};

template<class T>
void ThreadedTree<T>::clear(ThreadedNode<T>*p)
{
    if (p != 0) {
         clear(p->left);
         if (p->successor == 0) // go to the right only if
              clear(p->right);  // it is a descendant;
         delete p;
     }
}

template<class T>
void ThreadedTree<T>::insert(const T& el) {
    ThreadedNode<T> *p, *prev = 0, *newNode;
    newNode = new ThreadedNode<T>(el);
    if (root == 0) {              // tree is empty;
        root = newNode;
        return;
     }
     p = root;                   // find a place to insert newNode;
     while (p != 0) {
         prev = p;
         if (p->key > el)
              p = p->left;
         else if (p->successor == 0) // go to the right node only if it
              p = p->right;    // is a descendant, not a successor;
         else break;              // don't follow successor link;
     }
     if (prev->key > el) {        // if newNode is left child of
          prev->left  = newNode;  // its parent, the parent becomes
          newNode->successor = 1; // also its successor;
          newNode->right = prev;
     }
     else if (prev->successor == 1) {// if the parent of newNode
          newNode->successor = 1; // is not the right-most node,
          prev->successor = 0;    // make parent's successor
          newNode->right = prev->right; // newNode's successor,
          prev->right = newNode;
     }
     else prev->right = newNode;  // otherwise it has no successor;
}

template<class T>
void ThreadedTree<T>::inorder() {
    ThreadedNode<T> *prev, *p = root;
    if (p != 0) {                 // process only non-empty trees;
        while (p->left != 0)      // go to the leftmost node;
            p = p->left;
        while (p != 0) {
            visit(p);
            prev = p;
            p = p->right;         // go to the right node and only
            if (p != 0 && prev->successor == 0) // if it is a descendant
                while (p->left != 0)   // go to the leftmost node,
                    p = p->left;  // otherwise visit the successor;
        }
    }
}

template<class T>
void ThreadedTree<T>::preorder() {
    ThreadedNode<T> *p = root;
	while (p != 0) {              // process only non-empty trees;
        visit(p);
        if (p->left != 0)
             p = p->left;
        else if (p->right != 0 && p->successor == 0)
             p = p->right;
        else {                    // if p is a leaf, go through the chain
             while (p->successor == 1) // of its
                 p = p->right;    // (already visited) inorder successors
             p = p->right;        // and restart with the right descendant
        }                         // of the last successor;
    }
}

template<class T>
void ThreadedTree<T>::postorder() {
    ThreadedNode<T> *q, *r, *s, *p = new ThreadedNode<T>, *rightmost, *dummy = p;
    p->left = root;
    for (rightmost = root; rightmost->right != 0; rightmost = rightmost->right);
    rightmost->successor = 1;
    rightmost->right = p;
    enum direction {goLeft, goRight, visiting} dir = goLeft;
    while (p != 0) {
        if (dir == goLeft)
             if (p->left != 0)
                  p = p->left;
             else dir = goRight;
        else if (dir == goRight)
             if (p->right != 0 && p->successor == 0) {
                  p = p->right;
                  dir = goLeft;
             }
             else dir = visiting;
        else {
             if (p == dummy) {
                  rightmost->right = 0; // restore original setting of
                  rightmost->successor = 0; // rightmost node;
                  delete dummy;
                  return;
             }
             visit(p);
             if (p->right != 0 && p->right->left == p) { // parent == successor;
                  p = p->right;
                  dir = goRight;
             }
             else {
                  // scan a right-extended chain of nodes and
                  // reverse right pointers;
                  for (q = p->right->left, r = q->right, s = r->right;
                       r != p; q = r, r = s, s = s->right)
                      r->right = q;
                  // scan the chain backwards, visit each node, and
                  // restore the original setting of right pointers;
                  for (s = q->right; q != p->right->left;
                       q->right = r, r = q, q = s, s = s->right)
                      visit(q);
                  visit(q);
                  p = p->right;
                  dir = goRight;
             }
        }
    }
}

#endif


