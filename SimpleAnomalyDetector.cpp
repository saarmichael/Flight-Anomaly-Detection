
#include "SimpleAnomalyDetector.h"
#include <typeinfo>

//float threshold = 0.9;
float devMultiplier = 1.17;

void SimpleAnomalyDetector::deviationTDB(const TimeSeries &ts, bool isLearnNormal, vector<AnomalyReport> &vect)
{

    vector<correlatedFeatures> cf = this->getNormalModel();
    size_t cfSize = cf.size();
    size_t rowSize = ts.getNumOfRows();
    for (size_t i = 0; i < cfSize; i++)
    {   
        correlatedFeatures &cf = getCf(i);
        for (size_t j = 0; j < rowSize; j++)
        {
            vector<float> row = ts.getRowData(j);
            int index1 = ts.getIndexOfFeature(cf.feature1);
            int index2 = ts.getIndexOfFeature(cf.feature2);
            
            // float deltaValue = cf.calcDis(Point(row[index1], row[index2]));
            float deltaValue = calcDelta(Point(row[index1], row[index2]), i);
            if (isLearnNormal)
            {
                deltaValue *= devMultiplier;
            }
            // find an anomaly by comparing to threshold
            if (deltaValue > cf.threshold)
            {
                if (isLearnNormal)
                {
                    setCfThreshold(i, deltaValue);
                    // cf.threshold = deltaValue;
                }
                else
                {
                    // add the anomaly to the list
                    vect.push_back(AnomalyReport(cf.feature1 + "-" + cf.feature2, j + 1));
                }
            }
        }
    }
}

void SimpleAnomalyDetector::setCfThreshold(int cfIndex, float value)
{
    this->cf[cfIndex].threshold = value;
}
correlatedFeatures &SimpleAnomalyDetector::getCf(int index)
{
    return this->cf[index];
}

float SimpleAnomalyDetector::calcDelta(Point p, int cfIndex)
{
    return fabs(dev(p, cf[cfIndex].lin_reg));
}

SimpleAnomalyDetector::SimpleAnomalyDetector()
{
    return;
}

SimpleAnomalyDetector::~SimpleAnomalyDetector()
{
    return;
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries &ts)
{
    int n = ts.getNumOfFeatures();
    int numOfRows = ts.getNumOfRows();

    for (int i = 0; i < n; i++)
    {
        int correlatedCol = -1;
        float maxCorr = 0;
        const float *iData = &(ts.getFeatureData(i))[0];
        const float *correlatedColData;

        for (int j = i + 1; j < n; j++)
        {
            const float *jData = &(ts.getFeatureData(j))[0];
            float p = fabs(pearson(iData, jData, numOfRows));
            // removed "&& p > threshold" from here !!
            if (p > maxCorr)
            {
                correlatedCol = j;
                maxCorr = p;
                correlatedColData = jData;
            }
        }

        // adding the corralated column for i
        if (correlatedCol != -1)
        {
            // find maxDeviation
            extractCorrelatedFeatures(maxCorr, ts, i, correlatedCol, iData, correlatedColData, numOfRows);
        }
    }

    vector<AnomalyReport> junk;
    deviationTDB(ts, true, junk); // FIX
}

void SimpleAnomalyDetector::extractCorrelatedFeatures(float corrValue, const TimeSeries &ts,
                                                      int iCol, int jCol, const float *iData, const float *jData, int numOfRows)
{

    if (corrValue > threshold)
    {
        correlatedFeatures corFeat = {
            ts.getNameOfFeature(iCol),
            ts.getNameOfFeature(jCol),
            corrValue,
            linear_reg(iData, jData, numOfRows),
            0};

        SimpleAnomalyDetector::cf.push_back(corFeat);
    }
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries &ts)
{
    vector<AnomalyReport> anomalyReports;
    deviationTDB(ts, false, anomalyReports);
    return anomalyReports;
}

void SimpleAnomalyDetector::setThreshold(float threshold)
{
    this->threshold = threshold;
}

float SimpleAnomalyDetector::getThreshold()
{
    return this->threshold;
}
