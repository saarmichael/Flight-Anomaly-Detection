

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

struct hybridCF: public correlatedFeatures {
	Circle minCircle;
	bool isCircle = false;

    // constructor
	hybridCF(std::string feature1, std::string feature2, float corrlation, Line line,float threshold, 
             Circle minCircle, bool isCircle)
             :
             correlatedFeatures(feature1, feature2, corrlation, line, threshold),
             minCircle(minCircle),
             isCircle(isCircle)
             {}

	float calcDis(Point p) {
		if (isCircle) {
			return fabs(distance(p, Point(minCircle.center.x, minCircle.center.y)));
		} else {
			return fabs(dev(p, lin_reg));
		}
	}          
};




class HybridAnomalyDetector: public SimpleAnomalyDetector {

vector<hybridCF> cf;
vector<correlatedFeatures> simpleCf;
vector<AnomalyReport> anomalyReport;
bool isDetected = false;

public:
	HybridAnomalyDetector();
	// virtual void learnNormal(const TimeSeries& ts);
	virtual void extractCorrelatedFeatures(float corrValue, const TimeSeries &ts, 
                                int iCol, int JCol, const float *iData , const float *jData, int numOfRows);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
	virtual void setCfThreshold(int cfIndex, float value);
	virtual float calcDelta(Point p, int cfIndex);
    virtual correlatedFeatures& getCf(int index);

	virtual vector<correlatedFeatures> & getNormalModel() {
		// prevent adding duplicating values
		if (!this->simpleCf.size()) {
			for_each(cf.begin(), cf.end(), [this](hybridCF c) {
				correlatedFeatures cf = dynamic_cast<correlatedFeatures&>(c);
				this->simpleCf.push_back(cf);
			});
		}
		return this->simpleCf;
    }

	vector<AnomalyReport> getDetectedAnomalyReport() {
		if (isDetected) {
			return this->anomalyReport;
		}
		 
	}

	vector<AnomalyReport> getDetectedAnomalyReport(const TimeSeries& ts) {
		this->detect(ts);
		return this->anomalyReport;
	}



	virtual ~HybridAnomalyDetector();

};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
