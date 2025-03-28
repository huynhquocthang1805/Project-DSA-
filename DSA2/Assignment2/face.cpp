#include <iostream>
using namespace std;
int main () {
    int n = 1;
    double e = 1.0;
    int term = 0;
    int saiso = 1
    double fac = 1.0;
    while (saiso >= 0.000001) {
        fac = fac*n;
        term = e;
        e = e + 1/fac;
        saiso = abs(term - e);
        n = n + 1;
    }
    cout << e ;
}