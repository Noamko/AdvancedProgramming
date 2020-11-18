#include "timeseries.h"
#include <exception>

vector<vector<float> > transposeFloat(vector<vector<string> > v, int size) {
	vector<vector<float> > trans;
	for(int i = 0; i < size; i++) {
		vector<float> data;
		for(int j = 0; j < v.size(); j++) {
			data.push_back(std::stof(v.at(j).at(i)));
		}
		trans.push_back(data);
	}
	return trans;
}

void TimeSeries::parseCSV(const char* CSVfilename,vector<vector<string> > &matrix, vector<string> &prop) {
	std::ifstream csvfile(CSVfilename);
	std::string line;
	int previndex = 0;
	while(std::getline(csvfile, line)) {
		previndex = 0;
		vector<string> data;
		for(int i = 0; i < line.length(); i++) {
			if(line[i] == ','){
				data.push_back(line.substr(previndex, i - previndex));
				previndex = i + 1;
			}
			else if(i + 1 == line.length()) {
				data.push_back(line.substr(previndex));
			}			
		}
		matrix.push_back(data);
	}
	prop = matrix.front();
	matrix.erase(matrix.begin());
}

TimeSeries::TimeSeries(const char* CSVfilename) {
	parseCSV(CSVfilename, data_matrix, properties);
	vector<vector<float> > float_matrix = transposeFloat(data_matrix,properties.size());
	for(int i = 0; i < float_matrix.size(); i++) {
		propertyValues.insert(std::pair<string, vector<float> >(properties.at(i), float_matrix[i]));
	}
}

string TimeSeries::getProperty(int i) const{
	return this->properties.at(i);
}

const float TimeSeries::getValue(int r, int c) const{
	return std::stof(data_matrix.at(r).at(c));
}
	
vector<float> TimeSeries::getPropertyVector(string property) const{
	return propertyValues.at(property);
}
const int TimeSeries::propertyCount() const {
	return properties.size();
}

//not Tested
void TimeSeries::insert(string* data) {
	vector<string> temp;
	while(data) {
		temp.push_back(data[0]);
		data++;
	}
}