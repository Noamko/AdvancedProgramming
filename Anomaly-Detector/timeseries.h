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
	map<string, vector<string> > propertyValues;	
public:
	TimeSeries(const char* CSVfileName);
	string getProperty(int i);
	float getValue(int r, int c);
	vector<string> getPropertyVector(string property);
};


#endif /* TIMESERIES_H_ */
