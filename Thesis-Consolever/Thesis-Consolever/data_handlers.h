#ifndef data_handlers
#define data_handlers

#include <vector>

using std::vector;

template<typename inputFormat, typename outputFormat>
struct DataPair
{
public:
	vector<inputFormat> input;
	vector<outputFormat> output;
};


class TestDataHandler
{
public:
	//used to determine what kind of data is used
	//0 => use an equation
	//1 => use a csv file
	//2 => use a sql database
	int testMode;
	TestDataHandler(int mode)
	{
		testMode = mode;
	};

	auto ReturnData(int dataSetSize)
	{

		vector <DataPair<float, float>> dataresult;
		for (int i = 0; i < dataSetSize; i++) {
			dataresult.push_back(EquationDataGeneration());
		}
		return dataresult;

	};

	DataPair<float, float> EquationDataGeneration()
	{
		//equation 
		// y = x^2 +12
		float in = rand() % 1000;
		float out = (in*in) + 12;
		DataPair<float, float> newPair;
		newPair.input.push_back(in);
		newPair.output.push_back(out);
		return newPair;
	};


};
#endif // !data_handlers
