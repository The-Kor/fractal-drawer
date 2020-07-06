#include <iostream>
#include "Fractal.h"

/**
 * The main method of this program, reads the given csv file, and prints the fractal shapes to std::cout in reverse
 * order.
 * */
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: FractalDrawer <file path>" << std::endl;
        return EXIT_FAILURE;
    }
    CSVParser parser = CSVParser(argv[1]);
    try
    {
        parser.parseCSV();
    }
    catch (CSVException &e)
    {
        std::cerr << "Invalid input file" << std::endl;
        return EXIT_FAILURE;
    }
    parser.printShapes();
    return EXIT_SUCCESS;
}