#ifndef ARRAY_ARGS_HPP_
#define ARRAY_ARGS_HPP_

#include <ctime>
#include <cstdlib>
#include <cassert>

#include <vector>
#include <algorithm>
#include <numeric>

namespace wzp{

    /*the contatiner's args op for vector*/
    template<class T>
    class array_args {
    public:
        /*get the max value's index*/
        inline static size_t arg_max(const std::vector<T>& array){
            if(array.empty()) return 0;
            size_t max_index = 0;
            for(size_t i = 1; i < array.size(); ++i){
                if(array[i] > array[max_index]) max_index = i;
            }
            return max_index;
        }

        /*get the min value's index*/
        inline static size_t arg_min(const std::vector<T>& array){
            if(array.empty()) return 0;
            size_t min_index = 0;
            for(size_t i = 1; i < array.size(); ++i){
                if(array[i] < array[min_index]) min_index = i;
            }
            return min_index;
        }

        /*get any index of array*/
        inline static T& any_index(std::vector<T>& array, int index){
            if(index >= 0) return array[index];
            else{
                auto reindex = array.size() + index;
                return array[reindex];
            }
        }

        inline static const T& any_index(const std::vector<T>& array, int index) {
            if(index >= 0) return array[index];
            else{
                auto reindex = array.size() + index;
                return array[reindex];
            }
        }

        // extend another array into the raw array
        template<typename U>
        inline static void extend(std::vector<T>& array, const std::vector<U>& other) {
            for(auto& o : other) {
                array.emplace_back(static_cast<T>(o));
            }
        }

        /*shuffle an array*/
        inline static void shuffle(std::vector<T>& array) {
            // srand(time(NULL));
            int i, j ;
            for(i = array.size() - 1; i > 0; --i) {
                j = rand() % (i + 1);
                std::swap(array[i], array[j]);
            }
        }

        /*shuffle the two arrays*/
        template<typename U>
        inline static void shuffle(std::vector<T>& data_array, std::vector<U>& label_array) {
            assert(data_array.size() == label_array.size());
            // srand(time(NULL));
            int i, j;
            for(i = data_array.size() - 1; i > 0; --i) {
                j = rand() % (i + 1);
                std::swap(data_array[i], data_array[j]);
                std::swap(label_array[i], label_array[j]);
            }
        }

        /**
         * sum of a vector
         */
        inline static T sum(const std::vector<T>& array) {
            return std::accumulate(std::begin(array), std::end(array), T(0));
        }

        /**
         * mean of a vector
         */
        template<typename F = float>
        inline static F mean(const std::vector<T>& array) {
            auto sum_val = sum(array);
            return static_cast<F>(sum_val) / static_cast<F>(array.size());
        }

    };

}/*wzp namespace*/
#endif /*ARRAY_ARGS_HPP_*/