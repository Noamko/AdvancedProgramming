

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
    vector<correlatedFeatures> cf;

    float max_threshold = 0.9;
    float min_threshold = 0.5;
   public:
    HybridAnomalyDetector();
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    virtual ~HybridAnomalyDetector();
    vector<correlatedFeatures> getNormalModel() {
        return cf;
    }
    void maxThreshold(float t) {
        max_threshold = t;
    }
    void minThreshold(float t) {
        min_threshold = t;
    }
    float threshold() {
        return max_threshold;
    }
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
