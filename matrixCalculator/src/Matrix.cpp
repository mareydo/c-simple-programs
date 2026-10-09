#include "Matrix.h"
#include "MatrixFactory.h"
#include <iostream>
#include <vector>
#include <iomanip>

/**
 * Creates a sub matrix without row and column on skip index
 * Submatrix is used to recursively count determinant
 */
std::unique_ptr<Matrix> Matrix::omitIndex(const std::unique_ptr<Matrix> &matrix, const size_t &skipIndex) const {
    size_t size = this->dimensions.first;
    std::vector<double> result;
    for (size_t row = 1; row < size; ++row) {
        for (size_t column = 0; column < size; ++column) {
            if (column == skipIndex) continue;
            result.push_back(getAt(row, column));
        }
    }
    return MatrixFactory::createVector(size - 1, size - 1, result);
}

/**
 * Counts determinant recursively by using Laplace expansion
 * Counts smaller matrix determinants by cross rule or returns [0][0] for 0x0 matrix
 */
double Matrix::countDeterminant() const {
    if (!isSquare()) {
        return nan("");
    }
    double det = 0;
    size_t size = this->dimensions.first;
    if (size == 1) {
        return this->getAt(0, 0);
    }
    if (size == 2) {
        return (getAt(0, 0) * getAt(1, 1)) - (getAt(0, 1) * getAt(1, 0));
    }
    int sign = 1;
    for (size_t i = 0; i < size; i++) {
        std::unique_ptr<Matrix> subMatrix = omitIndex(clone(), i);
        det += sign * this->getAt(0, i) * subMatrix->countDeterminant();
        sign *= -1;
    }
    return det;
}

/**
 * Splits matrix by row [0,row](row,n]x[m]
 */
std::pair<std::unique_ptr<Matrix>, std::unique_ptr<Matrix>> Matrix::splitByRow(const int &splitRow) const {
    size_t rows = this->dimensions.first;
    if((splitRow < 0)||((size_t)splitRow > rows)) {
        std::cout << "[Row split]: Row To Split Out Of Bounds" << std::endl;
        return std::pair<std::unique_ptr<Matrix>, std::unique_ptr<Matrix>>();
    }
    size_t columns = this->dimensions.second;
    std::vector<double> upperMatrix;
    std::vector<double> lowerMatrix;
    for (size_t row = 0; row < (size_t)splitRow; ++row) {
        for (size_t column = 0; column < columns; ++column) {
            lowerMatrix.push_back(getAt(row, column));
        }
    }
    for (size_t row = splitRow; row < rows; ++row) {
        for (size_t column = 0; column < columns; ++column) {
            upperMatrix.push_back(getAt(row, column));
        }
    }
    return std::make_pair(
            MatrixFactory::createVector(splitRow, columns, lowerMatrix),
            MatrixFactory::createVector(rows - splitRow, columns, upperMatrix));
}

/**
 * Splits matrix by column [n]x[0,column](column,m]
 */
std::pair<std::unique_ptr<Matrix>, std::unique_ptr<Matrix>> Matrix::splitByColumn(const int &splitColumn) const {
    size_t rows = this->dimensions.first;
    size_t columns = this->dimensions.second;
    if ((splitColumn < 0)||((size_t)splitColumn > columns)) {
        std::cout << "[Column split]: Column To Split Out Of Bounds" << std::endl;
        return std::pair<std::unique_ptr<Matrix>, std::unique_ptr<Matrix>>();
    }
    std::vector<double> upperMatrix;
    std::vector<double> lowerMatrix;
    for (size_t row = 0; row < rows; ++row) {
        for (size_t column = 0; column < (size_t)splitColumn; ++column) {
            lowerMatrix.push_back(getAt(row, column));
        }
        for (size_t column = splitColumn; column < columns; ++column) {
            upperMatrix.push_back(getAt(row, column));
        }
    }

    return std::make_pair(
            MatrixFactory::createVector(rows, splitColumn, lowerMatrix),
            MatrixFactory::createVector(rows, columns - splitColumn, upperMatrix));
}
/**
 * Merge to same witdh matrixes by row
 * M1
 * M2
 */
std::unique_ptr<Matrix> Matrix::mergeByRow(const std::unique_ptr<Matrix> &lower) const {
    size_t rowsU = this->dimensions.first;
    size_t columnsU = this->dimensions.second;

    size_t rowsL = lower->dimensions.first;
    size_t columnsL = lower->dimensions.second;

    if (columnsL != columnsU) {
        std::cout << "[Merge by Row]: Matrixes Need to Have Same Number Of Columns" << std::endl;
        return std::unique_ptr<Matrix>();
    }
    std::vector<double> result;
    for (size_t row = 0; row < rowsU; ++row) {
        for (size_t column = 0; column < columnsU; ++column) {
            result.push_back(this->getAt(row, column));
        }
    }
    for (size_t row = 0; row < rowsL; ++row) {
        for (size_t column = 0; column < columnsL; ++column) {
            result.push_back(lower->getAt(row, column));
        }
    }
    return MatrixFactory::createVector(rowsL + rowsU, columnsL, result);
}
/**
 * Merge to same height matrixes by row
 * M1M2
 */
std::unique_ptr<Matrix> Matrix::mergeByColumn(const std::unique_ptr<Matrix> &right) const {
    size_t rowsL = this->dimensions.first;
    size_t columnsL = this->dimensions.second;

    size_t rowsR = right->dimensions.first;
    size_t columnsR = right->dimensions.second;

    if (rowsL != rowsR) {
        std::cout << "[Merge by Row]: Matrixes Need to Have Same Number Of Rows" << std::endl;
        return std::unique_ptr<Matrix>();
    }
    std::vector<double> result;
    for (size_t row = 0; row < rowsL; ++row) {
        for (size_t column = 0; column < columnsL; ++column) {
            result.push_back(this->getAt(row, column));
        }
        for (size_t column = 0; column < columnsR; ++column) {
            result.push_back(right->getAt(row, column));
        }
    }
    return MatrixFactory::createVector(rowsL, columnsL + columnsR, result);
}

/**
 * Simple gem to get upper triangular shape
 * ! Will not endup at identity !
 */
std::unique_ptr<Matrix> Matrix::gem(bool verbose = false) const {
    std::unique_ptr<Matrix> retMatrix = this->clone();
    size_t rows = this->dimensions.first;
    size_t columns = this->dimensions.second;

    //make 0 under diagonal
    for (size_t diagonalIndex = 0; diagonalIndex < rows - 1; ++diagonalIndex) {
        if (verbose) {
            std::cout << "[GEM]AT row: " << diagonalIndex << std::endl;
        }
        //if there is 0 at diagonal swap row 
        if (retMatrix->getAt(diagonalIndex, diagonalIndex) == 0) {
            size_t notZeroRow;
            //find nonzero
            for (notZeroRow = diagonalIndex + 1; notZeroRow < rows; ++notZeroRow) {
                if (retMatrix->getAt(notZeroRow, diagonalIndex) != 0) break;
            }
            //if there is non zero under zero determinant
            if (notZeroRow != rows) {
                for (size_t column = diagonalIndex; column < columns; ++column) {
                    double tmp = retMatrix->getAt(notZeroRow, column);
                    retMatrix->setAt(notZeroRow, column, retMatrix->getAt(diagonalIndex, column));
                    retMatrix->setAt(diagonalIndex, column, tmp);
                }
            }
        }
        //try to make zero in next row
        for (size_t row = diagonalIndex + 1; row < rows; ++row) {
            if (verbose) {
                std::cout << "[GEM]Making zeroes under main row on row: " << row << std::endl;
            }
            //get ratio to make zeroes as -1 * under diagnoal/diagonal or zero to prevend n/0
            double diagonal = retMatrix->getAt(diagonalIndex, diagonalIndex);
            double ratio;
            if (diagonal == 0) {
                ratio = 0;
            } else {
                ratio = -retMatrix->getAt(row, diagonalIndex) / diagonal;
            }
            if (verbose) {
                std::cout << "[GEM]Ratio to make row zero: " << ratio << std::endl;
            }

            //go through columns
            for (size_t column = diagonalIndex; column < columns; ++column) {
                if (verbose) {
                    std::cout << "[GEM]Dividing elements on column: " << column << std::endl;
                }
                //under actual diagonal
                if (diagonalIndex == column) {
                    // sum would endup at 0, saving time
                    retMatrix->setAt(row, column, 0);
                } else {
                    //rest of the row, that is not directly under diagonal
                    double newVal = retMatrix->getAt(row, column) + ratio * retMatrix->getAt(diagonalIndex, column);
                    retMatrix->setAt(row, column, newVal);
                }
                if (verbose) {
                    std::cout << "[GEM]New value at " << row << "x" << column << " = " << retMatrix->getAt(row, column)
                              << std::endl;
                }
            }
        }
    }
    //convert to vector and return new Matrix
    std::vector<double> result;
    for (size_t row = 0; row < rows; ++row) {
        for (size_t column = 0; column < columns; ++column) {
            result.push_back(retMatrix->getAt(row, column));
        }
    }
    return MatrixFactory::createVector(rows, columns, result);
}

/**
 * Simply changing rows with columns
 */
std::unique_ptr<Matrix> Matrix::transpose() const {

    std::vector<double> result;
    size_t rows = this->dimensions.first;
    size_t columns = this->dimensions.second;
    for (size_t column = 0; column < columns; ++column) {
        for (size_t row = 0; row < rows; ++row) {
            result.push_back(this->getAt(row, column));
        }
    }
    return MatrixFactory::createVector(columns, rows, result);
}

/**
 * Only square matrixes M with non zero determinant (regular) has inversion I
 * M|E -> GEM -> E|I -> split -> I
 * M*I = E
 */
std::unique_ptr<Matrix> Matrix::inverse() const {
    if ((!isSquare()) || (getDeterminant() == 0)) {
        std::cout << "[Inverse]Inverse Does Not Exist" << std::endl;
        return std::unique_ptr<Matrix>();
    }
    //original|indentity + gem
    std::unique_ptr<Matrix> extended = this->mergeByColumn(
            MatrixFactory::createIdentity(this->dimensions.first))->gem();

    size_t rows = extended->dimensions.first;
    size_t columns = extended->dimensions.second;

    //create identity from upper triangle
    //identity|iverse
    for (int row = rows - 1; row >= 0; --row) {
        //original diagonal value
        double ratio = extended->getAt(row, row);
        for (size_t column = 0; column < columns; ++column) {
            //divide row to get 1 on diagonal
            double newVal = extended->getAt(row, column) / ratio;
            extended->setAt(row, column, newVal);
        }
        //go up
        for (int lowerRow = row - 1; lowerRow >= 0; --lowerRow) {
            //get ratio of what is directly on top of diagonal
            double ratio = extended->getAt(lowerRow, row);
            for (size_t lowerColumn = 0; lowerColumn < columns; ++lowerColumn) {
                //lowerRow = actValue - actRow*diagonal
                double newVal = extended->getAt(lowerRow, lowerColumn) - extended->getAt(row, lowerColumn) * ratio;
                extended->setAt(lowerRow, lowerColumn, newVal);
            }
        }
    }
    //return right matrix
    return extended->splitByColumn(columns / 2).second;

}

double Matrix::getDeterminant() const {
    if (std::isnan(determinant)) {
        return countDeterminant();
    }
    return determinant;
}
/**
 * Non const type can save determinant for later
 */
double Matrix::getDeterminant() {
    if (std::isnan(determinant)) {
        determinant = countDeterminant();
    }
    return determinant;
}

/**
 * Rank as number of non zero lines after doing GEM
 */
size_t Matrix::getRank() const {

    std::unique_ptr<Matrix> gem = this->gem();
    size_t rows = gem->dimensions.first;
    size_t columns = gem->dimensions.second;

    size_t nonZeroLine = 0;
    for (size_t row = 0; row < rows; ++row) {
        for (size_t column = 0; column < columns; ++column) {
            if (gem->getAt(row, column) != 0) {
                ++nonZeroLine;
                break;
            }
        }
    }
    return nonZeroLine;
}

std::pair<size_t, size_t> Matrix::getDimensions() const {
    return dimensions;
}

bool Matrix::isSquare() const {
    return dimensions.first == dimensions.second;
}
/**
 * 1 2 3
 * 4 5 6
 *  ...
 */
std::ostream &operator<<(std::ostream &os, const std::unique_ptr<Matrix> &matrix)
{

        size_t rows = matrix->dimensions.first;
        size_t columns = matrix->dimensions.second;
        for (size_t row = 0; row < rows; ++row) {
            for (size_t column = 0; column < columns; ++column) {
                os << std::setw(4) << matrix->getAt(row, column);
                if (column + 1 != columns) {
                    os << " ";
                }
            }
            os << std::endl;
        }
        return os;

}

/**
 * Only same dimension matrixes have defined summation
 * S[i,j] = A[i,j] + B[i,j]
 */
std::unique_ptr<Matrix> operator+(const std::unique_ptr<Matrix> &l, const std::unique_ptr<Matrix> &r) {
    if (l->getDimensions() != r->getDimensions()) {
        std::cout << "[A+B]: Matrixes must have same dimensions" << std::endl;
        return std::unique_ptr<Matrix>();
    }
    size_t rows = l->dimensions.first;
    size_t columns = l->dimensions.second;
    std::vector<double> sum;
    for (size_t row = 0; row < rows; ++row) {
        for (size_t column = 0; column < columns; ++column) {
            sum.push_back(l->getAt(row, column) + r->getAt(row, column));
        }
    }
    return MatrixFactory::createVector(rows, columns, sum);
}

/**
 * Only same dimension matrixes have defined substracion
 * S[i,j] = A[i,j] - B[i,j]
 */
std::unique_ptr<Matrix> operator-(const std::unique_ptr<Matrix> &l, const std::unique_ptr<Matrix> &r) {
    if (l->getDimensions() != r->getDimensions()) {
        std::cout << "[A-B]: Matrixes must have same dimensions" << std::endl;
        return std::unique_ptr<Matrix>();
    }
    size_t rows = l->dimensions.first;
    size_t columns = l->dimensions.second;
    std::vector<double> sum;
    for (size_t row = 0; row < rows; ++row) {
        for (size_t column = 0; column < columns; ++column) {
            sum.push_back(l->getAt(row, column) - r->getAt(row, column));
        }
    }
    return MatrixFactory::createVector(rows, columns, sum);
}

/**
 * A[m,n], B[o,p]
 * Multiplication exist if n == o
 * Go by columns at A and rows at B and sums them at S[Arow,Bcolumn] 
 */
std::unique_ptr<Matrix> operator*(const std::unique_ptr<Matrix> &l, const std::unique_ptr<Matrix> &r) {
    if (l->getDimensions().second != r->getDimensions().first) {
        std::cout << "[A*B]: Incorrect dimensions for multiplying" << std::endl;
        return std::unique_ptr<Matrix>();
    }
    size_t rowsL = l->getDimensions().first;
    size_t columnsL = l->getDimensions().second;
    size_t columnsR = r->getDimensions().second;
    std::vector<double> result;
    for (size_t rowL = 0; rowL < rowsL; ++rowL) {
        for (size_t columnR = 0; columnR < columnsR; ++columnR) {
            double cell = 0;
            for (size_t columnL = 0; columnL < columnsL; ++columnL) {
                cell += l->getAt(rowL, columnL) * r->getAt(columnL, columnR);
            }
            result.push_back(cell);

        }
    }
    return MatrixFactory::createVector(rowsL, columnsR, result);

}
/**
 * Power is defined only for square matrix
 * Identity for pow == 0, Matrix for pow == 1
 * Iterative A^n = A*A*A*(n-times)
 */
std::unique_ptr<Matrix> operator^(const std::unique_ptr<Matrix> &l, const int &power) {
    if (!l->isSquare()) {
        std::cout << "[A^power]: Power is defined only for square matrix" << std::endl;
        return std::unique_ptr<Matrix>();
    }
    if (power < 0) {
        std::cout << "[A^power]: Power is defined only for positive numbers" << std::endl;
        return std::unique_ptr<Matrix>();
    }
    if (power == 0) {
        return MatrixFactory::createIdentity(l->getDimensions().first);
    }
    if (power == 1) {
        return l->clone();
    }
    std::unique_ptr<Matrix> last = l->clone();
    for (int i = 0; i < power - 1; ++i) {
        last = last * l;
    }

    std::vector<double> result;
    size_t rows = last->dimensions.first;
    size_t columns = last->dimensions.second;
    for (size_t row = 0; row < rows; ++row) {
        for (size_t column = 0; column < columns; ++column) {
            result.push_back(last->getAt(row, column));
        }
    }
    return MatrixFactory::createVector(rows, columns, result);
}


