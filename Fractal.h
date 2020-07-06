#ifndef EX2_FRACTAL_H
#define EX2_FRACTAL_H

#include <tuple>
#include <iostream>
#include <cmath>
#include <utility>
#include <vector>
#include <set>
#include <deque>

/**
 * A class that represents a part of a fractal shape.
 * Applying the rule of 5.
 * */
class FractalPart
{
protected:
    /**
     * The height of this part
     * */
    unsigned int                           _height;
    /**
     * The grid size of the fractal shape.
     * */
    unsigned int                          _gridSize;
    /**
     * A pointer to FractalPart instance which this instance is constructed from.
     * */
    FractalPart                   *_father;
    /**
     * A pointer to a set of pairs that represents the empty spots in the grid (indexes starts from 0).
     * */
    std::set<std::pair<int, int>> *_pNullPrints;
    /**
     * Static constant char which represents the char that will be printed when the spot is not empty.
     * */
    static const char             _sign     = '#';
    /**
    * Static constant char which represents the char that will be printed when the spot is empty.
    * */
    static const char             _nullSign = ' ';

    /**
     * A method that gets rvalue pair of 2 integers and returns true if the given pair is in _nullPrints set.
     * The count of the pair in the set is converted to boolean by the method, this may seem redundant but it makes the
     * code more clear.
     * */
    bool isNull(std::pair<int, int> && pair)
    {
        return _pNullPrints->count(pair);
    }

public:
    /**
     * A constructor.
     * */
    FractalPart(unsigned int height, unsigned int gridSize, FractalPart *father, std::set<std::pair<int, int>> *nullPrints) :
            _height(height), _gridSize(gridSize), _father(father), _pNullPrints(nullPrints)
    {}

    /**
     * A copy constructor, implemented using default c++ default constructor.
     * */
    FractalPart(const FractalPart &f) = default;

    /**
     * A move constructor, used to move the rvalue FractalPart instance into the current instance.
     * */
    FractalPart(FractalPart && f) noexcept;

    /**
     * A destructor.
     * */
    ~FractalPart();

    /**
     * A method that gets 2 FractalPart references and swaps them.
     * */
    friend void swap(FractalPart &first, FractalPart &second);

    /**
     * A method which is used to apply = operator on 2 FractalParts this method uses swap function to swap between the
     * FractalParts, using swap function saves calls to copy constructor and destructor, the memory of the current
     * instance is freed automatically ( it is moved to the rvalue which is freed automatically).
     * */
    FractalPart &operator=(FractalPart f);

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
    void drawPart(std::vector<std::string> &shape, bool shouldPrint = true, int startLine = 0);
};

/**
 * An abstract class that represents FractalShape, due to the recursive structure of fractal shapes, a fractal shape is
 * also a fractal part, and inherits from FractalPart class.
 * */
class FractalShape : public FractalPart
{
protected:
    /**
     * A vector of strings which represents the fractal shape lines.
     * */
    std::vector<std::string>      _shape;
    static const unsigned              GRID_SIZE;
    /**
    * A set of integer pairs that represents the null indxes in the fractal shape.
    * */
    std::set<std::pair<int, int>> _nullPrints;

    /**
     * A method which is used to fill _shape vector lines.
     * The method is using the _father of the FractalShape to start the recursive drawing process.
     * */
    void createShape()
    {
        _father->drawPart(_shape);
    }

    /**
     * A method which is used to construct the FractalParts that constructs the FractalShape.
     * The method is setting the father of this shape to a FractalPart instance which has the height of the shape.
     * */
    void buildFractalParts();

    /**
     * A method which is setting the nullPrints set, this part is relevant only to a concrete shape (not abstract).
     * */
    virtual void setNullPrints() = 0;

public:
    /**
     * A constructor.
     * Calling the constructor of FractalPart (has not default constructor) and initializing shape vector to a
     * correct size.
     * */
    FractalShape(unsigned int dimension, unsigned int gridSize) :
            FractalPart(dimension, gridSize, nullptr, nullptr), _shape(pow(sqrt(gridSize), dimension))
    {};

    /**
     * A default destructor.
     * */
    virtual ~FractalShape() = default;

    /**
     * A method that builds the shape FractalParts and prints the shape to std::cout.
     * */
    void printShape();
};

/**
 * A class that implements FractalShape class.
 * */
class CarpetFractal : public FractalShape
{
protected:
    /**
     * Static constant integer which represents the grid size of the fractal shape.
     * */
    static const unsigned int GRID_SIZE = 9;

    /**
     * A method that overrides the FractalShape method and fills the nullPrints set with pair of integers.
     * */
    void setNullPrints() override;

public:
    /**
     * A constructor, calling FractalShape constructor and calling setNullPrints.
     * */
    explicit CarpetFractal(unsigned int dimension) : FractalShape(dimension, GRID_SIZE)
    {
        setNullPrints();
    }
};

/**
 * A class that implements FractalShape class.
 * */
class SieveFractal : public FractalShape
{
protected:
    /**
    * Static constant integer which represents the grid size of the fractal shape.
    * */
    static const unsigned int              GRID_SIZE = 4;
    /**
    * A set of integer pairs that represents the null indxes in the fractal shape.
    * */
    std::set<std::pair<int, int>> nullPrints;

    /**
     * A method that overrides the FractalShape method and fills the nullPrints set with pair of integers.
     * */
    void setNullPrints() override;

public:
    /**
    * A constructor, calling FractalShape constructor and calling setNullPrints.
    * */
    explicit SieveFractal(unsigned int dimension) : FractalShape(dimension, GRID_SIZE)
    {
        setNullPrints();
        _pNullPrints = &nullPrints;
    }
};

/**
 * A class that implements FractalShape class.
 * */
class CantorFractal : public FractalShape
{
protected:
    /**
    * Static constant integer which represents the grid size of the fractal shape.
    * */

    static const unsigned int              GRID_SIZE = 9;
    /**
    * A set of integer pairs that represents the null indxes in the fractal shape.
    * */
    std::set<std::pair<int, int>> nullPrints;

    /**
     * A method that overrides the FractalShape method and fills the nullPrints set with pair of integers.
     * */
    void setNullPrints() override;

public:
    /**
    * A constructor, calling FractalShape constructor and calling setNullPrints.
    * */
    explicit CantorFractal(unsigned int dimension) : FractalShape(dimension, GRID_SIZE)
    {
        setNullPrints();
    }
};

/**
 * A class that is responsible of parsing csv files.
 * Applying the rule of 3.
 * */
class CSVParser
{
private:
    static const unsigned int           _maxHeight = 6;
    static const unsigned int           _minHeight = 1;
    static const unsigned int           _minType   = 1;
    static const unsigned int           _maxType   = 3;
    std::string                _filePath;
    std::deque<FractalShape *> _shapes;

    /**
    * A method that gets a vector of strings (the values in the line) and checks if the line if valid.
     * The method catch exceptions from stoi;
     * The method tries to parse strings in strValues into numValues
     * */
    static bool _isLegalLine(const std::vector<std::string> &strValues, std::vector<unsigned int> &numValues);


    /**
     * A method that gets a vector of legal integers (the values in the line) and creates a FractalShape and adds it into
     * _shapes vector.
     * */
    void _addShape(std::vector<unsigned int> vector);

public:

    /**
     * A static constant char which is the delimeter between the values in the csv file.
     * */
    static const char _delim = ',';

    /**
     * A constructor, getting a string by value, moving the value to the filePath.
     * */
    explicit CSVParser(std::string filePath) :
            _filePath(std::move(filePath))
    {};

    /**
     * A default copy constructor.
     * */
    CSVParser(const CSVParser &c) = default;

    /**
     * A destructor.
     * */
    ~CSVParser()
    { _deleteShapes(); };

    /**
     * An assigment operator, deletes the left CSVParser shapes.
     * */
    CSVParser &operator=(const CSVParser &c);

    /**
    * A method that deletes the all of the shapes.
    * */
    void _deleteShapes();


    /**
     * A method that is used to parse the _filePath (that was given to the constructor) into _shapes vector.
     * Throws CSVException if the csv is not valid.
     * */
    void parseCSV(); // fill up shapes deque, returns bool - false if failed true if ok
    /**
     * A method that prints to std::cout the shape that were parsed by the parser.
     * */
    void printShapes() const; //for loop on _shapes - print them.

};

/**
 * A simple exception that inherits from std::exception and represents an error in csv reading process.
 * */
struct CSVException : public std::exception
{
};

#endif //EX2_FRACTAL_H
