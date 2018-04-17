#include "neural_net.h"
#include "neural_components.h"


//definitions of operators
Network_Fragment operator - (Network const &left, Network const &right)
{
	Network_Fragment result;
	//need to compute  the difference of left and right networks
	//go layer by layer
	for each (Layer<Block>* ll in left.hiddenLayers)
	{
		bool matchedLayer = false;
		//search for matching layer in right
		for each (Layer<Block>* rl in right.hiddenLayers)
		{
			if (ll->ID == rl->ID)
			{
				matchedLayer = true;
				//found layer match
				//in matched layer check all blocks
				for each (Block* lb in ll->blocks)
				{
					bool matchedBlock = false;
					for each (Block* rb in rl->blocks)
					{
						if (lb->ID == rb->ID) { matchedBlock = true; }
					}
					//if matched block is still false then it doesn't exist in right
					if (!matchedBlock)
					{
						// must add block to network fragment result
						result.addBlock(lb, ll);
					}
				}
				//in matched layer check all connections
				for each (Connection* lc in ll->connections)
				{
					bool matchedConnection = false;
					for each (Connection* rc in rl->connections)
					{
						if (lc->ID == rc->ID) { matchedConnection = true; }
					}
					//if matched conn is still false then it does not exist in right
					if (!matchedConnection)
					{
						// must add connection to network fragment result
						result.addConnection(lc, ll);
					}
				}
			}
		}
		//if matched layer is stil false then no matches exist
		if (!matchedLayer)
		{
			//add layer to fragment result
			//todo
			//need way to get left layer?
			//result.addLayer(ll, )
		}
	}

	return result;
}

Network& Network::operator=(const Network &obj) 
{
	inputs = new Layer<IO_Block>;
	*inputs = *obj.inputs;
	outputs = new Layer<IO_Block>;
	*outputs = *obj.outputs;
	testRef = obj.testRef;
	for (int i = 0; i < obj.hiddenLayers.size(); i++)
	{
		hiddenLayers.push_back(new Layer<Block>);
		*hiddenLayers[i] = *obj.hiddenLayers[i];
	}
	return *this;
}
//todo
//combine a network fragment with an existing network
//Network operator + (Network_Fragment const &left, Network const &right)
//{
//
//	Network result;
//
//
//	return result;
//}