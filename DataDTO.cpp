//
// Created by jacks on 1/28/2020.
//

#include "DataDTO.h"
#include "TargetDTO.h"

DataDTO :: DataDTO(){
    this->rowNum = 0;
    this->id = 0;
    this->username = nullptr;
    this->data = nullptr;
}
DataDTO :: DataDTO(int rowNum, long id, JPString* username, JPString* data){
    this->rowNum = rowNum;
    this->id = id;
    this->username = username;
    this->data = data;
}
DataDTO :: ~DataDTO(){
    delete[] username;
    delete[] data;
}
bool DataDTO :: setRowNum(int Row){
    this->rowNum = Row;
    return true;
}
int DataDTO :: getRowNum(){
    return this->rowNum;
}
bool DataDTO :: setId(long ID){
    this->id = ID;
    return true;
}
long DataDTO :: getId(){
    return this->id;
}
bool DataDTO :: setUsername(JPString* Username){
    this->username = Username;
    return true;
}
JPString* DataDTO :: getUsername(){
    return this->username;
}
bool DataDTO :: setData(JPString* Data){
    this->data = Data;
    return true;
}
JPString* DataDTO :: getData(){
    return this->data;
}