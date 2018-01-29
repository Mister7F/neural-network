#include "Matrix.h"


bool Matrix::sameSizeAs(const Matrix & a)  const  {
	return (a.Row == Row && a.Column == Column);
}

Matrix::Matrix(unsigned int row, unsigned int column){

	allocateMemory(row, column);
	
}

Matrix::Matrix(const Matrix & a) {

	allocateMemory(a.Row, a.Column);

	for(int i          = 0; i < Row; i++)
		for(int j      = 0; j < Column; j++)
			Data[i][j] = a.Data[i][j];
}

Matrix::Matrix(const std::initializer_list<const std::initializer_list<double>>& a) {

	allocateMemory(a.size(), ( (const std::initializer_list<double> *) a.begin())->size());
	
	for (int i = 0; i < Row; i++) {

		const std::initializer_list<double> *row = (a.begin() + i);

		for (int j = 0; j < Column; j++) {
			Data[i][j] = *(row->begin() + j);
		}
	}
}

Matrix::~Matrix(){
	
	freeMemory();
}

void Matrix::allocateMemory(unsigned int row, unsigned int column) {

	this->Column = column;
	this->Row = row;

	Data = new double*[Row];

	for (int i = 0; i < Row; i++)
		Data[i] = new double[Column];

}

void Matrix::freeMemory() {

	for (int i = 0; i < Row; i++)
		delete[] Data[i];

	delete[] Data;
}

unsigned int Matrix::column() const {
	return Column;
}

unsigned int Matrix::row() const {
	return Row;
}

double& Matrix::operator()(const unsigned int & line, const unsigned int & column) {
	return Data[line][column];
}

const double Matrix::operator()(const unsigned int & line, const unsigned int & column) const {
	return Data[line][column];
}

void Matrix::set(unsigned int i, unsigned int j, double value){

	Data[i][j] = value;
}

Matrix Matrix::operator+(const Matrix& a)  const {

	if (!sameSizeAs(a))
		throw("Matrix operator '+' [dimension error]");

	Matrix result(Row, Column);

	for (int i = 0; i < Row; i++) 
		for (int j = 0; j < Column; j++) 
			result.Data[i][j] = Data[i][j] + a(i, j);
		
	return result;
}

Matrix Matrix::operator+(const double & a) const {
	Matrix result(Row, Column);

	for (int i = 0; i < Row; i++)
		for (int j = 0; j < Column; j++)
			result.Data[i][j] = Data[i][j] + a;

	return result;
}

Matrix Matrix::operator-(const Matrix& a)  const {

	if (!sameSizeAs(a))
		throw("Matrix operator '-' [dimension error]");

	Matrix result(Row, Column);

	for (int i = 0; i < Row; i++)
		for (int j = 0; j < Column; j++)
			result.Data[i][j] = Data[i][j] - a(i, j);

	return result;
}

Matrix Matrix::operator*(const double& a)  const {

	Matrix result(Row, Column);

	for (int i = 0; i < Row; i++)
		for (int j = 0; j < Column; j++)
			result.Data[i][j] = Data[i][j] * a;

	return result;
}

Matrix Matrix::operator*(const Matrix& a)  const {

	if (a.Row != Column)
		throw("Matrix operator '*' [dimension error]");

	Matrix result(Row, a.Column);
	result.fill(0);

	for (int i = 0; i < Row; i++) 
		for (int j = 0; j < a.Column; j++) 			
			for (int k = 0; k < Column; k++) 
				result(i, j) += Data[i][k] * a(k, j);
				
	return result;
}

Matrix & Matrix::operator+=(const Matrix& a) {

	if (!sameSizeAs(a))
		throw("Matrix operator '+=' [dimension error]");


	for (int i = 0; i < Row; i++)
		for (int j = 0; j < Column; j++) 
			Data[i][j] += a(i, j);
	
	return *this;
}

Matrix & Matrix::operator+=(const double &value) {

	for (int i = 0; i < Row; i++)
		for (int j = 0; j < Column; j++)
			Data[i][j] += value;

	return *this;
}


Matrix & Matrix::operator-=(const Matrix& a) {

	if (!sameSizeAs(a))
		throw "Not same size...";


	for (int i = 0; i < Row; i++)
		for (int j = 0; j < Column; j++)
			Data[i][j] -= a(i, j);

	return *this;
}

Matrix & Matrix::operator/=(const double & value){

	for (int i = 0; i < Row; i++)
		for (int j = 0; j < Column; j++)
			Data[i][j] /= value;

	return *this;
}

Matrix& Matrix::operator=(const Matrix& a) {

	freeMemory();


	allocateMemory(a.Row, a.Column);
	
	for (int i = 0; i < Row; i++) 
		for (int j = 0; j < Column; j++) 
			Data[i][j] = a(i, j);	

	return *this;
}

Matrix Matrix::column(int c)
{
	Matrix col(Row, 1);
	
	for (unsigned int i = 0; i < Row; i++) 
		col(i, 0) = Data[i][c];
	
	return col;
}

Matrix Matrix::row(int l)
{
	Matrix r(1, Column);

	for (unsigned int i = 0; i < Column; i++)
		r(0, i) = Data[l][i];

	return r;
}


void Matrix::fill(const double& value) {

	for (int i = 0; i < Row; i++) 
		for (int j = 0; j < Column; j++)
			Data[i][j] = value;	
}

void Matrix::fill(std::function<double(unsigned int, unsigned int)> f) {

	for (int i = 0; i < Row; i++)
		for (int j = 0; j < Column; j++)
			Data[i][j] = f(i, j);

}

void Matrix::apply(std::function<double(double)> f){

	for (int i = 0; i < Row; i++)
		for (int j = 0; j < Column; j++)
			Data[i][j] = f(Data[i][j]);
}

void Matrix::multiplyItemByItem(const Matrix & a){

	if (!sameSizeAs(a))
		throw "Not same size...";
	
	for (int i = 0; i < Row; i++)
		for (int j = 0; j < Column; j++)
			Data[i][j] *= a(i, j);

}

std::vector<double> Matrix::selectColumn(unsigned int column){

	std::vector<double> Column(Row);

	for (int i = 0; i < Row; i++)
		Column[i] = Data[i][column];
	
	return Column;
}

std::string Matrix::toString() const {
	std::string str;

	for (int i = 0; i < Row; i++) {
		for (int j = 0; j < Column; j++)
			str +=  std::to_string(Data[i][j]) + "\t";
		str += "\n";
	}
	
	return str;
}

Matrix Matrix::transpo()
{
	Matrix t(Column, Row);

	for (int i = 0; i < Row; i++) {
		for (int j = 0; j < Column; j++) {
			t(j, i) = Data[i][j];
		}
	}
	
	return t;
}

Matrix Matrix::row(std::vector<double> row){
	Matrix ro(1, row.size());

	for (int i = 0; i < row.size(); i++)
		ro(0, i) = row[i];

	return ro;
}

Matrix Matrix::column(std::vector<double> col){
	Matrix co(col.size(), 1);

	for (int i = 0; i < col.size(); i++)
		co(i, 0) = col[i];

	return co;
}

double Matrix::sum(){
	double s = 0;

	for (int i = 0; i < Row; i++)
		for (int j = 0; j < Column; j++)
			s += Data[i][j];
	
	return s;
}
