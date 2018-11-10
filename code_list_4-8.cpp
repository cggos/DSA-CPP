#include <iostream>
#include <list>
#include <algorithm>
#include <deque>

using namespace std;

template<class T>
void printDeque(const deque<T>& lst, char *s) {
    cout << s << ":  ";
    typename deque<T>::const_iterator i = lst.begin(); 
    for ( ; i != lst.end(); i++)
       	cout << *i << ' ';
    cout << endl;
}

int main() {
    deque<int> dq1;
    dq1.push_front(1);         // dq1 = (1)
    dq1.push_front(2);         // dq1 = (2 1)
    dq1.push_back(3);          // dq1 = (2 1 3)
    dq1.push_back(4);          // dq1 = (2 1 3 4)
    printDeque(dq1,"dq1");
    deque<int> dq2(dq1.begin()+1,dq1.end()-1); 
    printDeque(dq2,"dq2");     // dq2 = (1 3)
    dq1[1] = 5;
    printDeque(dq1,"dq1");     // dq1 = (2 5 3 4)
    dq1.erase(dq1.begin()); 
    printDeque(dq1,"dq1");     // dq1 = (5 3 4)
    dq1.insert(dq1.end()-1,2,6);
    printDeque(dq1,"dq1");     // dq1 = (5 3 6 6 4)
    sort(dq1.begin(),dq1.end()); 
    printDeque(dq1,"dq1");     // dq1 = (3 4 5 6 6)
    deque<int> dq3;
    dq3.resize(dq1.size()+dq2.size()); 
    printDeque(dq3,"dq3");     // dq3 = (0 0 0 0 0 0 0)
    merge(dq1.begin(),dq1.end(),dq2.begin(),dq2.end(),dq3.begin()); 
    // dq1 = (3 4 5 6 6) and dq2 = (1 3) ==> dq3 = (1 3 3 4 5 6 6)
    printDeque(dq1,"dq1");     // dq1 = (3 4 5 6 6)
    printDeque(dq2,"dq2");     // dq2 = (1 3)
    printDeque(dq3,"dq3");     // dq3 = (1 3 3 4 5 6 6)
    return 0;
}
