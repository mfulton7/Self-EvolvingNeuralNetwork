#ifndef neural_components
#define neural_components

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
	Neuron(bool random) {
		if (random) {
			//create random weight between 0 and 1
			nodePotential = ((float)rand() / RAND_MAX);
		}
		else {
			nodePotential = 1;
		}
	};


};

//encapsulating all connections via blocks allows for fine neuron interactions and the ability to scale up easily
class Block {

public:
	//list of all neurons in a block
	vector<Neuron*> population;

	//list of recieved inputs from connections
	//sum of this list should be the net input of the block
	vector<float> inputFromConnections;

	//summation of all inputs
	//need for backprop calculations
	float netInput;

	//list of all neurons in block outputs
	//where output of a neuron is defined as
	//the summation of all inputs to the neuron where each input is
	//(output from connected blocks * str of connection)
	// that summation + the bias of the neuron (nodepotential)
	vector<float> outputFromBlock;

	//summed and quashed output of this block
	float totalOutput;


	//constructors
	////////////////////////////////////////////////

	//default ctor, block only has one with default
	Block() {
		population.push_back(new Neuron(true));

	};

	//ctor for a specific block size
	Block(int size) {
		for (int i = 0; i < size; i++) {
			population.push_back(new Neuron(true));
		}

	};
	/////////////////////////////////////////////////

	//activation function
	//to be run after weights are summed into nodepotential
	void activateSigmoid(float summedOutput)
	{

		//quash sum using "fast sigmoid"
		totalOutput = summedOutput / (1 + abs(summedOutput));

	};

	//this sums the net input to a block
	//should be called after all inputs to a block have been sent
	void sumInputsAndWipeInputVector(bool isInput, bool isOutput)
	{
		//sum inputs
		float sumOfInputs = 0.0f;
		//if this isn't in the first (input) layer then pull from input buffer
		if (!isInput) {
			for each (float input in this->inputFromConnections)
			{
				sumOfInputs += input;
			}
		}
		//if this is in the first layer then input buffer isn't used to pull directly from loaded inputs
		else
		{
			sumOfInputs = this->population.front()->nodePotential;

		}

		//save total net input
		this->netInput = sumOfInputs;

		//wipe outputs before recalculatin
		this->outputFromBlock.clear();
		//apply summation of inputs to node bias
		for each (Neuron* n in this->population)
		{
			this->outputFromBlock.push_back(n->nodePotential + sumOfInputs);
			//n->nodePotential += sumOfInputs;

		}

		float outputSum = 0;
		//sum output array
		for each (float o in this->outputFromBlock)
		{
			outputSum += o;
		}

		//// only quash output if this is not an output layer
		//if (!isOutput)
		//{
		//	activateSigmoid(outputSum);
		//}
		//// if it is an output layer then do not quash down
		//else
		//{
		//	totalOutput = outputSum;
		//}

		//i guess quash even for outputs
		activateSigmoid(outputSum);

		//wipe
		this->inputFromConnections.clear();
	}
};

class IO_Block : public Block
{
public:
	std::string label;
	bool isInput;

	//true means input block, false means output block
	IO_Block(std::string nameOfLabel, bool amInput)
	{
		isInput = amInput;
		label = nameOfLabel;
		//create a single population block to rep one input
		//population.push_back(new Neuron());
	};



};


//need to create inherited connection subclass for input and output connections  
class Connection {
public:
	float strengthOfConnection;
	Block* originBlock;
	Block* destinationBlock;

	// total error of network with respect to this connection
	float connectionError;

	////////////////////
	//will probably need cut?
	float distanceBetweenBlock;
	//3 dimensional vector representing direction to neuron
	std::vector<float> angleOfConnection;
	///////////////////


	Connection() {
		strengthOfConnection = 1;
	};
	Connection(Block* dest, Block* origin) {
		destinationBlock = dest;
		originBlock = origin;
		strengthOfConnection = 1;
	};
	Connection(float dist, std::vector<float> angle, Block* dest, Block* origin) {
		distanceBetweenBlock = dist;
		angleOfConnection = angle;
		strengthOfConnection = 1;
		destinationBlock = dest;
		originBlock = origin;
	};

	//function to send forward prop through connection
	//input * str of connection + same for all other connections on dest block
	//this must be called after totaloutput for a block is calculated
	bool feedForward()
	{
		destinationBlock->inputFromConnections.push_back(originBlock->totalOutput * this->strengthOfConnection);
		return true;
	};





};



//convert to template?
template <typename T>
class Layer
{
public:

	vector<T*> blocks;

	//list of connections ORIGINATING in this layer
	vector<Connection*> connections;

	Layer()
	{

	}

	Layer(int blockCount, int blockSize)
	{
		for (int i = 0; i < blockCount; i++)
		{
			blocks.push_back(new T(blockSize));
		}
	}

	///need to add ad block layer function.
	void addBlock(int blockSize)
	{
		blocks.push_back(new T(blockSize));
	}

	void addIOBlock(std::string n, bool inputFlag)
	{
		blocks.push_back(new T(n, inputFlag));
	};

	void addConnection(Connection* c) {
		connections.push_back(c);
	};

};


#endif // neural_components