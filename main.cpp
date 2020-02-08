#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>
#include "JPString.h"
#include "TargetDTO.h"
#include "DataDTO.h"
#include <map>

//#define CATCH_CONFIG_RUNNER

//#include "catch.hpp"

void printTestData(vector<DataDTO *>);

vector<DataDTO *> *parseTestData(istream &, char *, vector<DataDTO *> *);

void stopWordOmit(JPString &);

void analyzeData(DataDTO *, ostream &, istream &);

using namespace std;

int main(int argc, char **argv) {
    //ifstream iss(argv[1]);
    //ofstream oss(argv[2]);

    ifstream iss("Test.csv");
    ofstream oss("output.txt");
    ifstream targetis("Target.csv");
    if (iss.is_open()) {
        cout << "iss opened" << endl;
    } else {
        cout << "not opened" << endl;
    }

    char *line = new char[1024];
    vector<DataDTO *> *dataVector = new vector<DataDTO *>;

    dataVector = parseTestData(iss, line, dataVector);

    cout << dataVector->size() << endl;

    for(int j = 0; j < dataVector->size(); j++){
        analyzeData(dataVector->at(j), oss, targetis);
    }
    for (int i = 0; i < dataVector->size(); i++) {
        delete[] dataVector->at(i);
    }
    cout << "end of program" << endl;
    iss.close();
    oss.close();
    targetis.close();

    return 0;
}

vector<DataDTO *> *parseTestData(istream &iss, char *line, vector<DataDTO *> *dataVector) {
    int rowNum = 0;
    long id = 0;
    char *username = new char[1024];
    char *data = new char[1024];
    JPString *JPusername = nullptr;
    JPString *JPdata = new JPString("");
    DataDTO *dataDto = nullptr;
    ifstream istream("StopWordList.csv");
    char *temp = new char[20];
    vector<JPString> charVec;
    vector<JPString> stopWordVector;
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
        int r = 0;
        while(data[r] != '\0'){
            data[r] = tolower(data[r]);
            r++;
        }
        char *token = strtok(data, " \"ï¿½.;,`@#$%^&*()_-!?\r");
        while (token != NULL) {
            JPString JPToken(token);
            charVec.push_back(JPToken);
            token = strtok(NULL, " \"ï¿½.;,`@#$%^&*()_-!?\r");
        }
        int k = 0;
        istream.getline(temp, 20);
        while (!istream.eof()) {
            JPString JPTemp(temp);
            stopWordVector.push_back(JPTemp);
            istream.getline(temp, 20);
            k++;
        }
        for (int i = 0; i < charVec.size(); i++) {
            int j = 0;
            bool isEqual = false;
            while (j < stopWordVector.size()) {
                if (charVec.at(i) == stopWordVector.at(j)) {
                    isEqual = true;
                }
                j++;
            }
            if (!isEqual && charVec.at(i).size() > 2) {
                *JPdata += charVec.at(i);
                *JPdata += " ";
            }
        }
        charVec.clear();
        istream.close();

        dataDto->setData(JPdata);
        dataDto->setUsername(JPusername);
        dataDto->setId(id);
        dataDto->setRowNum(rowNum);
        dataVector->push_back(dataDto);
        JPdata = new JPString();
    }
    return dataVector;
}

void printTestData(vector<DataDTO *> dataVector) {
    for (int i = 0; i < dataVector.size(); i++) {
        cout << dataVector[i]->getRowNum() << ":\t"
             << dataVector[i]->getId() << ":\t"
             << *dataVector[i]->getUsername() << ":\t"
             << *dataVector[i]->getData() << endl;
    }
}

void analyzeData(DataDTO *dataDto, ostream &os, istream & is) {
    int i = 0;
    JPString data;
    data = *dataDto->getData();
    while (i < data.size()) {
        os << data[i];
        i++;
    }
    os << endl;

    map<JPString, int> wordList;

    char* target = new char[1024];
    while(!is.eof()) {
        is.get(target, 1024, '\n');
        os << target << endl;
    }
}
