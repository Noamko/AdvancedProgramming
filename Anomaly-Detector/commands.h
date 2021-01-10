
#ifndef COMMANDS_H_
#define COMMANDS_H_

#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <iomanip>
#include "HybridAnomalyDetector.h"

#define COMMAND_COUNT 6
#define S_TIME 0
#define E_TIME 1

using namespace std;

class AnomalyDetectorHelper : public HybridAnomalyDetector {
	vector<AnomalyReport> report;
	int dcount;
public:
	void set_threshold(float f) {
		this->maxThreshold(f);
	}
	void reportAnomalies(const TimeSeries& ts) {
		report = this->detect(ts);
		dcount =ts.getPropertyVector(ts.getProperty(0)).size();
	}
	vector<AnomalyReport> getReport() {
		return report;
	}
	float get_threshold() {
		return this->threshold();
	}
	int dataCount(){
		return dcount;
	}
};

class AnomalyAnalyzer {
	vector<tuple<long,long> > user_reported;
	vector<tuple<long, long> > detector_reported;
public:
	void instert_user_reporetd(tuple<long, long> tp) {
		user_reported.push_back(tp);
	}
	void instert_detector_reporetd(tuple<long, long> tp) {
		detector_reported.push_back(tp);
	}

	int intercut(tuple<long,long> t1, tuple<long,long> t2) {
		int condition1 = (get<E_TIME>(t2) - get<S_TIME>(t1)) * (get<S_TIME>(t1) - get<S_TIME>(t2));
		int condition2 = (get<E_TIME>(t1) - get<S_TIME>(t2)) * (get<E_TIME>(t2) - get<E_TIME>(t1));
		return (condition1 > 0 || condition2 > 0);
	}
	void analayz(int size,float *&tpr, float *&far){
		int P = user_reported.size();
		int FP = 0;
		int TP = 0;
		int cut  = 0;
		int N = size;
		for(int i = 0; i < detector_reported.size(); i++){
			cut = 0;
			for(int j = 0; j < user_reported.size(); j++){
				if (intercut(user_reported[j], detector_reported[i])) {
					TP++;
					cut = 1;
				}
			}
			if(!cut) {
				FP++;
			}
		}

		for (int i = 0; i < detector_reported.size(); i++) {
			N = N - (get<1>(detector_reported[i]) - get<0>(detector_reported[i]) + 1);
		}
		*tpr = (float)TP / (float)P;
		*tpr = floor(*tpr * 1000) / 1000;
		
		*far = (float)FP / (float)N;
		*far = floor(*far * 1000) / 1000;
	}
};

class DefaultIO {
public:
	virtual string read() = 0;
	virtual void write(string text) = 0;
	virtual void writeLine(string text) {
		write(text);
		write("\n");
	}
	virtual void writeLine(float f) {
		write(f);
		write("\n");
	}
	virtual void writeLine() {
		write("\n");
	}
	virtual void write(float f) = 0;
	virtual void read(float* f) = 0;
	virtual ~DefaultIO() {}
};

class Command {
protected:
	DefaultIO* dio;
public:
	Command(DefaultIO* dio) :dio(dio) {}
	virtual void execute() = 0;
	virtual string desciption() = 0;
	virtual ~Command() {}
};

class UploadCSVCommand : public Command {
	DefaultIO* dio;
public:
	UploadCSVCommand(DefaultIO* dio) : Command(dio) {}
	int uploadfile(string filename) {
		string line = dio->read();
		ofstream ofile(filename);
		if(ofile.is_open()) {
			while (line.compare("done") != 0) {
				ofile << line << endl;
				line = dio->read();
				//create a csv file
			}
			ofile.close();
			return 1;
		}
		return -1;
	}
	virtual void execute() {
		dio->writeLine("Please upload your local train CSV file.");
		uploadfile("anomalyTrain.csv");
		dio->writeLine("Upload complete.");

		dio->writeLine("Please upload your local test CSV file.");
		uploadfile("anomalyTest.csv");
		dio->writeLine("Upload complete.");
	}
	virtual string desciption() override {
		return "upload a time series csv file";
	}
	virtual ~UploadCSVCommand() {}
};

class SettingCommand : public Command {
	AnomalyDetectorHelper* h;
	public:
	SettingCommand(DefaultIO* dio, AnomalyDetectorHelper* adh) : Command(dio),h(adh) {}
	virtual void execute() {
		dio->write("The current correlation threshold is ");
		dio->writeLine(h->get_threshold());
	}
	virtual string desciption() {
		return "algorithm settings";
	}
	virtual ~SettingCommand() {}
};

class DetectCommand : public Command {
	AnomalyDetectorHelper *helper;
public:
	DetectCommand(DefaultIO* dio, AnomalyDetectorHelper* helper) : Command(dio), helper(helper) {}
	virtual void execute() {
		TimeSeries ts_learn("anomalyTrain.csv");
		helper->learnNormal(ts_learn);
		TimeSeries ts_test("anomalyTest.csv");
		helper->reportAnomalies(ts_test);

		dio->writeLine("anomaly detection complete.");
	}
	virtual string desciption() {
		return "detect anomalies";
	}
	virtual ~DetectCommand() {}
};

class DisplayCommand : public Command {
	AnomalyDetectorHelper* helper;
public:
	DisplayCommand(DefaultIO* dio, AnomalyDetectorHelper* helper) : Command(dio), helper(helper) {}
	virtual void execute() {
		vector<AnomalyReport> v = helper->getReport();
		for(int i = 0; i < v.size(); i++){
			dio->write(v[i].timeStep);
			dio->writeLine("\t" + v[i].description);
		}
		dio->writeLine("done.");
		
	}
	virtual string desciption() {

		return "display results";
	}
	virtual ~DisplayCommand() {}
};

class UploadResultCommand : public Command {
	AnomalyDetectorHelper* helper;
public:
	UploadResultCommand(DefaultIO* dio, AnomalyDetectorHelper* helper) : Command(dio), helper(helper) {}
	virtual void execute() {
		dio->writeLine("Please upload your local anomalies file.");
		string line = dio->read();
		AnomalyAnalyzer aa;
		while(line.compare("done")) {
			int sep = line.find(",");
			string stime = line.substr(0, sep);
			string etime = line.substr(sep + 1);
			tuple <long, long> tup(stoi(stime), stoi(etime));
			aa.instert_user_reporetd(tup);
			line = dio->read();
		}
		dio->writeLine("Upload complete.");

		vector<AnomalyReport> v = helper->getReport();
		AnomalyReport rep = v.at(0);
		string dec = rep.description;
		long stime = rep.timeStep;
		long etime = stime;
		for (int i = 0; i < v.size(); i++) {
			AnomalyReport rep = v.at(i);
			if (dec.compare(rep.description)) {
				tuple<long,long> tup(stime,etime);
				aa.instert_detector_reporetd(tup);

				stime = rep.timeStep;
				dec = rep.description;
			}
			else {
				etime = rep.timeStep;
			}
		}
		tuple<long, long> tup(stime, etime);
		aa.instert_detector_reporetd(tup);
		float* tpr = new float;
		float* far = new float;
		aa.analayz(helper->dataCount(),tpr, far);
		dio->write("True Positive Rate: ");
		dio->writeLine(*tpr);
		dio->write("False Positive Rate: ");
		dio->writeLine(*far);
	}
	virtual string desciption() {
		return "upload anomalies and analyze results";
	}
	virtual ~UploadResultCommand() {}
};

class ExitCommand : public Command {
	int exit;
public:
	ExitCommand(DefaultIO* dio, int &exit) : Command(dio),exit(exit) {}
	virtual void execute() {
		exit = 1;
	}
	virtual string desciption() {
		return "exit";
	}
	virtual ~ExitCommand() {}
};

// implement here your command classes

#endif /* COMMANDS_H_ */

