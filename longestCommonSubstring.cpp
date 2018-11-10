#include <iostream>
#include <string>

using namespace std;

class SuffixTreeNode {
public:
    SuffixTreeNode **descendants;
    int *left, *right;
    SuffixTreeNode *suffixLink;
    int id;         // for printing only;
    SuffixTreeNode() {
	SuffixTreeNode(128);
    }
    SuffixTreeNode(int sz) {
	id = cnt++;
	descendants = new SuffixTreeNode*[sz];
	suffixLink = 0;
	left  = new int[sz];
	right = new int[sz];
	for (int i = 0; i < sz; i++) {
	     descendants[i] = 0;
	     left[i] = -1;
	}
    }
private:
    static int cnt; // for printing only;
};

int SuffixTreeNode::cnt;

class UkkonenSuffixTree {
public:
    UkkonenSuffixTree() {
	UkkonenSuffixTree(0,127);
    }
    UkkonenSuffixTree(int from, int to) {
	size = to - from + 1;
	offset = from;
	root = new SuffixTreeNode(size);
	root->suffixLink = root;
    }
    void printTree(int pos) {
	cout << endl;
	printTree(root,0,0,0,pos);
    }
    void createTree(string text) {
	T = text;
	int Lt = 1;
	bool endPoint;
	const int n = T.length(), pos = T[0]-offset;
	SuffixTreeNode *canonicalNodeAP = root, *canonicalNodeEP;
	root->left [pos] = 0;
	root->right[pos] = n-1;
	for (int i = 1; i < n; i++) {
	    canonicalNodeEP = update(canonicalNodeAP,i,Lt);
	    // and thus, endpoint = node(canonicalNodeEP,Lt,i);
	    canonicalNodeAP = findCanonicalNode(canonicalNodeEP,i,Lt);
	    // and so, active point = node(canonicalNodeAP,Lt,i);
	    printTree(i);
	}
    }
protected:
    SuffixTreeNode *root;
    int size, offset;
    string T;
private:
    void printTree(SuffixTreeNode *p, int lvl, int lt, int rt, int pos) {
	for (int i = 1; i <= lvl; i++)
	     cout << "   ";
        if (p != 0) {                          // if a nonleaf;
	     if (p == root)
		  cout << p->id << endl;
             else if (p->suffixLink != 0)      // to print in the middle
                  cout << T.substr(lt,rt-lt+1) // of update;
                       << " " << p->id << " " << p->suffixLink->id
                       << " [" << lt << " " << rt << "]\n";
             else cout << T.substr(lt,pos-lt+1) << " " << p->id;
	     for (char i = 0; i < size; i++)
                 if (p->left[i] != -1)         // if a tree node;
		     printTree(p->descendants[i],lvl+1,p->left[i],p->right[i],pos);
	}
        else cout << T.substr(lt,pos-lt+1) <<" [" << lt << " " << rt << "]\n";
    }
    SuffixTreeNode* testAndSplit(SuffixTreeNode *p, int i, int& Lt, bool& endPoint) {
	int Rt = i-1;
	if (Lt <= Rt) {
	     int pos = T[Lt]-offset;
	     SuffixTreeNode *pp = p->descendants[pos];
	     int lt = p->left[pos];
	     int rt = p->right[pos];
             if (T[i] == T[lt+Rt-Lt+1]) { // if T(lt..rt) is
		  endPoint = true;        // and extension of
                  return p;               // T(Lt..i);
	     }
	     else{// insert a new node r between s and ss by splitting
                  // edge(p,pp) = T(lt..rt) into
                  // edge(p,r)  = T(lt..lt+Rt-Lt) and
                  // edge(r,pp) = T(lt+Rt-Lt+1..rt);
		  pos = T[lt]-offset;
		  SuffixTreeNode *r = p->descendants[pos] = new SuffixTreeNode(size);
		  p->right[pos] = lt+Rt-Lt;
		  pos = T[lt+Rt-Lt+1]-offset;
		  r->descendants[pos] = pp;
		  r->left [pos] = lt+Rt-Lt+1;
		  r->right[pos] = rt;
		  endPoint = false;
		  return r;
	    }
	}
	else if (p->left[T[i]-offset] == -1)
	     endPoint = false;
	else endPoint = true;
	return p;
    }
    SuffixTreeNode* findCanonicalNode(SuffixTreeNode *p, int Rt, int& Lt) {
	if (Rt >= Lt) {
	    int pos = T[Lt]-offset;
	    SuffixTreeNode *pp = p->descendants[pos];
	    int lt = p->left[pos];
	    int rt = p->right[pos];
	    while (rt - lt <= Rt - Lt) {
		Lt = Lt + rt - lt + 1;
		p = pp;
		if (Lt <= Rt) {
		    pos = T[Lt]-offset;
		    pp = p->descendants[pos];
		    lt = p->left[pos];
		    rt = p->right[pos];
                    if (p == root)
			pp = root;
		}
	    }
	}
	return p;
    }
    SuffixTreeNode* update(SuffixTreeNode *p, int i, int& Lt) {
	bool endPoint;
	SuffixTreeNode *prev = 0, *r = testAndSplit(p,i,Lt,endPoint);
	while (!endPoint) {
	    int pos = T[i]-offset;
	    r->left [pos] = i;     // add a T(i)-edge to r;
	    r->right[pos] = T.length()-1;
	    if (prev != 0)
		prev->suffixLink = r;
	    prev = r;
	    if (p == root)
		 Lt++;
	    else p = p->suffixLink;
	    p = findCanonicalNode(p,i-1,Lt);
	    r = testAndSplit(p,i,Lt,endPoint); // check if not the endpoint;
	}
	if (prev != 0)
	    prev->suffixLink = p;
	return p;
    }
};

class LongestCommonSubstring : public UkkonenSuffixTree {
public:
    LongestCommonSubstring(int from, int to) : UkkonenSuffixTree(from,to+2) {
    }
    void run(string s1, string s2) {
        createTree(s1 + char(size+offset-2) + s2 + char(size+offset-1));
	findLongest(s1,s2);
    }
private:
    int s1length, position, length;
    void findLongest(string s1, string s2) {
	bool dummy[] = {false, false};
	position = length = 0;
	s1length = s1.length();
	traverseTree(root,0,0,dummy);
	if (length == 0)
             cout << "Strings \"" << s1 << "\" and \"" << s2
                  << "\" have no common substring\n";
	else cout << "A longest common substring for \""
                  << s1 << "\" and \"" << s2 << "\" is " << "\""
                  << T.substr(position-length,length) << "\" of length "
                  << length << endl;
    }
    void traverseTree(SuffixTreeNode *p, int lt, int len, bool *whichEdges) {
	bool edges[] = {false, false};
	for (char i = 0; i < size; i++)
	     if (p->left[i] != -1) {
		  if (p->descendants[i] == 0)     // if it is an edge to
		       if (p->left[i] <= s1length)// a leaf corresponding
                            whichEdges[0] = edges[0] = true; // to s1
		       else whichEdges[1] = edges[1] = true; // to s2
                  else {
		       traverseTree(p->descendants[i],p->left[i],
				    len+(p->right[i]-p->left[i]+1),edges);
		       if (edges[0])
			   whichEdges[0] = true;
                       if (edges[1])
                           whichEdges[1] = true;
                  }
		  if (edges[0] && edges[1] && len > length) {
		       position = p->left[i];
		       length = len;
		  }
	     }
    }
};

int main(int argc, string argv[]) {
    string s1 = "abcabc";
    string s2 = "cabaca";
    if (argc == 3) {
         s1 = argv[1];
         s2 = argv[2];
    }
    (new LongestCommonSubstring('a','z'))->run(s1,s2);
    return 0;
}

