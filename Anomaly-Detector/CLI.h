

#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;


class CLI {
	int exit;
	int command_count = 0;
	DefaultIO* dio;
	Command* commands[6];
	AnomalyDetectorHelper* helper;
public:
	CLI(DefaultIO* dio);
	void start();
	virtual ~CLI();
protected:
	void addCommand(Command* c);
	void printMenu();
};

#endif /* CLI_H_ */
