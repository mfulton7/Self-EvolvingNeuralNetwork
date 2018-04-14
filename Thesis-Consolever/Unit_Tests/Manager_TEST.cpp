#include "stdafx.h"
#include <iostream>
#include "CppUnitTest.h"
#include "../Thesis-Consolever/manager.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Unit_Tests
{
	TEST_CLASS(Manager_TESTS) 
	{
		TEST_METHOD(Check_Run_Pass) 
		{
			Manager testManager(1);
			//Network* testNetworkA = testManager.spawnStandardNetwork();

			//testManager.runPass(testNetworkA);
		}
	};
}