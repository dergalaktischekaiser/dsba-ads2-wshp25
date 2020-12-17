////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief      Main module for smoke-test-project for RMQ.
/// \author     Sergey Shershakov
/// \version    0.1.0
/// \date       8.10.2020
/// \copyright  © Sergey Shershakov 2020.
///             This code is for educational purposes of the course "Algorithms
///             and Data Structures" provided by the Faculty of Computer Science
///             at the Higher School of Economics.
///
/// When altering code, a copyright line must be preserved.
///
////////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include "rmq.h"

// TODO:
typedef DpRmqFinder<int> DpRmqFinderInt;
typedef SqRootDecompRmqFinder<int> SqRootDecompRmqFinderInt;

std::vector<int> ARR2 = { 12, 51, 32, 10, 27 },
                 ARR3 = { 12, 51, 32, 10, 27, 32, 29, 2, 38, 37, 4, 11, 1, 35, 24, 9, 21 };

// "...to allow printing out the precomputed table of minima according to its diagonal structure..."
// Defining the ostream output operator for matrix printing in the DynProg solution.
std::ostream& operator<<(std::ostream& s, const DpRmqFinderInt& ps)
{
    for (size_t i = 0; i < ps.matrix.size(); ++i) {
        for (size_t j = 0; j < ps.matrix[i].size(); ++j)
            s << ps.matrix[i][j] << " ";
        s << "\n";
    }
    return s;
}

// Defining the ostream output operator for printing of blocks in the Square Root Decomp. task.
std::ostream& operator<<(std::ostream& s, const SqRootDecompRmqFinderInt& ps)
{
    for (size_t i = 0; i < ps.partition_matrix.size(); ++i) {
        for (size_t j = 0; j < ps.partition_matrix[0].size(); ++j) {
            if (j == ps.partition_matrix[0].size() - 1)
                s << ps.partition_matrix[i][j] << ";" << "\t";
            else
                s << ps.partition_matrix[i][j] << "\t";
        }
        s << "the minimum of this block equals " << ps.info_matrix[i].first;
        s << "\n";
    }
    return s;
}

// Defining vector output generally.
template<typename P>
std::ostream& operator<<(std::ostream& s, const vector<P>& v)
{
    for (const auto& elem : v)
        s << elem << " ";
    s << "\n";
    return s;
}


// TODO:
void dynProgTest1()
{
    DpRmqFinderInt fi(ARR2);
    fi.process();

    std::cout << "DynProg Arr 1:\n";
    std::cout << fi << "\n";
    // std::cout << fi.solver(3, 5) << "\n";  // <--- Uncomment this section to try to solve the RMQ
                                             // Problem on an interval of your choice.
}

void SqRootDecompTest1()
{
    SqRootDecompRmqFinderInt fi(ARR3);
    fi.process();
    std::cout << "Partition Matrix 1:\n";
    std::cout << fi << "\n";
    // std::cout << fi.solver(4, 14) << "\n"; // <--- Uncomment this section to try to solve the RMQ
                                              // Problem using SQ Decomp. on an interval of your choice.
}


int main()
{
    std::cout << "\n" << "Hello there! Let's do some range minimum queries!\n\n";

    dynProgTest1();

    SqRootDecompTest1();                      // The bonus task.

    std::cout << "Bye-bye!\n\n";

    return 0;
}

/* =================
OUTPUT.
<<<
Hello there! Let's do some range minimum queries!

DynProg Arr 1:
0 0 0 3 3
0 1 2 3 3
0 0 2 3 3
0 0 0 3 3
0 0 0 0 4

Partition Matrix 1:
12      51      32      10;     the minimum of this block equals 10
27      32      29      2;      the minimum of this block equals 2
38      37      4       11;     the minimum of this block equals 4
1       35      24      9;      the minimum of this block equals 1
21      -1      -1      -1;     the minimum of this block equals 21

Bye-bye!

*/
