#ifndef manager
#define manager




#include<time.h>
#include<stdlib.h>
#include<iostream>
#include<fstream>
#include<numeric>

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
	vector<int> mutationHistory;

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
	
	netBundle() 
	{
		canMutate = false;
	}

	netBundle(Network* n, StatisticsHandler* s, std::string archiveName, bool mutationFlag) 
	{
		neuralNet = n;
		statsHandler = s;
		out.open(archiveName + ".txt");
		canMutate = mutationFlag;
		out << "Archive File:" << std::endl;
		out << "ID # " << this->neuralNet->branchID << std::endl;
		
	}

	//copy constructor
	netBundle& netBundle::operator=(const netBundle &obj);

	

	
	
	

	void logStats() 
	{
		if(!out.is_open()){
			out.open("evolved.txt");
		}
		//todo set dataset stuff for logging
		out << "Muatable : " << this->canMutate << std::endl;
		out << "Dataset : " <<  std::endl;
		out << "Epoch Size : " << statsHandler->epoch_size << std::endl;
		for (int k = 0; k < statsHandler->average_error.size(); k++) 
		{
			out << "Epoch " << k << " Error : " << statsHandler->average_error[k] << std::endl;
			out << "Run Time " << k << " (ms) : " << statsHandler->run_time[k].count() << std::endl;
			out << "---------------------------" << std::endl;

		}

		out << "Final Percent Error : " << statsHandler->finalPercentAccuracy << "%" <<std::endl;
		
		
		
		calculateSize();
		out << "Total Connection Count : " << connCount << std::endl;
		out << "Total Node Count : " << nodeCount << std::endl;
		out << "Mutation History: " << std::endl;
		for (int i = 0; i < this->mutationHistory.size(); i++) 
		{
			switch (mutationHistory[i])
			{
			default:
				break;
			case 0:
				out << "Block injected" << std::endl;
				break;
			case 1:
				out << "Connection Injected" << std::endl;
				break;
			case 2:
				out << "Layer Injected" << std::endl;
				break;
			case 3:
				out << "Block removed" << std::endl;
				break;
			case 4:
				out << "Connection removed" << std::endl;
				break;
			case 5:
				out << "Layer removed" << std::endl;
				break;

			}
		}
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

		std::string archiveName = "test";
		archiveName += std::to_string(layerC);
		archiveName += "x";
		archiveName += std::to_string(layerS);
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
		selectedNetwork->compareResults(false);
		selectedNetwork->completeBackwardPass();		
	};

	void runTestPass(Network* selectedNetwork) 
	{
		selectedNetwork->loadInputs(testDataSet[selectedNetwork->testRef]);
		selectedNetwork->loadOutputs(testDataSet[selectedNetwork->testRef]);
		selectedNetwork->completeForwardPass();
		//selectedNetwork->compareTestResults();
		selectedNetwork->compareResults(true);
	};

	void runRatioPass(Network* selectedNetwork) 
	{
		selectedNetwork->loadInputs(testDataSet[selectedNetwork->testRef]);
		selectedNetwork->loadOutputs(testDataSet[selectedNetwork->testRef]);
		selectedNetwork->completeForwardPass();
		selectedNetwork->compareRatioResults();
	};
	
	//return average error and total time to run pass
	void runEpoch()
	{
		vector<thread*> threads;
		for each (netBundle* selectedNet in networks)
		{
			thread* worker = new thread(&Manager::runEpochForNetwork, this, selectedNet);
			threads.push_back(worker);
		}
		for each (thread* t in threads)
		{
			t->join();
			t->~thread();
		}

	};

	void runEpochForNetwork(netBundle* selectedNet) 
	{
		//check if enough test data exists
		if (testDataSet.size() < selectedNet->neuralNet->testRef)
		{
			//todo
			//log this somehow
			//generate moar data?
			//probably replace this with try catch
			std::cout << "Error test data size limit reached...." << std::endl;
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
		generationAverage = generationAverage * 100;
		std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
		auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

		//save results into stats
		selectedNet->statsHandler->run_time.push_back(duration);
		selectedNet->statsHandler->average_error.push_back(generationAverage);
	};

	void runEpochs(int numberofPasses)
	{
		for (int i = 0; i < numberofPasses; i++) 
		{
			//get rid of all but most effective network
			//note does not wipe non mutable networks
			
			runEpoch();
			if (i != 0)
			{
				pruneNetworkVector();
			}
			std::cout << "Epoch " << i << " has been completed." << std::endl;
			
			//for each (netBundle* n in networks)
			if (i != (numberofPasses - 1)) 
			{
				vector<netBundle*> tmp;
				for (int i = 0; i < networks.size(); i++)
				{

					if (networks[i]->canMutate)
					{
						//call mutation stuff
						//make 6 copies
						for (int j = 0; j < 6; j++)
						{
							netBundle* cn = new netBundle();
							*cn = *networks[i];
							cn->evoHandler->Mutate(cn->neuralNet, j);
							cn->mutationHistory.push_back(j);
							tmp.push_back(cn);
						}
						//then mutate one of the copies

						//n->evoHandler->Mutate(n->neuralNet, 0);
						//function that does all mutations and then compares them?
					}
				}
				this->networks.insert(this->networks.end(), tmp.begin(), tmp.end());
			}
			
			
			
		}
	
	};


	void setOutputRatios(int batchSize) 
	{
		//set output Ratio for each network 
		for each (netBundle* selectedNet in networks)
		{
			
			vector<float> ratios;
			//for batchsize number of passes
			for (int j = 0; j < batchSize; j++) {
				float totalCorrect = 0;
				float totalGuess = 0;
				runRatioPass(selectedNet->neuralNet);
				selectedNet->neuralNet->testRef++;
				//for all outputs
				for (int i = 0; i < selectedNet->neuralNet->correctOutputs.size(); i++)
				{
					//totalGuess += selectedNet->neuralNet->outputs[i].blocks.front()->unQuashedOutput;
					////
					//totalCorrect += selectedNet->neuralNet->unCrushedCorrectOutputs[i];
					totalGuess += selectedNet->neuralNet->outputs[i].blocks.front()->totalOutput;
					//
					totalCorrect += selectedNet->neuralNet->correctOutputs[i];

				}
				float ratio = totalCorrect / totalGuess;
				ratios.push_back(ratio);
			}
			float average = std::accumulate(ratios.begin(), ratios.end(), 0.0f) / ratios.size();
			selectedNet->neuralNet->outputRatio = average;
		}
	
	};

	
	void testAccuracy(int batchSize) 
	{
		//get rid of all versions before testing
		pruneNetworkVector();

		//setup output ratio
		setOutputRatios(100);
		for each (netBundle* selectedNet in networks) 
		{
			

			
			//save it in stats handler
			//get stats handler to print it to file

			//check if enough test data exists
			if (testDataSet.size() < selectedNet->neuralNet->testRef)
			{
				//todo
				//log this somehow
				//generate moar data?
				//probably replace this with try catch
				std::cout << "Error test data size limit reached...." << std::endl;
			}

			int passesToRun = batchSize;
			float generationAverage = 0;
			vector<float> averages;
			std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
			for (int i = 0; i < passesToRun; i++)
			{
				runTestPass(selectedNet->neuralNet);
				selectedNet->neuralNet->testRef++;
				//generationAverage += abs(selectedNet->neuralNet->percentAccuracy);
				averages.push_back(abs(selectedNet->neuralNet->totalError));
			}
			for (int j = 0; j < averages.size(); j++) 
			{
				generationAverage = generationAverage + averages[j];
			}
			generationAverage = generationAverage / passesToRun;
			generationAverage = generationAverage * 100;
			std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

			//save results into stats
			selectedNet->statsHandler->run_time.push_back(duration);
			selectedNet->statsHandler->finalPercentAccuracy = generationAverage;
		}
	}

	//function to compare the networks inside the manager and delete all but the best
	//does not prune non mutable networks since those are assumed to be comparison networks
	void pruneNetworkVector() 
	{
		if (this->networks.size() == 1) { return; }
		//initializion only
		netBundle* currentLeader = this->networks[0];
		std::vector<netBundle*> comparisonCache;
		bool foundEvo = false;
		for each (netBundle* bundle in this->networks)
		{
			if(!bundle->canMutate)
			{
				//add to cache
				//continue since comparioson not needed
				comparisonCache.push_back(bundle);
				continue;
			}
			else { foundEvo = true; }
			float evaluation = 0.0f;
			//todo implement size weights that benefit smaller networks
			//evaluation = (bundle->statsHandler->average_error.back * accuracyWeight);
			
			if (currentLeader->statsHandler->average_error.back() > bundle->statsHandler->average_error.back()) 
			{
				currentLeader = bundle;
				foundEvo = true;
			}

			

		}
		//clear all elements of network
		//todo fix this memory leak :(
		//its an array of pointers to networks soo....
		networks.clear();
		
		//put comparison cache back in
		networks.insert(networks.end(), comparisonCache.begin(), comparisonCache.end());
		//add current maximum to the list
		if (foundEvo) { networks.push_back(currentLeader); }

	}
};


#endif // !manager