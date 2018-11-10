#include <vector>
#include <algorithm>

class HuffmanNode {
public:
    char symbol;
    unsigned long codeword, freq;
    unsigned int runLen, codewordLen;
    HuffmanNode *left, *right;
    HuffmanNode() {
        left = right = 0;
    }
    HuffmanNode(char s, unsigned long f, unsigned int r, 
        HuffmanNode *lt = 0, HuffmanNode *rt = 0) {
        symbol = s; freq = f; runLen = r; left = lt; right = rt;
    }
};

class ListNode {
public:
    HuffmanNode *tree;
    ListNode *next, *prev;
    ListNode() {
        next = prev = 0;
    }
    ListNode(ListNode *p, ListNode *n) {
        prev = p; next = n;
    }
};

class DataRec {
public:
    char symbol;
    unsigned int runLen;
    unsigned long freq;
    DataRec() {
    }
    bool operator== (const DataRec& dr) const { // used by find();
        return symbol == dr.symbol && runLen == dr.runLen;
    }
    bool operator< (const DataRec& dr) const {  // used by sort();
        return freq < dr.freq;
    }
};

class HuffmanCoding {
public:
    HuffmanCoding() : mask(0xff), bytes(4), bits(8), ASCII(256) {
        chars = new HuffmanNode*[ASCII+1];
    }
    void compress(char*,ifstream&);
    void decompress(char*,ifstream&);
private:
    const unsigned int bytes, bits, ASCII;
    unsigned int dataSize;
    const unsigned long mask;
    unsigned long charCnt;
    ofstream fOut;
    HuffmanNode *HuffmanTree, **chars;
    vector<DataRec> data;
    void error(char *s) {
        cerr << s << endl; exit(1);
    }
    void output(unsigned long pack);
    void garnerData(ifstream&);
    void outputFrequencies(ifstream&);
    void read2ByteNum(unsigned int&,ifstream&);
    void read4ByteNum(unsigned long&,ifstream&);
    void inputFrequencies(ifstream&);
    void createHuffmanTree();
    void createCodewords(HuffmanNode*,unsigned long,int);
    void transformTreeToArrayOfLists(HuffmanNode*);
    void encode(ifstream&);
    void decode(ifstream&);
};

void HuffmanCoding::output(unsigned long pack) {
    char *s = new char[bytes];
    for (int i = bytes - 1; i >= 0; i--) {
        s[i] = pack & mask;
        pack >>= bits;
    }
    for (int i = 0; i < bytes; i++)
        fOut.put(s[i]);
}

void HuffmanCoding::garnerData(ifstream& fIn) {
    char ch, ch2;
    DataRec r;
    vector<DataRec>::iterator i;
    r.freq = 1;
    for (fIn.get(ch); !fIn.eof(); ch = ch2) {
        for (r.runLen = 1, fIn.get(ch2); !fIn.eof() && ch2 == ch; r.runLen++)
            fIn.get(ch2);
        r.symbol = ch;
        if ((i = find(data.begin(),data.end(),r)) == data.end())
             data.push_back(r);
        else i->freq++;
    }
    sort(data.begin(),data.end());
}

void HuffmanCoding::outputFrequencies(ifstream& fIn) {
    unsigned long temp4;
    char ch = data.size(); 
    unsigned int temp2 = data.size();
    temp2 >>= bits;
    fOut.put(char(temp2)).put(ch);
    fIn.clear();
    output((unsigned long)fIn.tellg());
    for (int j = 0; j < data.size(); j++) {
        fOut.put(data[j].symbol);
        ch = temp2 = data[j].runLen;
        temp2 >>= bits;
        fOut.put(char(temp2)).put(ch);
        temp4 = data[j].freq;
        output(temp4);
    }
}

void HuffmanCoding::read2ByteNum(unsigned int& num, ifstream& fIn) {
    num = fIn.get();
    num <<= bits;
    num |= fIn.get();
}

void HuffmanCoding::read4ByteNum(unsigned long& num, ifstream& fIn) {
    num = (unsigned long) fIn.get();
    for (int i = 1; i < 4; i++) {
        num <<= bits;
        num |= (unsigned long) fIn.get();
    }
}

void HuffmanCoding::inputFrequencies(ifstream& fIn) {
    DataRec r;
    read2ByteNum(dataSize,fIn);
    read4ByteNum(charCnt,fIn);
    data.reserve(dataSize);
    for (int j = 0; !fIn.eof() && j < dataSize; j++) {
        r.symbol = fIn.get();
        read2ByteNum(r.runLen,fIn);
        read4ByteNum(r.freq,fIn);
        data.push_back(r);
    }
}

void HuffmanCoding::createHuffmanTree() {
    ListNode *p, *newNode, *head, *tail;
    unsigned long newFreq;   
    head = tail = new ListNode;             // initialize list pointers;
    head->tree = new HuffmanNode(data[0].symbol,data[0].freq,data[0].runLen);
    for (int i = 1; i < data.size(); i++) { // create the rest of the list;
        tail->next = new ListNode(tail,0);
        tail = tail->next;
        tail->tree =
            new HuffmanNode(data[i].symbol,data[i].freq,data[i].runLen);
    }
    while (head != tail) {                  // create one Huffman tree;
        newFreq = head->tree->freq + head->next->tree->freq; // two lowest frequencies
        for (p = tail; p != 0 && p->tree->freq > newFreq; p = p->prev);
        newNode = new ListNode(p,p->next);
        p->next = newNode;
        if (p == tail)
             tail = newNode;
        else newNode->next->prev = newNode;
        newNode->tree =
            new HuffmanNode('\0',newFreq,0,head->tree,head->next->tree);
        head = head->next->next;
        delete head->prev->prev;
        delete head->prev;
        head->prev = 0;
    }
    HuffmanTree = head->tree;
    delete head;
}

void HuffmanCoding::createCodewords(HuffmanNode *p, unsigned long codeword, int level) {
    if (p->left == 0 && p->right == 0) {         // if p is a leaf,
         p->codeword    = codeword;              // store codeword
         p->codewordLen = level;                 // and its lenght,
    }
    else {                                       // otherwise add 0
         createCodewords(p->left,  codeword<<1,   level+1); // for left branch
         createCodewords(p->right,(codeword<<1)+1,level+1); // and 1 for right;
    }
}

void HuffmanCoding::transformTreeToArrayOfLists(HuffmanNode *p) {
    if (p->left == 0 && p->right == 0) {       // if p is a leaf,
         p->right = chars[(unsigned char)p->symbol]; // include it in
         chars[(unsigned char)p->symbol] = p;  // a list associated
    }                                          // with symbol found in p;
    else {
         transformTreeToArrayOfLists(p->left);
         transformTreeToArrayOfLists(p->right);
    }
}

void HuffmanCoding::encode(ifstream& fIn) {
    unsigned long packCnt = 0, hold, maxPack = bytes*bits, pack = 0;
    char ch, ch2;
    int bitsLeft, runLength;
    HuffmanNode *p;
    for (fIn.get(ch); !fIn.eof(); ) {
        for (runLength = 1, fIn.get(ch2); !fIn.eof() && ch2 == ch; runLength++)
            fIn.get(ch2);
        for (p = chars[(unsigned char) ch];
             p != 0 && runLength != p->runLen; p = p->right)
            ;
        if (p == 0)
             error("A problem in encode()");
        if (p->codewordLen < maxPack - packCnt) {   // if enough room in
             pack = (pack << p->codewordLen) | p->codeword; // pack to store new
             packCnt += p->codewordLen;             // codeword, shift its
        }                                           // content to the left
                                                    // and attach new codeword;
        else {                                      // otherwise move
             bitsLeft = maxPack - packCnt;          // pack's content to
             pack <<= bitsLeft;                     // the left by the
             if (bitsLeft != p->codewordLen) {      // number of left
                  hold = p->codeword;               // spaces and if new
                  hold >>= p->codewordLen - bitsLeft;// codeword is longer than
                  pack |= hold;                     // room left, transfer
             }                                      // only as many bits as
                                                    // can be fitted in pack;
             else pack |= p->codeword;              // if new codeword
                                                    // exactly fits in
                                                    // pack, transfer it;
             output(pack);                          // output pack as
                                                    // four chars;
             if (bitsLeft != p->codewordLen) {      // transfer
                  pack = p->codeword;               // unprocessed bits
                  packCnt = maxPack - (p->codewordLen - bitsLeft); // of new
                  packCnt = p->codewordLen - bitsLeft; // codeword to pack;
             }
             else packCnt = 0;
        }
        ch = ch2;
    }
    if (packCnt != 0) {
        pack <<= maxPack - packCnt; // transfer leftover codewords and some 0's
        output(pack);
    }
}

void HuffmanCoding::compress(char *inFileName, ifstream& fIn) {
    char outFileName[30];
    strcpy(outFileName,inFileName);
    if (strchr(outFileName,'.'))               // if there is an extension
         strcpy(strchr(outFileName,'.')+1,"z");// overwrite it with 'z'
    else strcat(outFileName,".z");             // else add extension '.z';
    fOut.open(outFileName,ios::out);
//use this line on a PC:
//    fOut.open(outFileName,ios::out|ios::binary);
    garnerData(fIn);
    outputFrequencies(fIn);
    createHuffmanTree();
    createCodewords(HuffmanTree,0,0);
    for (int i = 0; i <= ASCII; i++)
        chars[i] = 0;
    transformTreeToArrayOfLists(HuffmanTree);
    fIn.clear();      // clear especially the eof flag;
    fIn.seekg(0,ios::beg);
    encode(fIn);
    fIn.clear();      // clear especially the eof flag;
    cout.precision(2);
    cout << fixed << "Compression rate = " <<
            100.0*(fIn.tellg()-fOut.tellp())/fIn.tellg() << "%\n"
         << "Compression rate without table = " <<
            100.0*(fIn.tellg()-fOut.tellp()+long(data.size()*(2+4)))/fIn.tellg() << endl;
    fOut.close();
}

void HuffmanCoding::decode(ifstream& fIn) {
    unsigned long chars;
    char ch, bitCnt = 1, mask = 1;
    mask <<= bits - 1;        // change 00000001 to 100000000;
    for (chars = 0, fIn.get(ch); !fIn.eof() && chars < charCnt; ) {
        for (HuffmanNode *p = HuffmanTree; ; ) {
            if (p->left == 0 && p->right == 0) {
                for (int j = 0; j < p->runLen; j++)
                    fOut.put(p->symbol);
                chars += p->runLen;
                break;
            }
            else if ((ch & mask) == 0)
                 p = p->left;
            else p = p->right;
            if (bitCnt++ == bits) { // read next character from fIn
                 fIn.get(ch);       // if all bits in ch are checked;
                 bitCnt = 1;
            }                       // otherwise move all bits in ch
            else ch <<= 1;          // to the left by one position;
        }
    }
}

void HuffmanCoding::decompress(char *inFileName, ifstream& fIn) {
    char outFileName[30];
    strcpy(outFileName,inFileName);
    if (strchr(outFileName,'.'))                  // if there is an extension
         strcpy(strchr(outFileName,'.')+1,"dec"); // overwrite it with 'z'
    else strcat(outFileName,".dec");              // else add extension '.z';
    fOut.open(outFileName,ios::out);
//use this line on a PC:
//    fOut.open(outFileName,ios::out|ios::binary);
    inputFrequencies(fIn);
    createHuffmanTree();
    createCodewords(HuffmanTree,0,0);
    decode(fIn);
    fOut.close();
}
