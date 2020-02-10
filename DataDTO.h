//
// Created by jacks on 1/28/2020.
//

#ifndef PROGRAM1_DATADTO_H
#define PROGRAM1_DATADTO_H


#include "JPString.h"
#include "TargetDTO.h"

class DataDTO {
private:
    int rowNum;
    long id;
    JPString* username;
    JPString* data;
public:
    DataDTO();
    DataDTO(int, long, JPString*, JPString*);
    bool setRowNum(int);
    int getRowNum();
    bool setId(long);
    long getId();
    bool setUsername(JPString*);
    JPString* getUsername();
    bool setData(JPString*);
    JPString* getData();
};


#endif //PROGRAM1_DATADTO_H
