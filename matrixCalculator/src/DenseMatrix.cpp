#include "DenseMatrix.h"
#include <iostream>


DenseMatrix::DenseMatrix(const size_t &rows, const size_t &columns, const std::vector<double> &data) : Matrix(rows,
                                                                                                              columns) {
    for (std::size_t item = 0; item < data.size(); ++item) {
        //add new row
        if (item % columns == 0) elements.resize(elements.size() + 1);
        //copy column
        elements[item / columns].push_back(data[item]);
    }
}

DenseMatrix::DenseMatrix(const size_t &rows, const size_t &columns) : Matrix(rows, columns) {
    elements.resize(rows, std::vector<double>(columns, 0));
}

std::string DenseMatrix::denseOrSparse() const {
    return "Dense";
}

double DenseMatrix::getAt(const size_t &row, const size_t &column) const {
    return elements.at(row).at(column);
}

std::unique_ptr<Matrix> DenseMatrix::clone() const {
    return std::make_unique<DenseMatrix>(*this);
}


bool DenseMatrix::setAt(const size_t &row, const size_t &column, const double &value) {
    try {
        elements.at(row).at(column) = value;
        determinant = nan("");
        return true;
    }
    catch (std::out_of_range &e) {
        return false;
    }
}


