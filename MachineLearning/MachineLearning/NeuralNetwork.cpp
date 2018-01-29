#include "NeuralNetwork.h"

#include <random>
#include <math.h>


NeuralNetwork::NeuralNetwork(int inputSize, int outputSize, RealFunction activation){

	Layers.push_back(Matrix(1, inputSize));
	Layers.push_back(Matrix(1, outputSize));

	Biais.push_back(Matrix(1, outputSize));

	this->activation = activation;

	Links.push_back(Matrix(inputSize, outputSize));
}


void NeuralNetwork::fillWeightsRandomly(int seed){
	std::default_random_engine generator;
	std::normal_distribution<double> distribution;
	generator.seed(seed);


	auto random = [&distribution, &generator](unsigned int i, unsigned int j) {
		return distribution(generator); 
	};

	for (Matrix &a : Links)
		a.fill(random);

	for (Matrix &a : Biais)
		a.fill(random);
}

void NeuralNetwork::fillWeights(double value){

	for (Matrix &a : Links)
		a.fill(value);

	for (Matrix &a : Biais)
		a.fill(value);
}

void NeuralNetwork::addLayer(int height){

	Layers.insert(Layers.end() - 1,  Matrix(1, height));
	Biais.insert(Biais.end() - 1, Matrix(1, height));

	Links.pop_back();

	Links.push_back(Matrix(Layers[Layers.size() - 3].column(), height));
	
	Links.push_back(Matrix(height, Layers[Layers.size() - 1].column()));	
}

//Propagation avant, en fonction des inputs
bool NeuralNetwork::forwardSpread(vector<double> input){
	
	if (input.size()!= Layers[0].column() || Links.size() != Layers.size() - 1 || Biais.size() != Layers.size() - 1)
		throw "Error forwardSpread [parameters]";
		
	//vector -> Matrix
	Layers[0] = Matrix::row(input);
	
	//Propagation avant
	for (int i = 0; i < Layers.size() - 1; i++) {
		Layers[i + 1] = (Layers[i] * Links[i])  + Biais[i];
		
		Layers[i + 1].apply(activation);
	}	
	
	return true;
}

void NeuralNetwork::backwardSpread(vector<double> output){
	
	if (output.size() != Layers.back().column() || Links.size() != Layers.size() - 1 || Biais.size() != Layers.size() - 1)
		throw "Error backwardSpread [parameters]";


	BiaisGradient = vector<Matrix>(Layers.size() - 1, Matrix(1,1));
	
	BiaisGradient.back() = Matrix::column(output) - Layers.back().transpo();
	BiaisGradient.back().multiplyItemByItem(derivate(Layers.back()));

	for (int i = BiaisGradient.size() - 2; i >= 0; i--) {
		
		BiaisGradient[i] = Links[i + 1] * BiaisGradient[i + 1];
		
		BiaisGradient[i].multiplyItemByItem(derivate(Layers[i + 1]));
	}

	//Links gradient
	LinksGradients.clear();
	for (int l = 0; l < Links.size(); l++) 
		LinksGradients.push_back(Layers[l].transpo() * BiaisGradient[l].transpo());
		
}

void NeuralNetwork::updateWeights(){

	//Update biais
	for (int i = 0; i < Biais.size(); i++)
		Biais[i] += BiaisGradient[i].transpo() * learningRate;

	//Update links
	for (int i = 0; i < Links.size(); i++)
		Links[i] += LinksGradients[i] * learningRate;
}

Matrix NeuralNetwork::output(){
	return Layers.back();
}

Matrix NeuralNetwork::input(){
	return Layers[0];
}

double NeuralNetwork::sigmoide(double x){
	
	return 1. / (1 + pow(2.71828182845904523536, -x));
}

//Transpose & dérive...
Matrix NeuralNetwork::derivate(const Matrix & a){
	
	if (a.row() != 1)
		throw "Need row matrix...";

	Matrix r(a.column(), 1);

	// i * (1 - i)
	for (int i = 0; i < r.row(); i++)
		r(i, 0) = a(0, i) * (1 - a(0, i));
	
	return r;
}




template<class T>
vector<T> copy(vector<T> a) {
	vector<T> r;

	for (T t : a)
		r.push_back(t);

	return r;
}


double NeuralNetwork::learnFromBatch(vector<vector<double>> inputs, vector<vector<double>> outputs){

	if (inputs.size() != outputs.size())
		throw "Error learnFromBatch [parameters]";

	forwardSpread(inputs[0]);
	backwardSpread(outputs[0]);

	double globalCost					= cost(outputs[0]);
	vector<Matrix> BatchGradientLink	= copy(this->LinksGradients);
	vector<Matrix> BatchGradientBiais	= copy(this->BiaisGradient);

	for (int i = 1; i < inputs.size(); i++) {

		forwardSpread(inputs[i]);
		backwardSpread(outputs[i]);

		globalCost += cost(outputs[i]);

		for (int i = 0; i < LinksGradients.size(); i++)
			BatchGradientLink[i] += LinksGradients[i];

		for (int i = 0; i < BiaisGradient.size(); i++)
			BatchGradientBiais[i] += BiaisGradient[i];
	}

	for (int i = 0; i < BatchGradientLink.size(); i++)
		BatchGradientLink[i] /= inputs.size();

	for (int i = 0; i < BatchGradientBiais.size(); i++)
		BatchGradientBiais[i] /= inputs.size();

	LinksGradients = BatchGradientLink;
	BiaisGradient = BatchGradientBiais;

	this->updateWeights();

	return globalCost / inputs.size();
}

double NeuralNetwork::cost(vector<double> output){

	Matrix delta = Matrix::row(output) - Layers.back();
	
	delta.apply([](double x) {return x * x; });

	return delta.sum();
}
