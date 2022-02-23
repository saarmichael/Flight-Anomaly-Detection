

#ifndef ANOMALYDETECTOR_H_
#define ANOMALYDETECTOR_H_

#include "math.h"
#include "timeseries.h"
#include <string>
#include <vector>
using namespace std;

class AnomalyReport {
public:
    const string description;
    const long timeStep;
    AnomalyReport(string description, long timeStep)
        : description(description)
        , timeStep(timeStep)
    {
    }
};

// an abstract class (interface) since there are 'pure virtual' (virtual ... () = 0;) methods
class TimeSeriesAnomalyDetector {
public:
	// virtual --> inheriting methods will override these methods (Dynamic Binding)
    virtual void learnNormal(const TimeSeries& ts) = 0;
	// real-time anomaly detection
    virtual vector<AnomalyReport> detect(const TimeSeries& ts) = 0;
    virtual ~TimeSeriesAnomalyDetector() { }
};

#endif /* ANOMALYDETECTOR_H_ */
