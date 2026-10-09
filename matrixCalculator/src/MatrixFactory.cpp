//
// Created by marey on 6/12/2021.
//

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>

#include "MatrixFactory.h"
#include "SparseMatrix.h"
#include "DenseMatrix.h"

/**
 * @brief Read input from cin
 * Allow user to ener values divided by whitespaces
 * Will hang on input till all parameters are filled
 * After input will clean rest of the line
 * 
 * @return std::unique_ptr<Matrix> 
 */
std::unique_ptr<Matrix> MatrixFactory::createHuman() {
    size_t rows;
    size_t columns;
    int userInput;
    //! Read dimensions
    std::cout << "Enter Number Of Rows:" << std::endl;
    std::cin >> userInput;
    if ((std::cin.fail()) || (userInput < 1)) {
        std::cout << "Invalid Dimensions" << std::endl;
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return std::unique_ptr<Matrix>();
    }
    rows = userInput;
    std::cout << "Enter Number Of Columns:" << std::endl;
    std::cin >> userInput;
    if ((std::cin.fail()) || (userInput < 1)) {
        std::cout << "Invalid Dimensions" << std::endl;
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return std::unique_ptr<Matrix>();
    }
    //! Read elements of matrix
    columns = userInput;
    std::vector<double> result;
    std::cout << "Enter whitespace separated values" << std::endl;
    for (size_t row = 0; row < rows; row++) {
        for (size_t column = 0; column < columns; ++column) {
            double input;
            std::cin >> input;
            result.push_back(input);
        }
    }
    std::cout << std::endl;
    //! If failbits were set, input is discarded
    if (std::cin.fail()) {
        std::cout << "Invalid Input" << std::endl;
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return std::unique_ptr<Matrix>();
    }
    std::cin.clear();
    std::cin.ignore(10000, '\n');
    return MatrixFactory::createVector(rows, columns, result);
}

/**
 * @brief Creates matrix form vector
 * Decides if is better to represent matrix as Sparse or Dense
 * @param colms
 * @param rows
 * @return std::unique_ptr<Matrix> to created matrix or empty ptr
 */
std::unique_ptr<Matrix>
MatrixFactory::createVector(const size_t &rows, const size_t &columns, const std::vector<double> &data) {
    if (rows * columns != data.size()) {
        std::cout << "[MatrixController]: Incoherent data, No Matrix Created" << std::endl;
        return std::unique_ptr<Matrix>();
    }
    size_t numOfZeroes = std::count(data.begin(), data.end(), 0.0);
    if (numOfZeroes >= rows * columns / 2) {
        return std::make_unique<SparseMatrix>(SparseMatrix(rows, columns, data));
    } else {
        return std::make_unique<DenseMatrix>(DenseMatrix(rows, columns, data));
    }
}


std::unique_ptr<Matrix> MatrixFactory::createIdentity(const size_t &diagonal) {
    return std::make_unique<SparseMatrix>(SparseMatrix(diagonal));
}

/**
 * @brief Load used text format file
 * rows columns
 * E1,E2,...
 * ...
 * @param path path for the file
 * @return std::unique_ptr<Matrix> to created matrix or empty ptr
 */
std::unique_ptr<Matrix> MatrixFactory::loadNice(const std::string &path) {
    std::ifstream iFile(path, std::ios::in);
    if (!iFile.is_open()) {
        std::cout << "[MatrixController]Cannot Open Text Input File" << std::endl;
        return std::unique_ptr<Matrix>();
    }
    std::string commandLine;
    getline(iFile, commandLine);
    std::vector<std::string> words;
    std::istringstream iss(commandLine);
    std::string word;
    //! Read size of matrix on the first line
    while (iss >> word) {
        words.push_back(word);
    }
    if (words.size() != 2) {
        std::cout << "[Load Text]File In Wrong Format" << std::endl;
        std::cout << "rows columns \n0 1\n3,4,..." << std::endl;
        iFile.close();
        return std::unique_ptr<Matrix>();
    }
    //! Convert string to int and check value
    int rows = stoi(words[0]);
    int columns = stoi(words[1]);
    if ((rows < 1) || (columns < 1)) {
        std::cout << "[Load Text]Invalid dimensions" << std::endl;
        iFile.close();
        return std::unique_ptr<Matrix>();
    }

    word.clear();
    std::vector<double> result;
    double scanNumber;
    /*!
      Iterate over file in matrix shape manner by rows, columns
      Shape of data in file must correspond to matrix type with [row,column] size
    */
    for (int row = 0; row < rows; ++row)
    {
        //! No more lines but matrix is not complete
        if (!getline(iFile, commandLine)) {
            std::cout << "[Load Text]Invalid data. Missing Rows" << std::endl;
            iFile.close();
            return std::unique_ptr<Matrix>();
        }
        std::istringstream line(commandLine);
        for (int column = 0; column < columns; ++column)
        {
            //! end of file
            if (line.eof()) {
                break;
            }
            if (!(line >> scanNumber)) {
                std::cout << "[Load Text]Invalid Data" << std::endl;
                iFile.close();
                return std::unique_ptr<Matrix>();
            }
            result.push_back(scanNumber);
            //! skip, 
            if (line.peek() == ',')
            {
                //! end of line but not filled all columns
                if(column == columns-1) {
                    std::cout << "[Load Text]Invalid Data. Excessive Column" << std::endl;
                    iFile.close();
                    return std::unique_ptr<Matrix>();
                //! skip dividing comma
                } else{
                    line.ignore();
                }
            }
        }

    }
    //! Error if some data are still left in file
    if(iFile.peek() != EOF)
    {
        std::cout << "[Load Text]Invalid data. Excessive Rows" << std::endl;
        iFile.close();
        return std::unique_ptr<Matrix>();
    }
    //! Loaded data are incoherient (theoretically will never happen and other error will rise sooner, just for vector.push_back check)
    if(rows*columns != (int)result.size())
    {
        std::cout << "[Load Text]Data Are Not ROWS*COLUMNS Size" << std::endl;
        iFile.close();
        return std::unique_ptr<Matrix>();
    }
    iFile.close();
    return MatrixFactory::createVector(rows, columns, result);
}

/**
 * @brief load matrix from binary file
 * 
 * @param path path for the file
 * @return std::unique_ptr<Matrix> to created matrix or empty ptr
 */
std::unique_ptr<Matrix> MatrixFactory::loadBinary(const std::string &path) {
    std::ifstream iFile(path, std::ios::binary | std::ios::in);
    if (!iFile.is_open()) {
        std::cout << "[MatrixController]Cannot Open Binary Input File" << std::endl;
        return std::unique_ptr<Matrix>();
    }
    //! read dimensions
    size_t rows;
    size_t columns;
    iFile.read((char *) &rows, sizeof(size_t));
    iFile.read((char *) &columns, sizeof(size_t));
    std::vector<double> result;
    //! read data of size rows*columns
    for(size_t i = 0; i < rows *columns; ++i)
    {
        double readValue;
        iFile.read(reinterpret_cast<char*>(&readValue), sizeof(double));
        result.push_back(readValue);
        if(iFile.fail()){
            std::cout << "[Load Binary]Error Occured while reading" << std::endl;
            iFile.close();
            return std::unique_ptr<Matrix>();
        }
    }
    //! some data left
    if(iFile.peek() != EOF)
    {
        std::cout << "[Load Binary]Data Are Not ROWS*COLUMNS Size" << std::endl;
        iFile.close();
        return std::unique_ptr<Matrix>();
    }
    iFile.close();
    return MatrixFactory::createVector(rows, columns, result);
}

/**
 * @brief save matrix in nice text format
 * Save matrix in format to be loaded in human readable form
 * rows columns
 * D1,D2,...
 * ...
 * @param matrix to store
 * @param path for the file to write
 * @return true if data were succesfully writen 
 */
bool MatrixFactory::saveNice(const std::unique_ptr<Matrix> &matrix, const std::string &path) {
    std::ofstream ofile(path, std::ios::out);

    if (!ofile.is_open()) {
        std::cout << "[Save Text]Cannot Open Text Output File" << std::endl;
        return false;
    }
    size_t rows = matrix->getDimensions().first;
    size_t columns = matrix->getDimensions().second;
    //! Write header
    ofile << rows << " " << columns << std::endl;
    //! Write cs data
    for (size_t row = 0; row < rows; ++row) {
        for (size_t column = 0; column < columns; ++column) {
            ofile << matrix->getAt(row, column);
            if (column + 1 != columns) {
                ofile << ",";
            }
        }
        ofile << std::endl;
    }
    std::cout << "[Save Text]File Saved" << std::endl;
    ofile.close();
    return (!ofile.bad());
}

/**
 * @brief save matrix to binary file
 * 
 * @param matrix to store
 * @param path for the file to write
 * @return badbit of writing operations
 */
bool MatrixFactory::saveBinary(const std::unique_ptr<Matrix> &matrix, const std::string &path) {
    std::ofstream ofile(path, std::ios::binary | std::ios::out);

    if (!ofile.is_open()) {
        std::cout << "[Save Binary]Cannot Open Binary Output File" << std::endl;
        return false;
    }
    size_t rows = matrix->getDimensions().first;
    size_t columns = matrix->getDimensions().second;
    //! Write header
    ofile.write(reinterpret_cast<char*>(&rows), sizeof(size_t));
    ofile.write(reinterpret_cast<char*>(&columns), sizeof(size_t));
    if (ofile.bad()) {
        std::cout << "[Save Binary]Writing operation error" << std::endl;
        ofile.close();
        return false;
    }
    std::vector<double> dataToWrite;
    //! Write data in temporary vector
    for (size_t row = 0; row < rows; ++row) {
        for (size_t column = 0; column < columns; ++column) {
            dataToWrite.push_back(matrix->getAt(row,column));
        }
    }
    //! Write vector to file
    ofile.write(reinterpret_cast<char *>(&dataToWrite[0]), rows * columns * sizeof(double));
    std::cout << "[Save Binary]File Saved" << std::endl;
    ofile.close();

    return (!ofile.bad());
}
