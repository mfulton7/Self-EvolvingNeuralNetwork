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
		Network* snet = new Network(10, 10, 10);
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
		//forward pass
		//need to loop through all connections

		// 1 - feed forward from input to hidden
		//wiping not required for input layer since it is the start
		for each (Connection* inputCon in selectedNetwork->inputs->connections)
		{
			inputCon->feedForward();
		}
		////////////////////////////////////////////////////////////////////////////////
		// 2 - feedforward through all hidden layers
		for each(Layer<Block>* hl in selectedNetwork->hiddenLayers) 
		{
			//for every block in the layer move the input data from previous layer to node potential
			for each(Block* hb in hl->blocks) 
			{
				hb->addInputsToNodeAndWipeInputVector();
			}

			//for each connection in layer feed forward data to input buffer in next layer
			for each(Connection* hiddenCon in hl->connections) 
			{
				hiddenCon->feedForward();
			}
		}
		////////////////////////////////////////////////////////////////////////////////
		// 3 - feed forward from hidden layers to output
		for each(IO_Block* ob in selectedNetwork->outputs->blocks) 
		{
			ob->addInputsToNodeAndWipeInputVector();
		}
		for each(Connection* outputCon in selectedNetwork->outputs->connections) 
		{
			outputCon->feedForward();
		}


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