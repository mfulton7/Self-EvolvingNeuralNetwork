#ifndef neural_net
#define neural_net


#include <vector>
#include <ratio>
#include <ctime>
#include <time.h>
#include <chrono>
#include <typeinfo>

#include "data_handlers.h"
#include "neural_components.h"

using std::vector;


// map that contains list and orientation of neurons(nodes) in network
class Network {

public:
	//id
	float branchID;

	//rate at which changes to network via backprop are multiplied
	float learningRate = 1;

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

	//return the summation of connection errors for all connections that the dest of this connections is an origin for
	float summationOfErrorCalculation(Connection* c) 
	{
		//get reference to the block that needs searched
		auto targetBlock = c->destinationBlock;

		float summation = 0;
		//now parse layers of network finding connections where target block is an origin
		//this definitely can be made more efficient
		//big TODO
		//only need to check hidden layers since inputs will never have this case and outputs are backpropagated differently
		for each (Layer<Block>* l in this->hiddenLayers)
		{
			for each (Connection* lc in l->connections)
			{
				//check if == works???
				if(lc->originBlock == targetBlock)
				{
					summation += lc->connectionError;
				}
			}
		}

		return summation;
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

		//
		for each (Layer<Block>* l in hiddenLayers)
		{


			for each (Connection* c in l->connections)
			{
				float errorC = 0;
				//if statement to catch all connections that lead to outputs
				//may want to refactor this check to something more efficient?
				if (typeid(c->destinationBlock).name() == "IO_Block") {
					//output back prop uses delta rule
					//error of this connection is equivalent to
					//Error(oc) = - (correctOutput - dest.totatloutput) * dest.totaloutput(1 - dest.totaloutput) * origin.totaloutput
					//where dest is in the outputs and origin is a member of hidden
					//TODO
					//warning will need to find way to find correct position in correct outputs
					//calculate error with respect to c
					errorC = -(this->correctOutputs.front() - c->destinationBlock->totalOutput) * (c->destinationBlock->totalOutput * (1 - c->destinationBlock->totalOutput)) * c->originBlock->totalOutput;
					
					
				
				}
				//if destination is not an ioblock then we are dealing with hidden to hidden connection
				else
				{
					//more complicated hidden layer stuff
					//error of hidden to hidden connection is equivalent to
					//Error(hc) = (summation of connected errors) * (dest.totaloutput * (1 - dest.totaloutput)) * dest.netinput
					//where the summation of connected errors are the calculated errors of all connections that have the destination block
					//of this connection as an origin block for another connection
					//and netinput refers to the prequashed value of all inputs to the destination block
					//TODO
					// 1 need a way to save net input total before quashing
					// done
					// 2 need a way to save error values for weights b/c they will be need in further passes
					// done
					// 3 need a method to get a summation of connected errors
					// done definitely needs tested....
					//float errorC = c->destinationBlock->totalOutput * (1 - c->destinationBlock->totalOutput) * c->destinationBlock->
					//get summation
					float summationOfErrors = this->summationOfErrorCalculation(c);
					float destinationOutput = c->destinationBlock->totalOutput;
					float destinationNet = c->destinationBlock->netInput;
					errorC = summationOfErrors * (destinationOutput * (1 - destinationOutput)) * destinationNet;
				}
				//save error value to connection for use later in backprop calc
				c->connectionError = errorC;
				//apply calculation
				c->strengthOfConnection = c->strengthOfConnection - (this->learningRate * errorC);
				
			}
		}
	};


};

//hierarchy
//the system holds multiple  networks some of which are different versions of one another
//Networks are a series of layers
//three kinds of layers inputs, hidden, and outputs
//layers are comprised of blocks
//blocks are connected to blocks in other layers via connections
//blocks are comprised of sets of individual neurons




#endif // !neural_net



