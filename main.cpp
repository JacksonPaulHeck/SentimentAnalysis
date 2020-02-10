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

void parseData(istream &dataIn, char *line, vector<DataDTO *> &dataVector);

void trainData(vector<DataDTO *> &, istream &, map<JPString, int> &, vector<TargetDTO *> &);

void analyzeData(vector<DataDTO *> &, map<JPString, int> &, vector<TargetDTO *> &);

void accuracyCalculation(vector<TargetDTO *> &, vector<TargetDTO *> &, ostream &);

void populateActualTargetVector(ifstream &testTargetIn, vector<TargetDTO *> &);

using namespace std;

int main(int argc, char **argv) {

//    ofstream outFile("output.txt");
//    ifstream trainDataIn("dev-train-data-sm.csv");
//    ifstream trainTargetIn("dev-train-target-sm.csv");
//    ifstream testDataIn("dev-test-data.csv");
//    ifstream testTargetIn("dev-test-target.csv");
//    ofstream outFile("output.txt");
//    ifstream trainDataIn("TrainData.csv");
//    ifstream trainTargetIn("TrainTarget.csv");
//    ifstream testDataIn("TestData.csv");
//    ifstream testTargetIn("TestTarget.csv");
    ofstream outFile(argv[1]);
    ifstream trainDataIn(argv[2]);
    ifstream trainTargetIn(argv[3]);
    ifstream testDataIn(argv[4]);
    ifstream testTargetIn(argv[5]);

    if (outFile.is_open() && trainDataIn.is_open() && trainTargetIn.is_open() && testDataIn.is_open() &&
        testTargetIn.is_open()) {
        cout << "Files Open" << endl;
    }
    char *line = new char[1024];
    vector<DataDTO *> trainingVector;
    vector<DataDTO *> dataVector;
    map<JPString, int> wordList;
    vector<TargetDTO *> trainingTargetVector;
    vector<TargetDTO *> actualTargetVector;
    cout << "Variables Created" << endl;

    parseData(trainDataIn, line, trainingVector);
    cout << "Parsed Training Data" << endl;
    trainData(trainingVector, trainTargetIn, wordList, trainingTargetVector);
    cout << "Trained Data" << endl;
    int k = 0;
    while(k < trainingVector.size()){
        delete(trainingVector.at(k));
        k++;
    }

    populateActualTargetVector(testTargetIn, actualTargetVector);
    cout << "Populated Actual Target Vector" << endl;

    trainDataIn.clear();
    trainDataIn.seekg(0, ios::beg);

    parseData(testDataIn, line, dataVector);
    cout << "Parsed Actual Data" << endl;
    analyzeData(dataVector, wordList, trainingTargetVector);
    cout << "Analyzed Data" << endl;

    k = 0;
    while(k < dataVector.size()){
        delete(dataVector.at(k));
        k++;
    }
    cout << "Data Vector Deleted" << endl;

    accuracyCalculation(trainingTargetVector, actualTargetVector, outFile);

    k = 0;
    while(k < trainingTargetVector.size()){
        delete(trainingTargetVector.at(k));
        k++;
    }
    k=0;
    while(k < actualTargetVector.size()){
        delete(actualTargetVector.at(k));
        k++;
    }

    trainDataIn.close();
    outFile.close();
    trainTargetIn.close();
    testDataIn.close();
    testTargetIn.close();
    cout << "end of program" << endl;

    return 0;
}

void populateActualTargetVector(ifstream &testTargetIn, vector<TargetDTO *> &actualTargetVector) {
    TargetDTO* targetDto;
    char *targetLine = new char[1024];
    while (!testTargetIn.eof()) {
        targetDto = new TargetDTO();
        for (int pos = 0; pos < 2; pos++) {
            testTargetIn.getline(targetLine, 1024, ',');
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
        actualTargetVector.push_back(targetDto);
    }
}

void accuracyCalculation(vector<TargetDTO *> &targetVector, vector<TargetDTO *> &actualTarget, ostream &testOut) {
    vector<long> accuracyVector;
    double total = 0;
    double correct = 0;
    while (total < targetVector.size() && total < actualTarget.size()) {
        if (targetVector.at(total)->getTarget() == actualTarget.at(total)->getTarget()) {
            correct++;
        } else {
            accuracyVector.push_back(targetVector.at(total)->getId());
        }
        total++;
    }
    testOut << "Accuracy: ";
    testOut << correct / total << endl;
    int index = 0;
    while (index < accuracyVector.size()) {
        testOut << accuracyVector.at(index);
        index++;
    }
}

void analyzeData(vector<DataDTO *> &dataVector, map<JPString, int> &wordList,
                 vector<TargetDTO *> &trainingTargetVector) {
    map<JPString, int>::iterator iteratorWordList;
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
        trainingTargetVector.push_back(targetDto);
        i++;
    }
    trainingTargetVector.pop_back();
}

void parseData(istream &dataIn, char *line, vector<DataDTO *> &dataVector) {
    //Declare Variables for parseData function
    int rowNum = 0;
    long id = 0;
    char *username = new char[1024];
    char *data = new char[1024];
    JPString *JPUsername = nullptr;
    auto JPData = new JPString();
    DataDTO *dataDto = nullptr;
    //Open StopWord File
    ifstream stopWordStream("StopWordList.csv");
    char *temp = new char[20];
    vector<JPString> jpStringVec;
    vector<JPString> stopWordVector;
    //Add stop words to vector
    int k = 0;
    stopWordStream.getline(temp, 20);
    while (!stopWordStream.eof()) {
        JPString JPTemp(temp);
        stopWordVector.push_back(JPTemp);
        stopWordStream.getline(temp, 20);
        k++;
    }
    stopWordStream.close();
    int r =0;
    //Loop to parse through data until end of file
    while (!dataIn.eof()) {
        dataDto = new DataDTO();
        //Parse through each line using comma as a delimeter
        for (int pos = 0; pos < 3; pos++) {
            dataIn.getline(line, 1024, ',');
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
        //get "string" of data
        dataIn.get(line, 1024, '\n');
        strncpy(data, line, 1024);
        //make data lowercase
        int r = 0;
        while (data[r] != '\0') {
            data[r] = tolower(data[r]);
            r++;
        }
        //remove all characters that are not letters
        char *token = strtok(data, "  :\t\t´И\u0012'\"£©\u009F¼¬ï¿½.;,`@#$%^&*()_-!?\r/1234567890");
        //add each word to vector jpStringVec
        while (token != NULL) {
            JPString JPToken(token);
            jpStringVec.push_back(JPToken);
            token = strtok(NULL, "  :\t\t´И\u0012'\"£©\u009F¼¬ï¿½.;,`@#$%^&*()_-!?\r/1234567890");
        }
        //compare each element of jpStringVec to each value in stopWordVector
        for (int i = 0; i < jpStringVec.size(); i++) {
            int j = 0;
            bool isEqual = false;
            while (j < stopWordVector.size()) {
                //if element is a stopWord remove it from the string
                if (jpStringVec.at(i) == stopWordVector.at(j)) {
                    isEqual = true;
                }
                j++;
            }
            //Create JPString of the data
            if (!isEqual && jpStringVec.at(i).size() > 2) {
                *JPData += jpStringVec.at(i);
                *JPData += " ";
            }
        }
        //clear vector
        jpStringVec.clear();

        //Populate a DataDTO with the data
        dataDto->setData(JPData);
        dataDto->setUsername(JPUsername);
        dataDto->setId(id);
        dataDto->setRowNum(rowNum);
        //Add each DataDTO to a vector
        dataVector.push_back(dataDto);
        //clear the string
        delete JPData;
        JPData = new JPString();
    }
    delete[] data;
    delete[] username;
    delete[] temp;
}

void trainData(vector<DataDTO *> &dataVector, istream &trainTargetIn, map<JPString, int> &wordList,
               vector<TargetDTO *> &targetVector) {
    dataVector.pop_back();
    map<JPString, int>::iterator iteratorWordList;

    char *targetLine = new char[1024];
    TargetDTO *targetDto = nullptr;
    while (!trainTargetIn.eof()) {
        targetDto = new TargetDTO();
        for (int pos = 0; pos < 2; pos++) {
            trainTargetIn.getline(targetLine, 1024, ',');
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
    int r =0;
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
