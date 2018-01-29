#pragma once

#include <iostream>
#include <random>
#include <time.h>
#include <fstream>
#include <string>

using namespace std;

#include "NeuralNetwork.h"
#include "Matrix.h"

/*	DOCUMENTATION CONCERNANT LES DATAS

TRAINING SET LABEL FILE (train-labels-idx1-ubyte):
[offset] [type]          [value]          [description]
0000     32 bit integer  0x00000801(2049) magic number (MSB first)
0004     32 bit integer  60000            number of items
0008     unsigned byte   ??               label
0009     unsigned byte   ??               label
........
xxxx     unsigned byte   ??               label

The labels values are 0 to 9.
TRAINING SET IMAGE FILE (train-images-idx3-ubyte):
[offset] [type]          [value]          [description]
0000     32 bit integer  0x00000803(2051) magic number
0004     32 bit integer  60000            number of images
0008     32 bit integer  28               number of rows
0012     32 bit integer  28               number of columns
0016     unsigned byte   ??               pixel
0017     unsigned byte   ??               pixel
........
xxxx     unsigned byte   ??               pixel

Pixels are organized row-wise. Pixel values are 0 to 255. 0 means background (white), 255 means foreground (black).


*/

#define PICSIZE			784
#define FILEDATAPATH	"exo2/train-images.idx3-ubyte"
#define FILELABELPATH	"exo2/train-labels.idx1-ubyte"
#define PICNUMBER		60000

int getNextLabe(ifstream &fileLabel) {

	char a;
	fileLabel.read(&a, 1);
	return (int)(unsigned char)a;
}



//Divise les pixels par 255 pour avoir des valeurs entre 0 et 1
vector<double> getNextImage(ifstream &fileData) {


	if (!fileData.is_open()) {
		cout << "Error, file not open" << endl;
		return vector<double>();
	}

	vector<double> pic(PICSIZE);

	unsigned char c[PICSIZE];
	fileData.read((char*)c, PICSIZE);

	for (int i = 0; i < PICSIZE; i++)
		pic[i] = c[i] / 255.;



	return pic;
}


//Dessine dans la console...
void drawPic(vector<double> pic) {

	for (int i = 0; i < pic.size(); i++) {

		cout << pic[i] << " ";

		if (pic[i] > 0.7)
			cout << 'O';
		else if (pic[i] > 0.5)
			cout << '.';
		else
			cout << ' ';

		if ((i + 1) % 28 == 0)
			cout << endl;
	}

}

double error(int a, Matrix b) {

	b(0, a) -= 1;

	double err = 0;

	for (int i = 0; i < b.column(); i++)
		err += b(0, i) * b(0, i);

	return err;
}

void learn(ifstream &fileData, ifstream &fileLabel) {

	static const int traineNumber		= 50000;
	static const int testNumber			= 5000;
	static const int validationNumber	= 5000;
	static const int roundNumber		= 5;
	static const int batchSize			= 100;

	NeuralNetwork neuralNetwork(784, 10);

	neuralNetwork.addLayer(16);
	neuralNetwork.addLayer(16);


	neuralNetwork.fillWeightsRandomly(1);


	neuralNetwork.learningRate = 1;



	for (int round = 0; round < roundNumber; round++) {
		
		fileData.seekg(16);
		fileLabel.seekg(8);

		double totalError = 0;

		for (int i = 0; i < traineNumber; i += batchSize) {
			
			vector<vector<double>> inputs;
			vector<vector<double>> outputs;
			for (int j = 0; j < batchSize; j++) {

				inputs.push_back(getNextImage(fileData));

				vector<double> o(10, 0);
				o[getNextLabe(fileLabel)] = 1;

				outputs.push_back( o );
			}
			
			double currentError = neuralNetwork.learnFromBatch(inputs, outputs);

			totalError += currentError;

			if (i % 100 == 0) {
				system("CLS");
				cout << "Apprentissage en cours..." << endl;
				cout << "Round: \t\t" << round << "/" << roundNumber << endl;
				cout << "Example :\t" << i << "/" << traineNumber << endl;
				cout << "Current error: \t" << currentError << endl;
			}
		}

		cout << "Erreur du round " << round << " " << totalError / traineNumber * batchSize  << endl;
		


		cout << "Test en cours..." << endl;
		totalError = 0;
		for (int i = 0; i < testNumber; i++) {


			vector<double> pic = getNextImage(fileData);
			int response = getNextLabe(fileLabel);

			vector<double> vecResponse(10, 0);
			vecResponse[response] = 1;

			neuralNetwork.forwardSpread(pic);

			totalError += error(response, neuralNetwork.Layers[neuralNetwork.Layers.size() - 1]);			
		}
		cout << "Erreur total sur les tests: " << totalError / testNumber << endl;
		system("pause");
	}


	
	cout << "Quelque exemple..." << endl;
	system("pause");
	for (int i = 0; i < validationNumber; i++) {
		
		vector<double> pic = getNextImage(fileData);
		int response = getNextLabe(fileLabel);

		vector<double> vecResponse(10, 0);
		vecResponse[response] = 1;

		neuralNetwork.forwardSpread(pic);


		double max = -99;
		int repe = 0;
		Matrix output = neuralNetwork.Layers[neuralNetwork.Layers.size() - 1];
		for (int i = 0; i < output.column(); i++) {
			if (max < output(0, i)) {
				max = output(0, i);
				repe = i;
			}
		}

		cout << "Solution: \t" << response << endl << "Prediction: \t" << repe << "\t" << output(0, repe) << endl << "Solution prediction: \t" << output(0, response) << endl << endl;
		system("pause");

	}



}






int maxIndex(Matrix v) {

	int index = -1;
	double max = -100000;

	for (int i = 0; i < v.column(); i++) {

		if (max < v(0, i)) {
			max = v(0, i);
			index = i;
		}
	}
	return index;
}



void learnClassic(ifstream &fileData, ifstream &fileLabel) {

	static const int traineNumber		= 50000;
	static const int testNumber			= 5000;
	static const int validationNumber	= 5000;
	static const int roundNumber		= 5;

	NeuralNetwork neuralNetwork(784, 10);

	neuralNetwork.addLayer(16);
	neuralNetwork.addLayer(16);


	neuralNetwork.fillWeightsRandomly(0);
	
	neuralNetwork.learningRate = 0.1;



	for (int round = 0; round < roundNumber; round++) {

		fileData.seekg(16);
		fileLabel.seekg(8);

		double totalError = 0;

		for (int i = 0; i < traineNumber; i++) {

			neuralNetwork.forwardSpread(getNextImage(fileData));

			int label = getNextLabe(fileLabel);
			vector<double> output(10, 0);
			output[label] = 1;

			double currentError = (double) maxIndex(neuralNetwork.Layers.back()) != label;


			neuralNetwork.backwardSpread(output);
			neuralNetwork.updateWeights();
		}



		cout << "Test en cours..." << endl;
		totalError = 0;
		for (int i = 0; i < testNumber; i++) {


			vector<double> pic = getNextImage(fileData);
			int response = getNextLabe(fileLabel);

			vector<double> vecResponse(10, 0);
			vecResponse[response] = 1;

			neuralNetwork.forwardSpread(pic);


			double max = -99;
			int repe = 0;
			Matrix output = neuralNetwork.Layers[neuralNetwork.Layers.size() - 1];
			for (int i = 0; i < output.column(); i++) {
				if (max < output(0, i)) {
					max = output(0, i);
					repe = i;
				}
			}


			totalError += (double)repe != response;
		}
		cout << "Erreur total sur les tests: " << totalError / testNumber << endl;
		system("pause");
	}


	

	cout << "Quelque exemple..." << endl;
	system("pause");
	for (int i = 0; i < validationNumber; i++) {

		vector<double> pic = getNextImage(fileData);
		int response = getNextLabe(fileLabel);

		vector<double> vecResponse(10, 0);
		vecResponse[response] = 1;

		neuralNetwork.forwardSpread(pic);


		double max = -99;
		int repe = 0;
		Matrix output = neuralNetwork.Layers[neuralNetwork.Layers.size() - 1];
		for (int i = 0; i < output.column(); i++) {
			if (max < output(0, i)) {
				max = output(0, i);
				repe = i;
			}
		}

		cout << "Solution: \t" << response << endl << "Prediction: \t" << repe << "\t" << output(0, repe) << endl << "Solution prediction: \t" << output(0, response) << endl << endl;
		//system("pause");

	}



}







//Source des données:	http://yann.lecun.com/exdb/mnist/
void exo2() {

	ifstream fileData;

	fileData.open(FILEDATAPATH, ios::binary);


	unsigned int magicNumber;
	unsigned int numberOfImages;
	unsigned int numberOfRow;
	unsigned int numberOfColumn;

	fileData.read((char*)&magicNumber, 4);
	fileData.read((char*)&numberOfImages, 4);
	fileData.read((char*)&numberOfRow, 4);
	fileData.read((char*)&numberOfColumn, 4);

	//On convertit les nombres en little endian...
	cout << "Header data..." << endl;
	cout << "Magic number \t" << _byteswap_ulong(magicNumber) << endl;
	cout << "Image number \t" << _byteswap_ulong(numberOfImages) << endl;
	cout << "Row number \t" << _byteswap_ulong(numberOfRow) << endl;
	cout << "Column number \t" << _byteswap_ulong(numberOfColumn) << endl;







	ifstream fileLabel;

	fileLabel.open(FILELABELPATH, ios::binary);

	fileLabel.read((char*)&magicNumber, 4);
	fileLabel.read((char*)&numberOfImages, 4);

	//On convertit les nombres en little endian...
	cout << endl << "Header label..." << endl;
	cout << "Magic number \t" << _byteswap_ulong(magicNumber) << endl;
	cout << "Image number \t" << _byteswap_ulong(numberOfImages) << endl;


	learnClassic(fileData, fileLabel);


}







