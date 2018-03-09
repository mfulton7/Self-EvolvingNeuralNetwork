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
					}
				}
			}
			//no layer match found for this iteration
			else
			{

			}
		}
		//if matched layer is stil false then no matches exist
		if (!matchedLayer)
		{
			//add layer to fragment result
		}
	}

	return result;
}

Network operator + (Network_Fragment const &left, Network const &right)
{
	Network result(1, 1, 1);

	return result;
}