#ifndef neural_net
#define neural_net

#include <iostream>
#include <fstream>

class eventLogger
{
public:
	std::ofstream out;

	eventLogger() 
	{
		out.open("mutationlog.txt");
		out << "Mutation Logs:" << std::endl;
		
	}

	void logMutation()
	{
	
	}
};

#endif