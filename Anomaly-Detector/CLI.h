

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {
	DefaultIO* dio;
	Menu menu = Menu(dio);
	// you can add data members
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
};

class Menu {
	Command** commands;
	DefaultIO* dio;
public:
	Menu(DefaultIO* dio) {
		this->dio = dio;
		UploadCommand *uploadCommand;
		commands[0] = uploadCommand;
	}
	void displayMenu() {
		this->dio->write("Welcome to the Anomaly Detection Server.");
		this->dio->write("Please choose an option:");
		for(int i = 0; i < 6; i ++){
			this->dio->write(commands[i]->desciption());
		}
	}
};

#endif /* CLI_H_ */
