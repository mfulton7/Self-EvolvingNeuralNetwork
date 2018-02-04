
#include "stdafx.h"
#include <iostream>
#include "CppUnitTest.h"
#include "../Thesis-Consolever/neural_net.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Unit_Tests
{		
	TEST_CLASS(Neural_Net_Default_Construction_TEST)
	{
	public:

		//help function to stop repetion of code in unit tests
		//TODO
		//finish
		void SetUp() 
		{
			std::cout << "Test?";
		}
		
		TEST_METHOD(Default_Creation_Of_Neuron)
		{
			Neuron testNeuron = Neuron();
			Assert::AreEqual(testNeuron.nodePotential, float(1));
		}

		TEST_METHOD(Defualt_Creation_Of_Block) 
		{
			Block testBlock = Block();
			Assert::AreEqual(int(testBlock.population.size()), 1);
		}

		TEST_METHOD(Creation_Of_Block_With_Specific_Size) 
		{
			Block testBlock = Block(100);
			Assert::AreEqual(int(testBlock.population.size()), 100);
		}

		TEST_METHOD(Creation_Of_Block_With_Negative_Size)
		{
			Block testBlock = Block(-15);
			Assert::AreEqual(int(testBlock.population.size()), 0);
		}

		TEST_METHOD(Creation_Of_Block_With_Null_Size)
		{
			Block testBlock = Block(NULL);
			Assert::AreEqual(int(testBlock.population.size()), 0);
		}

		TEST_METHOD(Creation_Of_Network)
		{
			int blockSize = 10;
			int blockCount = 10;
			Network testNetwork = Network(blockCount, blockSize);
			Assert::AreEqual(int(testNetwork.blocks.size()), blockCount);
			for each (Block b in testNetwork.blocks)
			{
				Assert::AreEqual(int(b.population.size()), blockSize);
			}

		}

		TEST_METHOD(Check_Sigmoid_Activation_Function) 
		{	
			Block testBlock = Block();
			testBlock.population[0]->nodePotential = 84;
			testBlock.population[0]->activateSigmoid();
			Assert::AreEqual(testBlock.population[0]->nodePotential, float(0.988235295));
		}

		TEST_METHOD(Check_Default_Creation) 
		{
			int blockSize = 10;
			int blockCount = 10;
			Network testNetwork = Network(blockCount, blockSize);
			Assert::AreEqual(int(testNetwork.connections.size()), blockCount - 1);
		}

		//
		//TODO
		//Write Tests for creationof connections
		//

	};

	TEST_CLASS(Neural_Net_Basic_Feed_Forward_TEST) 
	{
		Network setupTestNetwork() 
		{
			Network testNetwork = Network(2, 1);
			//set node potentials
			testNetwork.blocks[0].population[0]->nodePotential = 5;
			testNetwork.blocks[1].population[0]->nodePotential = 3;
			//set starting weights
			testNetwork.connections[0].strengthOfConnection = 0.5;
			return testNetwork;
		}

		TEST_METHOD(Check_Feed_Forward) 
		{
			Network testNetwork = setupTestNetwork();
			bool suceeded = testNetwork.connections[0].feedForward();
			//complete feed forward will be
			//input * weight + bias
			//should be (.5 * 5) + 3
			//for purpose of this test we are checking the passing of the weight and input to the dest block
			//so .5 * 5
		
			Assert::IsTrue(suceeded);
			Assert::AreEqual(int(testNetwork.blocks[1].inputFromConnections.size()), 1);
			Assert::AreEqual(testNetwork.blocks[1].inputFromConnections[0], float(2.5));

			//combine input and node potential and then cleanup input vector

			//quash funciton after cleaning input

		
		}
	};
}