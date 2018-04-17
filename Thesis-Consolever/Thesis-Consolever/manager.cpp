#include "manager.h"
#include "neural_net.h"

netBundle& netBundle::operator=(const netBundle &obj) 
{
	canMutate = obj.canMutate;
	statsHandler = new StatisticsHandler();
	*statsHandler = *obj.statsHandler;
	neuralNet = new Network();
	*neuralNet = *obj.neuralNet;
	//keep stats handler since only one version of the same network when logging stats
	return *this;

}