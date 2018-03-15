#ifndef evolution_matrix
#define evolution_matrix

#include "neural_net.h"
#include "neural_components.h"

//handles algorithms and parameters for neurevolution
class EvolutionMatrix {

	//method to create new branch
	void Mutate(Network* n, int mutationType) 
	{
		switch (mutationType)
		{
		default:
			break;

		case 0:
			break;

		case 1:
			break;

		case 2:
			break;

		case 3:
			break;

		case 4:
			break;

		case 5:
			break;

		case 6:
			break;


		}
	
	
	};

	//MUTATION TYPES
	////////////////////////////////////////////////////

	//block injection
	//0
	void injectBlock(Network* n, bool splice) {};

	//connection injection
	//1
	void injectConn(Network* n) {};

	//layer injection
	//2
	void injectLay(Network* n, int layerSize, bool splice) {};

	//block deletion
	//3
	void rmBlock(Network* n) {};

	//connection deletion
	//4
	void rmConn(Network* n) {};

	//layer deletion
	//5
	void rmLay(Network* n) {};

	//block adjustment
	//6
	void adjBlock(Network* n) {};

	//layer adjustment
	//7
	void adjLay(Network* n) {};
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