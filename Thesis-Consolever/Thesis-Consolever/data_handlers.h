#ifndef data_handlers
#define data_handlers

#include <vector>
#include <fstream>
#include <iostream>
#include <string>

using std::vector;


template<typename inputFormat, typename outputFormat>
struct DataPair
{
public:
	vector<inputFormat> input;
	//correct output
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

	TestDataHandler() {};
	TestDataHandler(int mode)
	{
		testMode = mode;
	};

	auto ReturnData(int dataSetSize)
	{

		vector <DataPair<float, float>> dataresult;

		//test mode for simple equation
		if (testMode == 0) {

			for (int i = 0; i < dataSetSize; i++) {
				dataresult.push_back(SimpleEquationDataGeneration());
			}
			
		}

		//test mode for moderate equation
		if (testMode == 1) {

			for (int i = 0; i < dataSetSize; i++) {
				dataresult.push_back(ModerateEquationDataGeneration());
			}

		}
		//test mode for cosine
		//todo
		if (testMode == 2) 
		{
			for (int i = 0; i < dataSetSize; i++) {
				dataresult.push_back(CosEquationDataGeneration());
			}
		
		}

		//test mode for stock data
		if (testMode == 3) 
		{
			std::ifstream in;
			std::cout << "Pleae enter the stock symbol ..." << std::endl;
			std::string stockSymbol;
			std::cin >> stockSymbol;
			in.open("Stocks/" + stockSymbol + "_data.csv");
			std::string date;
			std::string openPrice;
			std::string highPrice;
			std::string lowPrice;
			std::string closePrice;
			std::string volume;
			std::string name;
			std::string headers;

			//get rid of headers in csv file
			//todo make headers the dynamic string names
			//todo send this info to the names
			//todo add date as a param???
			std::getline(in, headers, '\n');

			//1260 IS THE MAX FOR STOCK DATA
			for (int i = 0; i < 1260; i++) {
				getline(in, date, ',');
				std::getline(in, openPrice, ',');
				std::getline(in, highPrice, ',');
				std::getline(in, lowPrice, ',');
				std::getline(in, closePrice, ',');
				std::getline(in, volume, ',');
				std::getline(in, name, '\n');
				DataPair<float, float> newPair;
				
				newPair.input.push_back(std::stof(openPrice));
				newPair.input.push_back(std::stof(highPrice));
				newPair.input.push_back(std::stof(lowPrice));
				
				newPair.input.push_back(std::stof(volume));

				//todo find result in the future?
				newPair.output.push_back(std::stof(closePrice));
				dataresult.push_back(newPair);
			}


		}
		return dataresult;
	};

	DataPair<float, float> SimpleEquationDataGeneration()
	{
		//equation 
		// y = x^2 +12
		float in = rand() % 2000;
		in - 1000;

		
		float out = .2 * in + 3;
		DataPair<float, float> newPair;
		newPair.input.push_back(in);
		newPair.output.push_back(out);
		return newPair;
	};

	DataPair<float, float> ModerateEquationDataGeneration() 
	{
		//equation
		// y = x^3 + z^2 + 5
		float inX = rand() % 2000;
		inX - 1000;
		float inZ = rand() & 2000;
		inZ - 1000;

		float out = (inX * inX * inX) + (inZ * inZ) + 5;

		DataPair<float, float> newPair;
		newPair.input.push_back(inX);
		newPair.input.push_back(inZ);
		newPair.output.push_back(out);
		return newPair;
	
	}

	DataPair<float, float> CosEquationDataGeneration() 
	{
		//equation 
		// y = cos(x)
		float inX = rand() % 2000;
		inX - 1000;

		float out = cos(inX);
		DataPair<float, float> newPair;
		newPair.input.push_back(inX);
		newPair.output.push_back(out);
		return newPair;
	
	}


};
#endif // !data_handlers
