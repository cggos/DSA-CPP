#include <iterator>
#include <iostream>
#include <set>

using namespace std;

class Person {
public:
    Person(char *n = "", int a = 0) {
        name = strdup(n);
        age = a;
    }
    bool operator<(const Person& p) const {
        return strcmp(name,p.name) < 0;
    }
    bool operator==(const Person& p) const {
        return strcmp(name,p.name) == 0 && age == p.age;
    }
private:
    char *name;
    int age;
    friend ostream& operator<< (ostream& out, const Person& p) {
        out << "(" << p.name << ", " << p.age << ") ";
    return out;
    }
    friend class lesserAge;
};

class lesserAge {
public:
    int operator()(const Person& p1, const Person& p2) const {
     	return (p1.age < p2.age);
    }
};

void printSet(const set<int>& s, char *c) {
    cout << c << ": ";
    for (set<int>::const_iterator i = s.begin(); i != s.end(); i++)
	cout << *i << ' ';
    cout << endl;
}

void printMSet(const multiset<int>& s, char *c) {
    cout << c << ": ";
    for (multiset<int>::const_iterator i = s.begin(); i != s.end(); i++)
       	cout << *i << ' ';
    cout << endl;
}

template<class T>
void Union(const set<T>& st1, const set<T>& st2, set<T>& st3) {
    set<T> tmp(st2);
    if (&st1 != &st2)
        for (typename set<T>::const_iterator i = st1.begin(); i != st1.end(); i++)
            tmp.insert(*i);
    tmp.swap(st3);
}

int main() {
    ostream_iterator<int> out(cout," ");
    int a[] = {1,2,3,4,5};
    set<int> st1;
    set<int,greater<int> > st2; 
    st1.insert(6); st1.insert(7); st1.insert(8);
    st2.insert(6); st2.insert(7); st2.insert(8);
    set<int> st3(a,a+5);
    set<int> st4(st3);
    printSet(st1,"st1");        // st1 = (6 7 8)
    cout << "st2: "; copy(st2.begin(),st2.end(),out); cout << endl; // st2 = (7 6 8)
    printSet(st3,"st3");	// st3 = (1 2 3 4 5)
    printSet(st4,"st4");       	// st4 = (1 2 3 4 5)
    pair<set<int>::iterator,bool> pr;
    pr = st1.insert(7);	
    printSet(st1,"st1");        // st1 = (6 7 8), 
    cout << *(pr.first) << ' ' << pr.second  << endl; // pr = (7 false)
    pr = st1.insert(9);	
    printSet(st1,"st1");        // st1 = (6 7 8 9), 
    cout << *(pr.first) << ' ' << pr.second  << endl; // pr = (9 true)
    set<int>::iterator i1 = st1.begin(), i2 = st1.begin();
    bool b1 = st1.key_comp()(*i1,*i1);		
    bool b2 = st1.key_comp()(*i1,*++i2);	
    bool b3 = st2.key_comp()(*i1,*i1);		
    bool b4 = st2.key_comp()(*i1,*i2);		
    cout << b1 << ' ' << b2 << endl;  // b1 = false // b2 = true
    cout << b3 << ' ' << b4 << endl;  // b3 = false // b4 = false
    st1.insert(2); st1.insert(4);
    Union(st1,st3,st4);
    printSet(st1,"st1");	    // st1 = (2 4 6 7 8 9)
    printSet(st3,"st3");	    // st3 = (1 2 3 4 5)
    printSet(st4,"st4");	    // st4 = (1 2 3 4 5 6 7 8 9)

    multiset<int> mst1;
    multiset<int,greater<int> > mst2; 
    mst1.insert(6); mst1.insert(7); mst1.insert(8); 
    mst2.insert(6); mst2.insert(7); mst2.insert(8); 
    multiset<int> mst3(a,a+5);		
    multiset<int> mst4(mst3);		
    printMSet(mst1,"mst1");         // mst1 = (6 7 8)
    cout << "mst2: "; copy(mst2.begin(),mst2.end(),out); cout <<endl; // mst2 = (7 6 8)
    printMSet(mst3,"mst3");         // mst3 = (1 2 3 4 5)
    printMSet(mst4,"mst4");         // mst4 = (1 2 3 4 5)
    multiset<int>::iterator mpr = mst1.insert(7);
    printMSet(mst1,"mst1");         // mst1 = (6 7 7 8)
    cout << *mpr << ' ';	    // 7
    mpr = mst1.insert(9);
    printMSet(mst1,"mst1");         // mst1 = (6 7 7 8 9)
    cout << *mpr << ' ';            // 9
    multiset<int>::iterator i5 = mst1.begin(), i6 = mst1.begin();
    i5++; i6++; i6++;	            // *i5 = 7, *i6 = 7
    cout << *i5 << ' ' << *i6 << endl;	
    b1 = mst1.key_comp()(*i5,*i6);
    cout << b1 << endl;        	    // b1 = false


    ostream_iterator<Person> out2(cout," ");
    Person p[] = {Person("Gregg",25),Person("Ann",30),Person("Bill",20),
				  Person("Gregg",35),Person("Kay",30)};
    set<Person> pSet1(p,p+5);		    
    set<Person,lesserAge> pSet2(p,p+5); 
    cout << "pSet1: "; copy(pSet1.begin(),pSet1.end(),out2); cout << endl;// pSet1 = ((Ann,30) (Bill,20) (Gregg,25) (Kay,30))
    cout << "pSet2: "; copy(pSet2.begin(),pSet2.end(),out2); cout << endl;// pSet2 = ((Bill,20) (Gregg,25) (Ann,30) (Gregg,35))
    cout << pSet1.count("Gregg") << ' ' << pSet2.count(Person("",35)) << endl; // 1	1

    multiset<Person> pSet3(p,p+5);		     
    multiset<Person,lesserAge> pSet4(p,p+5);
    pair<multiset<Person>::iterator,multiset<Person>::iterator> mprP;
    mprP = pSet3.equal_range("Gregg");
    for (multiset<Person>::iterator i = mprP.first; i != mprP.second; i++)
       	cout << *i;							                              // (Gregg,25) (Gregg,35) 
    cout << endl;
    cout << "pSet3: "; copy(pSet3.begin(),pSet3.end(),out2); cout << endl;// pSet3 = ((Ann,30) (Bill,20) (Gregg,25) (Gregg,35) (Kay,30))
    cout << "pSet4: "; copy(pSet4.begin(),pSet4.end(),out2); cout << endl;// pSet4 = ((Bill,20) (Gregg,25) (Ann,30) (Kay,30) (Gregg,35))
    cout << pSet3.count("Gregg") << ' ' << pSet4.count(Person("",30)) << endl; // 2	2
    return 0;
}
