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
			
			//Assert::AreNotEqual(copy->neuralNet->hiddenLayers.front()->blocks.front()->ID, (float)51515231);

			//testManager.runPass(testNetworkA);
		}
	};
}