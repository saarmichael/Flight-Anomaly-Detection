
#include "HybridAnomalyDetector.h"
float maxthreshold = 0.9;
float minthreshold = 0.5;

HybridAnomalyDetector::HybridAnomalyDetector() : SimpleAnomalyDetector()
{
}

HybridAnomalyDetector::~HybridAnomalyDetector()
{
}

// void HybridAnomalyDetector::learnNormal(const TimeSeries &ts) {
// 	SimpleAnomalyDetector::learnNormal(ts);

// }

void HybridAnomalyDetector::extractCorrelatedFeatures(float corrValue, const TimeSeries &ts,
                                                      int iCol, int jCol, const float *iData, const float *jData, int numOfRows)
{
    // if the correlation is between 0.5 and 0.9, use the hybrid methodology
    if (corrValue > minthreshold && corrValue < maxthreshold)
    {
        Point** points = new Point*[numOfRows];
        for (int i = 0; i <= numOfRows; i++) {
            points[i] = new Point(iData[i], jData[i]);
        };
        
        Circle minCircle = findMinCircle(points, numOfRows);

        hybridCF hybridCf(
        ts.getNameOfFeature(iCol),
        ts.getNameOfFeature(jCol),
        corrValue,
        Line(),
        minCircle.radius * 1.1,
        minCircle, 
        true 
        ); 

        this->cf.push_back(hybridCf);
    }  else if ( corrValue >= maxthreshold ) {
        hybridCF hybridCf(
        ts.getNameOfFeature(iCol),
        ts.getNameOfFeature(jCol),
        corrValue,
        linear_reg(iData, jData, numOfRows),
        0,
        Circle(Point(0,0), 0), 
        false 
        ); 
        this->cf.push_back(hybridCf);
        // vector<AnomalyReport> junk;
        // SimpleAnomalyDetector::deviationTDB(ts, true, junk); 
        
    }
}



vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries& ts) {
    vector<AnomalyReport> ar = SimpleAnomalyDetector::detect(ts);
    for_each(ar.begin(), ar.end(), [this](AnomalyReport ar) {
        this->anomalyReport.push_back(ar);
    });
    return this->anomalyReport;
}

void HybridAnomalyDetector::setCfThreshold(int cfIndex, float value) {
    if (!this->cf[cfIndex].isCircle) {
        this->cf[cfIndex].threshold = value;
    }

}


float HybridAnomalyDetector::calcDelta(Point p, int cfIndex) {
    hybridCF currentCf = cf[cfIndex];
    if (currentCf.isCircle) {
        return fabs(distance(p, Point(currentCf.minCircle.center.x, 
                                            currentCf.minCircle.center.y)));
    } else {
        return fabs(dev(p, currentCf.lin_reg));
    }

}

correlatedFeatures& HybridAnomalyDetector::getCf(int index) {
    return this->cf[index];
}