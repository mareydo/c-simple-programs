#ifndef SPARSEMATRIX_H
#define SPARSEMATRIX_H

#include "Matrix.h"
#include <vector>
#include <map>

/**
 * @brief Concrete Implementation of Virtual Class Matrix
 * Sparse has less(or equal) than half elements non zero
 * @see Matrix
 */
class SparseMatrix : public Matrix {
private:
    /**
     * @brief Storing only non zero elemens
     * [row,column] = element
     */
    std::map<std::pair<size_t, size_t>, double> elements;
public:

    /**
     * @brief Construct a new Sparse Matrix object from vector
     * 
     * @param rows of matrix 
     * @param columns of matrix
     * @param data elements to insert
     */
    SparseMatrix(const size_t &rows, const size_t &columns, const std::vector<double> &data);

    /**
     * @brief Copy constructor
     * 
     * @param m1 
     */
    SparseMatrix(const SparseMatrix &m1) : Matrix(m1.dimensions.first, m1.dimensions.second), elements(m1.elements) {}

    /**
     * @brief Construct a new Sparse Matrix object
     * 
     * @param rows of matrix
     * @param columns of matrix
     */
    SparseMatrix(const size_t &rows, const size_t &columns);

    /**
     * @brief Construct a identity matrix
     * 
     * @param diagonal size of square matrix
     */
    explicit SparseMatrix(const size_t &diagonal);


    /**
     * @brief Makes a unique_ptr clone
     * Used for Polymorphic copy
     * 
     * @return std::unique_ptr<Matrix> 
     */
    std::unique_ptr<Matrix> clone() const override;

    /**
     * @brief Information for user about matrix representation
     * 
     * @return Sparse
     */
    std::string denseOrSparse() const override;

    /**
     * @brief Get element at row, column
     * 
     * @param row in matrix
     * @param column in matrix
     * @throw std::out_of_range if row or column is out of bounds
     * @return double value
     */
    double getAt(const size_t &row, const size_t &column) const override;

    /**
     * @brief Set the value at row, column
     * Discards determinant if counted
     * @param row in matrix
     * @param column in matrix
     * @param value to set
     * @return true if indexes are in matrix
     * @return false if row,column is out of bounds
     */
    bool setAt(const size_t &row, const size_t &column, const double &value) override;
};

#endif //SRC_SPARSEMATRIX_H
