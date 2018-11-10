#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <iomanip>
#include <cstdio> // remove(), rename();

using namespace std;

const int bucketSize = 2, tableSize = 3, strLen = 20;
const int recordLen = strLen;

class File {
public:
    File() : empty('*'), delMarker('#') {
    }
    void processFile(char*);
private:
    const char empty, delMarker;
    long *pointers;
    fstream outfile, overflow, sorted;
    unsigned long hash(char*);
    void swap(long& i, long& j) {
        long tmp = i; i = j; j = tmp;
    }
    void getName(char*);
    void insert(char line[]) {
        getName(line); insertion(line);
    }
    void insertion(char*);
    void excise(char*);
    void partition(int,int,int&);
    void QSort(int,int);
    void sortFile();
    void combineFiles();
};

unsigned long File::hash(char *s) {
    unsigned long Xor = 0, pack;
    int i, j, slength; // exclude trailing blanks;
    for (slength = strlen(s); isspace(s[slength-1]); slength--);
    for (i = 0; i < slength; ) {
        for (pack = 0, j = 0; ; j++, i++) {
            pack |= (unsigned long) s[i];     // include s[i] in the rightmost
            if (j == 3 || i == slength - 1) { // byte of pack;
                i++;
                break;
            }
            pack <<= 8;
        }             // xor at one time 8 bytes from s;
        Xor ^= pack;  // last iteration may put less
    }                 // than 8 bytes in pack;
    return (Xor % tableSize) * bucketSize * recordLen;
}// return byte position of home bucket for s;

void File::getName(char line[]) {
    cout << "Enter a name: ";
    cin.getline(line,recordLen+1);
    for (int i = strlen(line); i < recordLen; i++)
        line[i] = ' ';
    line[recordLen] = '\0';
}

void File::insertion(char line[]) {
    int address = hash(line), counter = 0;
    char name[recordLen+1];
    bool done = false, inserted = false;
    outfile.clear();
    outfile.seekg(address,ios::beg);
    while (!done && outfile.get(name,recordLen+1)) {
        if (name[0] == empty || name[0] == delMarker) {
             outfile.clear();
             outfile.seekg(address+counter*recordLen,ios::beg);
             outfile << line << setw(strlen(line)-recordLen);
             done = inserted = true;
        }
        else if (!strcmp(name,line)) {
             cout << line << " is already in the file\n";
             return;
        }
        else counter++;
        if (counter == bucketSize)
             done = true;
        else outfile.seekg(address+counter*recordLen,ios::beg);
    }
    if (!inserted) {
        done = false;
        counter = 0;
        overflow.clear();
        overflow.seekg(0,ios::beg);
        while (!done && overflow.get(name,recordLen+1)) {
            if (name[0] == delMarker)
                 done = true;
            else if (!strcmp(name,line)) {
                 cout << line << " is already in the file\n";
                 return;
            }
            else counter++;
        }
        overflow.clear();
        if (done)
             overflow.seekg(counter*recordLen,ios::beg);
        else overflow.seekg(0,ios::end);
        overflow << line << setw(strlen(line)-recordLen);
    }
}

void File::excise(char line[]) {
    getName(line);
    int address = hash(line), counter = 0;
    bool done = false, removed = false;
    char name2[recordLen+1];
    outfile.clear();
    outfile.seekg(address,ios::beg);
    while (!done && outfile.get(name2,recordLen+1)) {
        if (!strcmp(line,name2)) {
             outfile.clear();
             outfile.seekg(address+counter*recordLen,ios::beg);
             outfile.put(delMarker);
             done = removed = true;
        }
        else counter++;
        if (counter == bucketSize)
             done = true;
        else outfile.seekg(address+counter*recordLen,ios::beg);
    }
    if (!removed) {
        done = false;
        counter = 0;
        overflow.clear();
        overflow.seekg(0,ios::beg);
        while (!done && overflow.get(name2,recordLen+1)) {
            if (!strcmp(line,name2)) {
                 overflow.clear();
                 overflow.seekg(counter*recordLen,ios::beg);
                 overflow.put(delMarker);
                 done = removed = true;
            }
            else counter++;
            overflow.seekg(counter*recordLen,ios::beg);
        }
    }
    if (!removed)
        cout << line << " is not in database\n";
}

void File::partition (int low, int high, int& pivotLoc) {
    char rec[recordLen+1], pivot[recordLen+1];
    register int i, lastSmall;
    swap(pointers[low],pointers[(low+high)/2]);
    outfile.clear();
    outfile.seekg(pointers[low]*recordLen,ios::beg);
    outfile.get(pivot,recordLen+1);
    for (lastSmall = low, i = low+1; i <= high; i++) {
        outfile.clear();
        outfile.seekg(pointers[i]*recordLen,ios::beg);
        outfile.get(rec,recordLen+1);
        if (strcmp(rec,pivot) < 0) {
            lastSmall++;
            swap(pointers[lastSmall],pointers[i]);
        }
    }
    swap(pointers[low],pointers[lastSmall]);
    pivotLoc = lastSmall;
}

void File::QSort(int low, int high) {
    int pivotLoc;
    if (low < high) {
        outfile.clear();
        partition(low, high, pivotLoc);
        QSort(low, pivotLoc-1);
        QSort(pivotLoc+1, high);
    }
}

void File::sortFile() {
    char rec[recordLen+1];
    QSort(1,pointers[0]);   // pointers[0] contains the # of elements;
    // put data from outfile in sorted order in file sorted:
    for (int i = 1; i <= pointers[0]; i++) {
        outfile.clear();
        outfile.seekg(pointers[i]*recordLen,ios::beg);
        outfile.get(rec,recordLen+1);
        sorted << rec << setw(strlen(rec)-recordLen);
    }
}

// data from overflow file and outfile are all stored in outfile and
// prepared for external sort by loading positions of the data to an array;

void File::combineFiles() {
    int counter = bucketSize*tableSize;
    char rec[recordLen+1];
    outfile.clear();
    overflow.clear();
    outfile.seekg(0,ios::end);
    overflow.seekg(0,ios::beg);
    while (overflow.get(rec,recordLen+1)) { // transfer from
        if (rec[0] != delMarker) {          // overflow to outfile only
            counter++;                      // valid (non-removed) items;
            outfile << rec << setw(strlen(rec)-recordLen);
        }
    }
    pointers = new long[counter+1];  // load to array pointers positions
    int arrCnt = 1;                  // of valid data stored in output file;
    for (int i = 0; i < counter; i++) {
        outfile.clear();
        outfile.seekg(i*recordLen,ios::beg);
        outfile.get(rec,recordLen+1);
        if (rec[0] != empty && rec[0] != delMarker)
            pointers[arrCnt++] = i;
    }
    pointers[0] = --arrCnt; // store the number of data in position 0;
}

void File::processFile(char *fileName) {
    ifstream fIn(fileName);
    if (fIn.fail()) {
         cerr << "Cannot open " << fileName << endl;
         return;
    }
    char command[strLen] = "";
    outfile.open("outfile",ios::in|ios::out|ios::trunc);
    sorted.open("sorted",ios::in|ios::out|ios::trunc);
    overflow.open("overflow",ios::in|ios::out|ios::trunc);
    for (int i = 1; i <= tableSize*bucketSize*recordLen; i++) // initialize
        outfile << empty;                                     // outfile;
    char line[recordLen+1];
    while (fIn.get(line,recordLen+1)) // load infile to outfile;
        insertion(line);
    while (strcmp(command,"exit")) {
        cout << "Enter command (insert, remove, or exit): ";
        cin.getline(command,strLen+1);
        if (!strcmp(command,"insert"))
             insert(line);
        else if (!strcmp(command,"remove"))
             excise(line);
        else if (strcmp(command,"exit"))
             cout << "Wrong command entered, please retry.\n";
    }
    combineFiles();
    sortFile();
    outfile.close();
    sorted.close();
    overflow.close();
    fIn.close();
    remove(fileName);
    rename("sorted",fileName);
}

int main(int argc, char* argv[]) {
    char fileName[80];
    if (argc != 2) {
         cout << "Enter a file name: ";
         cin.getline(fileName,80);
    }
    else strcpy(fileName,argv[1]);
    File fClass;
    fClass.processFile(fileName);
    return 0;
}
