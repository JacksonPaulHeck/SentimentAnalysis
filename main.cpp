#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include "JPString.h"
#include "TargetDTO.h"
#include "DataDTO.h"
#define CATCH_CONFIG_RUNNER
#include "catch.hpp"

void printTestData(vector<DataDTO *>);

vector<DataDTO *> parseTestData(istream &, char *, vector<DataDTO *> *);

using namespace std;

JPString stopWordOmit(JPString* line) {
    ofstream stopWordFile("StopWordList.txt");
    char* temp;
    int i = 0;
    //while(line)
    stopWordFile.close();
    return *line;
}

vector<DataDTO *> parseTestData(istream &iss, char *line, vector<DataDTO *> *dataVector) {
    int rowNum;
    long id;
    char *username = new char[1024];
    char *data = new char[1024];
    int pos;
    int i;
    JPString *JPusername;
    JPString *JPdata;
    DataDTO *dataDto;
    while (!iss.eof()) {
        dataDto = new DataDTO();
        for (int pos = 0; pos < 3; pos++) {
            iss.getline(line, 1024, ',');
            switch (pos % 3) {
                case 0:
                    rowNum = atoi(line);
                    break;
                case 1:
                    id = atol(line);
                    break;
                case 2:
                    strncpy(username, line, 1024);
                    JPusername = new JPString(username);
                    break;
            }
        }
        iss.get(line, 1024, '\n');

        strncpy(data, line, 1024);
        JPdata = new JPString(data);

        *JPdata = stopWordOmit(JPdata);

        dataDto->setData(JPdata);
        dataDto->setUsername(JPusername);
        dataDto->setId(id);
        dataDto->setRowNum(rowNum);

        dataVector->push_back(dataDto);
    }
    return *dataVector;
}



void printTestData(vector<DataDTO *> dataVector) {
    for (int i = 0; i < dataVector.size(); i++) {
        cout << dataVector[i]->getRowNum() << ":\t"
             << dataVector[i]->getId() << ":\t"
             << *dataVector[i]->getUsername() << ":\t"
             << *dataVector[i]->getData() << endl;
    }
}

void analyzeData(DataDTO *dataDto, ostream &os) {
    int i = 0;
    JPString data;
    data = *dataDto->getData();
    while (i < data.size()) {
        if(data[i] == ' ') {
            os << endl;
            i++;
        }
        os << data[i];
        i++;
    }
}

int main(int argc, char** argv) {
    ifstream iss(argv[1]);
    ofstream oss("output.txt");
    char *line = new char[1024];
    vector<DataDTO *> *dataVector = new vector<DataDTO *>;

    *dataVector = parseTestData(iss, line, dataVector);

    //printTestData(*dataVector);
    cout << dataVector->size() << endl;
    for (int i = 0; i < dataVector->size(); i++) {
        analyzeData(dataVector->at(i), oss);
    }

    iss.close();
    oss.close();
    return 0;
}


