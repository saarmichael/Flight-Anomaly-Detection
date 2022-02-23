// Bareket Damari
// Michael Saar

#include "CLI.h"

size_t numberOfCommands = 6;

CLI::CLI(DefaultIO* dio) {
    this->dio = dio;
    this->detector = new HybridAnomalyDetector();
    bool s = false;
    this->shouldStop = &s;
}

void CLI::initCommands() {
    this->commands = new Command* [numberOfCommands];
    commands[0] = new UploadCommand(this->dio, "anomalyTrain.csv", "anomalyTest.csv");
    commands[1] = new SetThresholdCommand(this->dio, this->detector);
    commands[2] = new DetectCommand(this->dio, this->detector, "anomalyTrain.csv");
    commands[3] = new DisplayAnomaliesCommand(this->dio, this->detector, "anomalyTest.csv");
    commands[4] = new AnalyzeCommand(this->dio, this->detector, "anomalyTrain.csv", "anomalyTest.csv");
    commands[5] = new ExitCommand(this->shouldStop);

}

void CLI::printMenu() {

    this->dio->writeLine("Welcome to the Anomaly Detection Server.\n"
                         "Please choose an option:");
    for (size_t i = 0; i < numberOfCommands; i++) {
        this->dio->write(i + 1);
        this->dio->writeLine("." + this->commands[i]->getDescription());
    }

}

void CLI::start() {
    initCommands();
    do {
    printMenu();
    std::string s = dio->read();
    int option = stoi(s);
    // cout << "menu option: " <<option << endl; //FOR DEBUG
    commands[option - 1]->execute();

    } while(!*(this->shouldStop));
}


CLI::~CLI() {
    //  for(size_t i = 0; i < numberOfCommands; i++) {
    //      delete commands[i];
    //  }
    //delete [] commands;
    //delete dio;
    // delete detector;
}

