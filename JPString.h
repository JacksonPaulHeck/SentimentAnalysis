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
    int length;
public:
    JPString();
    JPString(char);
    JPString(const char*);
    JPString(const JPString&);
    ~JPString();
    JPString& operator = (const JPString&);
    JPString& operator = (const char*);
    bool operator != (char);
    JPString operator += (const JPString&);
    char& operator[](int)const;
    int size()const;
    friend bool operator > (const JPString&, const JPString&);
    friend bool operator < (const JPString&, const JPString&);
    friend bool operator == (const JPString&, const JPString&);
    friend ostream& operator<<(ostream&, const JPString&);
    friend istream& operator>>(istream&, JPString&);
};

#endif //PROGRAM1_JPSTRING_H
