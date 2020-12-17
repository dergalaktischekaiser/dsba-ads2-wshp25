////////////////////////////////////////////////////////////////////////////////
/// \file
/// \brief      Implementations of RMQ algorithms.
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


#ifndef RMQ_HPP_
#define RMQ_HPP_

#include <climits>
#include <cmath>
#include <stdexcept>
#include <utility>
#include <vector>
using std::ceil;
using std::make_pair;
using std::round;
using std::pair;
using std::sqrt;
using std::vector;

////#include <cstddef> // size_t


/// Trivial implementation of search minimum in the given range [i; j].
template<typename T>
size_t findRMQ(const std::vector<T>& arr, size_t i, size_t j)
{
    if (i > j)
        throw std::invalid_argument("Bad i <= j");
    if (j >= arr.size())
        throw std::invalid_argument("i, j out of range");

    size_t minInd = i;
    //T curMin = arr[cmInd];

    for(++i; i <= j; ++i)
    {
        if (arr[i] < arr[minInd])
            minInd = i;
    }

    return minInd;
}

// TODO:

/// The implementation of the DynProg approach

// The function for observing minimum values by indices of the input array.
template<typename P>
P index_by_min(const vector<vector<P>>& mat, const vector<P>& inp, size_t hor, size_t ver) {
    if (inp[mat[hor][ver - 1]] <= inp[mat[hor + 1][ver]])
        return mat[hor][ver - 1];
    return mat[hor + 1][ver];
}

// The class of the problem solution.
template<typename P>
class DpRmqFinder {
public:
            vector<vector<P>> matrix;  // the DynProg matrix
            vector<P> input;           // to store the input array


            explicit DpRmqFinder(const vector<P>& vec) : input(vec)
            {   // a proper constructor with an std::vector<T> default destructor
                matrix.resize(vec.size());
                for (vector<P>& v : matrix)       // lower-bottom diagonal of them matrix is going to be
                    v.resize(vec.size(), 0);      // just filled with zeroes.
                for (size_t k = 0; k < vec.size(); ++k)
                    matrix[k][k] = k;  // calculate all 1-length minima
            }

            // THE PREPROCESS SCHEME for O(n^2).
            // A way to postpone the preprocessing required.
            // The building of the DynProg matrix is going to be done exactly after the construction.
            // So, this "process()" method fills the matrix sa required.
            void process()
            {
                for (size_t k = 1; k < input.size(); ++k) {
                    size_t now_horiz = 0, now_vert = k;
                    while (now_vert <= input.size() - 1) {
                        // see the earlier defined individual function
                        matrix[now_horiz][now_vert] = index_by_min(matrix, input, now_horiz, now_vert);
                        ++now_horiz;
                        ++now_vert;
                    }
                    // std::cout << *this << "\n"; // <--- You can uncomment this section to see the
                                                   // execution of "process()" step-by-step.
                }
            }

            // The "solver" function for the RMQ on A[i, j] where [i; j] is any interval chosen on A.
            // Returns the solution of the RMQ Problem on a manually chosen interval.
            // CAUTION! The answer is the index of an array with numeration starting from 1!
            P solver(P i, P j)
            {   // catching the out-of-range exception in case when input boundary points are greater
                // then an array's boundary points. Also checking whether
                // the left point is greater than the right one.
                if (i >= j)
                    throw std::exception("The left boundary is greater than the right one!");
                try
                {
                    i > input.size() || j > input.size();
                }
                catch(...)
                {
                    throw std::out_of_range("Ai-ai-ai!");
                }
                return matrix[i - 1][j - 1] + 1;
            }
        };

template <typename P>
class SqRootDecompRmqFinder {
// From the lecture we remember that in the most efficient way there must be ceil(n/sqrt(n)) partitions
// where n is an array's length.
public:
    vector<P> input;                     // An input vector.
    int size_of_a_block, num_of_blocks;  // Important values which are going to be used in the implementation.
    vector<vector<P>> partition_matrix;  // The storage of blocks. If the block is not full, then full it till the end
                                         // with '-1's.
    vector<pair<P, P>> info_matrix;      // There we are going to look for any block's minimum.
                                         // .first = element .second = its index

    explicit SqRootDecompRmqFinder(const vector<P>& vec) : input(vec)
    {
        float n = vec.size();
        num_of_blocks = ceil(n / sqrt(n));
        size_of_a_block = round(sqrt(n));
        partition_matrix.resize((num_of_blocks));
        for (vector<P>& v : partition_matrix) {
            v.resize(size_of_a_block, -1);
        }
        info_matrix.resize(num_of_blocks);
        for (size_t i = 0; i < info_matrix.size(); ++i) {
            info_matrix[i].first = INT_MAX;
            info_matrix[i].second = i;
        }
    }

    // THE PREPROCESS SCHEME
    // O(n) for preprocessing exactly after finishing construction of the object.
    void process()
    {
        // std::cout << size_of_a_block << " " << num_of_blocks << "\n";
        int block = 0;
        for (int i = 0; i < input.size(); ++i) {
            if ((i + 1) % size_of_a_block == 0) {
                partition_matrix[block][i % size_of_a_block] = input[i];
                if (input[i] <= info_matrix[block].first) {
                    info_matrix[block] = make_pair(input[i], i);
                }
                ++block;
            } else {
                partition_matrix[block][i % size_of_a_block] = input[i];
                if (input[i] <= info_matrix[block].first) {
                    info_matrix[block] = make_pair(input[i], i);
                }
            }
            //std::cout << *this << "\n";
        }
    }

    // The "solver" function for the RMQ on A[i, j] where [i; j] is any interval chosen on A.
    // Returns the solution of the RMQ Problem on a manually chosen interval.
    // CAUTION! The answer is the index of an array with numeration starting from 1!
    P solver(P _i, P _j)
    // catching the out-of-range exception in case when the right boundary point is
    // greater than an array's right boundary point. Also checking whether
    // the left point is greater than the right one.
    {
        if (_i >= _j)
            throw std::exception("Right value exceeds the array's right boundary!");
        try
        {
            _j > input.size();
        }
        catch(...)
        {
            throw std::out_of_range("Out of range!");
        }
        P i = _i - 1, j = _j - 1;
        // Accessing an arbitrary block for O(1).
        int block_of_i = i / size_of_a_block,
            block_of_j = j / size_of_a_block,
            min = INT_MAX;
        pair<P, P> p;
        // std::cout << block_of_i << " " << block_of_j;
        for (int k = block_of_i; k < block_of_j + 1; ++k) {
            if (info_matrix[k].first <= min) {
                p = info_matrix[k];
            }
        }
        return p.second;
    }
};

#endif // RMQ_HPP_