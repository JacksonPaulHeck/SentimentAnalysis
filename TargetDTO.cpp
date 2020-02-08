//
// Created by jacks on 1/28/2020.
//

#include "TargetDTO.h"
TargetDTO :: TargetDTO(){
    this->rowNum = 0;
    this->target = 0;
    this->id = 0;
}
TargetDTO :: TargetDTO(int RowNum, int Target, long ID){
    this->rowNum = RowNum;
    this->target = Target;
    this->id = ID;
}
bool TargetDTO :: setRowNum(int RowNum){
    this->rowNum = RowNum;
    return true;
}
int TargetDTO :: getRowNum(){
    return this->rowNum;
}
bool TargetDTO :: setTarget(int Target){
    this->target = Target;
    return true;
}
int TargetDTO :: getTarget(){
    return this->target;
}
bool TargetDTO :: setId(long ID){
    this->id = ID;
    return true;
}
long TargetDTO :: getId(){
    return this->id;
}
