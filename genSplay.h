//****************************  genSplay.h  ****************************
//                     generic splaying tree class

#ifndef SPLAYING
#define SPLAYING

template<class T> class SplayTree;

template<class T>
class SplayingNode {
public:
    SplayingNode() {
        left = right = parent = 0;
    }
    SplayingNode(const T& el, SplayingNode *l = 0, SplayingNode *r = 0,
                 SplayingNode *p = 0) {
        info = el; left = l; right = r; parent = p;
    }
    T info;
    SplayingNode *left, *right, *parent;
};

template<class T>
class SplayTree {
public:
    SplayTree() {
        root = 0;
    }
    void inorder() {
        inorder(root);
    }
    T* search(const T&);
    void insert(const T&);
protected:
    SplayingNode<T> *root;
    void rotateR(SplayingNode<T>*);
    void rotateL(SplayingNode<T>*);
    void continueRotation(SplayingNode<T>* gr, SplayingNode<T>* par,
                          SplayingNode<T>* ch, SplayingNode<T>* desc);
    void semisplay(SplayingNode<T>*);
    void inorder(SplayingNode<T>*);
    void virtual visit(SplayingNode<T>*) {
    }
};


template<class T>
void SplayTree<T>::continueRotation(SplayingNode<T>* gr, SplayingNode<T>* par,
                                    SplayingNode<T>* ch, SplayingNode<T>* desc) {
    if (gr != 0) { // if p has a grandparent;
         if (gr->right == ch->parent)
              gr->right = ch;
         else gr->left  = ch;
    }
    else root = ch;
    if (desc != 0)
        desc->parent = par;
    par->parent = ch;
    ch->parent = gr;
}

template<class T>
void SplayTree<T>::rotateR(SplayingNode<T>* p) {
    p->parent->left = p->right;
    p->right = p->parent;
    continueRotation(p->parent->parent,p->right,p,p->right->left);
}

template<class T>
void SplayTree<T>::rotateL(SplayingNode<T>* p) {
    p->parent->right = p->left;
    p->left = p->parent;
    continueRotation(p->parent->parent,p->left,p,p->left->right);
}

template<class T>
void SplayTree<T>::semisplay(SplayingNode<T>* p) {
    while (p != root) {
        if (p->parent->parent == 0)    // if p's parent is the root;
             if (p->parent->left == p)
                  rotateR(p);
             else rotateL(p);
        else if (p->parent->left == p) // if p is a left child;
             if (p->parent->parent->left == p->parent) {
                  rotateR(p->parent);
                  p = p->parent;
             }
             else {
                  rotateR(p); // rotate p and its parent;
                  rotateL(p); // rotate p and its new parent;
             }
        else                          // if p is a rigth child;
             if (p->parent->parent->right == p->parent) {
                  rotateL(p->parent);
                  p = p->parent;
             }
             else {
                  rotateL(p); // rotate p and its parent;
                  rotateR(p); // rotate p and its new parent;
             }
        if (root == 0)              // update the root;
            root = p;
    }
}

template<class T>
T* SplayTree<T>::search(const T& el) {
    SplayingNode<T> *p = root;
    while (p != 0)
        if (p->info == el) {        // if el is in the tree,
             semisplay(p);          // move it upward;
             return &p->info;
        }
        else if (el < p->info)
             p = p->left;
        else p = p->right;
    return 0;
}

template<class T>
void SplayTree<T>::insert(const T& el) {
    SplayingNode<T> *p = root, *prev = 0, *newNode;
    while (p != 0) {  // find a place for inserting a new node;
        prev = p;
        if (el < p->info)
             p = p->left;
        else p = p->right;
    }
    if ((newNode = new SplayingNode<T>(el,0,0,prev)) == 0) {
         cerr << "No room for new nodes\n";
         exit(1);
    }
    if (root == 0)    // the tree is empty;
         root = newNode;
    else if (el < prev->info)
         prev->left  = newNode;
    else prev->right = newNode;
}

template<class T>
void SplayTree<T>::inorder(SplayingNode<T> *p) {
     if (p != 0) {
         inorder(p->left);
         visit(p);
         inorder(p->right);
     }
}

#endif
