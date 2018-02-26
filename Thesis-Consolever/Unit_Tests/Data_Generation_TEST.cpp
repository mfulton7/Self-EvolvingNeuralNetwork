#include "stdafx.h"
#include <iostream>
#include "CppUnitTest.h"
#include "../Thesis-Consolever/data_handlers.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Unit_Tests 
{
	TEST_CLASS(Equation_Data_Generator_TESTS)
	{
	
		TEST_METHOD(Check_Proper_Return_Size_For_Equation_Data_Pairs) 
		{
			TestDataHandler handler(0);
			auto testDataPairs = handler.ReturnData(10);
			Assert::AreEqual(int(testDataPairs.size()), 10);

		}
	
		TEST_METHOD(Check_Data_Pair_Correctness_For_Equation)
		{
			TestDataHandler handler(0);
			auto testDataPairs = handler.ReturnData(1);
			float in = testDataPairs.front().input.front();
			float out = testDataPairs.front().output.front();
			Assert::AreEqual(out, ((in * in) + 12));

		}
	
	
	};
}