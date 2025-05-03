#ifndef __CUSTOM_ERROR_CLASS_H__
#define __CUSTOM_ERROR_CLASS_H__

#include <exception>
#include <string>

using namespace std;

class MyException : public exception {
public:
    MyException();
    MyException(const char*);
    MyException(const string&);
    virtual const char* what() const noexcept override;

private:
    string errString;
};

#endif
