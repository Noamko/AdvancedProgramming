#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    this->commands[0] = new UploadCSVCommand(dio);
    this->commands[1] = new SettingCommand(dio);
    this->commands[2] = new DetectCommand(dio);
    this->commands[3] = new DisplayCommand(dio);
    this->commands[4] = new UploadResultCommand(dio);
    this->commands[5] = new ExitCommand(dio);
}

void CLI::start(){
    this->dio->write("Welcome to the Anomaly Detection Server.\n");
    this->dio->write("Please choose an option:\n");
    for(int i = 0; i < COMMAND_COUNT; i++) {
        this->dio->write(to_string(i+1) + ".");
        this->dio->write(commands[i]->desciption());
        if(i + 1 != COMMAND_COUNT) this->dio->write("\n");
    }
}

CLI::~CLI() {
}

