#ifndef neural_net
#define neural_net


#include <vector>
#include <ratio>
#include <ctime>
#include <time.h>
#include <chrono>

#include "data_handlers.h"

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
		//wipe outputs before recalculatin
		this->outputFromBlock.clear();
		//apply summation of inputs 
		//and trigger activation function
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

		// only quash output if this is not an output layer
		if (!isOutput) 
		{
			activateSigmoid(outputSum);
		}
		// if it is an output layer then do not quash down
		else 
		{
			totalOutput = outputSum;
		}

		//wipe
		this->inputFromConnections.clear();
	}
};



//need to create inherited connection subclass for input and output connections  
class Connection {
public:
	float distanceBetweenBlock;
	float strengthOfConnection;
	//3 dimensional vector representing direction to neuron
	std::vector<float> angleOfConnection;
	Block* originBlock;
	Block* destinationBlock;
	

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

// map that contains list and orientation of neurons(nodes) in network
class Network {

public:
	//id
	float branchID;

	//inputs to network
	Layer<IO_Block>* inputs;

	//outputs to network
	Layer<IO_Block>* outputs;

	//correct outputs to network
	vector<float> correctOutputs;

	//list of error of each output
	vector<float> indErrorList;
	//calculated total error of network
	//error of 1 + 2 ... n
	float totalError;

	//reference number for datapairs in manager
	int testRef = 0;
	//hidden layers of network	
	vector<Layer<Block>*> hiddenLayers;

	//list of references evolved branches from this branch
	vector<Network*> children;

	//sets up input layer to be proper size and tags each item with appropriate name
	void initializeInputs(vector<std::string> inputNames) 
	{
		//this->inputs = new Layer<IO_Block>;
		//create ioblocks for inputs
		for each (std::string name in inputNames)
		{
			this->inputs->addIOBlock(name, true);
			//create connections between input layer and first layer of hidden layer
			for each (Block* d in hiddenLayers.front()->blocks)
			{
				//connections.push_back(Connection(d, this->inputs->blocks.back()));
				this->inputs->addConnection(new Connection(d, this->inputs->blocks.back()));
			}
		}
	};

	//sets up output layer to be proper size and tags each item with appropriate nam
	void initializeOutputs(vector<std::string> outputNames) 
	{
		for each (std::string name in outputNames)
		{
			this->outputs->addIOBlock(name, false);
			for each (Block* o in hiddenLayers.back()->blocks)
			{
				//connections reside in the layer that they start in 
				//so no connections should exist in the actual output layer
				this->hiddenLayers.back()->addConnection(new Connection(this->outputs->blocks.back(), o));
			}
		}
	};

	//loads input array from a passed datapair
	//needs to be done with each pass as new data is loaded
	//must have same parameter as loadoutputs for each pass!!!!
	void loadInputs(DataPair<float,float> dataPairList) 
	{
		int counter = 0;
		for each (float i in dataPairList.input)
		{
			//try to update the inputs for a pass
			try
			{
				this->inputs->blocks[counter]->population.front()->nodePotential = i;
			}
			catch (const std::exception&)
			{
				//this->inputs->blocks.pu
			}
			//if exception then the input hasn't been setup yet.
			counter++;
		}
	};

	//loads output array from passed datapair
	//needs to be done before each pass
	void loadOutputs(DataPair<float, float> dataPairList)
	{
		int counter = 0;
		for each (float i in dataPairList.output)
		{
			//try to update the inputs for a pass
			try
			{
				this->correctOutputs.push_back(i);
			}
			catch (const std::exception&)
			{
				//this->inputs->blocks.pu
			}
			//if exception then the output hasn't been setup yet.

			counter++;
		}
	
	};

	//compares generated and correct outputs
	void compareResults() 
	{
		//clear error list before beginning
		indErrorList.clear();
		int totalResult = 0;
		//using squared error function
		// 1/2(target - output) ^ 2
		for(int i = 0; i< correctOutputs.size(); i++)
		{
			float result = (correctOutputs[i] - outputs[i].blocks.front()->totalOutput);
			result * result;
			result / 2;
			indErrorList.push_back(result);
			totalResult += result;
		}

		totalError = totalResult;
		
	};

	//ctors
	//////////////////////////////////////
	

	//ctor for creating a basic network with x blocks of y size
	//note this creates a chain configuration
	Network(int layerCount, int blocksPerLayer, int blockSize) {
		branchID = rand() % 100;
		inputs = new Layer<IO_Block>;
		outputs = new Layer<IO_Block>;
		for (int i = 0; i < layerCount; i++) {
			hiddenLayers.push_back(new Layer<Block>(blocksPerLayer,blockSize));
			if (i != 0 ) {
				//add connection for each block in previous layer to connect to every block in next layer
				for each (Block* d in hiddenLayers[i]->blocks)
				{
					for each (Block* o in hiddenLayers[i-1]->blocks)
					{
						//connections.push_back(Connection(d, o));
						//changed implementation so that connections are stored inside layers instead of the main connections list
						//note all connections in a layer originate in that layer
						hiddenLayers[i - 1]->addConnection(new Connection(d, o));
					}
				}
			}
		}
					
	};
	//////////////////////////////////////

	//all steps to go forward from input to output
	void completeForwardPass() 
	{
		
		//forward pass
		//need to loop through all connections
		for each (IO_Block* ib in this->inputs->blocks)
		{
			ib->sumInputsAndWipeInputVector(true, false);
		}
		// 1 - feed forward from input to hidden
		for each (Connection* inputCon in this->inputs->connections)
		{
			inputCon->feedForward();
		}
		////////////////////////////////////////////////////////////////////////////////
		// 2 - feedforward through all hidden layers
		for each(Layer<Block>* hl in this->hiddenLayers)
		{
			//for every block in the layer move the input data from previous layer to node potential
			for each(Block* hb in hl->blocks)
			{
				hb->sumInputsAndWipeInputVector(false,false);
			}

			//for each connection in layer feed forward data to input buffer in next layer
			for each(Connection* hiddenCon in hl->connections)
			{
				hiddenCon->feedForward();
			}
		}
		////////////////////////////////////////////////////////////////////////////////
		// 3 - feed forward from hidden layers to output
		for each(IO_Block* ob in this->outputs->blocks)
		{
			ob->sumInputsAndWipeInputVector(false,true);
		}
		/*for each(Connection* outputCon in this->outputs->connections)
		{
			outputCon->feedForward();
		}*/

		//return an array of outputs??
	
	};

	//all steps to backpropagate from error to whole network
	//call after forwards and compare result
	void completeBackwardPass() 
	{
		//semantics for backprop are slightly different for output layer and hidden layer

		//output back prop

		//hidden back prop
	};


};

//hierarchy
//the system holds multiple  networks some of which are different versions of one another
//Networks are a series of interconnected blocks
//blocks are comprised of sets of individual neurons
//blocks are connected to other blocks via connections



#endif // !neural_net



