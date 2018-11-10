#include <iostream>
#include <vector>
#include <algorithm>
#include <functional> // greater<>

using namespace std;

class Person {
public:
    Person(char *n = "", int a = 0) {
       name = strdup(n);
       age = a;
    }
    bool operator==(const Person& p) const {
        return strcmp(name,p.name) == 0; // && age == p.age;
    }
    bool operator<(const Person& p) const {
        return strcmp(name,p.name) < 0;
    }
private:
    char *name;
    int age;
    friend ostream& operator<< (ostream& out, const Person& p) {
        out << "(" << p.name << "," << p.age << ")";
        return out;
    }
};

bool f1(int n) {
    return n < 5;
}

template<class T>
void printVector(char *s, const vector<T>& v) {
    cout << s << " = (";
    if (v.size() == 0) {
        cout << ")\n";
        return;
    }
    typename vector<T>::const_iterator i;
    for(i = v.begin(); i != v.end()-1; i++)
        cout << *i << ',';
    cout << *i << ")\n";
}

int main() {
    int a[] = {1,4,3,6,7,2,5}, i;
    vector<int> v1(a,a+7), v2(a,a+7), v3(6,9), v4(6,9);
    vector<int>::iterator i1, i2, i3, i4;
    partial_sort(v1.begin(),v1.begin()+3,v1.end()); 
    printVector("v1",v1);                    // v1 = (1,2,3,6,7,4,5)
    partial_sort(v2.begin()+1,v2.begin()+4,v2.end(),greater<int>()); 
    printVector("v2",v2);                    // v2 = (1,7,6,5,3,2,4)
    i3 = partial_sort_copy(v2.begin(),v2.begin()+4,v3.begin(),v3.end()); 
    printVector("v3",v3);                    // v3 = (1,5,6,7,9,9)
    cout << *(i3-1) << ' ' << *i3 << endl;   // 7 9
    i4 = partial_sort_copy(v1.begin(),v1.begin()+4,v4.begin(),v4.end(),greater<int>()); 
    printVector("v4",v4);                    // v4 = (6,3,2,1,9,9)
    cout << *(i4-1) << ' ' << *i4 << endl;   // 1 9
    i1 = partition(v1.begin(),v1.end(),f1);      
    printVector("v1",v1);                    // v1 = (1,2,3,4,7,6,5)
    cout << *(i1-1) << ' ' << *i1 << endl;   // 4 7
    i2 = partition(v2.begin(),v2.end(),bind2nd(less<int>(),5)); 
    printVector("v2",v2);                    // v2 = (1,4,2,3,5,6,7)
    cout << *(i2-1) << ' ' << *i2 << endl;   // 3 5
    sort(v1.begin(),v1.end());                           
    printVector("v1",v1);                    // v1 = (1,2,3,4,5,6,7)
    sort(v1.begin(),v1.end(),greater<int>());
    printVector("v1",v1);                    // v1 = (7,6,5,4,3,2,1)
    vector<Person> pv1, pv2;
    for (i = 0; i < 20; i++) {
        pv1.push_back(Person("Josie",60 - i));
        pv2.push_back(Person("Josie",60 - i));
    }
    printVector("pv1",pv1);       // pv2 = ((Josie,60) ... (Josie,41))
    sort(pv1.begin(),pv1.end());                
    printVector("pv1",pv1);       // pv1 = ((Josie,41) ... (Josie,60))
    printVector("pv2",pv2);       // pv2 = ((Josie,60) ... (Josie,41))
    stable_sort(pv2.begin(),pv2.end()); 
    printVector("pv2",pv2);       // pv2 = ((Josie,60) ... (Josie,41))

    vector<int> heap1, heap2, heap3(a,a+7), heap4(a,a+7);
    for (i = 1; i <= 7; i++) {
        heap1.push_back(i);
        push_heap(heap1.begin(),heap1.end());
        printVector("heap1",heap1);
    }
    // heap1 = (1)
    // heap1 = (2,1)
    // heap1 = (3,1,2)
    // heap1 = (4,3,2,1)
    // heap1 = (5,4,2,1,3)
    // heap1 = (6,4,5,1,3,2)
    // heap1 = (7,4,6,1,3,2,5)
    sort_heap(heap1.begin(),heap1.end()); // heap1 = (1,2,3,4,5,6,7)
    printVector("heap1",heap1);
    for (i = 1; i <= 7; i++) {
        heap2.push_back(i);
        push_heap(heap2.begin(),heap2.end(),greater<int>());
        printVector("heap2",heap2);
    }
    // heap2 = (1)
    // heap2 = (1,2)
    // heap2 = (1,2,3)
    // heap2 = (1,2,3,4)
    // heap2 = (1,2,3,4,5)
    // heap2 = (1,2,3,4,5,6)
    // heap2 = (1,2,3,4,5,6,7)
    sort_heap(heap2.begin(),heap2.end(),greater<int>());
    printVector("heap2",heap2);           // heap2 = (7,6,5,4,3,2,1)
    make_heap(heap3.begin(),heap3.end()); 
    printVector("heap3",heap3);           // heap3 = (7,6,5,1,4,2,3)
    sort_heap(heap3.begin(),heap3.end()); 
    printVector("heap3",heap3);           // heap3 = (1,2,3,4,5,6,7)
    make_heap(heap4.begin(),heap4.end(),greater<int>());
    printVector("heap4",heap4);           // heap4 = (1,4,2,6,7,3,5)
    sort_heap(heap4.begin(),heap4.end(),greater<int>());
    printVector("heap4",heap4);	          // heap4 = (7,6,5,4,3,2,1)
    return 0;
}
