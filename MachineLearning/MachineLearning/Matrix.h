#pragma once

#include <vector>
#include <iostream>
#include <initializer_list>
#include <functional>
#include <string>

class Matrix
{

public:
					Matrix(unsigned int row, unsigned int column);

					Matrix(const Matrix & a);

					Matrix(const std::initializer_list<const std::initializer_list<double>>& a);

					~Matrix();
	
	unsigned int	column() const;
	unsigned int	row() const;
	
	double &		operator()(const unsigned int & line, const unsigned int & coumn);

	const double	operator()(const unsigned int & line, const unsigned int & coumn) const;

	void			set(unsigned int i, unsigned int j, double value);

	Matrix			operator+(const Matrix& a) const;
	Matrix			operator+(const double& a) const;
	Matrix			operator-(const Matrix& a) const;
	Matrix			operator*(const Matrix& a) const;
	Matrix			operator*(const double& a) const;

	Matrix &		operator+=(const Matrix& a);
	Matrix &		operator+=(const double& value);
	Matrix &		operator-=(const Matrix& a);
	Matrix &		operator/=(const double& value);

	Matrix &		operator=(const Matrix& a);

	Matrix			column(int c);																					//Retourne la colonne
	Matrix			row(int l);																						//Retourne la ligne

	void			fill(const double& value);

	
	void			fill(std::function<double(unsigned int, unsigned int)>);
	void			apply(std::function<double(double)>);															//Applique une fonction sur tous les éléments


	void			multiplyItemByItem(const Matrix & a);															//Multiplie élément par élément

	std::vector<double>	selectColumn(unsigned int column);


	std::string			toString() const;

	Matrix			transpo();																						//Retourne la matrice transposée

	static Matrix	row(std::vector<double> row);																	//Transforme un vecteur en matrice ligne
	static Matrix	column(std::vector<double> col);																//Transforme un vecteur en matrice colonne

	double			sum();																							//Somme de tous les éléments de la matrice

	//Affichage/deboguage
	void			showDimension() {
		std::cout << Row << " x " << Column << std::endl;
	}


private:
	unsigned int	Row;
	unsigned int	Column;
	double **		Data;

	void			allocateMemory(unsigned int row, unsigned int column);
	void			freeMemory();

	bool			sameSizeAs(const Matrix & a)  const;

};

