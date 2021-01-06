#ifndef SIMPLEANOMALYDETECTOR_H_
#define SIMPLEANOMALYDETECTOR_H_

#include "anomaly_detection_util.h"
#include "AnomalyDetector.h"
#include <vector>
#include <algorithm>
#include <string.h>
#include <math.h>
#include <map>

struct correlatedFeatures{
	string feature1, feature2;
	float corrlation;
	float threshold;
	Line lin_reg;
	Point center;
};


class SimpleAnomalyDetector: public TimeSeriesAnomalyDetector {
	vector<correlatedFeatures> cf;
	
public:
	SimpleAnomalyDetector();
	virtual ~SimpleAnomalyDetector();

	virtual void learnNormal(const TimeSeries& ts);
	virtual vector<AnomalyReport> detect(const TimeSeries& ts);
	 
	vector<correlatedFeatures> getNormalModel(){
		return cf;
	}
protected:
 map<string, vector<Point*> > featurePointMap(const TimeSeries& ts) {
     map<string, vector<Point*> > _map;
     for (int i = 0; i < ts.propertyCount(); i++) {
         for (int j = i; j < ts.propertyCount(); j++) {
             string f1 = ts.getProperty(i);
             string f2 = ts.getProperty(j);

             if (f1.compare(f2) != 0) {
                 vector<Point*> point_vector;
                 for (int i = 0; i < ts.getPropertyVector(f1).size(); i++) {
                     point_vector.push_back(new Point(ts.getPropertyVector(f1).at(i), ts.getPropertyVector(f2).at(i)));
                 }
                 string corr_name = f1.append("-").append(f2);
                 _map.insert(std::pair<string, vector<Point*> >(corr_name, point_vector));
             }
         }
     }
     return _map;
 }
};



#endif /* SIMPLEANOMALYDETECTOR_H_ */
