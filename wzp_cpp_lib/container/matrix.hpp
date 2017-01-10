#ifndef WZP_MATRIX_HPP_
#define WZP_MATRIX_HPP_

#include <vector>
#include <cassert>
#include <iostream>

#include "thread/taskjob.hpp"

using std::vector;


namespace wzp
{

namespace details
{
/**
 * Operators!!!!!
 */
/**
 * DotProduct, N is the dim, T is the type
 */
template<size_t N, typename T>
class DotProduct {
public:
    static T eval(T* a, T* b) {
        return DotProduct<1, T>::eval(a, b) + DotProduct<N - 1, T>::eval(a + 1, b + 1);
    }
};

/**
 * Special
 */
template<typename T>
class DotProduct<1, T> {
public:
    static T eval(T* a, T* b) {
        return (*a) * (*b);
    }
};

template<typename T, size_t N>
struct MatDot
{
    static T eval(T* a, T* b, size_t C) {
        return MatDot<T, 1>::eval(a, b, C) + MatDot<T, N-1>::eval(a + 1, b + C, C);
    }
};

template<typename T>
struct MatDot<T, 1>
{
    static T eval(T* a, T* b, size_t C) {
        return (*a) * (*b);
    }
};

} //details

//the dot function
template<size_t N, typename T>
inline T dot(T*a, T* b) {
    return details::DotProduct<N, T>::eval(a, b);
}

template<typename T, size_t N>
inline T _dot(T* a, T* b, size_t C) {
    return details::MatDot<T, N>::eval(a, b, C);
}

/**
 * class of Matrix!!
 */

/**
 * Matrix Base, provite operators to avoid code bloat
 */
template<typename T>
class MatrixBase {};

/**
 * the Matrix!
 * M is the matrix rows
 * N is the matrix cols
 */

/**
 * Matrix Init Types
 */
enum class MatrixType {
    Zeros,
    Eyes
};
/**
 * Process Type
 */
enum class ProcessType {
    SingleThread,
    MultiThread
};

template<typename T>
class Matrix : private MatrixBase<T> {

public:
    vector<T> m_data; // the data container
    size_t m_row;
    size_t m_col; // Row and Col;
    ProcessType m_type = ProcessType::MultiThread;

public:
    /**
     * Constructors
     */
    Matrix() = default;

    //by a scalar value
    Matrix(size_t M, size_t N, T init_val = 0) : m_data(M * N, init_val)
                                            , m_row(M)
                                            , m_col(N)
    {}

    //handle the type of eye
    Matrix(size_t M, MatrixType type, T init_val = 1) : m_data(M * M, 0)
                                            , m_row(M)
                                            , m_col(M)
    {
        if(type == MatrixType::Eyes) {
            init(init_val);
        }
    }

    //init by vector<vector<T>>
    Matrix(size_t M, size_t N, const vector<vector<T>>& data) : m_data(M * N)
                                            , m_row(M)
                                            , m_col(N)
    {
        assert(M == data.size() && N == data[0].size());
        for(size_t i = 0; i < M; ++i) {
            for(size_t j = 0; j < N; ++j) {
                m_data[index(i, j)] = data[i][j];
            }
        }
    }

    /**
     * [reshape description]
     * @param new_row
     * @param new_col
     */
    void reshape(size_t new_row, size_t new_col) {
        m_row = new_row;
        m_col = new_col;
        auto new_capacity = new_row * new_col;
        if(new_capacity > m_data.size()) {
            m_data.resize(new_capacity);
        }
    }

    /**
     * reverse
     */
    Matrix<T>& t() {
        vector<T> tmp(m_data);
        for(size_t i = 0; i < m_row; ++i) {
            for(size_t j = 0; j < m_col; ++j) {
                tmp[j * m_row + i] = m_data[index(i, j)];
            }
        }
        std::swap(tmp, m_data);
        std::swap(m_row, m_col);
        return *this;
    }

    /**
     * Operations
     */
    //scalar product
    Matrix<T> operator*(T val) {
        Matrix<T> res(*this);
        for(size_t i = 0; i < m_row; ++i) {
            for(size_t j = 0; j < m_col; ++j) {
                res.at(i, j) *= val;
            }
        }
        return std::move(res);
    }

    Matrix<T>& operator*=(T val) {
        for(size_t i = 0; i < m_row * m_col; ++i) {
            m_data[i] *= val;
        }
        return *this;
    }

    //add function
    Matrix<T> operator+(const Matrix<T>& other) {
        assert(m_row == other.rows() && m_col == other.cols());
        Matrix<T> res(m_row, m_col);
        for(size_t i = 0; i < m_row; ++i) {
            for(size_t j = 0; j < m_col; ++j) {
                res(i, j) = this->at(i, j) + other(i, j);
            }
        }
        return std::move(res);
    }

    Matrix<T>& operator+=(const Matrix<T>& other) {
        assert(m_row == other.rows() && m_col == other.cols());
        for(size_t i = 0; i < m_row; ++i) {
            for(size_t j = 0; j < m_col; ++j) {
                this->at(i, j) = other(i, j);
            }
        }
        return *this;
    }

    /**
     * Dot Product, C is the new Col
     */
    Matrix<T> operator*(const Matrix<T>& other) {
        assert(m_col == other.rows());
        Matrix<T> res(m_row, other.cols());
        check(other.cols());
        if(m_type == ProcessType::SingleThread) {
            for(size_t i = 0; i < m_row; ++i) {
                for(size_t k = 0; k < other.cols(); ++k) {
                    T sum(0);
                    for(size_t j = 0; j < m_col; ++j) {
                        sum += (this->at(i, j) * other(j, k));
                    }
                    res(i, k) = sum;
                }
            }
        }
        else {
            for(size_t i = 0; i < m_row; ++i) {
                vector<size_t> cols_index;
                cols_index.reserve(other.cols());
                for(size_t k = 0; k < other.cols(); ++k) {
                    cols_index.push_back(k);
                }
                ParallelForeach(cols_index.begin(),
                 cols_index.end(), [&res, this, i, &other](size_t k){
                    T sum(0);
                    for(size_t j = 0; j < m_col; ++j) {
                        sum += at(i, j) * other(j, k);
                    }
                    res(i, k) = sum;
                });
            }
        }
        return res;
    }

    /**
     * Some getter
     */
    inline size_t rows() const { return m_row; }

    inline size_t cols() const { return m_col; }

    inline T at(size_t i, size_t j) const { return m_data[index(i, j)]; }
    inline T& at(size_t i, size_t j) { return m_data[index(i, j)]; }

    inline T operator() (size_t i, size_t j) const {
        return m_data[index(i, j)];
    }

    inline T& operator() (size_t i, size_t j) {
        return m_data[index(i, j)];
    }

    /**
     * Set funtions
     */
    void set_process_type(ProcessType type) {
        m_type = type;
    }

    /**
     * print function
     */
    void print() {
        for(size_t i = 0; i < m_row; ++i) {
            for(size_t j = 0; j < m_col; ++j) {
                std::cout<<at(i, j)<<" ";
            }
            std::cout<<std::endl;
        }
    }

private:
    void init(T init_val) {
        for(size_t i = 0; i < m_row; ++i) {
            m_data[index(i, i)] = init_val;
        }
    }
    //compute the index
    /**
     * compute the index
     * @param  i      x loc
     * @param  size_t y loc
     * @return        the index in m_data
     */
    inline size_t index(size_t i, size_t j) const {
        return i * m_col + j;
    }

    inline void check(size_t C) {
        if(m_col > 256 || C > 256) {
            m_type = ProcessType::MultiThread;
        }
        else {
            m_type = ProcessType::SingleThread;
        }
    }

};

/**
 * Simple Special
 */
} //wzp

#endif /*WZP_MATRIX_HPP_*/