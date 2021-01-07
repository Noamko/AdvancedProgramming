

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

#define COMMAND_COUNT 6

using namespace std;

class DefaultIO {
public:
	virtual string read() = 0;
	virtual void write(string text) = 0;
	virtual void write(float f) = 0;
	virtual void read(float* f) = 0;
	virtual ~DefaultIO() {}

	// you may add additional methods here
};

// you may add here helper classes


// you may edit this class
class Command {
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
	virtual void execute() {
		//todo
	}
	virtual string desciption() override {
		return "upload a time series csv file";
	}
	virtual ~UploadCSVCommand() {}
};

class SettingCommand : public Command {
	DefaultIO* dio;
	public:
	SettingCommand(DefaultIO* dio) : Command(dio) {}
	virtual void execute() {
		//todo
	}
	virtual string desciption() {
		return "algorithm settings";
	}
	virtual ~SettingCommand() {}
};

class DetectCommand : public Command {
	DefaultIO* dio;
public:
	DetectCommand(DefaultIO* dio) : Command(dio) {}
	virtual void execute() {
		//todo
	}
	virtual string desciption() {
		return "detect anomalies";
	}
	virtual ~DetectCommand() {}
};

class DisplayCommand : public Command {
	DefaultIO* dio;
public:
	DisplayCommand(DefaultIO* dio) : Command(dio) {}
	virtual void execute() {
		//todo
	}
	virtual string desciption() {
		return "display results";
	}
	virtual ~DisplayCommand() {}
};

class UploadResultCommand : public Command {
	DefaultIO* dio;
public:
	UploadResultCommand(DefaultIO* dio) : Command(dio) {}
	virtual void execute() {
		//todo
	}
	virtual string desciption() {
		return "upload anomalies and analyze results";
	}
	virtual ~UploadResultCommand() {}
};

class ExitCommand : public Command {
	DefaultIO* dio;
public:
	ExitCommand(DefaultIO* dio) : Command(dio) {}
	virtual void execute() {
		//todo
	}
	virtual string desciption() {
		return "exit";
	}
	virtual ~ExitCommand() {}
};

// implement here your command classes



#endif /* COMMANDS_H_ */

