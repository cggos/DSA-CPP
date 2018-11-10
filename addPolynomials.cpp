#include <iostream>
#include <cctype>
#include <cstdlib>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

class Variable {
public:
    char id;
    int exp;
    Variable() { // required by <vector>;
    }
    Variable(char c, int i) {
        id = c; exp = i;
    }
    bool operator== (const Variable& v) const { 
        return id  == v.id  && exp == v.exp;
    }
    bool operator< (const Variable& v) const { // used by sort();
        return id < v.id;
    }
};

class Term {
public:
    Term() {
        coeff = 0;
    }
    int coeff;
    vector<Variable> vars;
    bool operator== (const Term&) const;
    bool operator!= (const Term& term) const { // required by <list>
        return !(*this == term);
    }
    bool operator< (const Term&) const;
    bool operator> (const Term& term) const {  // required by <list>
        return *this != term && (*this < term);
    }
    int min(int n, int m) const {
        return (n < m) ? n : m;
    }
};

class Polynomial {
public:
    Polynomial() {
    }
    Polynomial operator+ (Polynomial&);
    void error(char *s) {
        cerr << s << endl; exit(1);
    }
private:
    list<Term> terms;
    friend istream& operator>> (istream& in, Polynomial& polyn) {
        char ch, sign, coeffUsed, id;
        int exp;
        Term term;
        in >> ch;
        while (true) {
            coeffUsed = 0;
            if (!isalnum(ch) && ch != ';' && ch != '-' && ch != '+')
                 polyn.error("Wrong character entered2");
            sign = 1;
            while (ch == '-' || ch == '+') { // first get sign(s) of Term
                 if (ch == '-')
                       sign *= -1;
                 ch = in.get();
                 if (isspace(ch))
                       in >> ch;
            }
            if (isdigit(ch)) {              // and then its coefficient;
                 in.putback(ch);
                 in >> term.coeff;
                 ch = in.get();
                 term.coeff *= sign;
                 coeffUsed = 1;
            }
            else term.coeff = sign;
	    int i;
            for (int i = 0; isalnum(ch); i++) { // process this term:
                id = ch;                    // get a variable name
                ch = in.get();
                if (isdigit(ch)) {          // and an exponent (if any);
                     in.putback(ch);
                     in >> exp >> ch;
                }
                else exp = 1;
                term.vars.push_back(Variable(id,exp));
            }
            polyn.terms.push_back(term);    // and include it in the linked list;
            term.vars.resize(0);    
            if (isspace(ch))
                 in >> ch;
            if (ch == ';')                  // finish if a semicolon is entered;
                 if (coeffUsed || i > 0)
                      break;
                 else polyn.error("Term is missing");  // e.g., 2x - ; or just ';'
            else if (ch != '-' && ch != '+')           // e.g., 2x  4y;
                 polyn.error("wrong character entered");
        }
        for (list<Term>::iterator it = polyn.terms.begin(); it != polyn.terms.end(); it++)
            if (it->vars.size() > 1)
                sort(it->vars.begin(),it->vars.end());
        return in;
    }

    friend ostream& operator<< (ostream& out, const Polynomial& polyn) {
        int afterFirstTerm = 0, i;
        for (list<Term>::const_iterator pol = polyn.terms.begin(); pol != polyn.terms.end(); pol++) {
            out.put(' ');
            if (pol->coeff < 0)             // put '-' before polynomial
                 out.put('-');              // and between terms (if needed);
            else if (afterFirstTerm)        // don't put '+' in front of
                 out.put('+');              // polynomial;
            afterFirstTerm++;
            if (abs(pol->coeff) != 1)       // print a coefficient
                 out << ' ' << abs(pol->coeff);// if it is not 1 nor -1, or
            else if (pol->vars.size() == 0) // the term has only a coefficient
                 out << " 1";               
            else out.put(' ');
            for (i = 1; i <= pol->vars.size(); i++) {
                 out << pol->vars[i-1].id;       // print a variable name
                 if (pol->vars[i-1].exp != 1)    // and an exponent, only
                      out << pol->vars[i-1].exp; // if it is not 1;
            }
        }
        out << endl;
        return out;
    }
};

// two terms are equal if all varibles are the same and
// corresponding variables are raised to the same powers;
// the first cell of the node containing a term is excluded
// from comparison, since it stores coefficient of the term;

bool Term::operator== (const Term& term) const {
    int i;
    for (i = 0; i < min(vars.size(),term.vars.size()) &&
                    vars[i] == term.vars[i]; i++);
    return i == vars.size() && vars.size() == term.vars.size();
}

bool Term::operator< (const Term& term2) const { // used by sort();
    if (vars.size() == 0)
        return false;           // *this is just a coefficient;
    else if (term2.vars.size() == 0)
        return true;            // term2 is just a coefficient;
    for (int i = 0; i < min(vars.size(),term2.vars.size()); i++)
        if (vars[i].id < term2.vars[i].id)
             return true;       // *this precedes term2;
        else if (term2.vars[i].id < vars[i].id)
             return false;      // term2 precedes *this;
        else if (vars[i].exp < term2.vars[i].exp)
             return true;       // *this precedes term2;
        else if (term2.vars[i].exp < vars[i].exp)
             return false;      // term2 precedes *this;
    return ((int)vars.size() - (int)term2.vars.size() < 0) ? true : false;
}

Polynomial Polynomial::operator+ (Polynomial& polyn2) {
    Polynomial result;
    list<Term>::iterator p1, p2;
    bool erased;
    for (p1 = terms.begin(); p1 != terms.end(); p1++) // create a new polyn
        result.terms.push_back(*p1);                  // from copies of *this
    for (p1 = polyn2.terms.begin(); p1 != polyn2.terms.end(); p1++) // and
        result.terms.push_back(*p1);                  // polyn2;
    for (p1 = result.terms.begin(); p1 != result.terms.end(); ) {
        for (p2 = p1, p2++, erased = false; p2 != result.terms.end(); p2++)
            if (*p1 == *p2) {             // if two terms are equal (except
                 p1->coeff += p2->coeff;  // for the coefficient), add the
                 result.terms.erase(p2);  // two coefficients and erase
                 if (p1->coeff == 0)      // a redundant term; if the 
                     result.terms.erase(p1);// coefficient in retained term 
                 erased = true;             // is zero, erase the term as well;
                 break;
            }
        if (erased)        // restart processing from the beginning
             p1 = result.terms.begin();  // if any node was erased;
        else p1++;
    }
    result.terms.sort();
    return result;
}

int main() {
    Polynomial polyn1, polyn2;
    cout << "Enter two polynomials, each ended with a semicolon:\n";
    cin  >> polyn1 >> polyn2;
    cout << "The result is:\n" << polyn1 + polyn2;
    return 0;
}
