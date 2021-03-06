#ifndef neural_components
#define neural_components

#include <vector>
#include <ratio>
#include <ctime>
#include <time.h>
#include <chrono>
#include <boost/serialization/vector.hpp>

using std::vector;

//indivual nodes inside of a block
class Neuron {
public:
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) 
	{
		ar & nodePotential;
	}

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

//forward declaration of class connection for the sake of block cache
class Connection;

//encapsulating all connections via blocks allows for fine neuron interactions and the ability to scale up easily
class Block {

public:

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) 
	{
		ar & population;
		ar & inConnectionCache;
		ar & outConnectionCache;
	}

	float ID;
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
	//uncrushed output
	float unQuashedOutput;


	//list of references to all connections that this block is a particpant in
	vector<Connection*> inConnectionCache;
	vector<Connection*> outConnectionCache;

	//constructors
	////////////////////////////////////////////////

	//default ctor, block only has one with default
	Block() {
		population.push_back(new Neuron(true));

	};

	Block& operator=(const Block &obj);

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
		unQuashedOutput = summedOutput;
		//quash sum using "fast sigmoid"
		totalOutput = summedOutput / (1 + abs(summedOutput));

	};

	//alternate activaiton function
	void activateReLU(float summedOutput) 
	{
		if (summedOutput < 0) { summedOutput = 0; }
		totalOutput = summedOutput;
	};

	//this sums the net input to a block
	//should be called after all inputs to a block have been sent
	void sumInputsAndWipeInputVector(bool isInput, bool isOutput)
	{
		//sum inputs
		float sumOfInputs = 0.0f;
		//if this isn't in the first (input) layer then pull from input buffer
		if (!isInput) {
			/*for each (float input in this->inputFromConnections)
			{
				sumOfInputs += input;
			}*/
			for (int i = 0; i < this->inputFromConnections.size(); i++) 
			{
				sumOfInputs += this->inputFromConnections[i];
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
		//if(!isOutput){ activateSigmoid(outputSum); }
		//else { totalOutput = outputSum; }
		activateSigmoid(outputSum);
		//activateReLU(outputSum);

		//wipe
		this->inputFromConnections.clear();
	}
};

class IO_Block : public Block
{
public:
	std::string label;
	bool isInput;

	IO_Block() {}

	//true means input block, false means output block
	IO_Block(std::string nameOfLabel, bool amInput)
	{
		isInput = amInput;
		label = nameOfLabel;
		//create a single population block to rep one input
		//population.push_back(new Neuron());
	};

	IO_Block& operator=(const IO_Block &obj);

};


//need to create inherited connection subclass for input and output connections  
class Connection {
public:
	
	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) 
	{
		ar & strengthOfConnection;
		ar & originBlock;
		ar & destinationBlock;
	}

	float ID;

	float strengthOfConnection;
	Block* originBlock;
	Block* destinationBlock;

	// total error of network with respect to this connection
	float connectionError;


	Connection() {
		//create random weight between 0 and 1
		strengthOfConnection = ((float)rand() / RAND_MAX);
	};

	Connection& operator = (const Connection &obj);

	//destination is first then origin
	Connection(Block* dest, Block* origin) {
		destinationBlock = dest;
		originBlock = origin;
		//create random weight between 0 and 1
		strengthOfConnection = ((float)rand() / RAND_MAX);
		//update cache in blocks
		dest->inConnectionCache.push_back(this);
		origin->outConnectionCache.push_back(this);
	};
	Connection(Block* dest, Block* origin, float str) 
	{
		destinationBlock = dest;
		originBlock = origin;
		//create random weight between 0 and 1
		strengthOfConnection = str;

		//update cache in blocks
		dest->inConnectionCache.push_back(this);
		origin->outConnectionCache.push_back(this);
	};
	
	void addDest(Block* dest) 
	{
		destinationBlock = dest;
		dest->inConnectionCache.push_back(this);
	};

	void addOrigin(Block* origin)
	{
		originBlock = origin;
		origin->outConnectionCache.push_back(this);
	}

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

	template<class Archive>
	void serialize(Archive & ar, const unsigned int version) 
	{
		ar & blocks;
		ar & connections;
	}

	float ID;

	vector<T*> blocks;

	//list of connections ORIGINATING in this layer
	vector<Connection*> connections;

	Layer()
	{

	}

	Layer& operator = (const Layer &obj) 
	{
		for (int i = 0; i < obj.blocks.size(); i++) 
		{
			blocks.push_back(new T());
			*blocks[i] = *obj.blocks[i];
		}

		for (int j = 0; j < obj.connections.size(); j++) 
		{
			connections.push_back(new Connection());
			*connections[j] = *obj.connections[j];
		}

		return *this;
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

//construct to associate neural components with their positions
template <typename T>
class Component 
{
public:

	Component(T item) { item = item; }
	Component(T item, float parent) { item = item; parentID = parent; }

	//need ref to component
	T item;
	// layer reference id
	// if T is a layer then this would be the layer immediately above T
	float parentID;
	//references for the blocks if this is a connection, and connection is an edge case
	float originID;
	float destID;
	//and wheather component links to existing network or is entirely in fragment
	//basically is this component an ege case that requires splicing into existing network 
	bool isEdge;
};

//subclass of network that holds data required to difference networks?
class Network_Fragment 
{
public:
	//need a list of all differences between two networks
	//and data to accompany the differences specifying where they fit
	vector<Component<Block*>*> blocks;
	vector<Component<Connection*>*> connections;
	//NOTE LAYERS ARE HANDLED DIFFERENTLY WHERE ONLY THE EXISTANCE IS CONTAINED IN THE FRAGMENT
	//SO THIS TELLS YOU THAT THERE IS A LAYER BUT NOT WHATS IN IT
	//TO POPULATE THE CONTENTS OF A LAYER WE USE THE BLOCKS AND CONNECTIONS VECTORS
	vector<Component<Layer<Block>*>*> layers;

	//todo investigate
	//will this break if layer to left is input layer??
	void addLayer(Layer<Block>* l, Layer<Block>* layerToLeft) 
	{
		//create new layer
		Layer<Block>* copyLayer = new Layer<Block>();
		
		//assign actual value
		//SHOULDN'T BE NEED DUE TO NOTE?
		//*copyLayer = *l;
		
		//update references in layer contents
		//should this be copylayer or l?
		for each (Block* b in copyLayer->blocks)
		{
			addBlock(b, copyLayer);
		}
		for each (Connection* c in copyLayer->connections)
		{
			addConnection(c, copyLayer);
		}

		layers.push_back(new Component<Layer<Block>*>(copyLayer, layerToLeft->ID));
	};
	void addBlock(Block* b, Layer<Block>* parentL) 
	{
		//create new pointer to new block
		Block* copyBlock = new Block();
		//assign the new block the value of the parameter
		*copyBlock = *b;
		//create component that combines copyBLock with location info
		blocks.push_back(new Component<Block*>(copyBlock, parentL->ID));
	};
	void addConnection(Connection* c, Layer<Block>* parentL) 
	{
		//need to update block references in connecion
		Block* replacementOriginBlock = new Block(c->originBlock->population.size());
		Block* replacementDestBlock = new Block(c->destinationBlock->population.size());

		*replacementOriginBlock = *c->originBlock;
		*replacementDestBlock = *c->destinationBlock;

		//create new conneciton
		Connection* copyConnection = new Connection(replacementDestBlock, replacementOriginBlock, c->strengthOfConnection);
		
		//create component that combines connection with location info
		connections.push_back(new Component<Connection*>(copyConnection, parentL->ID));
	};
};


#endif // neural_components