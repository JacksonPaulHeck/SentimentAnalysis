#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include "JPString.h"
#include "TargetDTO.h"
#include "DataDTO.h"
#include <map>

void analyzeData(istream&, map<JPString, int> &, char *, istream &, ostream&, char*);

void parseDataAndTrain(istream &, istream &, char *, map<JPString, int> &);

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
    char * line2 = new char[2048];
    char *line = new char[2048]; //used for file input
    vector<DataDTO *> dataVector; //holds the Data from the Testing Data File
    map<JPString, int> wordList; //map for word frequency analysis
    cout << "Variables Created" << endl;
    cout << "Parsed Actual Data" << endl;

    parseDataAndTrain(trainDataIn, trainTargetIn, line, wordList);

    cout << "Parsed Data" << endl;

    analyzeData(testDataIn, wordList, line, testTargetIn, outFile, line2);

    cout << "Analyzed Data" << endl;
    trainDataIn.close();
    outFile.close();
    trainTargetIn.close();
    testDataIn.close();
    testTargetIn.close();

    //deletes the trainingTarget vector
    delete[] line;
    cout << "Actual Target Vector Deleted" << endl;

    cout << "end of program" << endl;

    return 0;
}

void parseDataAndTrain(istream &trainingDataIn, istream &trainingTargetIn, char *line, map<JPString, int> &wordList) {
    //Declare Variables for parseData function
    int rowNum = 0;
    long id = 0;
    char *username = new char[2048];
    char *data = new char[2048];

    //Open StopWord File
    ifstream stopWordStream("StopWordList.csv");
    char *temp = new char[20];
    vector<JPString *> jpStringVec;
    vector<JPString *> stopWordVector;
    map<JPString, int>::iterator iteratorWordList;
    //Temporary Line for file
    char *targetLine = new char[2048];

    //Add stop words to vector
    int k = 0;
    int j = 0;
    stopWordStream.getline(temp, 20);
    while (!stopWordStream.eof()) {
        JPString *JPTemp = new JPString(temp);
        stopWordVector.push_back(JPTemp);
        stopWordStream.getline(temp, 20);
        k++;
    }
    stopWordStream.close();
    //Loop to parse through data until end of file

    while (!trainingDataIn.eof() && !trainingTargetIn.eof()) {
        auto JPData = new JPString();
        auto *dataDto = new DataDTO();
        auto *JPUsername = new JPString();
        auto *targetDto = new TargetDTO();
        //Parse through each line using comma as a delimeter
        for (int pos = 0; pos < 3; pos++) {
            trainingDataIn.getline(line, 2048, ',');
            switch (pos % 3) {
                case 0:
                    rowNum = atoi(line);
                    break;
                case 1:
                    id = atol(line);
                    break;
                case 2:
                    delete (JPUsername);
                    strncpy(username, line, 2048);
                    JPUsername = new JPString(username);
                    break;
            }
        }
        //get "string" of data
        trainingDataIn.get(line, 2048, '\n');
        strncpy(data, line, 2048);
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
            JPString *JPToken = new JPString(token);
            jpStringVec.push_back(JPToken);
            token = strtok(NULL, "  :\t\t´И\u0012'\"£©\u009F¼¬ï¿½.;,`@#$%^&*()_-!?\r/1234567890");
        }
        //compare each element of jpStringVec to each value in stopWordVector
        for (int i = 0; i < jpStringVec.size(); i++) {
            j = 0;
            bool isEqual = false;
            while (j < stopWordVector.size()) {
                //if element is a stopWord remove it from the string
                if (jpStringVec.at(i) == stopWordVector.at(j)) {
                    isEqual = true;
                }
                j++;
            }
            //Create JPString of the data
            if (!isEqual && jpStringVec.at(i)->size() > 2) {
                *JPData += *jpStringVec.at(i);
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


        //Iterates through file

        //Reads data in, delimites using a comma, sets the TargetDTO equal to the data from the file
        for (int pos = 0; pos < 2; pos++) {
            trainingTargetIn.getline(targetLine, 12, ',');
            switch (pos) {
                case 0:
                    targetDto->setRowNum(atoi(targetLine));
                    break;
                case 1:
                    targetDto->setTarget(atoi(targetLine));
                    break;
            }
        }
        trainingTargetIn.get(targetLine, 12, '\n');
        targetDto->setId(atol(targetLine));
        //Adds the dto to a vector

        //Iterates through the vector
        j = 0;
        JPString JPStringData = *dataDto->getData();
        JPString str;
        int add;
        //Iterates through the JPString checking for an end line character
        while (JPStringData[j] != '\0') {
            //Sets the add variable to what the entire tweets value is
            if (targetDto->getTarget() > 0) {
                add = 1;
            } else {
                add = -1;
            }
            if (JPStringData[j] != ' ') {
                str += JPStringData[j];
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
        //clear the string
        delete (dataDto);
        delete (targetDto);
        delete (JPUsername);
        delete (JPData);

        k = 0;
        while (k < jpStringVec.size()) {
            delete (jpStringVec.at(k));
            k++;
        }
    }
    k=0;
    while(k < stopWordVector.size()){
        delete(stopWordVector.at(k));
        k++;
    }
    delete[] data;
    delete[] username;
    delete[] temp;
} //creates the map

void analyzeData(istream& dataIn, map<JPString, int> &wordList, char *targetLine, istream &testTargetIn, ostream& testOut, char* line) {
    //Creates an iterator for the map
    map<JPString, int>::iterator iteratorWordList;
    vector<long> accuracyVector;
    int result = 0;
    int total = 0;
    double correct = 0;
    double totalTweets = 0;
    int rowNum = 0;
    long id = 0;
    char *username = new char[1024];
    char *data = new char[1024];
    JPString *JPUsername = nullptr;
    auto *JPData = new JPString();
    DataDTO *dataDto = new DataDTO();;
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

    dataIn.clear();
    dataIn.seekg(0, ios::beg);

    while (!dataIn.eof()) {

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
        char *token = strtok(data, "  :\t\t´И\u0012'\"£©\u009F¼¬ï¿½.;,`@#$%^&*()_-!?\r/1234567890");
        //add each word to vector jpStringVec
        while (token != NULL) {
            JPString JPToken(token);
            jpStringVec.push_back(JPToken);
            token = strtok(NULL, "  :\t\t´И\u0012'\"£©\u009F¼¬ï¿½.;,`@#$%^&*()_-!?\r/1234567890");
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
        //clear the string



    //iterates through the vector of each DataDTO

        int j = 0;
        JPString dataFromDTO = *dataDto->getData();
        JPString str;
        //itterates through each DataDTO's data for the endline character
        while (dataFromDTO[j] != '\0') {
            total = 0;
            //delimites using a " "
            if (dataFromDTO[j] != ' ') {
                str += dataFromDTO[j];
            }
                //if full word, look for it in the map
            else {
                iteratorWordList = wordList.find(str);
                //while itterating through the list, gets the total, either pos or neg
                if (iteratorWordList != wordList.end() &&
                    (iteratorWordList->second > 10 || iteratorWordList->second < -10)) {
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
        auto *targetDto = new TargetDTO(dataDto->getRowNum(), result, dataDto->getId());

        testTargetIn.getline(targetLine, 12);
        auto *actualTargetDto = new TargetDTO();
        //delimits using a ","
        for (int pos = 0; pos < 2; pos++) {
            testTargetIn.getline(targetLine, 12, ',');
            switch (pos) {
                case 0:
                    actualTargetDto->setRowNum(atoi(targetLine));
                    break;
                case 1:
                    actualTargetDto->setTarget(atoi(targetLine));
                    break;
            }
        }
        testTargetIn.get(targetLine, 12, '\n');
        actualTargetDto->setId(atol(targetLine));
        if (targetDto->getTarget() == actualTargetDto->getTarget()) {
            correct++;
        } else {
            accuracyVector.push_back(targetDto->getId());
        }
        totalTweets++;
        JPData = new JPString();
        dataDto = new DataDTO();
    }

    testOut << "Accuracy: ";
    testOut << correct / totalTweets << endl;
    int index = 0;
    //Iterates through the accuracyVector and outputs to a file the IDs of incorrectly analyzed tweets (Vector should be empty ;))
    while (index < accuracyVector.size()) {
        testOut << accuracyVector.at(index) << endl;
        index++;
    }
    delete[] data;
    delete[] username;
    delete[] temp;
}

