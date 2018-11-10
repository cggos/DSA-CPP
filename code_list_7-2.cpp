#include <iostream>
#include <map>

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
};

int main() {
    pair<Person,char*> p[] =
         {pair<Person,char*>(Person("Gregg",25),"Pittsburgh"),
          pair<Person,char*>(Person("Ann",30),"Boston"),
          pair<Person,char*>(Person("Bill",20),"Belmont")};
    map<Person,char*> cities;
    cities.insert(p[0]);
    cities.insert(p[1]);
    cities.insert(p[2]);
    cities[Person("Kay",40)] = "New York";
    cities["Jenny"] = "Newark";
    cities.insert(map<Person,char*>::value_type(Person("Kay",40),"Detroit"));
    cities.insert(pair<Person,char*>(Person("Kay",40),"Austin"));
    map<Person,char*>::iterator i;
    for (i = cities.begin(); i != cities.end(); i++)
        cout << (*i).first << ' ' << (*i).second << endl;
    // output:
    //   (Ann,30) Boston 
    //   (Bill,20) Belmont 
    //   (Gregg,25) Pittsburgh 
    //   (Jenny,0) Newark 
    //   (Kay, 40) New York
    cities[p[1].first] = "Chicago";
    for (i = cities.begin(); i != cities.end(); i++)
        cout << (*i).first << ' ' << (*i).second << endl;
    // output:
    //   (Ann,30) Chicago 
    //   (Bill,20) Belmont 
    //   (Gregg,25) Pittsburgh 
    //   (Jenny,0) Newark 
    //   (Kay, 40) New York

    multimap<Person,char*> mCities;
    mCities.insert(p[0]);
    mCities.insert(p[1]);
    mCities.insert(p[2]);
    mCities.insert(pair<Person,char*>(Person("Kay",40),"Austin"));
    mCities.insert(pair<Person,char*>(Person("Kay",40),"Austin"));
    mCities.insert(pair<Person,char*>(Person("Kay",40),"Detroit"));
    multimap<Person,char*>::iterator mi;
    for (mi = mCities.begin(); mi != mCities.end(); mi++)
        cout << (*mi).first << ' ' << (*mi).second << endl;
    // output:
    //   (Ann,30) Boston 
    //   (Bill,20) Belmont 
    //   (Gregg,25) Pittsburgh 
    //   (Kay, 40) Austin
    //   (Kay, 40) Austin
    //   (Kay, 40) Detroit
    (*(mCities.find(Person("Kay",40)))).second = "New York";
    for (mi = mCities.begin(); mi != mCities.end(); mi++)
        cout << (*mi).first << ' ' << (*mi).second << endl;
    // output:
    //   (Ann,30) Boston 
    //   (Bill,20) Belmont 
    //   (Gregg,25) Pittsburgh 
    //   (Kay, 40) New York
    //   (Kay, 40) Austin
    //   (Kay, 40) Detroit
    return 0;
}

