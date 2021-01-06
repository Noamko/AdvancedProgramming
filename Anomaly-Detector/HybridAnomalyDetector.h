

#ifndef HYBRIDANOMALYDETECTOR_H_
#define HYBRIDANOMALYDETECTOR_H_

#include "SimpleAnomalyDetector.h"
#include "minCircle.h"

class HybridAnomalyDetector:public SimpleAnomalyDetector {
    vector<correlatedFeatures> cf;

   public:
    HybridAnomalyDetector();
    virtual void learnNormal(const TimeSeries& ts);
    virtual vector<AnomalyReport> detect(const TimeSeries& ts);
    virtual ~HybridAnomalyDetector();
    vector<correlatedFeatures> getNormalModel() {
        return cf;
    }
};

#endif /* HYBRIDANOMALYDETECTOR_H_ */
