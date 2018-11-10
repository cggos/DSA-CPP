//*********************  genStack.h  *************************
//      generic class for vector implementation of stack

#ifndef STACK
#define STACK

#include <vector>

template<class T, int capacity = 30>
class Stack {
public:
    Stack() {
        pool.reserve(capacity);
    }
    void clear() { 
        pool.clear();
    }
    bool isEmpty() const { 
        return pool.empty();
    }
    T& topEl() { 
        return pool.back();
    }
    T pop() { 
        T el = pool.back();
        pool.pop_back();
        return el;
    }
    void push(const T& el) { 
        pool.push_back(el);
    }
private:
    vector<T> pool;
};

#endif
