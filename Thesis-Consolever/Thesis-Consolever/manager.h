#ifndef manager
#define manager




#include<time.h>
#include<stdlib.h>
#include<iostream>

#include "neural_net.h"
#include "data_handlers.h"
#include "statistics_handler.h"

//struct to pair networks with the statshandler with the network
struct netBundle 
{
	Network* neuralNet;
	StatisticsHandler* statsHandler;
};

//governs execution and management of networks
class Manager {
public:
	std::vector<netBundle*> networks;
	std::vector<DataPair<float, float>> testDataSet;
	TestDataHandler dataHandler;
	

	Manager() {
		//setup random
		srand(time(NULL));
		dataHandler = TestDataHandler(0);
	};

	//does all steps required to create a traditional network using algorithms 
	void spawnStandardNetwork(int layerC, int layerS) 
	{
		//create
		Network* snet = new Network(layerC, layerS, 1);
		//setup input and output specs
		snet->initializeInputs(vector<std::string>{ "x" });
		snet->initializeOutputs(vector<std::string>{"y"});

		//setup stats
		//each new network needs a stat tracker
		StatisticsHandler* stats = new StatisticsHandler(10);

		netBundle* result = new netBundle();
		result->neuralNet = snet;
		result->statsHandler = stats;

		this->networks.push_back(result);
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
	
	//return average error and total time to run pass
	void runEpoch(netBundle* selectedNet)
	{
		//check if enough test data exists
		if(testDataSet.size() < selectedNet->neuralNet->testRef)
		{
			//todo
			//log this somehow
			//generate moar data?
			//probably replace this with try catch
		}
		int passesToRun = selectedNet->statsHandler->epoch_size;
		float generationAverage = 0;
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < passesToRun; i++) 
		{
			runPass(selectedNet->neuralNet);			
			selectedNet->neuralNet->testRef++;
			generationAverage += abs(selectedNet->neuralNet->totalError);
		}
		generationAverage = generationAverage / passesToRun;
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
		
		//save results into stats
		selectedNet->statsHandler->run_time.push_back(duration);
		selectedNet->statsHandler->average_error.push_back(generationAverage);
	};

	void runEpochs(netBundle* selectedNet, int numberofPasses)
	{
		for (int i = 0; i < numberofPasses; i++) 
		{
			runEpoch(selectedNet);
		//mutate stuff??
		}
	
	};
};


#endif // !manager