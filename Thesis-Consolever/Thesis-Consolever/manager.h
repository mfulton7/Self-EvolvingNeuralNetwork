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
	Network spawnStandardNetwork() 
	{
		//create
		Network snet(10, 10, 1);
		//setup input and output specs
		snet.initializeInputs(vector<std::string>{ "x" });
		snet.initializeOutputs(vector<std::string>{"y"});
		populateTestDataList(10);
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
		//forward pass
		//need to loop through all connections
		//feed forward from input to hidden
		for each (Connection* inputCon in selectedNetwork->inputs->connections)
		{

		}
		//feedforward through all hidden layers
		for each(Layer<Block>* hl in selectedNetwork->hiddenLayers) 
		{
			//for each layer feed all connections in layer
			for each(Connection* hiddenCon in hl->connections) 
			{
				//stuff and things
			}
		}
		//feed forward from hidden layers to output
		for each(Connection* outputCon in selectedNetwork->outputs->connections) 
		{
		
		}
		//compare result to output
		//back propagate error
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