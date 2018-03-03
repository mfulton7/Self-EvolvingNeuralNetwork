#ifndef manager
#define manager




#include<time.h>
#include<stdlib.h>

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
		Network* snet = new Network(1, 1, 1);
		//setup input and output specs
		snet->initializeInputs(vector<std::string>{ "x" });
		snet->initializeOutputs(vector<std::string>{"y"});
		populateTestDataList(10);

		return snet;
	};

	//fils the test data array 
	void populateTestDataList(int size) 
	{
		testDataSet = dataHandler.ReturnData(size);
	};
	void runPass(Network* selectedNetwork) 
	{
		//load input and output
		selectedNetwork->loadInputs(testDataSet[selectedNetwork->testRef]);
		selectedNetwork->loadOutputs(testDataSet[selectedNetwork->testRef]);
		selectedNetwork->completeForwardPass();
		selectedNetwork->compareResults();
		//

		// 4 - compare result to output
		// 5 - back propagate error
	};
	// run x number of passes
	void runPass(int passesToRun, Network* selectedNetwok)
	{
		for (int i = 0; i < passesToRun; i++) 
		{
			runPass(selectedNetwok);
		}
	};
};

//handles algorithms and parameters for neurevolution
class EvolutionMatrix {

	//method to create new branch

	//method to compare effectiveness of two branches

	//method to fuze branches?

};

#endif // !manager