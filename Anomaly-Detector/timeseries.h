#ifndef TIMESERIES_H_
#define TIMESERIES_H_
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <map>

using namespace std;

class TimeSeries{
private:
	const char* filename;
	vector< vector<string> > data_matrix;
	vector<string> properties;
	map<string, vector<float> > propertyValues;	
public:
	TimeSeries(const char* CSVfileName);
	void insert(string* data);	
	string getProperty(int i) const;
	const float getValue(int r, int c) const;
	vector<float> getPropertyVector(string property) const;
	const int propertyCount() const;
};

#endif /* TIMESERIES_H_ */
