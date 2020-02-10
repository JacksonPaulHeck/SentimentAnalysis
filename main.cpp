#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "JPString.h"
#include "TargetDTO.h"
#include "DataDTO.h"
#include <map>


void parseData(istream &dataIn, char *line, vector<DataDTO *> &dataVector);

void trainData(vector<DataDTO *> &, istream &, map<JPString, int> &, vector<TargetDTO *> &);

void analyzeData(vector<DataDTO *> &, map<JPString, int> &, vector<TargetDTO *> &);

void accuracyCalculation(vector<TargetDTO *> &, vector<TargetDTO *> &, ostream &);

void populateActualTargetVector(ifstream &testTargetIn, vector<TargetDTO *> &);

using namespace std;

int main(int argc, char **argv) {

    cout << "TweetBot -> At ur service" << endl;
    cout << "Opening Files..." << endl;
    ofstream outFile(argv[1]);
    ifstream trainDataIn(argv[2]);
    ifstream trainTargetIn(argv[3]);
    ifstream testDataIn(argv[4]);
    ifstream testTargetIn(argv[5]);

    if (outFile.is_open() && trainDataIn.is_open() && trainTargetIn.is_open() && testDataIn.is_open() &&
        testTargetIn.is_open()) {
        cout << "Files Open" << endl;
    }
    cout << "Creating Variables..." << endl;
    char *line = new char[1024]; //used for file input
    vector<DataDTO *> trainingVector; //holds the Data from the Training Data File
    vector<DataDTO *> dataVector; //holds the Data from the Testing Data File
    map<JPString, int> wordList; //map for word frequency analysis
    vector<TargetDTO *> trainingTargetVector; //holds the Data from the Training Target File
    vector<TargetDTO *> actualTargetVector; //holds the data from the Testing Target File
    cout << "Variables Created" << endl;

    cout << "Parsing Training Data..." << endl;
    parseData(trainDataIn, line, trainingVector);
    cout << "Consider it Parsed" << endl;
    cout << "Training Data..." << endl;
    trainData(trainingVector, trainTargetIn, wordList, trainingTargetVector);
    cout << "The Data Has Been Trained" << endl;
    cout << "Populating Actual Target Vector..." << endl;
    populateActualTargetVector(testTargetIn, actualTargetVector);
    cout << "Target Vector Actually Populated" << endl;

    trainDataIn.clear();
    trainDataIn.seekg(0, ios::beg);
    cout << "Parsing Actual Data..." << endl;
    parseData(testDataIn, line, dataVector);
    cout << "Data Actually Parsed" << endl;
    cout << "Analyzing Data..." << endl;
    analyzeData(dataVector, wordList, trainingTargetVector);
    cout << "Data Analyzed" << endl;
    cout << "Calculating Accuracy..." << endl;
    accuracyCalculation(trainingTargetVector, actualTargetVector, outFile);
    cout << "Close Enough" << endl;
    cout << "Closing Files" << endl;
    trainDataIn.close();
    outFile.close();
    trainTargetIn.close();
    testDataIn.close();
    testTargetIn.close();
    cout << "Files Closed" << endl;

    //deletes the trainingTarget vector
    int k = 0;
    k = 0;
    while (k < trainingTargetVector.size()) {
        delete (trainingTargetVector.at(k));
        k++;
    }
    cout << "Training Target Vector Deleted" << endl;
    //deletes the actual target Vector
    k = 0;
    while (k < actualTargetVector.size()) {
        delete (actualTargetVector.at(k));
        k++;
    }
    cout << "Actual Target Vector Deleted" << endl;

    cout << "end of program" << endl;

    return 0;
}

void populateActualTargetVector(ifstream &testTargetIn, vector<TargetDTO *> &actualTargetVector) {
    TargetDTO *targetDto;
    //temp char*
    char *targetLine = new char[1024];
    //Itterates through the file of the correct target file
    while (!testTargetIn.eof()) {
        targetDto = new TargetDTO();
        //delimits using a ","
        for (int pos = 0; pos < 2; pos++) {
            testTargetIn.getline(targetLine, 12, ',');
            switch (pos) {
                case 0:
                    targetDto->setRowNum(atoi(targetLine));
                    break;
                case 1:
                    targetDto->setTarget(atoi(targetLine));
                    break;
            }
        }
        testTargetIn.get(targetLine, 12, '\n');
        targetDto->setId(atol(targetLine));
        //adds the TargetDTOs to a vector
        actualTargetVector.push_back(targetDto);
    }
}

void accuracyCalculation(vector<TargetDTO *> &targetVector, vector<TargetDTO *> &actualTarget, ostream &testOut) {
    //creates a vector of longs
    vector<long> accuracyVector;
    double total = 0;
    double correct = 0;
    //iterates through each of the target and actual vectors
    while (total < targetVector.size() && total < actualTarget.size()) {
        //if they are equal add one to correct, else add the ID of the incorrect tweet to the vector
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
    //Iterates through the accuracyVector and outputs to a file the IDs of incorrectly analyzed tweets (Vector should be empty ;))
    while (index < accuracyVector.size()) {
        testOut << accuracyVector.at(index) << endl;
        index++;
    }
}

void analyzeData(vector<DataDTO *> &dataVector, map<JPString, int> &wordList,
                 vector<TargetDTO *> &trainingTargetVector) {
    //Creates an iterator for the map
    map<JPString, int>::iterator iteratorWordList;
    int i = 0;
    int result = 0;
    int total = 0;
    //iterates through the vector of each DataDTO
    while (i < dataVector.size()) {
        int j = 0;
        JPString data = *dataVector.at(i)->getData();
        JPString str;
        //itterates through each DataDTO's data for the endline character
        while (data[j] != '\0') {
            total = 0;
            //delimites using a " "
            if (data[j] != ' ') {
                str += data[j];
            }
                //if full word, look for it in the map
            else {
                iteratorWordList = wordList.find(str);
                //while itterating through the list, gets the total, either pos or neg
                if (iteratorWordList != wordList.end() &&
                    (iteratorWordList->second > 3 || iteratorWordList->second < -3)) {
                    total += iteratorWordList->second;
                } else {
                    total += 0;
                }
                str = "";
            }
            j++;
        }
        //sets the result of the whole string to the sum of its parts
        if (total < 0) {
            result = 0;
        } else if (total > 0) {
            result = 4;
        } else {
            srand(3141592);
            result = (rand() % 2) * 4;
        }
        //adds a new TargetDTO with the Guesses of sentiment to a vector
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
        JPData = new JPString();
    }
    delete[] data;
    delete[] username;
    delete[] temp;
}

void trainData(vector<DataDTO *> &trainingVector, istream &trainTargetIn, map<JPString, int> &wordList,
               vector<TargetDTO *> &targetVector) {
    cout << "Start of Training" << endl;
    //Pops the part of the data with header of file off the vector
    trainingVector.pop_back();
    //creates an iterator for the map
    map<JPString, int>::iterator iteratorWordList;

    //Temporary Line for file
    char *targetLine = new char[1024];
    TargetDTO *targetDto = nullptr;

    //Iterates through file
    while (!trainTargetIn.eof()) {
        targetDto = new TargetDTO();
        //Reads data in, delimites using a comma, sets the TargetDTO equal to the data from the file
        for (int pos = 0; pos < 2; pos++) {
            trainTargetIn.getline(targetLine, 12, ',');
            switch (pos) {
                case 0:
                    targetDto->setRowNum(atoi(targetLine));
                    break;
                case 1:
                    targetDto->setTarget(atoi(targetLine));
                    break;
            }
        }
        trainTargetIn.get(targetLine, 12, '\n');
        targetDto->setId(atol(targetLine));
        //Adds the dto to a vector
        targetVector.push_back(targetDto);
    }
    int i = 0;
    //Iterates through the vector
    while (i < trainingVector.size()) {
        int j = 0;
        JPString data = *trainingVector.at(i)->getData();
        JPString str;
        int add;
        //Iterates through the JPString checking for an end line character
        while (data[j] != '\0') {
            //Sets the add variable to what the entire tweets value is
            if (targetVector.at(i)->getTarget() > 0) {
                add = 1;
            } else {
                add = -1;
            }
            if (data[j] != ' ') {
                str += data[j];
            }
                //Delimites by space to find full words
            else {
                //Looks for the word in the map
                iteratorWordList = wordList.find(str);
                //if found, add the value from the training target to that word
                if (iteratorWordList != wordList.end()) {
                    wordList[str] = iteratorWordList->second + add;
                }
                    //Creates the word in the map if not found
                else if (iteratorWordList == wordList.end()) {
                    wordList[str] = add;
                }
                //resets the word to ""
                str = "";
            }
            j++;
        }
        i++;
    }
}
