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

} //details

//the dot function
template<size_t N, typename T>
inline T dot(T*a, T* b) {
    return details::DotProduct<N, T>::eval(a, b);
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

template<typename T, size_t M, size_t N>
class Matrix : private MatrixBase<T> {

private:
    vector<T> m_data; // the data container
    size_t Row = M;
    size_t Col = N; // Row and Col;
    ProcessType m_type = ProcessType::MultiThread;

public:
    /**
     * Constructors
     */
    Matrix() : m_data(M * N)
    {}

    //by a scalar value
    Matrix(T init_val) : m_data(M * N, init_val)
    {}

    //handle the type of eye
    Matrix(MatrixType type, T init_val = 1) : m_data(M * N, 0)
    {
        if(type == MatrixType::Eyes) {
            assert(M == N);
            init(init_val);
        }
    }

    //init by vector<vector<T>>
    Matrix(const vector<vector<T>>& data) : m_data(M * N)
    {
        assert(M == data.size() && N == data[0].size());
        for(size_t i = 0; i < M; ++i) {
            for(size_t j = 0; j < N; ++j) {
                m_data[index(i, j)] = data[i][j];
            }
        }
    }

    /**
     * Operations
     */
    //scalar product
    Matrix<T, M, N> operator*(T val) {
        Matrix<T, M, N> res(*this);
        for(size_t i = 0; i < M; ++i) {
            for(size_t j = 0; j < N; ++j) {
                res.at(i, j) *= val;
            }
        }
        return std::move(res);
    }

    Matrix<T, M, N>& operator*=(T val) {
        for(size_t i = 0; i < M * N; ++i) {
            m_data[i] *= val;
        }
        return *this;
    }

    //add function
    Matrix<T, M, N> operator+(const Matrix<T, M, N>& other) {
        Matrix<T, M, N> res;
        for(size_t i = 0; i < M; ++i) {
            for(size_t j = 0; j < N; ++j) {
                res(i, j) = this->at(i, j) + other(i, j);
            }
        }
        return std::move(res);
    }

    Matrix<T, M, N>& operator+=(const Matrix<T, M, N>& other) {
        for(size_t i = 0; i < M; ++i) {
            for(size_t j = 0; j < N; ++j) {
                this->at(i, j) = other(i, j);
            }
        }
        return *this;
    }

    /**
     * Dot Product, C is the new Col
     */
    template<size_t C>
    Matrix<T, M, C> operator*(const Matrix<T, N, C>& other) {
        Matrix<T, M, C> res;
        check(C);
        if(m_type == ProcessType::SingleThread) {
            // vector<T> col_data;
            for(size_t i = 0; i < M; ++i) {
                for(size_t k = 0; k < C; ++k) {
                    // col_data = std::move(other.get_col_at(k));
                    T sum(0);
                    for(size_t j = 0; j < N; ++j) {
                        sum += (this->at(i, j) * other(j, k));
                    }
                    // res(i, k) = dot<N>(&m_data[index(i, 0)], &col_data[0]);
                    res(i, k) = sum;
                }
            }
        }
        else {
            for(size_t i = 0; i < M; ++i) {
                vector<size_t> cols_index;
                cols_index.reserve(C);
                for(size_t k = 0; k < C; ++k) {
                    cols_index.push_back(k);
                }
                ParallelForeach(cols_index.begin(),
                 cols_index.end(), [&res, this, i, &other](size_t k){
                    T sum(0);
                    for(size_t j = 0; j < N; ++j) {
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
    inline size_t rows() const { return Row; }

    inline size_t cols() const { return Col; }

    inline T at(size_t i, size_t j) const { return m_data[index(i, j)]; }
    inline T& at(size_t i, size_t j) { return m_data[index(i, j)]; }

    inline T operator() (size_t i, size_t j) const {
        return m_data[index(i, j)];
    }

    inline T& operator() (size_t i, size_t j) {
        return m_data[index(i, j)];
    }

    /**
     * get one col data
     */
    vector<T> get_col_at(size_t j) const {
        vector<T> col_data(M);
        for(size_t i = 0; i < M; ++i) {
            col_data[i] = this->at(i, j);
        }
        return std::move(col_data);
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
        for(size_t i = 0; i < M; ++i) {
            for(size_t j = 0; j < N; ++j) {
                std::cout<<at(i, j)<<" ";
            }
            std::cout<<std::endl;
        }
    }

private:
    void init(T init_val) {
        for(size_t i = 0; i < M; ++i) {
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
        return i * N + j;
    }

    inline void check(size_t C) {
        if(N > 256 || C > 256) {
            m_type = ProcessType::MultiThread;
        }
        else {
            m_type = ProcessType::SingleThread;
        }
    }

};

/**
 * Operators!!!!
 */
} //wzp

#endif /*WZP_MATRIX_HPP_*/