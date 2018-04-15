#ifndef manager
#define manager




#include<time.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>

#include "neural_net.h"
#include "data_handlers.h"
#include "statistics_handler.h"
#include "evolution_matrix.h"
#include<boost\archive\text_oarchive.hpp>

//struct to pair networks with the statshandler with the network
struct netBundle 
{
	Network* neuralNet;
	StatisticsHandler* statsHandler;
	EvolutionMatrix* evoHandler;
	bool canMutate = false;

	int nodeCount = 0;
	int connCount = 0;

	void calculateSize()
	{
		for (int i = 0; i < neuralNet->hiddenLayers.size(); i++)
		{
			nodeCount += neuralNet->hiddenLayers[i]->blocks.size();
			connCount += neuralNet->hiddenLayers[i]->connections.size();
		}

		//grab input connections
		connCount += neuralNet->inputs->connections.size();
	
	}
	

	std::ofstream out;
	
	netBundle(Network* n, StatisticsHandler* s, std::string archiveName, bool mutationFlag) 
	{
		neuralNet = n;
		statsHandler = s;
		out.open(archiveName + ".txt");
		canMutate = mutationFlag;
		out << "Archive File:" << std::endl;
		out << "ID # " << this->neuralNet->branchID << std::endl;
		
	}

	
	
	

	void logStats() 
	{
		//todo set dataset stuff for logging
		out << "Dataset : " <<  std::endl;
		out << "Epoch Size : " << statsHandler->epoch_size << std::endl;
		for (int k = 0; k < statsHandler->average_error.size(); k++) 
		{
			out << "Epoch " << k << " Error : " << statsHandler->average_error[k] << std::endl;
			out << "Run Time " << k << " (ms) : " << statsHandler->run_time[k].count() << std::endl;
			out << "---------------------------" << std::endl;

		}
		
		
		
		calculateSize();
		out << "Total Connection Count : " << connCount << std::endl;
		out << "Total Node Count : " << nodeCount << std::endl;
	}

	//todo fix this
	void logTopography() 
	{
		boost::archive::text_oarchive a(out);
		out << neuralNet;
	}

	//called after logging is done
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

	//weights for pruning criteria
	//how much increases in accuracy are valued
	float accuracyWeight = 1.0f;
	//how much the size of the network is valued
	float sizeWeight = 1.0f;
	

	Manager(int dataType) {
		//setup random
		srand(time(NULL));
		//set data type for experiments here
		dataHandler = TestDataHandler(dataType);
	};

	void initializeIO() 
	{
		vector<std::string> inputList;
		vector<std::string> outputList;

		//simple equation and cosine are both x,y
		if (this->dataHandler.testMode == 0 || this->dataHandler.testMode == 2)
		{
			inputList = { "X" };
			outputList = { "Y" };
		}

		if (this->dataHandler.testMode == 1) 
		{
			inputList = { "X", "Z" };
			outputList = { "Y" };
		}

		if (this->dataHandler.testMode == 3) 
		{
			inputList = { "open", "high", "low", "volume" };
			outputList = { "close" };
		}

		for each (netBundle* b in this->networks)
		{
			b->neuralNet->initializeInputs(inputList);
			b->neuralNet->initializeOutputs(outputList);
		}
	}

	//does all steps required to create a traditional network using algorithms 
	void spawnStandardNetwork(int layerC, int layerS) 
	{
		//create
		Network* snet = new Network(layerC, layerS, 1);
		//setup input and output specs
		//todo make this dynamic and move it somewhere else probably to datahandlers or manager
		//snet->initializeInputs(vector<std::string>{ "open", "high", "low", "volume" });
		//snet->initializeOutputs(vector<std::string>{"close"});

		//setup stats
		//each new network needs a stat tracker
		StatisticsHandler* stats = new StatisticsHandler(EPOCH_SIZE);

		std::string archiveName = "test" + layerC + 'x' + layerS;
		netBundle* result = new netBundle(snet, stats, archiveName, false);
		

		this->networks.push_back(result);
	};

	//does all steps to create a enn
	void spawnEvolvedNetwork(int layerC, int layerS)
	{
		//create
		Network* snet = new Network(layerC, layerS, 1);

		//setup inputs???

		
		//setup stats
		//each new network needs a stat tracker
		StatisticsHandler* stats = new StatisticsHandler(EPOCH_SIZE);

		netBundle* result = new netBundle(snet, stats, "evolved", true);


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
	void runEpoch()
	{
		for each (netBundle* selectedNet in networks)
		{

			//check if enough test data exists
			if (testDataSet.size() < selectedNet->neuralNet->testRef)
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
		}
	};

	void runEpochs(int numberofPasses)
	{
		for (int i = 0; i < numberofPasses; i++) 
		{
			runEpoch();
			std::cout << "Epoch " << i << " has been completed." << std::endl;
			
			for each (netBundle* n in networks)
			{
				if (n->canMutate) 
				{
					//call mutation stuff
					n->evoHandler->Mutate(n->neuralNet, 0);
				}
			}

			//get rid of all but most effective network
			//note do not wipe non mutable networks
			pruneNetworkVector();
			
		}
	
	};

	//function to compare the networks inside the manager and delete all but the best
	//does not prune non mutable networks since those are assumed to be comparison networks
	void pruneNetworkVector() 
	{
		//initializion only
		netBundle* currentLeader = this->networks[0];
		std::vector<netBundle*> comparisonCache;
		for each (netBundle* bundle in this->networks)
		{
			if(!bundle->canMutate)
			{
				//add to cache
				//continue since comparioson not needed
				comparisonCache.push_back(bundle);
				continue;
			}
			float evaluation = 0.0f;
			//todo implement size weights that benefit smaller networks
			//evaluation = (bundle->statsHandler->average_error.back * accuracyWeight);
			
			if (currentLeader->statsHandler->average_error > bundle->statsHandler->average_error) 
			{
				currentLeader = bundle;
			}

		}
		//clear all elements of network
		//todo fix this memory leak :(
		//its an array of pointers to networks soo....
		networks.clear();
		
		//put comparison cache back in
		networks.insert(networks.end(), comparisonCache.begin(), comparisonCache.end());
		//add current maximum to the list
		networks.push_back(currentLeader);

	}
};


#endif // !manager