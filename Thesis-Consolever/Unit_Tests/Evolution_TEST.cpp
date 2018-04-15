#include "stdafx.h"
#include <iostream>
#include "CppUnitTest.h"
#include "../Thesis-Consolever/evolution_matrix.h"
#include "../Thesis-Consolever/manager.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Unit_Tests 
{
	
	TEST_CLASS(Evolution_TESTS)
	{
		
		Manager SetupTestEnvironment() 
		{
			Manager testManager(1);
			testManager.populateTestDataList(100);
			testManager.spawnEvolvedNetwork(3, 3);
			testManager.initializeIO();

			return testManager;
		}
		TEST_METHOD(Check_Block_Injection)
		{
			Manager m = SetupTestEnvironment();
			m.networks.front()->evoHandler->injectBlock(m.networks.front()->neuralNet);

			//update size
			m.networks.front()->calculateSize();
			Assert::AreEqual(10, m.networks.front()->nodeCount);
		}

		TEST_METHOD(Check_Connection_Injection) 
		{
			Manager m = SetupTestEnvironment();
			m.networks.front()->evoHandler->injectConn(m.networks.front()->neuralNet);

			//update size
			m.networks.front()->calculateSize();
			//28 bc its moderate equation
			//6 + 10 + 9 + 3
			Assert::AreEqual(28, m.networks.front()->connCount);
		}

		TEST_METHOD(Check_Layer_Injection) 
		{
			Manager m = SetupTestEnvironment();
			//todo variablize the ten param
			m.networks.front()->evoHandler->injectLay(m.networks.front()->neuralNet, 10);

			//update size
			m.networks.front()->calculateSize();
			Assert::AreEqual(4, (int)m.networks.front()->neuralNet->hiddenLayers.size());
		}

		TEST_METHOD(Check_Block_Removal)
		{
			Manager m = SetupTestEnvironment();
			
			m.networks.front()->evoHandler->rmBlock(m.networks.front()->neuralNet);

			//update size
			m.networks.front()->calculateSize();
			Assert::AreEqual(8, m.networks.front()->nodeCount);
			Assert::AreNotEqual(27, m.networks.front()->connCount);
		}
		
		TEST_METHOD(Check_Connection_Removal)
		{
			Manager m = SetupTestEnvironment();
			//todo variablize the ten param
			m.networks.front()->evoHandler->rmConn(m.networks.front()->neuralNet);

			//update size
			m.networks.front()->calculateSize();
			Assert::AreEqual(26, m.networks.front()->connCount);
		}

	};
}