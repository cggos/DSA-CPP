#include <iostream>
#include <list>
#include <algorithm>
#include <deque>
#include <functional>

using namespace std;

template<class T>
void printList(const list<T>& lst, char *s) {
    cout << s << ":  ";
    for (typename list<T>::const_iterator i = lst.begin(); i != lst.end(); i++)
       cout << *i << ' ';
    cout << endl;
}

int main() {
    list<int> lst1;                         
    printList(lst1,"lst1");     // lst1 is empty
    list<int> lst2(3,7);            
    printList(lst2,"lst2");     // lst2 = (7 7 7)
    for (int j = 1; j <= 5; j++)// lst1 = (1 2 3 4 5)
        lst1.push_back(j);
    list<int>::iterator i1 = lst1.begin(), i2 = i1, i3;
    i2++; i2++; i2++;
    list<int> lst3(++i1,i2);        
    printList(lst3,"lst3");     // lst3 = (2 3)
    list<int> lst4(lst1);
    printList(lst4,"lst4");     // lst4 = (1 2 3 4 5)
    i1 = lst4.begin();
    lst4.splice(++i1,lst2);         
    printList(lst2,"lst2");     // lst2 is empty
    printList(lst4,"lst4");     // lst4 = (1 7 7 7 2 3 4 5)
    lst2 = lst1;
    printList(lst2,"lst2");     // lst2 = (1 2 3 4 5)
    i2 = lst2.begin();
    lst4.splice(i1,lst2,++i2);      
    printList(lst2,"lst2");     // lst2 = (1 3 4 5)
    printList(lst4,"lst4");     // lst4 = (1 7 7 7 2 2 3 4 5)
    i2 = lst2.begin();
    i3 = i2;
    lst4.splice(i1,lst2,i2,++i3);
    printList(lst2,"lst2");     // lst2 = (3 4 5)
    printList(lst4,"lst4");     // lst4 = (1 7 7 7 2 1 2 3 4 5)
    lst4.remove(1);         
    printList(lst4,"lst4");     // lst4 = (7 7 7 2 2 3 4 5)
    lst4.sort();                            
    printList(lst4,"lst4");     // lst4 = (2 2 3 4 5 7 7 7)
    lst4.unique();                          
    printList(lst4,"lst4");     // lst4 = (2 3 4 5 7)
    lst1.merge(lst2);                        
    printList(lst1,"lst1");     // lst1 = (1 2 3 3 4 4 5 5),
    printList(lst2,"lst2");     // lst2 is empty
    lst3.reverse();                         
    printList(lst3,"lst3");     // lst3 = (3 2)     
    lst4.reverse();
    printList(lst4,"lst4");     // lst4 = (7 5 4 3 2)
    lst3.merge(lst4,greater<int>());  
    printList(lst3,"lst3");     // lst3 = (7 5 4 3 3 2 2)
    printList(lst4,"lst4");     // lst4 is empty
    lst3.remove_if(bind2nd(not_equal_to<int>(),3));
    printList(lst3,"lst3");     // lst3 = (3 3)
    lst3.unique(not_equal_to<int>());
    printList(lst3,"lst3");     // lst3 = (3 3)
    return 0;
}
