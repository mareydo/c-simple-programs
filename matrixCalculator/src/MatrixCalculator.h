//
// Created by marey on 6/12/2021.
//

#ifndef MATRIXCALCULATOR_H
#define MATRIXCALCULATOR_H

#include <string>
#include <map>
#include <utility>


#include "Matrix.h"
#include "CommandFactory.h"
#include "MatrixFactory.h"

class MatrixCalculator {
private:
    //typename std::map<std::string, std::unique_ptr<Matrix>>::iterator iterator;
    std::map<std::string, std::unique_ptr<Matrix>> matrixDB;
    CommandFactory commandFactory;
    //insert to database
    bool insertMatrix(const std::string &, std::unique_ptr<Matrix>);

    //-------------------------------------------------------------//
    //load from user
    bool loadHumanMatrix(const std::string &);

    //load from text file
    bool loadNiceMatrix(const std::string &, const std::string &);

    //load from binary file
    bool loadBinaryMatrix(const std::string &, const std::string &);

    //remove from local database
    bool removeMatrix(const std::string &);

    //rename get local database
    bool renameMatrix(const std::string &, const std::string &);

    //save to text file
    bool saveNiceMatrix(const std::string &, const std::string &) const;

    //save to binary file
    bool saveBinaryMatrix(const std::string &, const std::string &) const;

    //print one
    void printMatrix(const std::string &) const;

    void printMatrix(const std::map<std::string, std::unique_ptr<Matrix>>::const_iterator &) const;

    //print all
    void printAll() const;

    //summation
    bool sumMatrixes(const std::string &, const std::string &);

    //substract
    bool subMatrixes(const std::string &, const std::string &);

    //multiply
    bool multiplyMatrixes(const std::string &, const std::string &);

    //power
    bool powerMatrix(const std::string &, const int &);

    //split to two by row
    bool splitMatrixByRow(const std::string &, const int &);

    //split to two by column
    bool splitMatrixByColumn(const std::string &, const int &);

    //split to two by row
    bool mergeMatrixesByRow(const std::string &, const std::string &);

    //split to two by column
    bool mergeMatrixesByColumn(const std::string &, const std::string &);

    //GEM [non]verbal
    bool GEM(const std::string &, const bool &);

    //inversion
    bool inverseMatrix(const std::string &);

    //transposition
    bool transposeMatrix(const std::string &);

    //determinant
    bool getDeterminant(const std::string &) const;

    //rank
    bool getRank(const std::string &) const;

    //get dimensions
    bool getDimensions(const std::string &) const;

    //sqare
    bool isSquare(const std::string &) const;

    //is Dense or Sparse
    bool denseOrSparse(const std::string &);

    //get element on index
    bool getElement(const std::string &, const int &, const int &) const;

    //helpers
    void displayHelp() const;

    bool overwriteMessage(const std::string &) const;

    bool notFoundMessage(const std::string &) const;

public:
    int run();
};

#endif //MATRIXCALCULATOR_H
