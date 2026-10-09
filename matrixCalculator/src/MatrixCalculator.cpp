//
// Created by marey on 6/12/2021.
//
#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <vector>
#include <string>

#include "MatrixCalculator.h"

/**
 * @brief inserts matrix in database
 *
 * @param matrixName name in local database
 * 
 * @param matrix saved matrix in database
 *
 * @retval bool (false if matrix does not exist or emplace fails)
 */
bool MatrixCalculator::insertMatrix(const std::string &matrixName, std::unique_ptr<Matrix> matrix) {
    if (!matrix)
        return false;
    matrixDB[matrixName] = matrix->clone();
    return true;
}

/**
 * @brief load from user to local database
 *
 * @param matrixName name in local database
 *
 * @retval bool (true if matrix was loaded correctly)
 */
bool MatrixCalculator::loadHumanMatrix(const std::string &matrixName) {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator it = matrixDB.find(matrixName);
    if (it != matrixDB.cend()) {
        if (!overwriteMessage(matrixName)) {
            return false;
        }
    }
    bool retVal = insertMatrix(matrixName, MatrixFactory::createHuman());
    if (retVal) {
        printMatrix(matrixName);
    }
    return retVal;
}

/**
 * @brief load from text file to local database
 *
 * @param matrixName name in local database
 *
 * @param path path to the file to load from
 *
 * @retval bool (true if matrix was loaded correctly)
 */
bool MatrixCalculator::loadNiceMatrix(const std::string &matrixName, const std::string &path) {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator it = matrixDB.find(matrixName);
    if (it != matrixDB.cend()) {
        if (!overwriteMessage(matrixName)) {
            return false;
        }
    }
    bool retVal = insertMatrix(matrixName, MatrixFactory::loadNice(path));
    if (retVal) {
        printMatrix(matrixName);
    }
    return retVal;
}

/**
 * @brief load from binary file to local database
 *
 * @param matrixName name in local database
 *
 * @param path path to the file to load from
 *
 * @retval bool (true if matrix was loaded correctly)
 */
bool MatrixCalculator::loadBinaryMatrix(const std::string &matrixName, const std::string &path) {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator it = matrixDB.find(matrixName);
    if (it != matrixDB.cend()) {
        if (!overwriteMessage(matrixName)) {
            return false;
        }
    }
    bool retVal = insertMatrix(matrixName, MatrixFactory::loadBinary(path));
    if (retVal) {
        printMatrix(matrixName);
    }
    return retVal;
}

/**
 * @brief remove from local database
 *
 * @param matrixName name in local database
 *
 * @retval bool (true if matrix was removed correctly)
 */
bool MatrixCalculator::removeMatrix(const std::string &matrixName) {
    std::map<std::string, std::unique_ptr<Matrix>>::iterator it;
    it = matrixDB.find(matrixName);
    if (it == matrixDB.end()) {
        return notFoundMessage(matrixName);
    }
    matrixDB.erase(it);
    return true;
}

/**
 * @brief rename get local database
 *
 * @param matrixName name in local database
 * 
 * @param newMatrixName 
 *
 * @retval bool (true if matrix was renamed correctly)
 */
bool MatrixCalculator::renameMatrix(const std::string &matrixName, const std::string &newMatrixName) {
    std::map<std::string, std::unique_ptr<Matrix>>::iterator oldMatrixt;
    std::map<std::string, std::unique_ptr<Matrix>>::iterator newMatrix;
    oldMatrixt = matrixDB.find(matrixName);
    if (oldMatrixt == matrixDB.end()) {
        return notFoundMessage(matrixName);
    }
    newMatrix = matrixDB.find(newMatrixName);
    if (newMatrix == matrixDB.end()) {
        if (!overwriteMessage(matrixName)) {
            return false;
        }
    }
    //copy to new key
    std::swap(matrixDB[newMatrixName], oldMatrixt->second);
    //remove at old key
    return removeMatrix(matrixName);

}

/**
 * @brief save to text file
 *
 * @param matrixName name in local database
 * 
 * @param path path to the file to load from 
 *
 * @retval bool (true if text file was saved correctly)
 */
bool MatrixCalculator::saveNiceMatrix(const std::string &matrixName, const std::string &path) const {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator it = matrixDB.find(matrixName);
    if (it == matrixDB.cend()) {
        return notFoundMessage(matrixName);
    }
    return MatrixFactory::saveNice(it->second, path);

}

/**
 * @brief save to binary file
 *
 * @param matrixName name in local database
 * 
 * @param path path to the file to load from 
 *
 * @retval bool (true if binary file was saved correctly)
 */
bool MatrixCalculator::saveBinaryMatrix(const std::string &matrixName, const std::string &path) const {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator it = matrixDB.find(matrixName);
    if (it == matrixDB.cend()) {
        return notFoundMessage(matrixName);
    }
    return MatrixFactory::saveBinary(it->second, path);
}

/**
 * @brief print one matrix assigned by name
 * If matrix exists uses iterator version to print it
 * 
 * @param matrixName name in local database
 * 
 * @retval void
 */
void MatrixCalculator::printMatrix(const std::string &matrixName) const {
    const std::map<std::string, std::unique_ptr<Matrix>>::const_iterator it = matrixDB.find(matrixName);
    if (it == matrixDB.cend()) {
        notFoundMessage(matrixName);
        return;
    }
    printMatrix(it);
}

/**
 * @brief print one matrix assigned by iterator
 * 
 * @param it iterator in database
 */
void MatrixCalculator::printMatrix(const std::map<std::string, std::unique_ptr<Matrix>>::const_iterator &it) const {
    std::cout << "-------------" << std::endl;
    std::cout << it->first << std::endl;
    std::cout << it->second << std::endl;
    std::cout << "-------------" << std::endl;
}

/**
 * @brief print all matrixes in database
 *
 * @param void 
 *
 * @retval void
 */
void MatrixCalculator::printAll() const {
    std::cout << "ALL SAVED MATRIXES" << std::endl;
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator it;
    for (it = matrixDB.cbegin(); it != matrixDB.cend(); ++it) {
        printMatrix(it);
    }
}

/**
 * @brief  matrixName1+matrixName2 =matrixName1 + matrixName2 
 *
 * @param matrixName1 name in local database
 *
 * @param matrixName2 name in local database
 *
 * @retval bool (true if the operation is correct)
 */
bool MatrixCalculator::sumMatrixes(const std::string &matrixName1, const std::string &matrixName2) {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator m1 = matrixDB.find(matrixName1);
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator m2 = matrixDB.find(matrixName2);
    m1 = matrixDB.find(matrixName1);
    if (m1 == matrixDB.cend()) {
        return notFoundMessage(matrixName1);
    }
    if (m2 == matrixDB.cend()) {
        return notFoundMessage(matrixName2);
    }
    std::string resultName = matrixName1 + "+" + matrixName2;
    bool retVal = insertMatrix(resultName, m1->second + m2->second);
    if (retVal) {
        printMatrix(resultName);
    }
    return retVal;
}

/**
 * @brief  matrixName1-matrixName2 =matrixName1 + matrixName2 
 *
 * @param matrixName1 name in local database
 *
 * @param matrixName2 name in local database
 *
 * @retval bool (true if the operation is correct)
 */
bool MatrixCalculator::subMatrixes(const std::string &matrixName1, const std::string &matrixName2) {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator m1 = matrixDB.find(matrixName1);
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator m2 = matrixDB.find(matrixName2);
    m1 = matrixDB.find(matrixName1);
    if (m1 == matrixDB.cend()) {
        return notFoundMessage(matrixName1);
    }
    if (m2 == matrixDB.cend()) {
        return notFoundMessage(matrixName2);
    }
    std::string resultName = matrixName1 + "-" + matrixName2;
    bool retVal = insertMatrix(resultName, m1->second - m2->second);
    if (retVal) {
        printMatrix(resultName);
    }
    return retVal;
}

/**
 * @brief  matrixName1*matrixName2 =matrixName1 + matrixName2 
 *
 * @param matrixName1 name in local database
 *
 * @param matrixName2 name in local database
 *
 * @retval bool (true if the operation is correct)
 */
bool MatrixCalculator::multiplyMatrixes(const std::string &matrixName1, const std::string &matrixName2) {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator m1 = matrixDB.find(matrixName1);
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator m2 = matrixDB.find(matrixName2);
    m1 = matrixDB.find(matrixName1);
    if (m1 == matrixDB.cend()) {
        return notFoundMessage(matrixName1);
    }
    if (m2 == matrixDB.cend()) {
        return notFoundMessage(matrixName2);
    }
    std::string resultName = matrixName1 + "*" + matrixName2;
    bool retVal = insertMatrix(resultName, m1->second * m2->second);
    if (retVal) {
        printMatrix(resultName);
    }
    return retVal;
}

/**
 * @brief  matrixName^power = power of matrixName
 *
 * @param matrixName name in local database
 *
 * @param power of the final matrix
 *
 * @retval bool (true if the operation is correct)
 */
bool MatrixCalculator::powerMatrix(const std::string &matrixName, const int &power) {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator it = matrixDB.find(matrixName);
    if (it == matrixDB.cend()) {
        return notFoundMessage(matrixName);
    }
    std::string resultName = matrixName + "^" + std::to_string(power);
    bool retVal = insertMatrix(resultName, it->second ^ power);
    if (retVal) {
        printMatrix(resultName);
    }
    return retVal;
}

/**
 * @brief split to two by row: matrixNameS1 , matrixNameS2
 *
 * @param matrixName name in local database
 *
 * @param row 
 *
 * @retval bool (true if the operation is correct)
 */
bool MatrixCalculator::splitMatrixByRow(const std::string &matrixName, const int &row) {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator it = matrixDB.find(matrixName);
    if (it == matrixDB.cend()) {
        return notFoundMessage(matrixName);
    }
    std::string resultName1 = matrixName + "S1";
    std::string resultName2 = matrixName + "S2";
    std::pair<std::unique_ptr<Matrix>, std::unique_ptr<Matrix>> res = it->second->splitByRow(row);
    bool retVal = (insertMatrix(resultName1, move(res.first)) && insertMatrix(resultName2, move(res.second)));
    if (retVal) {
        printMatrix(resultName1);
        printMatrix(resultName2);
    }
    return retVal;
}

/**
 * @brief split to two by column: matrixNameS1 , matrixNameS2
 *
 * @param matrixName name in local database
 *
 * @param column 
 *
 * @retval bool (true if the operation is correct)
 */
bool MatrixCalculator::splitMatrixByColumn(const std::string &matrixName, const int &column) {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator it = matrixDB.find(matrixName);
    if (it == matrixDB.cend()) {
        return notFoundMessage(matrixName);
    }
    std::string resultName1 = matrixName + "S1";
    std::string resultName2 = matrixName + "S2";
    std::pair<std::unique_ptr<Matrix>, std::unique_ptr<Matrix>> res = it->second->splitByColumn(column);
    bool retVal = (insertMatrix(resultName1, move(res.first)) && insertMatrix(resultName2, move(res.second)));
    if (retVal) {
        printMatrix(resultName1);
        printMatrix(resultName2);
    }
    return retVal;
}

/**
 * @brief merge matrixes by row to matrixName1MatrixName2ROW
 *
 * @param matrixName1 name in local database
 *
 * @param matrixName2 name in local database
 *
 * @retval bool (true if the operation is correct)
 */
bool MatrixCalculator::mergeMatrixesByRow(const std::string &matrixName1, const std::string &matrixName2) {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator m1 = matrixDB.find(matrixName1);
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator m2 = matrixDB.find(matrixName2);
    m1 = matrixDB.find(matrixName1);
    if (m1 == matrixDB.cend()) {
        return notFoundMessage(matrixName1);
    }
    if (m2 == matrixDB.cend()) {
        return notFoundMessage(matrixName2);
    }
    std::string resultName = matrixName1 + matrixName2 + "ROW";
    bool retVal = (insertMatrix(resultName, m1->second->mergeByRow(m2->second)));
    if (retVal) {
        printMatrix(resultName);
    }
    return retVal;
}

/**
 * @brief merge matrixes by column to matrixName1MatrixName2COL
 *
 * @param matrixName1 name in local database
 *
 * @param matrixName2 name in local database
 *
 * @retval bool (true if the operation is correct)
 */
bool MatrixCalculator::mergeMatrixesByColumn(const std::string &matrixName1, const std::string &matrixName2) {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator m1 = matrixDB.find(matrixName1);
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator m2 = matrixDB.find(matrixName2);
    m1 = matrixDB.find(matrixName1);
    if (m1 == matrixDB.cend()) {
        return notFoundMessage(matrixName1);
    }
    if (m2 == matrixDB.cend()) {
        return notFoundMessage(matrixName2);
    }
    std::string resultName = matrixName1 + matrixName2 + "COL";
    bool retVal = (insertMatrix(resultName, m1->second->mergeByColumn(m2->second)));
    if (retVal) {
        printMatrix(resultName);
    }
    return retVal;
}

/**
 * @brief GEM [non]verbal
 *
 * @param matrixName name in local database
 *
 * @param verbose if true will print information about steps
 *
 * @retval bool (true if the operation is correct)
 */
bool MatrixCalculator::GEM(const std::string &matrixName, const bool &verbose) {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator it = matrixDB.find(matrixName);
    if (it == matrixDB.cend()) {
        return notFoundMessage(matrixName);
    }
    std::string resultName = matrixName + "GEM";
    bool retVal = insertMatrix(resultName, it->second->gem(verbose));
    if (retVal) {
        printMatrix(resultName);
    }
    return retVal;
}

/**
 * @brief inversion matrixNameINV
 *
 * @param matrixName name in local database
 *
 * @retval bool (true if the operation is correct)
 */
bool MatrixCalculator::inverseMatrix(const std::string &matrixName) {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator it = matrixDB.find(matrixName);
    if (it == matrixDB.cend()) {
        return notFoundMessage(matrixName);
    }
    std::string resultName = matrixName + "INV";
    bool retVal = insertMatrix(resultName, it->second->inverse());
    if (retVal) {
        printMatrix(resultName);
    }
    return retVal;
}

/**
 * @brief transposition matrixNameTRANS
 *
 * @param matrixName name in local database
 *
 * @retval bool (true if the operation is correct)
 */
bool MatrixCalculator::transposeMatrix(const std::string &matrixName) {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator it = matrixDB.find(matrixName);
    if (it == matrixDB.cend()) {
        return notFoundMessage(matrixName);
    }
    std::string resultName = matrixName + "TRANS";
    bool retVal = insertMatrix(resultName, it->second->transpose());
    if (retVal) {
        printMatrix(resultName);
    }
    return retVal;
}

/**
 * @brief determinant
 * Values of  DET: value / Matrix Doesnt Have Determinant
 * @param matrixName name in local database
 *
 * @retval bool (true if matrix exist)
 */
bool MatrixCalculator::getDeterminant(const std::string &matrixName) const {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator it = matrixDB.find(matrixName);
    if (it == matrixDB.cend()) {
        return notFoundMessage(matrixName);
    }
    std::cout << "DET: ";
    double det = it->second->getDeterminant();
    if (std::isnan(det)) {
        std::cout << "Matrix Doesnt Have Determinant" << std::endl;
    } else {
        std::cout << det << std::endl;
    }
    return true;
}

/**
 * @brief rank of the matrix 
 *
 * @param matrixName name in local database
 *
 * @retval bool (true if matrix exists)
 */
bool MatrixCalculator::getRank(const std::string &matrixName) const {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator it = matrixDB.find(matrixName);
    if (it == matrixDB.cend()) {
        return notFoundMessage(matrixName);
    }
    std::cout << "RANK: " << it->second->getRank() << std::endl;
    return true;
}

/**
 * @brief Print dimension on the screen
 * DIM: value
 * @param matrixName name in local database
 *
 * @retval bool (true if matrix exists)
 */
bool MatrixCalculator::getDimensions(const std::string &matrixName) const {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator it = matrixDB.find(matrixName);
    if (it == matrixDB.cend()) {
        return notFoundMessage(matrixName);
    }
    std::pair<size_t, size_t> dimensions = it->second->getDimensions();
    std::cout << "DIM: " << dimensions.first << "x" << dimensions.second << std::endl;
    return true;
}

/**
 * @brief Print true or false on the screen
 * 
 * @param matrixName name in local database
 *
 * @retval bool (true if matrix exists)
 */
bool MatrixCalculator::isSquare(const std::string &matrixName) const {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator it = matrixDB.find(matrixName);
    if (it == matrixDB.cend()) {
        return notFoundMessage(matrixName);
    }
    std::cout << std::boolalpha << "SQUARE?: " << it->second->isSquare() << std::endl;
    return true;
}

/**
 * @brief Prints either Dense/Sparse by the type of stored matrix
 *  
 * @param matrixName name in local database
 *
 * @retval bool (true if matrix exists)
 */
bool MatrixCalculator::denseOrSparse(const std::string &matrixName) {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator it = matrixDB.find(matrixName);
    if (it == matrixDB.cend()) {
        return notFoundMessage(matrixName);
    }
    std::cout << it->second->denseOrSparse() << std::endl;
    return true;
}

/**
 * @brief get element on index
 *
 * @param matrixName name in local database
 *
 * @param row
 *
 * @param column
 *
 * @retval bool (true if matrix exists and if indexes are correct)
 */
bool MatrixCalculator::getElement(const std::string &matrixName, const int &row, const int &column) const {
    std::map<std::string, std::unique_ptr<Matrix>>::const_iterator it = matrixDB.find(matrixName);
    if (it == matrixDB.cend()) {
        return notFoundMessage(matrixName);
    }
    std::cout << "ELEMENT AT[" << row << "][" << column << "]: ";
    try {
        std::cout << it->second->getAt(row, column) << std::endl;
    }
    catch (std::out_of_range &e) {
        std::cout << "Does not exist. Indexes out of range" << std::endl;
        return false;
    }
    return true;
}

/**
 * @brief Simple menu print
 * 
 */
void MatrixCalculator::displayHelp() const {
    std::cout << "--MATRIX CALCULATOR--" << std::endl;
    std::cout << "! WILL OVERWRITE matrixName IF EXISTS !" << std::endl;
    std::cout << "Options at selected commands are mandatory" << std::endl;
    std::cout << "usage: CMD: COMMAND [option] [arg1 [arg2 [...]]]" << std::endl;
    std::cout << "COMMAND |  option | argument | description" << std::endl;
    std::cout << "EXIT      |--|--| Exits program" << std::endl;
    std::cout << "HELP      |--|--| Display this page" << std::endl;
    std::cout << "LOAD      | -n nice text file, -b binary file | matrixName path | Load matrix from file at path"
              << std::endl;
    std::cout << "ENTER     |--| matrixName | Load matrixName from user input" << std::endl;
    std::cout << "REMOVE    |--| matrixName | Removes matrix from memory" << std::endl;
    std::cout << "RENAME    |--| matrixName newMatrixName | Renames matrix in memory" << std::endl;
    std::cout << "SAVE      | -n nice text file, -b binary file | matrixName path | Save matrix to file at path"
              << std::endl;
    std::cout << "SUM       |--| matrixName1 matrixName2 | matrixName1 + matrixName2 = matrixName1+matrixName2"
              << std::endl;
    std::cout << "SUB       |--| matrixName1 matrixName2 | matrixName1 - matrixName2 = matrixName1-matrixName2"
              << std::endl;
    std::cout << "MULT      |--| matrixName1 matrixName2 | matrixName1 * matrixName2 = matrixName1*matrixName2"
              << std::endl;
    std::cout << "POW       |--| matrixName power | matrixName^power = matrixName^Power" << std::endl;
    std::cout
            << "SPLIT     | -r row -c column | matrixName splitIndex | Split matrix by row or column to matrixNameS1 matrixNameS2"
            << std::endl;
    std::cout
            << "MERGE     | -r row -c column | matrixName1 matrixName2 | Merge matrixName2 under or on the right side of matrixName1 with suffix COL/ROW"
            << std::endl;
    std::cout
            << "GEM       | -v verbose -n nonverbal | matrixName | Do Gaussian elimination and save result to matrixNameGEM"
            << std::endl;
    std::cout << "INV       |--| matrixName | Create inverse matrix matrixNameINV if possible" << std::endl;
    std::cout << "TRANS     |--| matrixName | Create transposed matrix matrixNameTRANS" << std::endl;
    std::cout << "DET       |--| matrixName | Count determinant " << std::endl;
    std::cout << "RANK      |--| matrixName | Count rank of matrix" << std::endl;
    std::cout << "DIM       |--| matrixName | Display <rows,columns> of matrixName" << std::endl;
    std::cout << "SQ        |--| matrixName | Display if matrix is square or not (nxn)" << std::endl;
    std::cout << "DS        |--| matrixName | Display if matrix is Dense or Sparse" << std::endl;
    std::cout << "ELEM      |--| matrixName rowIndex columnIndex | Display element matrixName[rowIndex][columnIndex]"
              << std::endl;
    std::cout << "PRINT     |--| matrixName | Print matrixName on screen" << std::endl;
    std::cout << "PRINTALL  |--|--| Print all matrixes on screen" << std::endl;
    std::cout
            << "------------------------------------------------------------------------------------------------------"
            << std::endl;
}

/**
 * @brief will warn user of matrix name conflict
 *
 * @param matrixName name in local database
 *
 * @retval bool (answer from user to overwrite existing matrix)
 */
bool MatrixCalculator::overwriteMessage(const std::string &matrixName) const {
    std::cout << "Overwriting: " << matrixName << " Continue y/n ?" << std::endl;
    char decision;
    std::cin >> decision;
    if ((std::cin.fail()) ||
        ((decision != 'y') && (decision != 'Y') && (decision != 'n') && (decision != 'N'))) {
        std::cout << "Invalid Input" << std::endl;
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return false;
    }
    std::cin.clear();
    std::cin.ignore(10000, '\n');
    return (decision == 'y' || decision == 'Y');
}

/**
 * @brief warning message when matrixName in argument was not found
 *
 * @param matrixName name in local database
 *
 * @retval always false
 */
bool MatrixCalculator::notFoundMessage(const std::string &matrixName) const {
    std::cout << "Matrix: " << matrixName << " Not Found" << std::endl;
    return false;
}

/**
 * @brief main loop for user input
 *
 * @param void
 *
 * @retval 0 (#TODO add more error codes)
 */
int MatrixCalculator::run() {
    while (true) {
        std::cout << "CMD: " << std::endl;
        std::string commandLine;
        getline(std::cin, commandLine);

        std::vector<std::string> words;
        std::istringstream iss(commandLine);
        std::string word;
        while (iss >> word) {
            words.push_back(word);
        }
        if (words.empty()) {
            std::cout << "Enter command or type HELP for help" << std::endl;
            continue;
        }
        switch (commandFactory.getCommand(words[0])) {
            case ECommand::EXIT:
                std::cout << "----------EXIT----------" << std::endl;
                return 0;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::HELP:
                displayHelp();
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::LOAD: {
                std::cout << "----------LOAD----------" << std::endl;
                if (words.size() < 3 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout << "Options get selected commands are mandatory" << std::endl;
                    std::cout
                            << "LOAD | -n nice text file, -b binary file | matrixName path | Load matrix from file get path"
                            << std::endl;
                } else {
                    if (words[1] == "-n") {
                        if (!loadNiceMatrix(words[2], words[3])) {
                            std::cout << "Operation ended with error" << std::endl;
                        }
                    } else if (words[1] == "-b") {
                        if (!loadBinaryMatrix(words[2], words[3])) {
                            std::cout << "Operation ended with error" << std::endl;
                        }
                    } else {
                        std::cout << "Usage:" << std::endl;
                        std::cout << "Options get selected commands are mandatory" << std::endl;
                        std::cout
                                << "LOAD | -n nice text file, -b binary file | matrixName path | Load matrix from file get path"
                                << std::endl;
                    }
                }
                std::cout << "----------LOAD END----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::ENTER: {
                std::cout << "----------ENTER----------" << std::endl;
                if (words.size() < 1 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout << "ENTER |--| matrixName | Load matrixName from user input" << std::endl;
                } else {
                    if (!loadHumanMatrix(words[1])) {
                        std::cout << "Operation ended with error" << std::endl;
                    }
                }
                std::cout << "----------ENTER END----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::REMOVE: {
                std::cout << "----------REMOVE----------" << std::endl;
                if (words.size() < 1 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout << "REMOVE |--| matrixName | Removes matrix from memory" << std::endl;
                } else {
                    if (!removeMatrix(words[1])) {
                        std::cout << "Operation ended with error" << std::endl;
                    }
                }
                std::cout << "----------REMOVE END----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::RENAME: {
                std::cout << "----------RENAME----------" << std::endl;
                if (words.size() < 2 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout << "Options get selected commands are mandatory" << std::endl;
                    std::cout << "RENAME |--| matrixName newMatrixName | Renames matrix in memory" << std::endl;
                } else if (!renameMatrix(words[1], words[2])) {
                    std::cout << "Operation ended with error" << std::endl;
                }
                std::cout << "----------RENAME END----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::SAVE: {
                std::cout << "----------SAVE----------" << std::endl;
                if (words.size() < 3 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout << "Options get selected commands are mandatory" << std::endl;
                    std::cout
                            << "SAVE | -n nice text file, -b binary file | matrixName path | Save matrix to file get path"
                            << std::endl;
                } else {
                    if (words[1] == "-n") {
                        if (!saveNiceMatrix(words[2], words[3])) {
                            std::cout << "Operation ended with error" << std::endl;
                        }
                    } else if (words[1] == "-b") {
                        if (!saveBinaryMatrix(words[2], words[3])) {
                            std::cout << "Operation ended with error" << std::endl;
                        }
                    } else {
                        std::cout << "Usage:" << std::endl;
                        std::cout << "Options get selected commands are mandatory" << std::endl;
                        std::cout
                                << "SAVE | -n nice text file, -b binary file | matrixName path | Save matrix to file get path"
                                << std::endl;
                    }
                }
                std::cout << "----------SAVE END----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::SUM: {
                std::cout << "----------SUM BEGIN----------" << std::endl;
                if (words.size() < 2 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout
                            << "SUM |--| matrixName1 matrixName2 | matrixName1 + matrixName2 = matrixName1+matrixName2"
                            << std::endl;
                } else {
                    if (!sumMatrixes(words[1], words[2])) {
                        std::cout << "Operation ended with error" << std::endl;
                    }
                }
                std::cout << "----------SUM END----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::SUB: {
                std::cout << "----------SUB BEGIN----------" << std::endl;
                if (words.size() < 2 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout
                            << "SUB |--| matrixName1 matrixName2 | matrixName1 - matrixName2 = matrixName1-matrixName2"
                            << std::endl;
                } else {
                    if (!subMatrixes(words[1], words[2])) {
                        std::cout << "Operation ended with error" << std::endl;
                    }
                }
                std::cout << "-----------SUB END-----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::MULT: {
                std::cout << "----------MULT BEGIN----------" << std::endl;
                if (words.size() < 2 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout
                            << "MULT |--| matrixName1 matrixName2 | matrixName1 * matrixName2 = matrixName1*matrixName2"
                            << std::endl;
                } else {
                    if (!multiplyMatrixes(words[1], words[2])) {
                        std::cout << "Operation ended with error" << std::endl;
                    }
                }
                std::cout << "-----------MULT END-----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::POW: {
                std::cout << "----------POW BEGIN----------" << std::endl;
                if (words.size() < 2 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout << "POW |--| matrixName power | matrixName^power = matrixName^Power" << std::endl;
                } else {
                    int power;
                    try {
                        power = stoi(words[2]);
                    }
                    catch (const std::invalid_argument& ia)
                    {
                        std::cout << "Invalid power" << std::endl;
                        return false;
                    }
                    if (!powerMatrix(words[1], power)) {
                        std::cout << "Operation ended with error" << std::endl;
                    }
                }
                std::cout << "-----------POW END-----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::SPLIT: {
                std::cout << "----------SPLIT----------" << std::endl;
                if (words.size() < 3 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout << "Options get selected commands are mandatory" << std::endl;
                    std::cout
                            << "SPLIT | -r row -c column | matrixName splitIndex | Split matrix by row or column to matrixNameS1 matrixNameS2"
                            << std::endl;
                } else {

                    int index;
                    try {
                        index = stoi(words[3]);
                    }
                    catch (const std::invalid_argument& ia)
                    {
                        std::cout << "Invalid Index" << std::endl;
                        return false;
                    }
                    if (words[1] == "-r") {
                        if (!splitMatrixByRow(words[2], index)) {
                            std::cout << "Operation ended with error" << std::endl;
                        }
                    } else if (words[1] == "-c") {
                        if (!splitMatrixByColumn(words[2], index)) {
                            std::cout << "Operation ended with error" << std::endl;
                        }
                    } else {
                        std::cout << "Usage:" << std::endl;
                        std::cout << "Options get selected commands are mandatory" << std::endl;
                        std::cout
                                << "SPLIT | -r row -c column | matrixName splitIndex | Split matrix by row or column to matrixNameS1 matrixNameS2"
                                << std::endl;
                    }
                }
                std::cout << "----------SPLIT END----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::MERGE: {
                std::cout << "----------MERGE BEGIN----------" << std::endl;
                if (words.size() < 3 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout << "Options get selected commands are mandatory" << std::endl;
                    std::cout
                            << "MERGE | -r row -c column | matrixName1 matrixName2 | Merge matrixName2 under or on the right side of matrixName1 with suffix COL/ROW"
                            << std::endl;
                } else {
                    if (words[1] == "-r") {
                        if (!mergeMatrixesByRow(words[2], words[3])) {
                            std::cout << "Operation ended with error" << std::endl;
                        }
                    } else if (words[1] == "-c") {
                        if (!mergeMatrixesByColumn(words[2], words[3])) {
                            std::cout << "Operation ended with error" << std::endl;
                        }
                    } else {
                        std::cout << "Usage:" << std::endl;
                        std::cout << "Options get selected commands are mandatory" << std::endl;
                        std::cout
                                << "MERGE | -r row -c column | matrixName1 matrixName2 | Merge matrixName2 under or on the right side of matrixName1 with suffix COL/ROW"
                                << std::endl;
                    }
                    std::cout << "-----------MERGE END-----------" << std::endl;
                }
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::GEM: {
                std::cout << "----------GEM----------" << std::endl;
                if (words.size() < 2 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout << "Options get selected commands are mandatory" << std::endl;
                    std::cout
                            << "GEM  | -v verbose -n nonverbal| matrixName | Do Gaussian elimination and save result to matrixNameGEM"
                            << std::endl;
                } else {
                    if (words[1] == "-n") {
                        if (!GEM(words[2], false)) {
                            std::cout << "Operation ended with error" << std::endl;
                        }
                    } else if (words[1] == "-v") {
                        if (!GEM(words[2], true)) {
                            std::cout << "Operation ended with error" << std::endl;
                        }
                    } else {
                        std::cout << "Usage:" << std::endl;
                        std::cout << "Options get selected commands are mandatory" << std::endl;
                        std::cout
                                << "GEM | -v verbose -n nonverbal| matrixName | Do Gaussian elimination and save result to matrixNameGEM"
                                << std::endl;
                    }
                }
                std::cout << "----------GEM END----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::INV: {
                std::cout << "----------INV BEGIN----------" << std::endl;
                if (words.size() < 1 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout << "INV |--| matrixName | Create inverse matrix matrixNameINV if possible"
                              << std::endl;
                } else {
                    if (!inverseMatrix(words[1])) {
                        std::cout << "Operation ended with error" << std::endl;
                    }
                }
                std::cout << "-----------INV END-----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::TRANS: {
                std::cout << "----------TRANS BEGIN----------" << std::endl;
                if (words.size() < 1 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout << "TRANS |--| matrixName | Create transposed matrix matrixNameTRANS" << std::endl;
                } else {
                    if (!transposeMatrix(words[1])) {
                        std::cout << "Operation ended with error" << std::endl;
                    }
                }
                std::cout << "-----------TRANS END-----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::DET: {
                std::cout << "----------DET BEGIN----------" << std::endl;
                if (words.size() < 1 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout << "DET |--| matrixName | Count determinant " << std::endl;
                } else {
                    if (!getDeterminant(words[1])) {
                        std::cout << "Operation ended with error" << std::endl;
                    }
                }
                std::cout << "-----------DET END-----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::RANK: {
                std::cout << "----------RANK BEGIN----------" << std::endl;
                if (words.size() < 1 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout << "RANK |--| matrixName | Count rank of matrix" << std::endl;
                } else {
                    if (!getRank(words[1])) {
                        std::cout << "Operation ended with error" << std::endl;
                    }
                }
                std::cout << "-----------RANK END-----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::DIM: {
                std::cout << "----------DIM BEGIN----------" << std::endl;
                if (words.size() < 1 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout << "DIM |--| matrixName | Display <rows,columns> of matrixName" << std::endl;
                } else {
                    if (!getDimensions(words[1])) {
                        std::cout << "Operation ended with error" << std::endl;
                    }
                }
                std::cout << "-----------DIM END-----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::SQ: {
                std::cout << "----------SQ BEGIN----------" << std::endl;
                if (words.size() < 1 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout << "SQ |--| matrixName | Display if matrix is square or not (nxn)" << std::endl;
                } else {
                    if (!isSquare(words[1])) {
                        std::cout << "Operation ended with error" << std::endl;
                    }
                }
                std::cout << "-----------SQ END-----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::DS: {
                std::cout << "----------DS BEGIN----------" << std::endl;
                if (words.size() < 1 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout << "DS |--| matrixName | Display if matrix is Dense or Sparse" << std::endl;
                } else {
                    if (!denseOrSparse(words[1])) {
                        std::cout << "Operation ended with error" << std::endl;
                    }
                }
                std::cout << "-----------DS END-----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::ELEM: {
                std::cout << "----------ELEM BEGIN----------" << std::endl;
                if (words.size() < 3 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout
                            << "ELEM |--| matrixName rowIndex columnIndex | Display element matrixName[rowIndex][columnIndex]"
                            << std::endl;
                } else {
                    int row;
                    int column;
                    try {
                        row = stoi(words[2]);
                        column = stoi(words[3]);
                    }
                    catch (const std::invalid_argument& ia)
                    {
                        std::cout << "Invalid Index" << std::endl;
                        return false;
                    }
                    if (!getElement(words[1], row, column)) {
                        std::cout << "Operation ended with error" << std::endl;
                    }
                }
                std::cout << "-----------ELEM END-----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::PRINT: {
                std::cout << "----------PRINT BEGIN----------" << std::endl;
                if (words.size() < 1 + 1) {
                    std::cout << "Usage:" << std::endl;
                    std::cout << "PRINT |--| matrixName | Print matrixName on screen" << std::endl;
                } else {
                    printMatrix(words[1]);
                }
                std::cout << "-----------PRINT END-----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            case ECommand::PRINTALL: {
                std::cout << "----------PRINTALL BEGIN----------" << std::endl;
                printAll();
                std::cout << "-----------PRINTALL END-----------" << std::endl;
            }
                break;
                //--------------------------------------------------------------------------------------------------------//
            default:
                std::cout << "INVALID COMMAND" << std::endl;
                std::cout << "Type HELP for displaying help" << std::endl;
        }


    }
}