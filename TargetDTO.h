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
    JPString* id;
public:
    TargetDTO();
    TargetDTO(int, int, JPString*);
    bool setRowNum(int);
    int getRowNum();
    bool setTarget(int);
    int getTarget();
    bool setId(JPString*);
    JPString* getId();
};


#endif //PROGRAM1_TARGETDTO_H
