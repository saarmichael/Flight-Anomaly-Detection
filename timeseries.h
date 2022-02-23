#ifndef TIMESERIES_H_
#define TIMESERIES_H_

#include <fstream>
#include <string>
#include <map>
#include <vector>


using namespace std;

class TimeSeries {

    vector<string> featuresByIndex;
    map<string, int> featuresByName;
    vector<vector<float>>  dataByRows; // might be vector<vector<string>>
    vector<vector<float>>  dataByCols; 
    int numOfFeatures;
    int numOfRows;

public:
    TimeSeries(const char* CSVfileName);

    // & fits?
    const vector<float>& getFeatureData(int index) const; //To DO: return the data of feature i

    int getNumOfFeatures() const; // TO DO: return numOfFeatures

    // & fits?
    const vector<float>& getRowData(int index) const;

    int getNumOfRows() const; // TODO: return the num of rows

    const string getNameOfFeature(int index) const; //TODO : return the name by the map

    int getIndexOfFeature(string feature) const; //TODO : return the name by the map

private:
    void initData(fstream& file); //TO do :

    void initMap(string& line);

    void initColVectors();
};

#endif /* TIMESERIES_H_ */
