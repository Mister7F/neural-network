#pragma once

#include "Matrix.h"

#include <vector>
#include <time.h>
using namespace std;


typedef std::function<double(double)> RealFunction;


class NeuralNetwork
{
public:
					NeuralNetwork(int inputSize, int outputSize, RealFunction activation = sigmoide);

	void			fillWeightsRandomly(int seed = time(0));
	void			fillWeights(double value);

	void			addLayer(int height);

	bool			forwardSpread(vector<double> input);				//Calcul l'output
	void			backwardSpread(vector<double> output);				//Calcul les gradient
	void			updateWeights();									//Met à jour les poids


	Matrix			output();
	Matrix			input();

	static double	sigmoide(double x);
	static Matrix	derivate(const Matrix& a);																		// i * (1 - i)

	//Apprend depuis un batch, retourne le coût moyen lors de l'apprentissage...
	double			learnFromBatch(vector<vector<double>> inputs, vector<vector<double>> outputs);					

	double			cost(vector<double> output);

//private:
	
	vector<Matrix>	Layers;		//Matrice sur une seule ligne
	vector<Matrix>	Links;
	vector<Matrix>	Biais;		//Biais, Matrice sur une seule ligne

	//Stock les gradients calculés lors d'une propagation arrière
	vector<Matrix>	LinksGradients;
	vector<Matrix>	BiaisGradient;
	
	double			learningRate = 0.1;
	RealFunction	activation;
		


	//Pour le test
	void showLinks() {
		for (auto l : Links)
			cout << l.toString() << endl;
	}

	void showLayers() {
		for (auto l : Layers)
			cout << l.toString() << endl;
	}

};

