#pragma hdrstop

#include "AntRun.h"
#include <math.h>
#include <list>
#include <set>
#include <cstdlib>
#include <algorithm>
#include <iostream>     
#include <fstream>
#include <string>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
using namespace std;

int main(){

    close(1);   //zamkniecie standardowego wyjscia zeby wywalil wyniki do pliku
    creat("testowy.txt", 0600);
    double droga = AntRun();
    cout << "\nDroga: " << droga << endl;
    return 0;
}