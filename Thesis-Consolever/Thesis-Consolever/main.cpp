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
	std::cout << "Using " << THREAD_COUNT << " threads per network." << std::endl;
	std::cout << "Hardware supports up to " << std::thread::hardware_concurrency() << std::endl;
	std::cout << "Please select data type for testing....." << std::endl;
	std::cout << "0 -> Simple Equation\n 1 -> Moderate Equation\n 2 -> Cosine\n 3 -> Stock Market" << std::endl;
	int dataID;
	std::cin >> dataID;

	Manager testManager(dataID);
	std::cout << "Populating test data.... " << std::endl;
	testManager.populateTestDataList(5000);
	std::cout << "Creating network ... " << std::endl;
	
	//testManager.spawnStandardNetwork(1, 50);
	testManager.spawnStandardNetwork(1, 50);
	testManager.spawnStandardNetwork(1, 10);
	//testManager.spawnStandardNetwork(2, 50);
	testManager.spawnStandardNetwork(2, 25);
	testManager.spawnStandardNetwork(2, 10);
	//testManager.spawnStandardNetwork(3, 50);
	testManager.spawnStandardNetwork(3, 25);
	testManager.spawnStandardNetwork(3, 10);
	//testManager.spawnStandardNetwork(4, 50);
	testManager.spawnStandardNetwork(4, 25);
	testManager.spawnStandardNetwork(4, 10);
	//testManager.spawnStandardNetwork(5, 50);
	testManager.spawnStandardNetwork(5, 25);
	testManager.spawnStandardNetwork(5, 10);
	//testManager.spawnStandardNetwork(6, 50);
	//testManager.spawnStandardNetwork(6, 25);
	//testManager.spawnStandardNetwork(6, 10);
	
	
	

	

	//sets up input labels for current data type
	testManager.initializeIO();
	std::cout << "How many epochs would you like to run?" << std::endl;
	int numberOfEpochs;
	std::cin >> numberOfEpochs;

	std::cout << "Training..." << std::endl;
	testManager.runEpochs(numberOfEpochs);
	testManager.testAccuracy(50);

	for each (netBundle* b in testManager.networks)
	{
		b->logStats();
		b->terminateConnection();
	}
	


	system("PAUSE");

}