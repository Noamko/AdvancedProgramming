#include "timeseries.h"
#include <exception>

vector<vector<string> > transpose(vector<vector<string> > v, int size) {
	vector<vector<string> > trans;
	for(int i = 0; i < size; i++) {
		vector<string> data;
		for(int j = 0; j < v.size(); j++) {
			data.push_back(v[j].at(i));
		}
		trans.push_back(data);
	}
	return trans;
}

TimeSeries::TimeSeries(const char* CSVfilename) {
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
		}
		data_matrix.push_back(data);
	}
	properties = data_matrix.front();
	data_matrix.erase(data_matrix.begin());
	vector<vector<string> > pv = transpose(data_matrix,properties.size());
	for(int i = 0; i < pv.size(); i++) {
		propertyValues.insert(std::pair<string, vector<string> >(properties.at(i), pv[i]));
	}
}

vector<string> TimeSeries::getPropertyVector(string property) {
	return propertyValues[property];
}
string TimeSeries::getProperty(int index) {
	return properties.at(index);
}

float TimeSeries::getValue(int row, int cell) {
	return std::stof(data_matrix.at(row).at(cell));
}

