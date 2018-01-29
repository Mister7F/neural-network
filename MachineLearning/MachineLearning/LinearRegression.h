#pragma once


#include "Matrix.h"


double cost(Matrix X, Matrix Y, Matrix theta) {

	Matrix costs = ((X * theta) - Y);

	double cost = 0;

	for (int i = 0; i < Y.row(); i++)
		cost += costs(i, 0) * costs(i, 0);

	cost /= (2 * X.row());

	return cost;
}

Matrix featureNormalize(Matrix X) {

	vector<double> means(X.column(), 0);
	vector<double> standardDeviation(X.column(), 0);

	//Moyennes
	for (int i = 0; i < X.row(); i++)
		for (int j = 0; j < X.column(); j++)
			means[j] += X(i, j);

	for (int j = 0; j < means.size(); j++)
		means[j] /= X.row();



	//Déviation standard
	for (int i = 0; i < X.row(); i++)
		for (int j = 0; j < X.column(); j++)
			standardDeviation[j] += pow(X(i, j) - means[j], 2);

	for (int j = 0; j < standardDeviation.size(); j++)
		standardDeviation[j] = sqrt(standardDeviation[j] / (X.row() - 1));


	//On normalise les données
	for (int i = 0; i < X.row(); i++)
		for (int j = 0; j < X.column(); j++) {

			if (standardDeviation[j] == 0)
				continue;

			X(i, j) = (X(i, j) - means[j]) / standardDeviation[j];
		}
	
	return X;
}


Matrix linearRegression(Matrix X, Matrix Y, double alpha, unsigned int iteration = 400, Matrix theta = Matrix(0,0) ) {

	int m = X.row();
	int n = X.column();

	if (theta.column() != 1 || theta.row() != n) {
		theta = Matrix(n, 1);
		theta.fill(0);
	}

	for (int it = 0; it < iteration; it++)		
		theta -=  X.transpo() * (X * theta - Y) * (alpha / m);
		
	return theta;
}