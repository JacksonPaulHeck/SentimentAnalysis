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

void analyzeData(vector<DataDTO *>*, ostream &, istream &);

using namespace std;

int main(int argc, char **argv) {
    //ifstream iss(argv[1]);
    //ofstream oss(argv[2]);

    ifstream iss("dev-train-data.csv");
    ofstream oss("output.txt");
    ifstream targetis("dev-train-target.csv");
    if (iss.is_open()) {
        cout << "iss opened" << endl;
    } else {
        cout << "not opened" << endl;
    }

    char *line = new char[1024];
    vector<DataDTO *> *dataVector = new vector<DataDTO *>;

    dataVector = parseTestData(iss, line, dataVector);

    cout << dataVector->size() << endl;

    analyzeData(dataVector, oss, targetis);

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
    vector<JPString> jpStringVec;
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
        while (data[r] != '\0') {
            data[r] = tolower(data[r]);
            r++;
        }
        char *token = strtok(data, " \"ï¿½.;,`@#$%^&*()_-!?\r");
        while (token != NULL) {
            JPString JPToken(token);
            jpStringVec.push_back(JPToken);
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
        for (int i = 0; i < jpStringVec.size(); i++) {
            int j = 0;
            bool isEqual = false;
            while (j < stopWordVector.size()) {
                if (jpStringVec.at(i) == stopWordVector.at(j)) {
                    isEqual = true;
                }
                j++;
            }
            if (!isEqual && jpStringVec.at(i).size() > 2) {
                *JPdata += jpStringVec.at(i);
                *JPdata += " ";
            }
        }
        jpStringVec.clear();
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

void analyzeData(vector<DataDTO *>* dataVector, ostream &os, istream &is) {
    dataVector->pop_back();
    map<JPString, int> wordList;
    map<JPString, int>::iterator iteratorWordList;

    char *targetLine = new char[1024];
    vector<TargetDTO> targetVector;
    TargetDTO *targetDto = nullptr;
    while (!is.eof()) {
        targetDto = new TargetDTO();
        for (int pos = 0; pos < 2; pos++) {
            is.getline(targetLine, 1024, ',');
            switch (pos % 2) {
                case 0:
                    targetDto->setRowNum(atoi(targetLine));
                    break;
                case 1:
                    targetDto->setTarget(atoi(targetLine));
                    break;
                case 2:
                    targetDto->setId(atol(targetLine));
                    break;
            }
        }
        targetVector.push_back(*targetDto);
    }
    int i = 0;
    while(i < dataVector->size()){
        int j = 0;
        JPString data = *dataVector->at(i)->getData();
        JPString str;
        while(data[j] != '\0'){
            if(data[j] != ' '){
                str += data[j];
            }else{
                iteratorWordList = wordList.find(str);
                if(iteratorWordList != wordList.end()){
                    wordList[str] = iteratorWordList->second + targetVector.at(i).getTarget()/4;
                }else if(iteratorWordList == wordList.end()){
                    wordList[str] = targetVector.at(i).getTarget()/4;
                }
                str = "";
            }
            j++;
        }
        i++;
    }
    iteratorWordList = wordList.begin();
    while(iteratorWordList != wordList.end()){
        os << iteratorWordList->first << " : " << iteratorWordList->second << endl;
        iteratorWordList++;
    }
}
