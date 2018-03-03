
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
			Neuron testNeuron = Neuron(false);
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
			int layerCount = 10;
			int blocksPerLay = 1;
			int blockSize = 10;
			Network testNetwork = Network(layerCount,blocksPerLay, blockSize);
			Assert::AreEqual(int(testNetwork.hiddenLayers.size()), layerCount);
			for each (Layer<Block>* l in testNetwork.hiddenLayers)
			{
				for each (Block* b in l->blocks)
				{
					Assert::AreEqual(int(b->population.size()), blockSize);
				}
				
			}

		}

		TEST_METHOD(Check_Sigmoid_Activation_Function) 
		{	
			//needs redone since feedforward is no a layer thing

			Block testBlock = Block();
			testBlock.population[0]->nodePotential = 84;
			
			//testBlock.population[0]->activateSigmoid();
			//testBlock.activateSigmoid();
			Assert::AreEqual(testBlock.population[0]->nodePotential, float(0.988235295));
		}

		TEST_METHOD(Check_Default_Creation) 
		{
			int layerCount = 10;
			int blockSize = 10;
			int blocksPerLayer = 10;
			Network testNetwork = Network(layerCount, blocksPerLayer, blockSize);
			for each (Layer<Block>* l in testNetwork.hiddenLayers)
			{
				//last layer will have no connections in this case
				//fix later when adding testing with output layer
				if (l == testNetwork.hiddenLayers.back()) 
				{
					continue;
				};
				Assert::AreEqual(int(l->connections.size()), 100);
			}
			
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
			Network testNetwork = Network(2, 1, 1);
			//set node potentials
			testNetwork.hiddenLayers[0]->blocks[0]->population[0]->nodePotential = 5;
			testNetwork.hiddenLayers[1]->blocks[0]->population[0]->nodePotential = 3;
			//set starting weights
			testNetwork.hiddenLayers[0]->connections[0]->strengthOfConnection = 0.5;
			return testNetwork;
		}

		TEST_METHOD(Check_Feed_Forward) 
		{
			Network testNetwork = setupTestNetwork();
			bool suceeded = testNetwork.hiddenLayers[0]->connections[0]->feedForward();
			//complete feed forward will be
			//input * weight + bias
			//should be (.5 * 5) + 3
			//for purpose of this test we are checking the passing of the weight and input to the dest block
			//so .5 * 5
		
			Assert::IsTrue(suceeded);
			Assert::AreEqual(int(testNetwork.hiddenLayers[1]->blocks[0]->inputFromConnections.size()), 1);
			Assert::AreEqual(testNetwork.hiddenLayers[1]->blocks[0]->inputFromConnections[0], float(2.5));

			//combine input and node potential 
			//now should be (.5 * 5) +3 = 5.5
			//quash function 5.5/6.5 = ~.84
			testNetwork.hiddenLayers[0]->blocks[0]->sumInputsAndWipeInputVector(false,false);
			Assert::AreEqual(testNetwork.hiddenLayers[1]->blocks[0]->population[0]->nodePotential, float(0.846153846));
			
			//then cleanup input vector
			for each (Layer<Block>* l in testNetwork.hiddenLayers)
			{
				for each(Block* b in l->blocks) 
				{
					Assert::IsTrue(b->inputFromConnections.empty());
				}
			}
			

		
		}

		TEST_METHOD(Check_Data_Refresh_On_Input_Connections) 
		{
		
		
		}

	};
}