#include <iostream>
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
	std::cout << std::thread::hardware_concurrency();
	Manager testManager;
	testManager.populateTestDataList(10000);
	Network* testNetworkA = testManager.spawnStandardNetwork(5, 40);

	testManager.runPass(10000, testNetworkA);


	system("PAUSE");

}