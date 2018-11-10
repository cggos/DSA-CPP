//************************  trie.h  *********************************

class Trie;

class TrieNonLeafNode {
public:
    TrieNonLeafNode() {
    }
    TrieNonLeafNode(char);
private:
    bool leaf, endOfWord;
    char *letters;
    TrieNonLeafNode **ptrs;
    friend class Trie;
};

class TrieLeafNode {
public:
    TrieLeafNode() {
    }
    TrieLeafNode(char*);
private:
    bool leaf;
    char *word;
    friend class Trie;
};

class Trie {
public:
    Trie() : notFound(-1) {
    }
    Trie(char*);
    void printTrie() {
        *prefix = '\0';
        printTrie(0,root,prefix);
    }
    void insert(char*);
    bool wordFound(char*);
private:
    TrieNonLeafNode *root;
    const int notFound;
    char prefix[80];
    int  position(TrieNonLeafNode*,char);
    void addCell(char,TrieNonLeafNode*,int);
    void createLeaf(char,char*,TrieNonLeafNode*);
    void printTrie(int,TrieNonLeafNode*,char*);
};

