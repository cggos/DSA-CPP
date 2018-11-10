//************************  sorts.h  ***************************
//                 Generic sorting algorithms
//               overloading of < and = required

//conflict with <algorithms>, <queue>
//template<class T>
//inline void swap (T& e1, T& e2) {
//    T tmp = e1; e1 = e2; e2 = tmp;
//}

template<class T>
void insertionsort(T data[], const int n) {
    for (int i = 1, j; i < n; i++) {
        T tmp = data[i];
        for (j = i; j > 0 && tmp < data[j-1]; j--)
            data[j] = data[j-1];
        data[j] = tmp;
    }
}

template<class T>
void selectionsort(T data[], const int n) {
    for (int i = 0, least, j; i < n-1; i++) {
        for (j = i+1, least = i; j < n; j++)
            if (data[j] < data[least])
                least = j;
        swap(data[least],data[i]);
    }
}

template<class T>
void bubblesort(T data[], const int n) {
    for (int i = 0; i < n-1; i++)
        for (int j = n-1; j > i; --j)
            if (data[j] < data[j-1])
                swap(data[j],data[j-1]);
}

template <class T>
void combsort(T data[], const int n) {
    int step = n, j, k;
    while ((step = int(step/1.3)) > 1)        // phase 1
        for (j = n-1; j >= step; j--) {
            k = j-step;
            if (data[j] < data[k])
                swap(data[j],data[k]);
        } 
    bool again = true;
    for (int i = 0; i < n-1 && again; i++)    // phase 2
        for (j = n-1, again = false; j > i; --j)
            if (data[j] < data[j-1]) {
                swap(data[j],data[j-1]);
                again = true;
            }
}

template<class T>
void Shellsort(T data[], const int n) {
    register int i, j, hCnt, h;
    int increments[20], k;
//  create an appropriate number of increments h
    for (h = 1, i = 0; h < n; i++) {
        increments[i] = h;
        h = 3*h + 1;
    }
 // loop on the number of different increments h
    for (i--; i >= 0; i--) {
        h = increments[i];
     // loop on the number of subarrays h-sorted in ith pass
        for (hCnt = h; hCnt < 2*h; hCnt++) {
         // insertion sort for subarray containing every hth element of array data
            for (j = hCnt; j < n; ) {
                T tmp = data[j];
                k = j;
                while (k-h >= 0 && tmp < data[k-h]) {
                    data[k] = data[k-h];
                    k -= h;
                }
                data[k] = tmp;
                j += h;
            }
        }
    }
}

template<class T>
void moveDown (T data[], int first, int last) {
    int largest = 2*first + 1;
    while (largest <= last) {
        if (largest < last && // first has two children (at 2*first+1 and
            data[largest] < data[largest+1]) // 2*first+2) and the second
             largest++;                      // is larger than the first;

        if (data[first] < data[largest]) {   // if necessary,
             swap(data[first],data[largest]);// swap child and parent,
             first = largest;                // and move down;
             largest = 2*first+1;
        }
        else largest = last+1;  // to exit the loop: the heap property
    }                           // isn't violated by data[first];
}

template<class T>
void heapsort(T data[], const int n) {
    int i;
    for (i = n/2 - 1; i >= 0; --i) // create the heap;
        moveDown (data,i,n-1);
    for (i = n-1; i >= 1; --i) {
        swap(data[0],data[i]);     // move the largest item to data[i];
        moveDown(data,0,i-1);      // restore the heap property;
    }
}

template<class T>
void quicksort(T data[], int first, int last) {
    int lower = first+1, upper = last;
    swap(data[first],data[(first+last)/2]);
    T bound = data[first];
    while (lower <= upper) {
        while (data[lower] < bound)
             lower++;
        while (bound < data[upper])
             upper--;
        if (lower < upper)
             swap(data[lower++],data[upper--]);
        else lower++;
    }
    swap(data[upper],data[first]);
    if (first < upper-1)
        quicksort (data,first,upper-1);
    if (upper+1 < last)
        quicksort (data,upper+1,last);
}

template<class T>
void quicksort(T data[], const int n) {
    int i, max;
    if (n < 2)
        return;
    for (i = 1, max = 0; i < n; i++)// find the largest
        if (data[max] < data[i])    // element and put it
            max = i;                // at the end of data[];
    swap(data[n-1],data[max]); // largest el is now in its
    quicksort(data,0,n-2);     // final position;
}

template<class T>
void insertionsort(T data[], int first, int last) {
    for (int i = first, j; i <= last; i++) {
        T tmp = data[i];
        for (j = i; j > 0 && tmp < data[j-1]; j--)
            data[j] = data[j-1];
        data[j] = tmp;
    }
}

template<class T>
void quicksort2(T data[], int first, int last) {
    if (last - first < 30)
         insertionsort(data,first,last);
    else {
         int lower = first+1, upper = last;
         swap(data[first],data[(first+last)/2]);
         T bound = data[first];
         while (lower <= upper) {
             while (data[lower] < bound)
                 lower++;
             while (bound < data[upper])
                 upper--;
             if (lower < upper)
                 swap(data[lower++],data[upper--]);
             else lower++;
         }
         swap(data[upper],data[first]);
         if (first < upper-1)
             quicksort2(data,first,upper-1);
         if (upper+1 < last)
             quicksort2(data,upper+1,last);
    }
}

template<class T>
void quicksort2(T data[], const int n) {
    int i, max;
    if (n < 2)
        return;
    for (i = 1, max = 0; i < n; i++)// find the largest
        if (data[max] < data[i])    // element and put it
            max = i;                // at the end of data[];
    swap(data[n-1],data[max]);  // largest el is now in its
    quicksort2(data,0,n-2);     // final position;
}

template<class T>
void merge(T array1[], T temp[], int first, int last) {
    int mid = (first + last) / 2;
    int i1 = 0, i2 = first, i3 = mid + 1;
    while (i2 <= mid && i3 <= last)
        if (array1[i2] < array1[i3])
             temp[i1++] = array1[i2++];
        else temp[i1++] = array1[i3++];
    while (i2 <= mid)
        temp[i1++] = array1[i2++];
    while (i3 <= last)
        temp[i1++] = array1[i3++];
    for (i1 = 0, i2 = first; i2 <= last; array1[i2++] = temp[i1++]);
}

template<class T>
void mergesort(T data[], T temp[], int first, int last) {
    if (first < last) {
        int mid = (first + last) / 2;
        mergesort(data, temp, first, mid);
        mergesort(data, temp, mid+1, last);
        merge(data, temp, first, last);
    }
}

template<class T>
void mergesort(T data[], const int n) {
    T *temp = new T[n];
    mergesort(data,temp,0,n-1);
}

#include <queue>

template<class T>
class Queue : public queue<T> {
public:
    T dequeue() {
        T tmp = front();
        queue<T>::pop();
        return tmp;
    }
    void enqueue(const T& el) {
        push(el);
    }
};

const int bits = 31;
const int radix = 10;
const int digits = 10;

template<class T>
void radixsort(T data[], const int n) {
    register int d, j, k, factor;
    Queue<T> queues[radix];
    for (d = 0, factor = 1; d < digits; factor *= radix, d++) {
        for (j = 0; j < n; j++)
            queues[(data[j] / factor) % radix].enqueue(data[j]);
        for (j = k = 0; j < radix; j++)
            while (!queues[j].empty())
                 data[k++] = queues[j].dequeue();
    }
}

void bitRadixsort(long data[], const int n, int b) {
    int pow2b = 1;
    pow2b <<= b;
    int i, j, k, pos = 0, mask = pow2b-1;
    int last = (bits % b == 0) ? (bits/b) : (bits/b + 1);
    Queue<long> *queues = new Queue<long>[pow2b];
    for (i = 0; i < last; i++) {
        for (j = 0; j < n; j++)
            queues[(data[j] & mask) >> pos].enqueue(data[j]);
        mask <<= b;
        pos = pos+b;
        for (j = k = 0; j < pow2b; j++) 
            while (!queues[j].empty())
                data[k++] = queues[j].dequeue();
    }
}

void inline clear(long& q) {
    q = -1;
}
int inline isEmpty(long q) {
    return q == -1;
}

template<class T>
class RadixSortNode {
public:
    T *arr;
    RadixSortNode *next;
};

void radixsort2(long data[], const int n) {
    register int d, j, k, factor, where;
    RadixSortNode<long> n1, n2, *p1, *p2;
    n1.arr = data;
    n2.arr = new long[n];
    for (j = 0; j < n; j++)
        n2.arr[j] = data[j];
    long *queue = new long[n], queueHeads[radix], queueTails[radix];
    p1 = n2.next = &n1;
    p2 = n1.next = &n2;
    for (d = 0, factor = 1; d < digits; factor *= radix, d++) {
        for (j = 0; j < radix; j++)
            clear(queueHeads[j]);
        for (j = 0; j < n; j++) {
            where = (p1->arr[j] / factor) % radix;
            if (isEmpty(queueHeads[where]))
                 queueTails[where] = queueHeads[where] = j;
            else {
                 queue[queueTails[where]] = j;
                 queueTails[where] = j;
            }
        }
        for (j = 0; j < radix; j++)
            if (!(isEmpty(queueHeads[j])))
                 clear(queue[queueTails[j]]);
        for (j = k = 0; j < radix; j++)
            while (!(isEmpty(queueHeads[j]))) {
                 p2->arr[k++] = p1->arr[queueHeads[j]];
                 queueHeads[j] = queue[queueHeads[j]];
            }
        p2 = p2->next;
        p1 = p1->next;
    }
    if (digits % 2 != 0) // if digits is an odd number;
        for (d = 0; d < n; d++)
            data[d] = p1->arr[d];
}

class RadixsortNode {
public:
    long *arr;
    RadixsortNode *next;
    RadixsortNode() {
        next = 0;
    }
    RadixsortNode(long *a, int n) {
        arr = new long[n];
        for (int i = 0; i < n; i++)
            arr[i] = a[i];
        next = 0;
    }
    RadixsortNode(int n) {
        arr = new long[n];
        next = 0;
    }
};

void bitRadixsort2(long data[], const int n, int b) {
        int pow2b = 1;
        pow2b <<= b;
        int d, j, k, where, pos = 0, mask = pow2b-1;
        int last = (bits % b == 0) ? (bits/b) : (bits/b + 1);
        long *queues = new long[n], *queueHeads = new long[pow2b];
        long *queueTails = new long[pow2b];
        RadixsortNode *n2 = new RadixsortNode(data,n), *n1 = new RadixsortNode(n);
        n1->arr = data;
        n2->next = n1;
        n1->next = n2;
        for (d = 0; d < last; d++) {
            for (j = 0; j < pow2b; j++) 
                clear(queueHeads[j]);
            for (j = 0; j < n; j++) {
                where = (n1->arr[j] & mask) >> pos;
                if (isEmpty(queueHeads[where]))
                     queueTails[where] = queueHeads[where] = j;
                else {
                     queues[queueTails[where]] = j;
                     queueTails[where] = j;
                }
            }
            mask <<= b;
            pos = pos+b;
            for (j = 0; j < pow2b; j++)
                if (!(isEmpty(queueHeads[j])))
                     clear(queues[queueTails[j]]);
            for (j = k = 0; j < pow2b; j++)
                while (!(isEmpty(queueHeads[j]))) {
                     n2->arr[k++] = n1->arr[queueHeads[j]];
                     queueHeads[j] = queues[queueHeads[j]];
                }
            n2 = n2->next;
            n1 = n1->next;
        }
        if (last % 2 != 0) // if bits is an odd number;
            for (d = 0; d < n; d++)
                data[d] = n1->arr[d];
}

void countingsort(long data[], const long n) {
    long i;
    long largest = data[0];
    long *tmp = new long[n];
    for (i = 1; i < n; i++)        // find the largest number
        if (largest < data[i])     // in data and create the array
            largest = data[i];     // of counters accordingly;
    unsigned long *count = new unsigned long[largest+1];
    for (i = 0; i <= largest; i++)
        count[i] = 0;
    for (i = 0; i < n; i++)        // count numbers in data[];
        count[data[i]]++;
    for (i = 1; i <= largest; i++) // count numbers <= i; 
        count[i] = count[i-1] + count[i];
    for (i = n-1; i >= 0; i--) {   // put numbers in order in tmp[];
        tmp[count[data[i]]-1] = data[i];
        count[data[i]]--;
    }
    for (i = 0; i < n; i++)        // transfer numbers from tmp[]
        data[i] = tmp[i];          // to the original array;
}
