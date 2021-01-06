#include "HybridAnomalyDetector.h"

#define CORRELATION_THRESHOLD 0.9
#define MIN_CORRELATION_THRESHOLD 0.5
#define THRESHOLD_MUL 1.1

HybridAnomalyDetector::HybridAnomalyDetector() {
    // TODO Auto-generated constructor stub
}

void HybridAnomalyDetector::learnNormal(const TimeSeries& ts) {
    for (size_t i = 0; i < ts.propertyCount(); i++) {
        for (size_t j = i; j < ts.propertyCount(); j++) {
            string f1 = ts.getProperty(i);
            string f2 = ts.getProperty(j);

            if (f1.compare(f2) != 0) {
                int size = ts.getPropertyVector(f1).size();
                Point* points[size];
                for (size_t i = 0; i < size; i++) {
                    points[i] = new Point(ts.getPropertyVector(f1).at(i), ts.getPropertyVector(f2).at(i));
                }
                float threshold = 0;
                float correlation = std::abs(pearson(ts.getPropertyVector(f1).data(), ts.getPropertyVector(f2).data(), size));
                if (correlation >= CORRELATION_THRESHOLD) {
                    //learn using linear regression.
                    Line regLine = linear_reg(points, size);
                    for (size_t i = 0; i < size; i++) {
                        float temp = dev(*points[i], regLine);
                        if (temp > threshold) {
                            threshold = temp;
                        }
                    }
                    correlatedFeatures features = {f1, f2, correlation, threshold, regLine, Point(0, 0)};
                    cf.push_back(features);
                } else if (correlation < CORRELATION_THRESHOLD && correlation >= MIN_CORRELATION_THRESHOLD) {
                    //learn using minimalCircle
                    Circle min = findMinCircle(points, size);
                    threshold = min.radius;
                    correlatedFeatures features = {f1, f2, correlation, threshold, Line(), min.center};
                    cf.push_back(features);
                }
                for (size_t i = 0; i < size; i++)
                    delete points[i];
            }
        }
    }
}

vector<AnomalyReport> HybridAnomalyDetector::detect(const TimeSeries& ts) {
    map<string, vector<Point*> > pointMap = featurePointMap(ts);
    vector<correlatedFeatures> vec = cf;
    vector<AnomalyReport> ar;

    for (int i = 0; i < vec.size(); i++) {
        string corr_f = vec[i].feature1 + "-" + vec[i].feature2;

        if (vec[i].corrlation >= CORRELATION_THRESHOLD) {
            for (int j = 1; j < pointMap[corr_f].size(); j++) {
                Point p = *pointMap[corr_f].data()[j];
                float deviation = dev(p, vec[i].lin_reg);

                if (deviation > vec[i].threshold * THRESHOLD_MUL) {
                    //ANOMALY DETECTED!
                    ar.push_back(AnomalyReport(corr_f, j + 1));
                }
            }
        } else if (vec[i].corrlation < CORRELATION_THRESHOLD && vec[i].corrlation >= 0.5) {
            for (size_t j = 1; j < pointMap[corr_f].size(); j++) {
                Point p = *pointMap[corr_f].data()[j];
                float dist = dist2Points(vec[i].center, p);
                if (dist > vec[i].threshold * THRESHOLD_MUL) {
                    //ANOMALY DETECTED!
                    ar.push_back(AnomalyReport(corr_f, j + 1));
                }
            }
        }
    }
    return ar;
}
HybridAnomalyDetector::~HybridAnomalyDetector() {
    // TODO Auto-generated destructor stub
}
