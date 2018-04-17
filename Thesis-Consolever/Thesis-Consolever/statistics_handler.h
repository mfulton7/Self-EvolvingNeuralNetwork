#ifndef statistics_handler
#define statistics_handler

#include<fstream>


//used for gathering data on running networks and determining which networks are more efficient
class StatisticsHandler 
{
public:
	//epoch size is the number of passes run before comparison and mutation occurs
	int epoch_size;
	vector<std::chrono::milliseconds> run_time;
	vector<float> average_error;

	float finalPercentAccuracy;
			
	StatisticsHandler() { epoch_size = 100; }

	StatisticsHandler(int epochSize) 
	{
		epoch_size = epochSize;
	}

	
};
#endif // !statistics_handler

