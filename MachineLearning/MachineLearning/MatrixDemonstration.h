#pragma once

/*
	Fichier montrant comment utiliser ma classe matrice, ainsi que ces différentes fonctionnalités...
*/


#include <iostream>
#include "Matrix.h"
using namespace std;


std::ostream& operator<<(std::ostream &flux, const Matrix & a)
{
	for (int i = 0; i < a.row(); i++) {
		for (int j = 0; j < a.column(); j++) {
			flux << a(i, j) << "\t";
		}
		std::cout << std::endl;
	}
	return flux;
}


double fillMatrix(unsigned int i, unsigned int j) {

	return 3 * j + i;

}

void demonstrationMatrix() {

	Matrix a(2, 2);

	//Créé une matrice unité
	a.fill([](int i, int j) { return i == j; });


	a.fill(fillMatrix);


	Matrix b(2, 2);
	b.fill(1);

	Matrix c = {
		{ 1, 2, 3 },
		{ 4, 5, 6 },
		{ 7, 8, 9 }
	};

	Matrix d(c);

	cout << a << endl;

	cout << a(0, 1) << endl;

	cout << b << endl;

	cout << c << endl;

	cout << d << endl;


	a += b;



	cout << a << b << endl;

	cout << a + b << endl;

	cout << a * 7 << endl;

	cout << "a & b" << endl;
	a = {
		{ 1,2,3 },
		{ 4,5,6 }
	};

	b = {
		{ 1,2, 3 },
		{ 6,4, 9 },
		{ 7,6, 1 },
	};


	cout << a << "-------" << endl << b << endl;
	cout << "a * b" << endl;
	cout << a * b << endl;

	cout << c - b << endl;


}



