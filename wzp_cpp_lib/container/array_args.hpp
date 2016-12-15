#ifndef ARRAY_ARGS_HPP_
#define ARRAY_ARGS_HPP_

#include <ctime>
#include <cstdlib>
#include <assert.h>

#include <vector>
#include <algorithm>

namespace wzp{

    /*the contatiner's args op for vector*/
    template<class T>
    class array_args{
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

        /*shuffle an array*/
        inline static void shuffle(std::vector<T>& array) {
            srand(time(NULL));
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
            srand(time(NULL));
            int i, j;
            for(i = data_array.size() - 1; i > 0; --i) {
                j = rand() % (i + 1);
                std::swap(data_array[i], data_array[j]);
                std::swap(label_array[i], label_array[j]);
            }
        }

    };

}/*wzp namespace*/
#endif /*ARRAY_ARGS_HPP_*/