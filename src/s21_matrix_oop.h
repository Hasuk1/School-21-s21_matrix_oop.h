#ifndef S21_MATRIX_OOP_H
#define S21_MATRIX_OOP_H

#include <cmath>
#include <iostream>
#include <memory>

class S21Matrix {
 public:
  /*Конструкторы и деструкторы*/
  S21Matrix() : rows_(0), cols_(0), matrix_(nullptr) {}  //Базовый конструктор
  S21Matrix(int rows, int cols);  //Параметризированный конструктор
  S21Matrix(const S21Matrix& other);  //Конструктор копирования
  S21Matrix(S21Matrix&& other);       //Конструктор переноса
  ~S21Matrix() { RemoveMatrix(); };  //Деструктор

  /*Операции над матрицами*/
  bool EqMatrix(const S21Matrix& other);
  void SumSubMatrix(const S21Matrix& other, bool isSum);
  void SumMatrix(const S21Matrix& other);
  void SubMatrix(const S21Matrix& other);
  void MulNumber(const double num);
  void MulMatrix(const S21Matrix& other);
  S21Matrix Transpose();
  double Determinant();
  S21Matrix CalcComplements();
  S21Matrix InverseMatrix();

  /*Операторы и перегруженные операторы*/
  double& operator()(int row, int col);
  bool operator==(const S21Matrix& other) { return this->EqMatrix(other); }
  S21Matrix operator+(const S21Matrix& other) { return *this += other; }
  S21Matrix operator-(const S21Matrix& other) { return *this -= other; }
  S21Matrix operator*(const S21Matrix& other) { return *this *= other; }
  S21Matrix operator*(const double num) { return *this *= num; }
  friend S21Matrix operator*(const double num, const S21Matrix& other);
  S21Matrix& operator=(const S21Matrix& other);
  S21Matrix& operator+=(const S21Matrix& other);
  S21Matrix& operator-=(const S21Matrix& other);
  S21Matrix& operator*=(const S21Matrix& other);
  S21Matrix& operator*=(const double num);

  /*Геттеры и сеттеры*/
  void SetRows(int rows) { SetSize(rows, cols_); }
  void SetCols(int cols) { SetSize(rows_, cols); }
  int GetRows() { return rows_; }
  int GetCols() { return cols_; }

 private:
  int rows_, cols_;
  std::unique_ptr<std::unique_ptr<double[]>[]> matrix_;
  const long double epsilon = 1e-7;
  void CreateMatrix();  //Выделение памяти
  void RemoveMatrix();  //Очистка памяти
  void CopyValues(const S21Matrix& other);
  void SetSize(int rows, int cols);
  void GetMinor(S21Matrix& minor, int i, int j);
};
#endif  // S21_MATRIX_OOP_H