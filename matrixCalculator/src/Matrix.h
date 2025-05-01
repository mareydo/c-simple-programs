#ifndef MATRIX_H
#define MATRIX_H

#include <memory>
#include <cmath>
#include <vector>

/**
 * @brief Abstract class to represent matrix
 * Implements operations on matrixes
 * Some methodes are virtual for specific SubClass implementation
 * Operations returning std::unique_ptr<Matrix> returns std::unique_ptr<Matrix>() as empty reply
 */
class Matrix {
protected:
    std::pair<size_t, size_t> dimensions;
    double determinant; ///real value or nan

    ///count actual value of determinant
    double countDeterminant() const;

    /**
     * @brief Creates submatrix withou row&column at skipIndex
     * 
     * @param matrix to make submatrix from
     * @param skipIndex index to omit at row, column
     * @return std::unique_ptr<Matrix> submatrix
     */
    std::unique_ptr<Matrix> omitIndex(const std::unique_ptr<Matrix> &matrix, const size_t &skipIndex) const;

public:
    /**
     * @brief Construct a new Matrix object
     * Initialize common variables for subclasses
     * 
     * @param rows of matrix
     * @param columns of matrix
     */
    Matrix(const size_t &rows, const size_t &columns) {
        dimensions.first = rows;
        dimensions.second = columns;
        determinant = nan("");
    }

    ///CRTP copy 
    virtual std::unique_ptr<Matrix> clone() const = 0;

    /**
     * @brief Split Matrix Into Two by selected row
     * 
     * @param splitRow row to make split at
     * @return std::pair<std::unique_ptr<Matrix>, std::unique_ptr<Matrix>> splited matrix in pair
     */
    std::pair<std::unique_ptr<Matrix>, std::unique_ptr<Matrix>> splitByRow(const int &splitRow) const;

    /**
     * @brief Split Matrix into two by selected column
     * 
     * @param splitColumn column to make split at
     * @return std::pair<std::unique_ptr<Matrix>, std::unique_ptr<Matrix>> splited matrix in pair
     */
    std::pair<std::unique_ptr<Matrix>, std::unique_ptr<Matrix>> splitByColumn(const int &splitColumn) const;

    /**
     * @brief Merge to Matrixes under each other
     * 
     * @param lower matrix to be under the caller
     * @return std::unique_ptr<Matrix> created matrix
     */
    std::unique_ptr<Matrix> mergeByRow(const std::unique_ptr<Matrix> &lower) const;

    /**
     * @brief Merge to Matrixes nexto each other
     * 
     * @param right matrix to be on the right of the caller
     * @return std::unique_ptr<Matrix> created matrix
     */
    std::unique_ptr<Matrix> mergeByColumn(const std::unique_ptr<Matrix> &right) const;

    /**
     * @brief Do basic GEM on matrix
     * 
     * @param verbose if true will comment used steps
     * @return std::unique_ptr<Matrix> 
     */
    std::unique_ptr<Matrix> gem(bool verbose) const;

    /**
     * @brief Transpose matrix if possible
     * 
     * @return std::unique_ptr<Matrix> 
     */
    std::unique_ptr<Matrix> transpose() const;

    /**
     * @brief Inverse of matrix if exists
     * 
     * @return std::unique_ptr<Matrix> 
     */
    std::unique_ptr<Matrix> inverse() const;

    /**
     * @brief Get the Determinant object
     * 
     * @return double 
     */
    double getDeterminant() const;

    /**
     * @brief Get the Determinant object and stores it in local variable
     * 
     * @return double 
     */
    double getDeterminant();

    /**
     * @brief Count the Rank of Matrix
     * 
     * @return size_t 
     */
    size_t getRank() const;

    /**
     * @brief Get the Dimensions object
     * 
     * @return std::pair<size_t, size_t> dimensions
     */
    std::pair<size_t, size_t> getDimensions() const;

    /**
     * @brief Checks shape of matrix
     * 
     * @return true if matrix is square
     * @return false if matrix is not square
     */
    bool isSquare() const;

    /**
     * @brief Subclasses must return their representation type
     * 
     * @return std::string Dense/Sparse
     */
    virtual std::string denseOrSparse() const = 0;

    /**
     * @brief Virtual methode to get the element at row, column
     * 
     * @param row in matrix
     * @param column in matrix
     * @return double value of element
     */
    virtual double getAt(const size_t &row, const size_t &column) const = 0;

    /**
     * @brief Virtual methode to set the value at row, column
     * 
     * @param row in matrix
     * @param column in matrix
     * @param value to set
     * @return true operation was correct
     * @return false operation error
     */
    virtual bool setAt(const size_t &row, const size_t &column, const double &value) = 0;


    ///Overloaded arithmetic operators
    friend std::ostream &operator<<(std::ostream &out, const std::unique_ptr<Matrix> &m);

    friend std::unique_ptr<Matrix> operator+(const std::unique_ptr<Matrix> &l, const std::unique_ptr<Matrix> &r);

    friend std::unique_ptr<Matrix> operator-(const std::unique_ptr<Matrix> &l, const std::unique_ptr<Matrix> &r);

    friend std::unique_ptr<Matrix> operator*(const std::unique_ptr<Matrix> &l, const std::unique_ptr<Matrix> &r);

    ///Power
    friend std::unique_ptr<Matrix> operator^(const std::unique_ptr<Matrix> &l, const int &pow);

};

#endif //SRC_MATRIX_H
