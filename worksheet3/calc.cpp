//begin -- calc.cpp --------

#include <sstream>
#include <iostream>
#include "adder.h"

using namespace std;

int main (int argc , char *argv[]) {

int a, b, c;
if (argc != 3) return 1;

string sa(argv [1]);
string sb(argv [2]);
stringstream ssa (sa);
stringstream ssb (sb);
ssa >> a;
ssb >> b;
c = add(a,b);
cout << c;
return 0;
}
