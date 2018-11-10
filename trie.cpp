//************************  trie.cpp  *********************************

#include <iostream>
#include <cstring>
#include <cstdlib>
#include "trie.h"

using namespace std;

TrieLeafNode::TrieLeafNode(char *suffix) {
    leaf = true;
    word = new char[strlen(suffix)+1];
    if (word == 0) {
        cerr << "Out of memory2.\n";
        exit(-1);
    }
    strcpy(word,suffix);
}

TrieNonLeafNode::TrieNonLeafNode(char ch) {
    ptrs = new TrieNonLeafNode*;
    letters = new char[2];
    if (ptrs == 0 || letters == 0) {
        cerr << "Out of memory3.\n";
        exit(1);
    }
    leaf = false;
    endOfWord = false;
    *ptrs = 0;
    *letters = ch;
    *(letters+1) = '\0';
}

Trie::Trie(char* word) : notFound(-1) {
    root = new TrieNonLeafNode(*word); // initialize the root
    createLeaf(*word,word+1,root); // to avoid later tests;
}

void Trie::printTrie(int depth, TrieNonLeafNode *p, char *prefix) {
    register int i;             // assumption: the root is not a leaf
    if (p->leaf) {              // and it is not null;
         TrieLeafNode *lf = (TrieLeafNode*) p;
         for (i = 1; i <= depth; i++)
             cout << "   ";
         cout << " >>" << prefix << "|" << lf->word << endl;
    }
    else {
         for (i = strlen(p->letters)-1; i >= 0; i--)
             if (p->ptrs[i] != 0) {             // add the letter
                 prefix[depth] = p->letters[i]; // corresponding to
                 prefix[depth+1] = '\0';        // position i to prefix;
                 printTrie(depth+1,p->ptrs[i],prefix);
             }
         if (p->endOfWord) {
              prefix[depth] = '\0';
              for (i = 1; i <= depth+1; i++)
                  cout << "   ";
              cout << ">>>" << prefix << "\n";
         }
    }
}

int Trie::position(TrieNonLeafNode *p, char ch) {
    int i;
    for (i = 0; i < strlen(p->letters) && p->letters[i] != ch; i++);
    if (i < strlen(p->letters))
         return i;
    else return notFound;
}

bool Trie::wordFound(char *word) {
    TrieNonLeafNode *p = root;
    TrieLeafNode *lf;
    int pos;
    while (true)
        if (p->leaf) {                      // node p is a leaf
            lf = (TrieLeafNode*) p;         // where the matching
            if (strcmp(word,lf->word) == 0) // suffix of word
                 return true;               // should be found;
            else return false;
        }
        else if (*word == '\0')             // the end of word has
             if (p->endOfWord)              // to correspond with
                  return true;              // the endOfWord marker
             else return false;             // in node p set to true;
        else if ((pos = position(p,*word)) != notFound &&
                 p->ptrs[pos] != 0) {       // continue
             p = p->ptrs[pos];              // path, if possible,
             word++;
        }
        else return false;                  // otherwise failure;
}

void Trie::addCell(char ch, TrieNonLeafNode *p, int stop) {
    int i, len = strlen(p->letters);
    char *s = p->letters;
    TrieNonLeafNode **tmp = p->ptrs;
    p->letters = new char[len+2];
    p->ptrs    = new TrieNonLeafNode*[len+1];
    if (p->letters == 0 || p->ptrs == 0) {
        cerr << "Out of memory1.\n";
        exit(1);
    }
    for (i = 0; i < len+1; i++)
        p->ptrs[i] = 0;
    if (stop < len)                // if ch does not follow all letters in p,
         for (i = len; i >= stop+1; i--) { // copy from tmp letters > ch;
             p->ptrs[i]    = tmp[i-1];
             p->letters[i] = s[i-1];
         }
    p->letters[stop] = ch;
    for (i = stop-1; i >= 0; i--) {           // and letters < ch;
        p->ptrs[i]    = tmp[i];
        p->letters[i] = s[i];
    }
    p->letters[len+1] = '\0';
    delete [] s;
}

void Trie::createLeaf(char ch, char *suffix, TrieNonLeafNode *p) {
    int pos = position(p,ch);
    if (pos == notFound) {
        for (pos = 0; pos < strlen(p->letters) &&
                      p->letters[pos] < ch; pos++);
        addCell(ch,p,pos);
    }
    p->ptrs[pos] = (TrieNonLeafNode*) new TrieLeafNode(suffix);
}

void Trie::insert(char *word) {
    TrieNonLeafNode *p = root;
    TrieLeafNode *lf;
    int offset, pos;
    char *hold = word;
    while (true) {
        if (*word == '\0') {            // if the end of word reached,
             if (p->endOfWord)
                  cout << "Duplicate entry1 " << hold << endl;
             else p->endOfWord = true;  // set endOfWord to true;
             return;
        }                               // if position in p indicated
        pos = position(p,*word);
        if (pos == notFound) {          // by the first letter of word
             createLeaf(*word,word+1,p);// does not exist, create
             return;                    // a leaf and store in it the
        }                               // unprocessed suffix of word;
        else if (pos != notFound &&     // if position *word is
                 p->ptrs[pos]->leaf) {  // occupied by a leaf,
             lf = (TrieLeafNode*) p->ptrs[pos];    // hold this leaf;
             if (strcmp(lf->word,word+1) == 0) {
                 cout << "Duplicate entry2 " << hold << endl;
                 return;
             }
             offset = 0;
             // create as many non-leaves as the length of identical
             // prefix of word and the string in the leaf (for cell `R',
             // leaf `EP', and word `REAR', two such nodes are created);
             do {
                 pos = position(p,word[offset]);
                 // word == "ABC", leaf = "ABCDEF" => leaf = "DEF";
                 if (strlen(word) == offset+1) {
                      p->ptrs[pos] = new TrieNonLeafNode(word[offset]);
                      p->ptrs[pos]->endOfWord = true;
                      createLeaf(lf->word[offset],lf->word + offset+1,p->ptrs[pos]);
                      return;
                 }
                 // word == "ABCDE", leaf = "ABC" => leaf = "DEF";
                 else if (strlen(lf->word) == offset) {
                      p->ptrs[pos] = new TrieNonLeafNode(word[offset+1]);
                      p->ptrs[pos]->endOfWord = true;
                      createLeaf(word[offset+1],word+offset+2,p->ptrs[pos]);
                      return;
                 }
                 p->ptrs[pos] = new TrieNonLeafNode(word[offset+1]);
                 p = p->ptrs[pos];
                 offset++;
             } while (word[offset] == lf->word[offset-1]);
             offset--;
             // word = "ABCDEF", leaf = "ABCPQR" =>
             //    leaf('D') = "EF", leaf('P') = "QR";
             // check whether there is a suffix left:
             // word = "ABCD", leaf = "ABCPQR" =>
             //    leaf('D') = null, leaf('P') = "QR";
             char *s = "";
             if (strlen(word) > offset+2)
                  s = word+offset+2;
             createLeaf(word[offset+1],s,p);
             // check whether there is a suffix left:
             // word = "ABCDEF", leaf = "ABCP" =>
             //    leaf('D') = "EF", leaf('P') = null;
             if (strlen(lf->word) > offset+1)
                  s = lf->word+offset+1;
             else s = "";
             createLeaf(lf->word[offset],s,p);
             delete [] lf->word;
             delete lf;
             return;
        }
        else {
             p = p->ptrs[pos];
             word++;
        }
    }
}

