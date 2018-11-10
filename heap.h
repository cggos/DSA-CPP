//************************   heap.h   ******************************

#ifndef HEAP_CLASS
#define HEAP_CLASS

#include <fstream>

class Cell {
public:
    bool atom, marked;
    int prev, next;
    Cell() {
        prev = next = info.links[0] = info.links[1] = -1;
    }
    union {
        int value;     // value for atom,
        int links[2];  // head and tail for non-atom;
    } info;
};

class Heap {
public:
    int rootCnt;
    Heap();
    void updateHead(int p, int q) {         // Lisp's rplaca;
        if (roots[p] != empty && !atom(roots[p]))
            Head(roots[p]) = roots[q];
    }
    void updateTail(int p, int q) {         // Lisp's rplacd;
        if (roots[p] != empty && !atom(roots[p]))
            Tail(roots[p]) = roots[q];
    }
    void allocateAtom(int,int);
    void allocateList(int,int,int);
    void deallocate(int);
    void printList(int,char*);
private:
    const int empty, OK, head, tail, maxHeap, maxRoot;
    Cell *heap;
    int *roots, freeCells, nonFreeCells;
    int& Head(int p) {
        return heap[p].info.links[head];
    }
    int& Tail(int p) {
        return heap[p].info.links[tail];
    }
    int& value(int p) {
        return heap[p].info.value;
    }
    int& prev(int p) {
        return heap[p].prev;
    }
    int& next(int p) {
        return heap[p].next;
    }
    bool& atom(int p) {
        return heap[p].atom;
    }
    bool& marked(int p) {
        return heap[p].marked;
    }
    void insert(int,int&);
    void detach(int,int&);
    void transfer(int cell, int& list1, int& list2) {
        detach(cell,list1); insert(cell,list2);
    }
    void collect();
    int allocateAux(int);
    friend ostream& operator<< (ostream&,Heap&);
};

Heap::Heap() : empty(-1), OK(1), head(0), tail(1), maxHeap(6), maxRoot(50) {
    freeCells = nonFreeCells = empty;
    rootCnt = 0;
    heap = new Cell[maxHeap];
    roots = new int[maxRoot];
    int i;
    for (i = maxRoot-1; i >= 0; i--) 
        roots[i] = empty;
    for (i = maxHeap-1; i >= 0; i--) {
        insert(i,freeCells);
        marked(i) = false;
    }
}

void Heap::detach(int cell, int& list) {
    if (next(cell) != empty)
        prev(next(cell)) = prev(cell);
    if (prev(cell) != empty)
        next(prev(cell)) = next(cell);
    if (cell == list)                 // head of the list;
        list = next(cell);
}

void Heap::insert(int cell, int& list) {
    prev(cell) = empty;
    if (cell == list)      // don't create a circular list;
         next(cell) = empty;
    else next(cell) = list;
    if (list != empty)
        prev(list) = cell;
    list = cell;
}

void Heap::collect() {
    int p, markDescendants = empty, markedCells = empty;
    for (p = 0; p < rootCnt; p++) {
        if (roots[p] != empty) {
            transfer(roots[p],nonFreeCells,markDescendants);
            marked(roots[p]) = true;
        }
    }
    printList(markDescendants,"markDescendants");
    for (p = markDescendants; p != empty; p = markDescendants) {
        transfer(p,markDescendants,markedCells);
        if (!atom(p) && !marked(Head(p))) {
            transfer(Head(p),nonFreeCells,markDescendants);
            marked(Head(p)) = true;
        }
        if (!atom(p) && !marked(Tail(p))) {
            transfer(Tail(p),nonFreeCells,markDescendants);
            marked(Tail(p)) = true;
        }
    }
    cout << *this;
    printList(markedCells,"markedCells");
    for (p = markedCells; p != empty; p = next(p))
        marked(p) = false;
    freeCells = nonFreeCells;
    nonFreeCells = markedCells;
}

int Heap::allocateAux(int p) {
    if (p == maxRoot) {
         cout << "No room for new roots\n";
         return !OK;
    }
    if (freeCells == empty)
         collect();
    if (freeCells == empty) {
         cout << "No room in heap for new cells\n";
         return !OK;
    }
    if (p == rootCnt)
         roots[rootCnt++] = p;
    roots[p] = freeCells;
    transfer(freeCells,freeCells,nonFreeCells);
    return OK;
}

void Heap::allocateAtom (int p, int val) {    // an instance of Lisp's setf;
    if (allocateAux(p) == OK) {
        atom(roots[p]) = true;
        value(roots[p]) = val;
    }
}

void Heap::allocateList(int p, int q, int r) { // Lisp's cons;
    if (allocateAux(p) == OK) {
        atom(roots[p]) = false;
        Head(roots[p]) = roots[q];
        Tail(roots[p]) = roots[r];
    }
}

void Heap::deallocate(int p) {
    if (rootCnt > 0)
        if (rand() % 2 == 0)
             roots[p] = roots[--rootCnt]; // remove variable when exiting a block;
        else roots[p] = empty; // set variable to null;
}

void Heap::printList(int list, char *name) {
    cout << name << ": ";
    for (int i = list; i != empty; i = next(i))
        cout << "(" << i << " " << Head(i) << " " << Tail(i) << ")";
    cout << endl;
}

ostream& operator<< (ostream& out, Heap& h) {
    int i;
    cout << "roots: ";
    for (i = 0; i < h.rootCnt; i++)
        cout << h.roots[i] << " ";
    cout << endl;
    for (i = 0; i < h.maxHeap; i++)
        cout << "(" << i << ": " << h.prev(i) << " " << h.next(i) 
             << h.atom(i) << " " << h.marked(i) << " "  
             << " " << h.Head(i) << " " << h.Tail(i) << ") ";
    cout << endl;
    h.printList(h.freeCells,"freeCells");
    h.printList(h.nonFreeCells,"nonFreeCells");
    return out;
}

#endif

