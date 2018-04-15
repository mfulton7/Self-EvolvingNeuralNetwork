#ifndef evolution_matrix
#define evolution_matrix

#include "neural_net.h"
#include "neural_components.h"

//handles algorithms and parameters for neurevolution
class EvolutionMatrix {

public:



	//method to create new branch
	void Mutate(Network* n, int evolutionType) 
	{
		int mutationType;
		int numberOfAvailableMutations = 6;
		//determine how the mutation is selected
		switch(evolutionType)
		{
		default:
			break;
		//random
		case 0:
			mutationType = rand() & numberOfAvailableMutations;
			break;
		//weights
		case 1:
			//create a struct to hold weights for the effectiveness of each mutation type
			break;
		}

		//call the function for the mutation selected
		switch (mutationType)
		{
		default:
			break;

		case 0:
			injectBlock(n);
			break;

		case 1:
			injectConn(n);
			break;

		case 2:
			injectLay(n, 10);
			break;

		case 3:
			rmBlock(n);
			break;

		case 4:
			rmConn(n);
			break;

		case 5:
			rmLay(n);
			break;

		case 6:
			bisectConn(n);
			break;


		}
	
	
	};

	//MUTATION TYPES
	////////////////////////////////////////////////////
	//note splice parameter references wheather an injected object should be fully connected
	//to surrounding objects or only randomly connected

	

	//block injection
	//0 - 1
	void injectBlock(Network* n) 
	{
		//don't have to worry about dataflow integrity as much when adding
		int hiddenLayerChoice = rand() & n->hiddenLayers.size();

		
		//add it to hidden layer
		n->hiddenLayers[hiddenLayerChoice]->addBlock(1);
		
		int oNode, dNode = 0;

		//connect at least one backwards connection
		//if its the first hidden then the backwards must be to inputs
		if (hiddenLayerChoice == 0) 
		{
			oNode = rand() & n->inputs->blocks.size();
			n->inputs->addConnection(new Connection(n->hiddenLayers[hiddenLayerChoice]->blocks.back(), n->inputs->blocks[oNode]));
		}
		else 
		{
			oNode = rand() & n->hiddenLayers[hiddenLayerChoice - 1]->blocks.size();
			
			n->hiddenLayers[hiddenLayerChoice - 1]->addConnection(new Connection(n->hiddenLayers[hiddenLayerChoice]->blocks.back(), n->hiddenLayers[hiddenLayerChoice - 1]->blocks[oNode]));
		}

		//connect at least one forwards connection
		//if its the last hidden layer then the forwards must be an output
		if (hiddenLayerChoice == n->hiddenLayers.size()) 
		{
			dNode = rand() & n->outputs->blocks.size();
			n->hiddenLayers.back()->addConnection(new Connection(n->outputs->blocks[dNode], n->hiddenLayers.back()->blocks.back()));
		}
		else
		{
			dNode = rand() & n->hiddenLayers[hiddenLayerChoice + 1]->blocks.size();
			n->hiddenLayers[hiddenLayerChoice]->addConnection(new Connection(n->hiddenLayers[hiddenLayerChoice + 1]->blocks[dNode], n->hiddenLayers[hiddenLayerChoice]->blocks.back()));
		}
	};

	//connection injection
	//2
	void injectConn(Network* n) 
	{
		//todo find way to stop multiple connections between two nodes

		bool isOutputConn = false;
		bool isInputConn = false;
		
		Connection* injection = new Connection();
		
		//get two random numbers for hidden layers
		//note numbers cannot be the same
		//need some functionality to hit output layer
		int hiddenLayerChoice1 = (rand() & n->hiddenLayers.size()) - 1;
		int diff = n->hiddenLayers.size() - (hiddenLayerChoice1);
		int hiddenLayerChoice2 = (rand() & diff) + hiddenLayerChoice1;
		//get two random numbers for nodes in chosen hidden layers
		int nodeChoice1;
		int nodeChoice2;

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
			nodeChoice1 = rand() & n->hiddenLayers[hiddenLayerChoice1]->blocks.size() - 1;
			injection->addOrigin(n->hiddenLayers[hiddenLayerChoice1]->blocks[nodeChoice1]);
			//add pointer to connection to starting hidden layer
			n->hiddenLayers[hiddenLayerChoice1]->addConnection(injection);
		}

		// if the dest is in the output layer
		if (hiddenLayerChoice2 == n->hiddenLayers.size())
		{
			isOutputConn = true;
			nodeChoice2 = rand() & n->outputs->blocks.size() - 1;
			injection->addDest(n->outputs->blocks[nodeChoice2]);
		}
		else 
		{
			nodeChoice2 = rand() & n->hiddenLayers[hiddenLayerChoice2]->blocks.size();
			injection->addDest(n->hiddenLayers[hiddenLayerChoice2]->blocks[nodeChoice2]);
		}

		
			

		
	};

	//create a new block inside of a connection between two blocks
	//effectively creating a new layer
	void bisectConn(Network* n) {};

	//layer injection
	//creates a layer of x nodes then connects each node once forwards and once backwards
	//3
	void injectLay(Network* n, int layerSize) 
	{
		//be lazy here and just add to the end?
		Layer<Block> * newLayer = new Layer<Block>(layerSize, 1);
		n->hiddenLayers.push_back(newLayer);

		//now do connections backwards
		for each (Block* o in n->hiddenLayers[n->hiddenLayers.size() - 2]->blocks)
		{
			//pick random destination
			int dest = rand() & n->hiddenLayers.back()->blocks.size() -1;
			Block* d = n->hiddenLayers.back()->blocks[dest];
			n->hiddenLayers[n->hiddenLayers.size() - 2]->addConnection(new Connection(d, o));
		}

		//connections forwards
		for each (Block* o in n->hiddenLayers.back()->blocks)
		{
			//pick random dest in output
			int dest = rand() & n->outputs->blocks.size() -1;
			Block* d = n->outputs->blocks[dest];
			n->hiddenLayers.back()->addConnection(new Connection(d, o));
		}
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
			//todo this breaks xutility find out why and fixxx
			for (int i = 0; i < n->hiddenLayers.size(); i++) 
			{
				for (int j = 0; j < n->hiddenLayers[i]->connections.size(); j++) 
				{
					if (n->hiddenLayers[i]->connections[j] == c) 
					{
						//matched
						n->hiddenLayers[i]->connections.erase(n->hiddenLayers[i]->connections.begin() + j);
					}
				}				
			}

			for (int k = 0; k < n->inputs->connections.size(); k++)
			{
				if (n->inputs->connections[k] == c) 
				{
					n->inputs->connections.erase(n->inputs->connections.begin() + k);
				}
			}

			
		}
		for each (Connection* c in outBuffer)
		{
			for (int i = 0; i < n->hiddenLayers.size(); i++)
			{
				for (int j = 0; j < n->hiddenLayers[i]->connections.size(); j++)
				{
					if (n->hiddenLayers[i]->connections[j] == c)
					{
						//matched
						n->hiddenLayers[i]->connections.erase(n->hiddenLayers[i]->connections.begin() + j);
					}
				}
			}

			for (int k = 0; k < n->outputs->connections.size(); k++)
			{
				if (n->outputs->connections[k] == c)
				{
					n->outputs->connections.erase(n->outputs->connections.begin() + k);
				}
			}
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