#ifndef evolution_matrix
#define evolution_matrix

#include "neural_net.h"
#include "neural_components.h"

//handles algorithms and parameters for neurevolution
class EvolutionMatrix {

	//method to create new branch
	void Mutate(Network* n, int mutationType) 
	{
		switch (mutationType)
		{
		default:
			break;

		case 0:
			injectBlock(n, false);
			break;

		case 1:
			injectBlock(n, true);
			break;

		case 2:
			injectConn(n);
			break;

		case 3:
			injectLay(n, 10, true);
			break;

		case 4:
			break;

		case 5:
			break;

		case 6:
			break;


		}
	
	
	};

	//MUTATION TYPES
	////////////////////////////////////////////////////
	//note splice parameter references wheather an injected object should be fully connected
	//to surrounding objects or only randomly connected


	//block injection
	//0 - 1
	void injectBlock(Network* n, bool splice) 
	{
		//don't have to worry about dataflow integrity as much when adding
		int hiddenLayerChoice = rand() & n->hiddenLayers.size();

		//create new block

		//add it to hidden layer

		//connect at least one backwards connection

		//connect at least one forwards connection
	};

	//connection injection
	//2
	void injectConn(Network* n) 
	{
		bool isOutputConn = false;
		bool isInputConn = false;
		
		Connection* injection = new Connection();
		
		//get two random numbers for hidden layers
		//note numbers cannot be the same
		//need some functionality to hit output layer
		int hiddenLayerChoice1 = (rand() & n->hiddenLayers.size()) - 1;
		int hiddenLayerChoice2 = (rand() & n->hiddenLayers.size() + 1) + hiddenLayerChoice1;
		//get two random numbers for nodes in chosen hidden layers
		int nodeChoice1 = rand() & n->hiddenLayers[hiddenLayerChoice1]->blocks.size();
		int nodeChoice2 = rand() & n->hiddenLayers[hiddenLayerChoice2]->blocks.size();

		// if the origin is in the input layer
		if(hiddenLayerChoice1 == -1)
		{
			isInputConn = true;
			nodeChoice1 = rand() & n->inputs->blocks.size();
			injection->addOrigin(n->inputs->blocks[nodeChoice1]);

			//add pointer to connection to inputs
			n->inputs->addConnection(injection);
				
		}
		else 
		{
			injection->addOrigin(n->hiddenLayers[hiddenLayerChoice1]->blocks[nodeChoice1]);
			//add pointer to connection to starting hidden layer
			n->hiddenLayers[hiddenLayerChoice1]->addConnection(injection);
		}

		// if the dest is in the output layer
		if (hiddenLayerChoice2 == n->hiddenLayers.size() + 1)
		{
			isOutputConn = true;
			nodeChoice2 = rand() & n->outputs->blocks.size();
			injection->addDest(n->outputs->blocks[nodeChoice2]);
		}
		else 
		{
			injection->addDest(n->hiddenLayers[hiddenLayerChoice2]->blocks[nodeChoice2]);
		}

		
		
		//create connection
		//add it to origin layer
		//update caches of blocks
			

	

		

		
	};

	//create a new block inside of a connection between two blocks
	//effectively creating a new layer
	void bisectConn(Network* n) {};

	//layer injection
	//3
	void injectLay(Network* n, int layerSize, bool splice) 
	{
		//be lazy here and just add to the end?
	};

	//block deletion
	//3
	void rmBlock(Network* n) 
	{
		//select random element
		int layerNum = rand() % (n->hiddenLayers.size());
		int blockNum = rand() & n->hiddenLayers[layerNum]->blocks.size();

		//buffer for connections that need to be cleaned at the end of the removal
		vector<Connection*> inBuffer;
		vector<Connection*> outBuffer;

		//tidy up connections attached to element
		//for every output connection must create a new connection linking the output to the inputs
		for each (Connection* inC in n->hiddenLayers[layerNum]->blocks[blockNum]->inConnectionCache)
		{
			for each (Connection* outC in n->hiddenLayers[layerNum]->blocks[blockNum]->outConnectionCache)
			{
				//given an input connection A
				//and output connections B & C
				// create connection AB
				// create connection AC
				Connection* linkedCon = new Connection(inC->originBlock, outC->destinationBlock);
				//then push connection to layer
				//since the connection list in each layer contains only connections originating in said layer we can use layernum for this
				n->hiddenLayers[layerNum]->connections.push_back(linkedCon);

				//add output to buffer
				outBuffer.push_back(outC);
			}
			//add input to buffer
			inBuffer.push_back(inC);
			
	
		}

		//delete original inputs and outputs
		//i.e. delete A B C
		//leaving AB AC
		for each (Connection* c in inBuffer)
		{
			//find the pointer in the hidden layers
			std::find(n->hiddenLayers.begin(), n->hiddenLayers.end(), c);
		}
		for each (Connection* c in outBuffer)
		{

		}


		//actually delete the block now that the connections have been spliced
		n->hiddenLayers[layerNum]->blocks.erase(n->hiddenLayers[layerNum]->blocks.begin() + blockNum);
	};

	//connection deletion
	//4
	void rmConn(Network* n) 
	{
		//similar to add with pickiing a connection

		//need to find a way to make sure data integrity is kept though
	
	};

	//layer deletion
	//5
	void rmLay(Network* n) 
	{
		//if hidden layers size is greater then one

		//random vs full spilcing?
	};


	////////////////////////////////////////////////////

	//



	//todo
	//need a function splice which smoothes over holes in networks
	//i.e. nodes x y z
	// x -> y -> z
	// if we remove y splice needs to smooth the connection
	// x -> z

};

#endif //evolution_matrix