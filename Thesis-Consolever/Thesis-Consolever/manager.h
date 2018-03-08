#ifndef manager
#define manager




#include<time.h>
#include<stdlib.h>
#include<iostream>

#include "neural_net.h"
#include "data_handlers.h"




//governs execution and management of networks
class Manager {
public:
	std::vector<Network> neuralNets;
	std::vector<DataPair<float, float>> testDataSet;
	TestDataHandler dataHandler;
	Manager() {
		//setup random
		srand(time(NULL));
		dataHandler = TestDataHandler(0);
	};

	//does all steps required to create a traditional network using algorithms 
	Network* spawnStandardNetwork(int layerC, int layerS) 
	{
		//create
		Network* snet = new Network(layerC, layerS, 1);
		//setup input and output specs
		snet->initializeInputs(vector<std::string>{ "x" });
		snet->initializeOutputs(vector<std::string>{"y"});
		

		return snet;
	};

	//fils the test data array 
	void populateTestDataList(int size) 
	{
		testDataSet = dataHandler.ReturnData(size);
	};
	void runPass(Network* selectedNetwork) 
	{
		selectedNetwork->loadInputs(testDataSet[selectedNetwork->testRef]);
		selectedNetwork->loadOutputs(testDataSet[selectedNetwork->testRef]);
		selectedNetwork->completeForwardPass();
		selectedNetwork->compareResults();
		selectedNetwork->completeBackwardPass();
		
	};
	// run x number of passes
	void runPass(int passesToRun, Network* selectedNet)
	{
		float generationAverage = 0;
		for (int i = 0; i < passesToRun; i++) 
		{
			std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
			runPass(selectedNet);
			std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
			std::cout << "A single pass takes " << duration << std::endl;
			selectedNet->testRef++;
			generationAverage += abs(selectedNet->totalError);
			if (selectedNet->testRef % 100 == 0) {
				std::cout << "Average Error for generation " << selectedNet->testRef / 100 <<" is " << generationAverage / 100 << std::endl;
				generationAverage = 0;
			}
		}
	};
};


#endif // !manager