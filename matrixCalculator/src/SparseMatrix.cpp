//
// Created by marey on 6/13/2021.
//

#include "SparseMatrix.h"
#include <iostream>

/**
 * Not storing zero data from vector
 * Goes trough 1D vector and stores them on dimensions by entered parameters
 */
SparseMatrix::SparseMatrix(const size_t &rows, const size_t &columns, const std::vector<double> &data) : Matrix(rows,
                                                                                                                columns) {
    size_t row = 0;
    size_t column = 0;
    for (auto &it : data) {
        if (row == rows) {
            std::cout << "[SparseMatrix]:Dimension Too Small For Data. Will Omit Rest" << std::endl;
            break;
        }
        if (it != 0) {
            elements[std::make_pair(row, column)] = it;
        }
        ++column;
        if (column == columns) {
            column = 0;
            ++row;
        }
    }
}

SparseMatrix::SparseMatrix(const size_t &rows, const size_t &columns) : Matrix(rows, columns) {}

//creates identity matrix
SparseMatrix::SparseMatrix(const size_t &diagonal) : Matrix(diagonal, diagonal) {
    for (size_t index = 0; index < diagonal; ++index) {
        elements[std::make_pair(index, index)] = 1;
    }
}

std::string SparseMatrix::denseOrSparse() const {
    return "Sparse";
}

/**
 * Return element at row,column from stored values or zero
 * Zeroes are not stored to save space
 */
double SparseMatrix::getAt(const size_t &row, const size_t &column) const {
    if ((row > dimensions.first) || (column > dimensions.second)) {
        throw std::out_of_range("Out of range");
    }
    std::map<std::pair<size_t, size_t>, double>::const_iterator it = elements.find(std::make_pair(row, column));
    if (it == elements.cend()) {
        return 0;
    }
    return it->second;
}

std::unique_ptr<Matrix> SparseMatrix::clone() const {
    return std::make_unique<SparseMatrix>(*this);
}


bool SparseMatrix::setAt(const size_t &row, const size_t &column, const double &value) {
    if ((row > dimensions.first) || (column > dimensions.second)) {
        return false;
    }
    ///if new values is zero, delete entery on that position
    if (value == 0.0) {
        elements.erase(std::make_pair(row, column));
    } else {
        elements[std::make_pair(row, column)] = value;
    }
    ///discard precounted determinant
    determinant = nan("");
    return true;
}