#include "test_comparison.h"

using namespace std;

void CompareResult(const string errorInfo, stringstream& expectedStringStream, stringstream& resultStringStream) {
    string word;
    string expectedString;
    while(expectedStringStream >> word) {
        expectedString += word;
    }
    string resultString;
    while(resultStringStream >> word) {
        resultString += word;
    }
    if(resultString != expectedString) {
        throw TestFailedException(errorInfo, expectedString, resultString);
    }
}

void CompareResult(const string errorInfo, stringstream& expectedStringStream, stringstream& resultStringStream, int& failedCount) {
    string word;
    string expectedString;
    while(expectedStringStream >> word) {
        expectedString += word;
    }
    string resultString;
    while(resultStringStream >> word) {
        resultString += word;
    }
    if(resultString != expectedString) {
        cout << errorInfo + ":Test failed:\n\tExpected: " + expectedString + "\n\tResult: " + resultString << endl;
        failedCount++;
    }
}
