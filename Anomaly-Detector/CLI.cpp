#include "CLI.h"

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
}

/**
 * @brief simple CLI menu
 * 
 */
void CLI::start(){
    this->menu.displayMenu();
}

CLI::~CLI() {
}

