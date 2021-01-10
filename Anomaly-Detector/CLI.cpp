#include "CLI.h"

bool isParam(string line) {
    char* p;
    strtol(line.c_str(), &p, 10);
    return *p == 0;
}

CLI::CLI(DefaultIO* dio) {
    this->exit = 0;
    this->dio = dio;
    this->helper = new AnomalyDetectorHelper();
    //add menu commands
    this->addCommand(new UploadCSVCommand(dio));
    this->addCommand(new SettingCommand(dio, helper));
    this->addCommand(new DetectCommand(dio, helper));
    this->addCommand(new DisplayCommand(dio, helper));
    this->addCommand(new UploadResultCommand(dio, helper));
    this->addCommand(new ExitCommand(dio,exit));
}

void CLI::start(){
    while(!exit) {
        printMenu();
        string input = dio->read();
        //todo verifay input is an integer
        if(isParam(input)){
            int in = stoi(input);
            commands[in - 1]->execute();
        }
    }
}

void CLI::addCommand(Command* c) {
    this->commands[command_count] = c;
    command_count++;
}

void CLI::printMenu() {
    this->dio->writeLine("Welcome to the Anomaly Detection Server.");
    this->dio->writeLine("Please choose an option:");

    //print out the menu
    for (int i = 0; i < COMMAND_COUNT; i++) {
        this->dio->write(to_string(i + 1) + ".");
        this->dio->writeLine(commands[i]->desciption());
    }
}

CLI::~CLI() {
    for (int i = 0; i < COMMAND_COUNT; i++) {
        delete commands[i];
    }
}

