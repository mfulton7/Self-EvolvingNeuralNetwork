#include <iostream>
#include <chrono>
#include "neural_net.h"
#include "manager.h"



//
int main() {

	//preprocessing to determine machine number will need to occur for running on cluster



	//steps
	//1 generate initial neurons
	//2 run network
	//3 backprop
	//4 evolve into branches
	//5 run steps 2-3 on branches
	//6 evaulate branch results
	//7 merge successful branches into master
	//8 goto step 4

	std::cout << "Hello world" << std::endl;
	std::cout << "Using " << THREAD_COUNT << " threads." << std::endl;
	std::cout << "Hardware supports up to " << std::thread::hardware_concurrency() << std::endl;
	std::cout << "Please select data type for testing....." << std::endl;
	std::cout << "0 -> Simple Equation\n 1 -> Moderate Equation\n 2 -> Cosine\n 3 -> Stock Market" << std::endl;
	int dataID;
	std::cin >> dataID;

	Manager testManager(dataID);
	std::cout << "Populating test data.... " << std::endl;
	testManager.populateTestDataList(1000);
	std::cout << "Creating network ... " << std::endl;
	testManager.spawnStandardNetwork(5, 40);
	testManager.spawnEvolvedNetwork(3, 3);

	//sets up input labels for current data type
	testManager.initializeIO();
	
	std::cout << "Training..." << std::endl;
	testManager.runEpochs(10);
	testManager.networks[0]->logStats();
	//testManager.networks[0]->logTopography();
	testManager.networks[0]->terminateConnection();


	system("PAUSE");

}