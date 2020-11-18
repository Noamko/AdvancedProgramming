
#include "SimpleAnomalyDetector.h"

SimpleAnomalyDetector::SimpleAnomalyDetector() {
 	
}

SimpleAnomalyDetector::~SimpleAnomalyDetector() {
	// TODO Auto-generated destructor stub
}

//TODO:
//add threshold as relative error ?

void SimpleAnomalyDetector::learnNormal(const TimeSeries& ts){
	for(int i = 0; i < ts.propertyCount(); i++) {
		for(int j = i; j < ts.propertyCount(); j++) {
			string f1 = ts.getProperty(i);
			string f2 = ts.getProperty(j);

			if(f1.compare(f2) != 0) {
				float* v1 = new float[ts.getPropertyVector(f1).size()];
				float* v2 = new float[ts.getPropertyVector(f2).size()];
				vector<Point*> vp;
				for(int i = 0; i < ts.getPropertyVector(f1).size(); i++) {
					v1[i] = ts.getPropertyVector(f1).at(i);
					v2[i] = ts.getPropertyVector(f2).at(i);
					vp.push_back(new Point(v1[i], v2[i]));
				}
				Point** points = vp.data();
				Line reg = linear_reg(points, vp.size());
				float max = 0;
				for (int i = 0; i < vp.size(); i++) {
					float temp = dev(*vp.at(i), reg);
					if(temp > max) {
						max = temp;
					}
				}

				float correlation = std::abs(pearson(v1,v2, ts.getPropertyVector(f1).size()));
				correlatedFeatures features = {f1, f2, correlation, reg, max, 0.9};
				cf.push_back(features);
				//std::cout << f1 << "-" << f2 << " " << correlation << endl;
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
//TODO:
//itarate trough the map and find anomalys
//classify how to read A-C type of string
vector<AnomalyReport> SimpleAnomalyDetector::detect(const TimeSeries& ts){
	vector<AnomalyReport> ar;
	map<string,vector<Point*> > pointMap = featurePointMap(ts);
	for(int i = 0; i < pointMap["A-C"].size(); i++) {
		cout << pointMap.at("A-C").data()[i]->x << endl;
	}
	return ar;
}