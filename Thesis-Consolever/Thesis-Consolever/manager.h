#ifndef manager
#define manager




#include<time.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>

#include "neural_net.h"
#include "data_handlers.h"
#include "statistics_handler.h"
#include<boost\archive\text_oarchive.hpp>

//struct to pair networks with the statshandler with the network
struct netBundle 
{
	Network* neuralNet;
	StatisticsHandler* statsHandler;
	std::ofstream out;
	
	netBundle(Network* n, StatisticsHandler* s, std::string archiveName) 
	{
		neuralNet = n;
		statsHandler = s;
		out.open(archiveName + ".txt");
		out << "Archive File:" << std::endl;
		out << "ID # " << this->neuralNet->branchID << std::endl;
		
	}

	void logStats() 
	{
		//todo set dataset stuff for logging
		out << "Dataset : " << std::endl;
		out << "Epoch Size : " << statsHandler->epoch_size << std::endl;
		for (int k = 0; k < statsHandler->average_error.size(); k++) 
		{
			out << "Epoch " << k << " Error : " << statsHandler->average_error[k] << std::endl;
			out << "Run Time " << k << " (ms) : " << statsHandler->run_time[k].count() << std::endl;
			out << "---------------------------" << std::endl;

		}
		
		
		
		int nodeCount = 0;
		int connCount = 0;
		for (int i = 0; i < neuralNet->hiddenLayers.size(); i++) 
		{
			nodeCount += neuralNet->hiddenLayers[i]->blocks.size();
			connCount += neuralNet->hiddenLayers[i]->connections.size();
		}
		out << "Total Connection Count : " << connCount << std::endl;
		out << "Total Node Count : " << nodeCount << std::endl;
	}

	void logTopography() 
	{
		boost::archive::text_oarchive a(out);
		out << neuralNet;
	}

	void terminateConnection() 
	{
		out.close();
	}
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
		//set data type for experiments here
		dataHandler = TestDataHandler(1);
	};

	//does all steps required to create a traditional network using algorithms 
	void spawnStandardNetwork(int layerC, int layerS) 
	{
		//create
		Network* snet = new Network(layerC, layerS, 1);
		//setup input and output specs
		//todo make this dynamic and move it somewhere else probably to datahandlers or manager
		snet->initializeInputs(vector<std::string>{ "open", "high", "low", "volume" });
		snet->initializeOutputs(vector<std::string>{"close"});

		//setup stats
		//each new network needs a stat tracker
		StatisticsHandler* stats = new StatisticsHandler(10);

		netBundle* result = new netBundle(snet, stats, "test");
		

		this->networks.push_back(result);
	};

	//fils the test data array 
	//todo add option for end of file reading
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
			std::cout << "Epoch " << i << " has been completed." << std::endl;
		//mutate stuff??
		}
	
	};
};


#endif // !manager