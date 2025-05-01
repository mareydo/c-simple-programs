//
// Created by marey on 6/12/2021.
//

#ifndef MATRIXFACTORY_H
#define MATRIXFACTORY_H

#include <memory>
#include <vector>
#include "Matrix.h"

/**
 * @brief Factory to create Matrix
 * Used to allways create memory saving representation of Matrix
 * 
 * Read or store in file
 * Can read from stdin
 * File support binary and text form
 * Can create Identity matrix or matrix from vector
 * 
 */
class MatrixFactory {
private:
public:

    ///make matrix from vector values
    static std::unique_ptr<Matrix> createVector(const size_t &, const size_t &, const std::vector<double> &);

    ///make identity square matrix diagonalxdiagonal
    static std::unique_ptr<Matrix> createIdentity(const size_t &diagonal);

    ///load matrix from stdin (user)
    static std::unique_ptr<Matrix> createHuman();

    ///load matrix in text format
    static std::unique_ptr<Matrix> loadNice(const std::string &path);

    ///load matrix from binary file
    static std::unique_ptr<Matrix> loadBinary(const std::string &path);

    ///save matrix in nice text format
    static bool saveNice(const std::unique_ptr<Matrix> &, const std::string &);

    ///save matrix to binary file
    static bool saveBinary(const std::unique_ptr<Matrix> &, const std::string &);
};

#endif //MATRIXFACTORY_H
