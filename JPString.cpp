//
// Created by jacks on 1/26/2020.
//
#include <iostream>
#include <cstring>
#include "JPString.h"
using namespace std;

JPString :: JPString() {
    this->data = nullptr;
}
JPString :: JPString(char * input) {
    this->data = new char[strlen(input)+1];
    strcpy(this->data, input);
    int index = 0;
    while (input[index] != '\0') {
        index++;
    }
}
JPString :: JPString(const JPString &) {}
JPString :: ~JPString() {
    cout << "JPString Destructor" << endl;
    delete[] data;
}
JPString& JPString :: operator=(const JPString & jpString) {
    this->data = new char[strlen(jpString.data)+1];
    strcpy(this->data, jpString.data);
    this->data[strlen(jpString.data)+1] = '\0';
    return *this;
}

JPString& JPString :: operator=(const char * Data) {
    this->data = new char[strlen(Data)+1];
    strcpy(this->data, Data);
    this->data[strlen(Data)+1] = '\0';
    return *this;
}
bool JPString :: operator==(char character) {
    return *this->data == character;
}
bool JPString :: operator!=(char character) {
    return *this->data != character;
}
JPString JPString :: operator+(const JPString &) {}
char JPString :: operator[](int i){
    return data[i];
}
int JPString :: size(){
    int i = 0;
    while(data[i] != '\0'){
        i++;
    }
    return i;
}

ostream& operator<<(ostream& outStream, const JPString& jpString){
    outStream << jpString.data;
    return outStream;
}
istream& operator>>(istream& inStream, const JPString& jpString){
    inStream >> jpString.data;
    return inStream;
}
