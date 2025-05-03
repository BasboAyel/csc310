#include <iostream>
#include "customErrorClass.h"

using namespace std;

MyException::MyException() {
    this->errString = "Error: Something went wrong.";
}

MyException::MyException(const string &err) {
    this->errString = err;
}

MyException::MyException(const char *err) {
    this->errString = err;
}

const char* MyException::what() const noexcept {
    return this->errString.c_str();
}
