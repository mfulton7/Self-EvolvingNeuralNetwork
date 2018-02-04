#ifndef neural_net
#define neural_net


#include <vector>
#include <ratio>
#include <ctime>
#include <time.h>
#include <chrono>

using std::vector;

//indivual nodes inside of a block
class Neuron {
public:

	//value of neuron
	float nodePotential;

	//default ctor
	Neuron() {
		nodePotential = 1;
	};

	//activation function
	//to be run after weights are summed into nodepotential
	void activateSigmoid() 
	{
		//using "fast sigmoid"
		nodePotential = nodePotential / (1 + abs(nodePotential));
	
	};
};

//encapsulating all connections via blocks allows for fine neuron interactions and the ability to scale up easily
class Block {

public:
	//list of all neurons in a block
	vector<Neuron*> population;

	//list of recieved inputs from connections
	vector<float> inputFromConnections;


	//constructors
	////////////////////////////////////////////////

	//default ctor, block only has one with default
	Block() {
		population.push_back(new Neuron());

	};

	//ctor for a specific block size
	Block(int size) {
		for (int i = 0; i < size; i++) {
			population.push_back(new Neuron());
		}

	};
	/////////////////////////////////////////////////

	


};


class Connection {
public:
	float distanceBetweenBlock;
	float strengthOfConnection;
	//3 dimensional vector representing direction to neuron
	std::vector<float> angleOfConnection;
	Block* originBlock;
	Block* destinationBlock;

	Connection(Block* dest, Block* origin) {
		destinationBlock = dest;
		originBlock = origin;
	};
	Connection(float dist, std::vector<float> angle, Block* dest, Block* origin) {
		distanceBetweenBlock = dist;
		angleOfConnection = angle;
		strengthOfConnection = 0;
		destinationBlock = dest;
		originBlock = origin;
	};

	//function to send forward prop through connection
	//input * str of connection + same for all other connections on dest block
	//after doing this the neurons in the destination block must sum up their inputs, squash them, and then freee input vector
	bool feedForward() 
	{
	

		if (int(originBlock->population.size()) == 0 || int(destinationBlock->population.size()) == 0) 
		{
			return false;
		}
		else 
		{
			float blockTotal = 0;
			for each (Neuron* n in originBlock->population)
			{
				blockTotal += n->nodePotential;
			}
			float totalOutput = blockTotal * strengthOfConnection;
			destinationBlock->inputFromConnections.push_back(totalOutput);
			return true;
		}
	};

	//called after feedforward 
	//maybe combine with feedforward function
	void addInputsToNodeAndWipeInputVector() 
	{
		//sum inputs
		float sumOfInputs = 0.0f;
		for each (float input in destinationBlock->inputFromConnections)
		{
			sumOfInputs += input;
		}
		//apply summation of inputs to every neuron in block
		//and trigger activation function
		for each (Neuron* n in originBlock->population)
		{
			n->nodePotential += sumOfInputs;
			n->activateSigmoid();
		}
		//wipe
		destinationBlock->inputFromConnections.clear();
	}

};

class Layer 
{
public:

	vector<Block*> blocks;

	Layer(int blockCount, int blockSize) 
	{
		for (int i = 0; i < blockCount; i++) 
		{
			blocks.push_back(new Block(blockSize));
		}
	}

	///need to add ad block layer function.
	void addBlock(int blockSize) 
	{
		blocks.push_back(new Block(blockSize));
	}

};

// map that contains list and orientation of neurons(nodes) in network
class Network {

public:
	//id
	float branchID;

	//this is created after network generation and is used to allow backpropagation
	//need to add layer funcitonality here?
	vector<Layer*> layers;

	//list of references evolved branches from this branch
	vector<Network*> children;

	//list of connections between blocks
	vector<Connection> connections;

	//ctors
	//////////////////////////////////////
	

	//ctor for creating a basic network with x blocks of y size
	//note this creates a chain configuration
	Network(int blockCount, int blockSize) {
		branchID = rand() % 100;
		for (int i = 0; i < blockCount; i++) {
			layers.push_back(new Layer(blockCount,blockSize));
			if (i > 0) {
				//add connection in between created blocks
				connections.push_back(Connection(layers[i]->blocks.front(), layers[i-1]->blocks.front()));
			}
		}
	};
	//////////////////////////////////////

	//function to sum all connections for each neuron in a layer




};

//hierarchy
//the system holds multiple  networks some of which are different versions of one another
//Networks are a series of interconnected blocks
//blocks are comprised of sets of individual neurons
//blocks are connected to other blocks via connections

#endif // !neural_net



