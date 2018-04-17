#include "neural_components.h"

Block& Block::operator=(const Block &obj) 
{
	//outputFromBlock = obj.outputFromBlock;
	//totalOutput = obj.totalOutput;
	//netInput = obj.netInput;

	for (int i = 0; i < obj.population.size(); i++)
	{
		population.push_back(new Neuron(false));
		*population[i] = *obj.population[i];
	}

	/*for (int j = 0; j < obj.inConnectionCache.size(); j++)
	{
		inConnectionCache.push_back(new Connection());
		*inConnectionCache[j] = *obj.inConnectionCache[j];
	}

	for (int k = 0; k < obj.outConnectionCache.size(); k++)
	{
		outConnectionCache.push_back(new Connection());
		*outConnectionCache[k] = *obj.outConnectionCache[k];
	}*/

	return *this;
}


IO_Block& IO_Block::operator=(const IO_Block &obj) 
{
	outputFromBlock = obj.outputFromBlock;
	totalOutput = obj.totalOutput;
	netInput = obj.netInput;

	for (int i = 0; i < obj.population.size(); i++)
	{
		population.push_back(new Neuron(false));
		*population[i] = *obj.population[i];
	}

	/*for (int j = 0; j < obj.inConnectionCache.size(); j++)
	{
		inConnectionCache.push_back(new Connection());
		*inConnectionCache[j] = *obj.inConnectionCache[j];
	}

	for (int k = 0; k < obj.outConnectionCache.size(); k++)
	{
		outConnectionCache.push_back(new Connection());
		*outConnectionCache[k] = *obj.outConnectionCache[k];
	}*/

	return *this;
}

//todo move connection operator here and overload for IOblocks?
Connection& Connection::operator=(const Connection &obj) 
{
	ID = obj.ID;
	connectionError = obj.connectionError;
	strengthOfConnection = obj.strengthOfConnection;
	this->addOrigin(new Block());
	this->addDest(new Block());
	*originBlock = *obj.originBlock;
	*destinationBlock = *obj.destinationBlock;
	return *this;
}