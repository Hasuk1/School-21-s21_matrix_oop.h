#include "s21_matrix_oop.h"

/*===КОНСТРУКТОРЫ КЛАССА S21Matrix===*/

/*Параметризированный конструктор*/
S21Matrix::S21Matrix(int rows, int cols) : rows_(rows), cols_(cols) {
  if (rows_ <= 0 || cols_ <= 0) {
    throw std::domain_error(
        "ERROR: Rows and columns must be greater than zero");
  }
  CreateMatrix();
}

/*Конструктор копирования*/
S21Matrix::S21Matrix(const S21Matrix& other)
    : rows_(other.rows_), cols_(other.cols_) {
  CreateMatrix();
  CopyValues(other);
}

/*Конструктор переноса*/
S21Matrix::S21Matrix(S21Matrix&& other)
    : rows_(other.rows_),
      cols_(other.cols_),
      matrix_(std::move(other.matrix_)) {
  other.RemoveMatrix();
}

/*===КОНСТРУКТОРЫ ДЛЯ ОПЕРАТОРОВ===*/

double& S21Matrix::operator()(int row, int col) {
  if (row >= rows_ || col >= cols_ || row < 0 || col < 0) {
    throw std::domain_error("ERROR: segmentation fault");
  }
  return this->matrix_[row][col];
}

S21Matrix& S21Matrix::operator=(const S21Matrix& other) {
  RemoveMatrix();
  rows_ = other.rows_;
  cols_ = other.cols_;
  CreateMatrix();
  CopyValues(other);
  return *this;
}

S21Matrix& S21Matrix::operator+=(const S21Matrix& other) {
  SumMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator-=(const S21Matrix& other) {
  SubMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(const S21Matrix& other) {
  MulMatrix(other);
  return *this;
}

S21Matrix& S21Matrix::operator*=(const double num) {
  MulNumber(num);
  return *this;
}

/*Дополнительный метод для перегрузки "*", возникает конфликт из-за кол-ва уже
 * созданных похожих операторов*/
S21Matrix operator*(const double num, const S21Matrix& other) {
  S21Matrix result(other);
  result.MulNumber(num);
  return result;
}

/*===МЕТОДЫ КОНСТРУКТОРОВ И ДЕСТРУКТОРОВ===*/

/*Метод для выделения памяти через уникальные указатели, которые не требуют
 * последующей очистки, т.к. после выхода за область видимости или уничтожается,
 * память автоматически освобождается. */
void S21Matrix::CreateMatrix() {
  matrix_ = std::make_unique<std::unique_ptr<double[]>[]>(rows_);
  for (int i = 0; i < rows_; i++) {
    matrix_[i] = std::make_unique<double[]>(cols_);
  }
}

/*Метод для зануления переменных класса*/
void S21Matrix::RemoveMatrix() {
  matrix_ = nullptr;
  rows_ = cols_ = 0;
}

/*Метод для копирования значений*/
void S21Matrix::CopyValues(const S21Matrix& other) {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = other.matrix_[i][j];
    }
  }
}

/*Метод для сеттеров*/
void S21Matrix::SetSize(int rows, int cols) {
  S21Matrix buff(*this);
  RemoveMatrix();
  rows_ = rows;
  cols_ = cols;
  CreateMatrix();
  for (int i = 0; i < rows_; ++i) {
    for (int j = 0; j < cols_; ++j) {
      matrix_[i][j] = buff.matrix_[i][j];
    }
  }
}

/*===МЕТОДЫ ОПЕРАЦИЙ НАД МАТРИЦАМИ===*/

/*Сравнение матрицы*/
bool S21Matrix::EqMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) return false;
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      if (fabs(matrix_[i][j] - other.matrix_[i][j]) >= epsilon) return false;
    }
  }
  return true;
}

/*Вспомогательная функция, которая избегает дублирования кода в соответствуюих
 * функциях*/
void S21Matrix::SumSubMatrix(const S21Matrix& other, bool isSum) {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      if (isSum) {
        matrix_[i][j] += other.matrix_[i][j];
      } else
        matrix_[i][j] -= other.matrix_[i][j];
    }
  }
}

/*Суммирование матриц*/
void S21Matrix::SumMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::invalid_argument("ERROR: Matrix dimensions do not match");
  }
  SumSubMatrix(other, true);
}

/*Вычитание матриц*/
void S21Matrix::SubMatrix(const S21Matrix& other) {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    throw std::invalid_argument("ERROR: Matrix dimensions do not match");
  }
  SumSubMatrix(other, false);
}

/*Умножение матрицы на число*/
void S21Matrix::MulNumber(const double num) {
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] *= num;
    }
  }
}

/*Умножение матриц*/
void S21Matrix::MulMatrix(const S21Matrix& other) {
  if (cols_ != other.rows_ || rows_ != other.cols_) {
    throw std::invalid_argument("ERROR: Matrix rows and columns do not match");
  }
  S21Matrix result(rows_, other.cols_);
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < other.cols_; j++) {
      result.matrix_[i][j] = 0.0;
      for (int q = 0; q < cols_; q++)
        result.matrix_[i][j] += matrix_[i][q] * other.matrix_[q][j];
    }
  }
  *this = result;
}

/*Транспонирование матрицы.
 * для этого поменяли местами i и j*/
S21Matrix S21Matrix::Transpose() {
  S21Matrix result(cols_, rows_);
  for (int i = 0; i < cols_; i++) {
    for (int j = 0; j < rows_; j++) {
      result.matrix_[i][j] = matrix_[j][i];
    }
  }
  return result;
}

/*Вычисление минора*/
void S21Matrix::GetMinor(S21Matrix& minor, int i, int j) {
  int row = 0;
  for (int k = 0; k < rows_; k++) {
    if (k == i) continue;
    int col = 0;
    for (int l = 0; l < rows_; l++) {
      if (l == j) continue;
      minor.matrix_[row][col++] = matrix_[k][l];
    }
    row++;
  }
}

/*Вычисление детерминанта. Использовал матрицу миноров, далее рекурсивно вызываю
 * детерминант пока матрица не станет 2*2.*/
double S21Matrix::Determinant() {
  if (rows_ != cols_ || rows_ <= 0) {
    throw std::invalid_argument("ERROR: Matrix must be of square type");
  }
  if (rows_ == 1) {
    return matrix_[0][0];
  } else if (rows_ == 2) {
    return matrix_[0][0] * matrix_[1][1] - matrix_[0][1] * matrix_[1][0];
  } else {
    double det = 0.0;
    for (int j = 0; j < rows_; j++) {
      S21Matrix minor(rows_ - 1, cols_ - 1);
      GetMinor(minor, 0, j);
      int sign = ((j + 1) % 2 == 0) ? -1 : 1;
      det += sign * matrix_[0][j] * minor.Determinant();
    }
    return det;
  }
}

/*Вычисление матрицы алгебраических дополнений с использованием минора и
 * детерминанта из предыдущего метода*/
S21Matrix S21Matrix::CalcComplements() {
  if (rows_ != cols_ || rows_ <= 0) {
    throw std::invalid_argument("ERROR: Matrix must be of square type");
  }
  S21Matrix result(rows_, rows_);
  if (rows_ == 1) result.matrix_[0][0] = 1;
  if (rows_ > 1) {
    S21Matrix minor(rows_ - 1, rows_ - 1);
    for (int j = 0; j < rows_; j++) {
      for (int i = 0; i < rows_; i++) {
        GetMinor(minor, i, j);
        result.matrix_[i][j] = pow(-1, i + j) * minor.Determinant();
      }
    }
  }
  return result;
}

/*Вычисление обратной матрицы. Для расчета нужно вызвать поочерёдно ранее
 * написанные функции*/
S21Matrix S21Matrix::InverseMatrix() {
  double determ = Determinant();
  if (determ == 0) {
    throw std::invalid_argument(
        "ERROR: The determinant of this matrix is 0. The inverse matrix does "
        "not exist.");
  }
  S21Matrix result(rows_, rows_);
  S21Matrix temp = CalcComplements();
  S21Matrix transpose_matrix = temp.Transpose();
  result = transpose_matrix * (1.0 / determ);
  return result;
}