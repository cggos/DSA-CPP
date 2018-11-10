#include <iostream>
#include <cstdlib>

using namespace std;

#include "genQueue.h"

int option(int percents[]) {
    register int i = 0, choice = rand()%100+1, perc;
    for (perc = percents[0]; perc < choice; perc += percents[i+1], i++);
    return i;
}

int main() {
    int arrivals[] = {15,20,25,10,30};
    int service[] = {0,0,0,10,5,10,10,0,15,25,10,15};
    int clerks[] = {0,0,0,0}, numOfClerks = sizeof(clerks)/sizeof(int);
    int customers, t, i, numOfMinutes = 100, x;
    double maxWait = 0.0, currWait = 0.0, thereIsLine = 0.0;
    Queue<int> simulQ;
    cout.precision(2);
    for (t = 1; t <= numOfMinutes; t++) {
	cout << " t = " << t;
	for (i = 0; i < numOfClerks; i++)// after each minute subtract
	    if (clerks[i] < 60)	         // at most 60 seconds from time
		 clerks[i] = 0;		 // left to service the current
	    else clerks[i] -= 60;	 // customer by clerk i;
	customers = option(arrivals);
	for (i = 0; i < customers; i++) {// enqueue all new customers
	    x = option(service)*10;	 // (or rather service time
	    simulQ.enqueue(x);		 // they require);
	    currWait += x;
	}
	// dequeue customers when clerks are available:
	for (i = 0; i < numOfClerks && !simulQ.isEmpty(); )
	   if (clerks[i] < 60)	{
		 x = simulQ.dequeue();   // assign more than one customer
		 clerks[i] += x;         // to a clerk if service time
		 currWait  -= x;         // is still below 60 sec;
	   }
	   else i++;
	if (!simulQ.isEmpty()) {
	    thereIsLine++;
	    cout << " wait = " << currWait/60.0;
	    if (maxWait < currWait)
		 maxWait = currWait;
	}
	else cout << " wait = 0;";
    }
    cout << "\nFor " << numOfClerks << " clerks, there was a line "
	 << thereIsLine/numOfMinutes*100.0 << "% of the time;\n"
	 << "maximum wait time was " << maxWait/60.0 << " min.";
    return 0;
}
