// Bareket Damari
// Michael Saar


#ifndef CLI_H_
#define CLI_H_

#include <string.h>
#include "commands.h"

using namespace std;

class CLI {

	DefaultIO* dio;
	Command** commands;
	HybridAnomalyDetector* detector;
	bool* shouldStop; 
	// you can add data members

	void initCommands();
	void printMenu();

public:

	CLI(DefaultIO* dio);

	void start();

	virtual ~CLI();
};

#endif /* CLI_H_ */
