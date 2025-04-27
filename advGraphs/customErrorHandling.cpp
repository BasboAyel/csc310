#include <iostream>
#include "customErrorClass.h"

using namespace std;

double division(int a, int b){
    if( b == 0 ){
        throw MyException("myerror");
    }
    return a/b;
}

