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
	Network* spawnStandardNetwork() 
	{
		//create
		Network* snet = new Network(2, 2, 1);
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
		for (int i = 0; i < passesToRun; i++) 
		{
			float generationAverage = 0;
			runPass(selectedNet);
			selectedNet->testRef++;
			generationAverage += selectedNet->totalError;
			if (selectedNet->testRef % 100 == 0) {
				std::cout << "Average Error for generation " << selectedNet->testRef <<" is " << selectedNet->totalError << std::endl;
				generationAverage = 0;
			}
		}
	};
};


#endif // !manager