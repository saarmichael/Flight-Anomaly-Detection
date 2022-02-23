#include "timeseries.h"
#include <sstream>
#include <iostream>

using namespace std;

TimeSeries::TimeSeries(const char* CSVfileName)
{
    fstream file(CSVfileName);
    // check if file was succesfully opened
    if (!file.is_open()) {
        cout << "file did not open" << endl;
        return;
    }
    
    initData(file);
    file.close();
}

void TimeSeries::initData(fstream& file)
{
    string s, line, value;
    // get the first input line, which contains the features names and initialize features map
    file >> s;
    // getline(file, line);
    initMap(s);
    initColVectors();
    // run through the file and get the data
    while (file >> s) {
        vector<float> row;
        // extract current line from file and put it in line
        // getline(file, line);
        stringstream commaDivider(s);
        int i = 0;
        // extract the comma-seperated values in line
        while (getline(commaDivider, value, ',')) {
            //add the value (converted to float), to the back of the row data vector
            float realVal = stof(value);
            //  -- MIGHT CRASH EVERYTHING - uninitialized vector!!!! will it work?
            row.push_back(realVal);
            // in dataByCols, go to the i'th collumn and add the current element
            this->dataByCols[i].push_back(realVal);
            i++;
        }
        //  -- MIGHT CRASH EVERYTHING - uninitialized vector!!!! will it work?
        // add the row to the data table
        this->dataByRows.push_back(row);
    }
}

void TimeSeries::initMap(string& line)
{
    string featureName;
    int numOfElement = 0;
    stringstream commaDivider(line);
    while (getline(commaDivider, featureName, ',')) {
        this->featuresByIndex.push_back(featureName);
        this->featuresByName.insert({ featureName, numOfElement });
        numOfElement++;
        // MUST CHECK IF WORKS
    }
    this->numOfFeatures = numOfElement;
}

void TimeSeries::initColVectors() {
    //  -- MIGHT CTASH EVERYTHING - uninitialized vector!!!! will it work?
    vector<float> v;
    this->dataByCols.resize(this->numOfFeatures);
}


const vector<float>& TimeSeries::getFeatureData(int index) const {
    return this->dataByCols[index];
}

int TimeSeries::getNumOfFeatures() const {
    return this->dataByCols.size();
}

const vector<float>& TimeSeries::getRowData(int index) const {
    return this->dataByRows[index];
}

int TimeSeries::getNumOfRows() const {
    return this->dataByRows.size();
}

const string TimeSeries::getNameOfFeature(int index) const {
    // SOLVE
    return this->featuresByIndex[index];
}

int TimeSeries::getIndexOfFeature(string feature) const {
    try {
        return this->featuresByName.at(feature);
    } catch (const std::out_of_range&) {
        cout << "did not find key in map" << endl;
        return -1;
    }
    
}