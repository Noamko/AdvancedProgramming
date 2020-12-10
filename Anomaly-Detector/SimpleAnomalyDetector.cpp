#include "SimpleAnomalyDetector.h"

#define CORRELATION_THRESHOLD 0.9
#define THRESHOLD_MUL 1.2

SimpleAnomalyDetector::SimpleAnomalyDetector() {
     
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
}

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
    for(int i = 0; i < ts.propertyCount(); i++) {
        for(int j = i; j < ts.propertyCount(); j++) {
            string f1 = ts.getProperty(i);
            string f2 = ts.getProperty(j);

            //ts.getPropertyVector(f1).size(); should be equal to ts.getPropertyVector(f2).size();
            //add someting to check that.
            if(f1.compare(f2) != 0) {
                vector<Point*> vp;
                for(int i = 0; i < ts.getPropertyVector(f1).size(); i++) {
                    vp.push_back(new Point(ts.getPropertyVector(f1).at(i), ts.getPropertyVector(f2).at(i)));
                }

                //Learn
                Line reg = linear_reg(vp.data(), vp.size());
                float threshold = 0;
                for (int i = 0; i < vp.size(); i++) {
                    float temp = dev(*vp.at(i), reg);
                    if(temp > threshold) {
                        threshold = temp;
                    }
                }
                float correlation = std::abs(pearson(ts.getPropertyVector(f1).data(),
                                    ts.getPropertyVector(f2).data(), ts.getPropertyVector(f1).size()));
                correlatedFeatures features = {f1, f2, correlation, reg, threshold};
                if(correlation > CORRELATION_THRESHOLD) cf.push_back(features);
            }
        }
    }
}

map<string, vector<Point*> > featurePointMap(const TimeSeries& ts) {
    map<string, vector<Point*> > _map;
    for(int i = 0; i < ts.propertyCount(); i++) {
        for(int j = i; j < ts.propertyCount(); j++) {
            string f1 = ts.getProperty(i);
            string f2 = ts.getProperty(j);

            if(f1.compare(f2) != 0) {
                vector<Point*> point_vector;
                for(int i = 0; i < ts.getPropertyVector(f1).size(); i++) {
                    point_vector.push_back(new Point(ts.getPropertyVector(f1).at(i), ts.getPropertyVector(f2).at(i)));
                }
                string corr_name = f1.append("-").append(f2);
                _map.insert(std::pair<string,vector<Point*> >(corr_name, point_vector));
            }
        }
    }
    return _map;
}

vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){

    map<string,vector<Point*> > pointMap = featurePointMap(ts);
    vector<correlatedFeatures> vec = cf;
    vector<AnomalyReport> ar;

    for(int i = 0; i < cf.size(); i++) {

        string corr_f = cf[i].feature1 + "-" + cf[i].feature2;

        if(cf[i].corrlation > CORRELATION_THRESHOLD) {

            for(int j = 1; j < pointMap[corr_f].size(); j++) {
                Point p = *pointMap[corr_f].data()[j];
                float deviation = dev(p,cf[i].lin_reg);
                
                if(deviation > cf[i].threshold * THRESHOLD_MUL) {
                    
                    //ANOMALY DETECTED!
                    ar.push_back(AnomalyReport(corr_f, j + 1));
                }
            }
        }
    }
    return ar;
}