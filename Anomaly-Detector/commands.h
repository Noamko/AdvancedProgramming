

#ifndef COMMANDS_H_
#define COMMANDS_H_

#include<iostream>
#include <string.h>

#include <fstream>
#include <vector>
#include "HybridAnomalyDetector.h"

using namespace std;

class DefaultIO{
public:
	virtual string read()=0;
	virtual void write(string text)=0;
	virtual void write(float f)=0;
	virtual void read(float* f)=0;
	virtual ~DefaultIO(){}
        // you may add additional methods here
};

class StandardIO : public DefaultIO {
public:
    virtual string read() {
		string in;
		cin >> in;
		return in;
	}
    virtual void write(string text) {
		cout << text;
	}
    virtual void write(float f) {
		cout << f;
	}
    virtual void read(float* f) {
		//?
	}
    virtual ~StandardIO() {}

};


// you may edit this class
class Command{
	DefaultIO* dio;
public:
	Command(DefaultIO* dio):dio(dio){}
	virtual void execute()=0;
	virtual string desciption() = 0;
    virtual ~Command(){}
};

// implement here your command classes
class UploadCommand: public Command {
public:
    virtual void execute() {
		//todo
	}
	virtual string desciption(){
		return "upload a time series csv file";
    }
    virtual ~UploadCommand() {}
};



#endif /* COMMANDS_H_ */
