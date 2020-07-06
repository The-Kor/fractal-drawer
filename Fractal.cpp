#include <sstream>
#include <algorithm>
#include <fstream>
#include "Fractal.h"

/**
 * A method that gets vector of strings (which represents a shape) and fills the given vector strings with the lines
 * that constructs the fractal part shape.
 * Due to the fact the fractal shapes are recursive , this method is recursive, each FractalPart is calling it's
 * father drawPart to construct the part itself, the recursion has base case in height = 0 which is responsible of
 * filling the shape lines.
 * @param shouldPrint shape - A reference to vector of strings that is filled by the method.
 * @param shouldPrint - A boolean which represents if the current part should be printed in the shape.
 * @param startLine - An integer represents the starting line that the fractal part should be printed to.
 * */
void FractalPart::drawPart(std::vector <std::string> &shape, bool shouldPrint, int startLine) {
    if (_height == 0) {
        shape[startLine] += shouldPrint ? _sign : _nullSign;
        return;
    } else {
        int curLine;
        for (int i = 0; i < sqrt(_gridSize); ++i) {
            for (int j = 0; j < sqrt(_gridSize); ++j) {
                curLine = (int) pow(sqrt(_gridSize), _height - 1) * i + startLine;
                if (!shouldPrint) {
                    _father->drawPart(shape, shouldPrint, curLine);
                } else {
                    _father->drawPart(shape, !isNull(std::make_pair(i, j)), curLine);
                }
            }
        }
    }
}

/**
* A move constructor, used to move the rvalue FractalPart instance into the current instance.
* */
FractalPart::FractalPart(FractalPart &&f) noexcept
        :
        _height(f._height), _gridSize(f._gridSize) {
    _father = f._father;
    f._father = nullptr;
    _pNullPrints = f._pNullPrints;
    f._pNullPrints = nullptr;
}

/**
 * A method that gets 2 FractalPart references and swaps them.
 * */
void swap(FractalPart &first, FractalPart &second) {
    std::swap(first._height, second._height);
    std::swap(first._gridSize, second._gridSize);
    std::swap(first._father, second._father);
    std::swap(first._pNullPrints, second._pNullPrints);
}

/**
 * A method which is used to apply = operator on 2 FractalParts this method uses swap function to swap between the
 * FractalParts, using swap function saves calls to copy constructor and destructor, the memory of the current
 * instance is freed automatically ( it is moved to the rvalue which is freed automatically).
 * */
FractalPart &FractalPart::operator=(FractalPart f) {
    swap(*this, f);
    return *this;
}

/**
 * A destructor.
 * */
FractalPart::~FractalPart() {
    if (_height != 0) {
        delete (_father);
    }
}

/**
 * A method that builds the shape FractalParts and prints the shape to std::cout.
 * */
void FractalShape::printShape() {
    buildFractalParts();
    for (int i = 0; i < pow(sqrt(_gridSize), _height); ++i) {
        std::cout << _shape[i] << std::endl;
    }
    std::cout << std::endl;
}

/**
 * A method which is used to construct the FractalParts that constructs the FractalShape.
 * The method is setting the father of this shape to a FractalPart instance which has the height of the shape.
 * */
void FractalShape::buildFractalParts() {
    {
        auto *cur = new FractalPart(0, _gridSize, nullptr, _pNullPrints);
        for (unsigned int i = 1; i <= _height; ++i) {
            cur = new FractalPart(i, _gridSize, cur, _pNullPrints);
        }
        _father = cur;
        createShape();
    }
}

/**
 * A method that is used to parse the _filePath (that was given to the constructor) into _shapes vector.
 * Throws CSVException if the csv is not valid.
 * */
void CSVParser::parseCSV() {
    std::string curLine;
    std::string curValue;
    std::vector <std::string> curValues;
    std::vector<unsigned int> curValuesNum(2);
    std::ifstream file(_filePath);
    if (!file.good()){
        throw CSVException();
    }
    while (getline(file, curLine)) {
        curLine.erase(std::remove(curLine.begin(), curLine.end(), '\r'), curLine.end());
        std::istringstream s(curLine);
        while (getline(s, curValue, _delim)) {
            if (curValue.size() > 1){
//                Removing redundant characters added to some csv lines (3 chars for a line)
                curValues.push_back(curValue.substr(3,curValue.size()-3));
            } else{
                curValues.push_back(curValue);
            }
        }
        if (_isLegalLine(curValues, curValuesNum)) {
            _addShape(curValuesNum);
            curValues.clear();
        } else {
            file.close();
            std::cerr << "illegal line found: " << curLine << std::endl;
            throw CSVException();
        }
    }
    file.close();
}


/**
* A method that gets a vector of strings (the values in the line) and checks if the line is valid.
 * The method does not catch exceptions from stoi, the method relies that the values that were given were already
 * checked as integers.
 * The method tries to parse strings in strValues into numValues
 * */
bool CSVParser::_isLegalLine(const std::vector <std::string> &strValues, std::vector<unsigned int> &numValues) {
    try {
        if (strValues.size() == 2) {
            unsigned int curType = std::stoul(strValues[0]);
            unsigned int curHeight = std::stoul(strValues[1]);
            if (curType >= _minType && curType <= _maxType) {
                if (curHeight >= _minHeight && curHeight <= _maxHeight) {
                    numValues[0] = curType;
                    numValues[1] = curHeight;
                    return true;
                }
            }
        }
        return false;
    }
    catch (const std::exception &e) //catching stoi exception
    {
        std::cerr << "Invalid argument2" << std::endl;
        return false;
    };
}

/**
 * A method that gets a vector of legal integers (the values in the line) and creates a FractalShape and adds it into
 * _shapes vector.
 * */
void CSVParser::_addShape(std::vector<unsigned int> values) {
    switch (values[0]) {
        case 1:
            _shapes.push_front(new CarpetFractal(values[1]));
            break;
        case 2:
            _shapes.push_front(new SieveFractal(values[1]));
            break;
        case 3:
            _shapes.push_front(new CantorFractal(values[1]));
            break;
    }
}

/**
 * A method that deletes the all of the shapes.
 * */
void CSVParser::_deleteShapes() {
    for (FractalShape *shape : _shapes) {
        delete (shape);
    }
}

/**
 * An assigment operator, deletes the left CSVParser shapes.
 * */
CSVParser &CSVParser::operator=(const CSVParser &c) {
    if (this != &c) {
        _deleteShapes();
        _shapes = c._shapes;
        _filePath = c._filePath;
    }
    return *this;
}

/**
 * A method that prints to std::cout the shape that were parsed by the parser.
 * */
void CSVParser::printShapes() const {
    for (FractalShape *shape :_shapes) {
        shape->printShape();
    }
}

/**
 * A method that overrides the FractalShape method and fills the nullPrints set with pair of integers.
 * */
void CantorFractal::setNullPrints() {
    nullPrints.insert(std::make_pair(1, 1));
    nullPrints.insert(std::make_pair(0, 1));
    nullPrints.insert(std::make_pair(1, 0));
    nullPrints.insert(std::make_pair(2, 1));
    nullPrints.insert(std::make_pair(1, 2));
    _pNullPrints = &nullPrints;
}

/**
 * A method that overrides the FractalShape method and fills the nullPrints set with pair of integers.
 * */
void SieveFractal::setNullPrints() {
    nullPrints.insert(std::make_pair(1, 1));
    _pNullPrints = &nullPrints;
}

/**
 * A method that overrides the FractalShape method and fills the nullPrints set with pair of integers.
 * */
void CarpetFractal::setNullPrints() {
    _nullPrints.insert(std::make_pair(1, 1));
    _pNullPrints = &_nullPrints;
}