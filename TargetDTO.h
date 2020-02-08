//
// Created by jacks on 1/28/2020.
//

#ifndef PROGRAM1_TARGETDTO_H
#define PROGRAM1_TARGETDTO_H


#include "JPString.h"

class TargetDTO {
private:
    int rowNum;
    int target;
    long id;
public:
    TargetDTO();
    TargetDTO(int, int, long);
    bool setRowNum(int);
    int getRowNum();
    bool setTarget(int);
    int getTarget();
    bool setId(long);
    long getId();
};


#endif //PROGRAM1_TARGETDTO_H
