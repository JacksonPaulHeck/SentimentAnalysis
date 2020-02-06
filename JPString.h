//
// Created by jacks on 1/26/2020.
//
# include <iostream>
using namespace std;

#ifndef PROGRAM1_JPSTRING_H
#define PROGRAM1_JPSTRING_H

class JPString{
private:
    char* data;
public:
    JPString();
    JPString(char*);
    JPString(const JPString&);
    ~JPString();
    JPString& operator = (const JPString&);
    JPString& operator = (const char*);
    bool operator == (char character);
    bool operator != (char);
    JPString operator + (const JPString&);
    char operator[](int);
    int size();
    friend ostream& operator<<(ostream&, const JPString&);
    friend istream& operator>>(istream&, const JPString&);


};

#endif //PROGRAM1_JPSTRING_H
