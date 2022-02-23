

#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "AnomalyDetector.h"

#include <string.h>
#include <vector>
#include <algorithm>
#include "minCircle.h"

struct correlatedFeatures {
    std::string feature1, feature2; // names of the correlated features
    float corrlation;
    Line lin_reg;
    float threshold;
    // constructor
    correlatedFeatures(std::string feature1, std::string feature2, float correlation, Line lin_reg, float threshold)
    :
    feature1(feature1), feature2(feature2), corrlation(correlation), lin_reg(lin_reg), threshold(threshold)
    {} 
    float calcDis(Point p) {
        return fabs(dev(p, lin_reg));
    } 
}; 

class SimpleAnomalyDetector : public TimeSeriesAnomalyDetector {

    vector<correlatedFeatures> cf;
    float threshold = 0.9;

public:
    SimpleAnomalyDetector();
    virtual ~SimpleAnomalyDetector();

    /* creats the correlatedFeatures of ts, and stores them in the field cf */
    virtual void learnNormal(const TimeSeries& ts);
    virtual void extractCorrelatedFeatures(float corrValue, const TimeSeries &ts, 
                                int iCol, int JCol, const float *iData , const float *jData, int numOfRows);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    virtual void setCfThreshold(int cfIndex, float value);
    virtual float calcDelta(Point p, int cfIndex);
    void deviationTDB(const TimeSeries& ts, bool isLearnNormal, 
                            vector<AnomalyReport>& vect);
    virtual correlatedFeatures& getCf(int index);
    /* getter */
    virtual vector<correlatedFeatures> & getNormalModel() {
        return cf;
    }
    void setThreshold(float threshold);
    float getThreshold();
};

#endif /* SIMPLEANOMALYDETECTOR_H_ */
