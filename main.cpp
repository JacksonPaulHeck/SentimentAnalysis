#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "JPString.h"
#include "TargetDTO.h"
#include "DataDTO.h"
#include <map>

//#define CATCH_CONFIG_RUNNER
//#include "catch.hpp"
void parseTestData(istream &, char *, vector<DataDTO *> &);
void trainData(vector<DataDTO *> &, ostream &, istream &, map<JPString, int> &, vector<TargetDTO *> &);
void analyzeData(vector<DataDTO *> &, ofstream &, ifstream &, map<JPString, int> &, vector<TargetDTO *> &);
void accuracyCalculation(vector<TargetDTO*> &, vector<TargetDTO *> &, ostream&, vector<DataDTO*> &);
using namespace std;

int main(int argc, char **argv) {

    ofstream testOut("output.txt");
    ofstream dataOut("output2.txt");
    ifstream trainingIn("Test.csv");
    ifstream targetIn("Target.csv");
    char *line = new char[1024];
    vector<DataDTO *> trainingVector;
    vector<DataDTO *> dataVector;
    map<JPString, int> wordList;
    vector<TargetDTO *> targetVector;
    vector<TargetDTO *> actualTarget;

    parseTestData(trainingIn, line, trainingVector);
    trainData(trainingVector, testOut, targetIn, wordList, actualTarget);

    trainingIn.clear();
    trainingIn.seekg(0, ios::beg);

    parseTestData(trainingIn, line, dataVector);
    analyzeData(dataVector, dataOut, trainingIn, wordList, targetVector);

    accuracyCalculation(targetVector, actualTarget, testOut, dataVector);

    trainingIn.close();
    testOut.close();
    targetIn.close();
    dataOut.close();
    cout << "end of program" << endl;

    return 0;
}

void accuracyCalculation(vector<TargetDTO*> & targetVector, vector<TargetDTO *> &actualTarget, ostream& testOut, vector<DataDTO*> & dataVector){
    double total = 0;
    double correct = 0;
    while (total < targetVector.size() && total < actualTarget.size()) {
        if (targetVector.at(total)->getTarget() == actualTarget.at(total)->getTarget()) {
            correct++;
        } else {
            testOut << dataVector.at(total)->getRowNum()
                    << "," << dataVector.at(total)->getId()
                    << "," << *dataVector.at(total)->getUsername()
                    << "," << *dataVector.at(total)->getData()
                    << "\tTarget: " << targetVector.at(total)->getTarget()
                    << "\tActualTarget: " << actualTarget.at(total)->getTarget()
                    << endl;
        }
        total++;
    }
    cout << "Accuracy: ";
    cout << correct / total << endl;
}
void analyzeData(vector<DataDTO *> &dataVector, ofstream &oss, ifstream &iss, map<JPString, int> &wordList,
                 vector<TargetDTO *> &targetVector) {
    map<JPString, int>::iterator iteratorWordList;
    iteratorWordList = wordList.begin();
    while (iteratorWordList != wordList.end()) {
        oss << iteratorWordList->first << " : " << iteratorWordList->second << endl;
        iteratorWordList++;
    }
    int i = 0;
    int result = 0;
    int total = 0;
    while (i < dataVector.size()) {
        int j = 0;
        JPString data = *dataVector.at(i)->getData();
        JPString str;
        while (data[j] != '\0') {
            total = 0;
            if (data[j] != ' ') {
                str += data[j];
            } else {
                iteratorWordList = wordList.find(str);
                if (iteratorWordList != wordList.end()) {
                    total += iteratorWordList->second;
                } else {
                    total += 0;
                }
                str = "";
            }
            j++;
        }
        if (total < 0) {
            result = 0;
        } else if (total > 0) {
            result = 4;
        } else {
            result = rand() % 2 * 4;
        }
        auto *targetDto = new TargetDTO(dataVector.at(i)->getRowNum(), result, dataVector.at(i)->getId());
        targetVector.push_back(targetDto);
        i++;
    }
    targetVector.pop_back();
}
void parseTestData(istream &iss, char *line, vector<DataDTO *> &dataVector) {
    int rowNum = 0;
    long id = 0;
    char *username = new char[1024];
    char *data = new char[1024];
    JPString *JPUsername = nullptr;
    auto JPData = new JPString();
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
                    JPUsername = new JPString(username);
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
        char *token = strtok(data, "  :\t\t´И\u0012'\"£©\u009F¼¬ï¿½.;,`@#$%^&*()_-!?\r/1234567890");
        while (token != NULL) {
            JPString JPToken(token);
            jpStringVec.push_back(JPToken);
            token = strtok(NULL, "  :\t\t´И\u0012'\"£©\u009F¼¬ï¿½.;,`@#$%^&*()_-!?\r/1234567890");
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
                *JPData += jpStringVec.at(i);
                *JPData += " ";
            }
        }
        jpStringVec.clear();
        istream.close();

        dataDto->setData(JPData);
        dataDto->setUsername(JPUsername);
        dataDto->setId(id);
        dataDto->setRowNum(rowNum);
        dataVector.push_back(dataDto);
        JPData = new JPString();
    }
    delete[] data;
    delete[] username;
    delete[] temp;
}
void trainData(vector<DataDTO *> &dataVector, ostream &os, istream &is, map<JPString, int> &wordList,
               vector<TargetDTO *> &targetVector) {
    dataVector.pop_back();
    map<JPString, int>::iterator iteratorWordList;

    char *targetLine = new char[1024];
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
        targetVector.push_back(targetDto);
    }
    int i = 0;
    while (i < dataVector.size()) {
        int j = 0;
        JPString data = *dataVector.at(i)->getData();
        JPString str;
        int add;
        while (data[j] != '\0') {
            if (targetVector.at(i)->getTarget() > 0) {
                add = 1;
            } else { add = -1; }
            if (data[j] != ' ') {
                str += data[j];
            } else {
                iteratorWordList = wordList.find(str);
                if (iteratorWordList != wordList.end()) {
                    wordList[str] = iteratorWordList->second + add;
                } else if (iteratorWordList == wordList.end()) {
                    wordList[str] = add;
                }
                str = "";
            }
            j++;
        }
        i++;
    }
}
