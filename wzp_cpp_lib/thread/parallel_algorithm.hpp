#ifndef LIB_PARALLEL_ALGORITHM_HPP
#define LIB_PARALLEL_ALGORITHM_HPP

#include <algorithm>
#include <future>
#include <vector>
#include <iterator>

#include "wzp_cpp_lib/thread/task_group.hpp"

/**
 * How to Use:
 * a parallel function interfaces
 * ParallelRange: just like the loop functions in openmp, parallel use for index
 * ParallelForeach: std::for_each in parallel
 * ParallelInvoke: run a list of functions immeditaly
 * ParallelReduce: parallel reduce a range, just like obtain the max value in a vector
 * ParallelSort: std::sort in parallel
 */

namespace wzp {

/**
 * ParallelRange:input range i to len, and a function, the function need receive the index
 */
template<typename Index, typename Function>
void ParallelRange(Index begin, Index end, Function&& fun) {
    auto thread_num = std::thread::hardware_concurrency();
    auto block_size = (end - begin) / thread_num;
    TaskGroup group;
    Index last = begin;
    if(block_size) {
        last += (thread_num - 1) * block_size;
    }
    else {
        last = end;
        block_size = 1;
    }
    group.reserve(thread_num + 1);
    for(; begin < last; begin += block_size) {
        group.run([&fun, begin, block_size] () {
            for(Index i = begin; i < begin + block_size; ++i) {
                fun(i);
            }
        });
    }
    //the last
    if(begin < end) {
        group.run([&fun, begin, end] () {
            for(Index i = begin; i < end; ++i) {
                fun(i);
            }
        });
    }
    group.wait();
}

/**
 * the simple one
 */
template<typename Index, typename Function>
void ParallelRange(Index end, Function&& fun) {
    ParallelRange(Index(0), end, fun);
}

//use the hardware thread to parallel handle the fun
template<typename Iterator, typename Function>
void ParallelForeach(Iterator begin, Iterator end, Function&& fun) {
    auto thread_num = std::thread::hardware_concurrency();
    auto block_size = std::distance(begin, end) / thread_num;
    Iterator last = begin;
    if(block_size) {
        std::advance(last, (thread_num - 1) * block_size);
    }
    else {
        last = end;
        block_size = 1;
    }
    std::vector<std::future<void>> futures;
    futures.reserve(thread_num + 1);
    //the first
    for(; begin != last; std::advance(begin, block_size)) {
        futures.emplace_back(std::async(std::launch::async,
        [begin, block_size, &fun] {
            std::for_each(begin, begin + block_size, fun);
        }
        ));
    }
    //the last thread
    futures.emplace_back(std::async(std::launch::async, [begin, end, &fun]{
        std::for_each(begin, end, fun);
    }));
    for(auto& fut : futures) {
        fut.get();
    }
}

//use the task group, give it a list of functions
//use a task group tu run these functions
//these funs need to be return void
template<typename... Funs>
void ParallelInvoke(Funs&&... rest)
{
    TaskGroup group;
    group.run(std::forward<Funs>(rest)...);
    group.wait();
}

//the map-reduce function
//just like the pre parallel foreach function, foreach is for range
//which don't need answer as they are independent
//ParallelReduce function is for which not depentent each other
//so map in different thread, then reduce to get answers
template<typename Range, typename RangeFunc, typename ReduceFunc>
typename std::remove_reference<Range>::type::value_type ParallelReduce(Range&& range,
    typename std::remove_reference<Range>::type::value_type init, RangeFunc&& range_func, ReduceFunc&& reduce_func) {
    auto thread_num = std::thread::hardware_concurrency();
    auto begin = std::begin(std::forward<Range>(range));
    auto end = std::end(std::forward<Range>(range));
    auto block_size = std::distance(begin, end) / thread_num;
    typename std::remove_reference<Range>::type::iterator last = begin;
    if(block_size) {
        std::advance(last, (thread_num - 1) * block_size);
    }
    else {
        last = end;
        block_size = 1;
    }
    typedef typename std::remove_reference<Range>::type::value_type ValueType;
    std::vector<std::future<ValueType>> futures;
    futures.reserve(thread_num + 1);
    //first thread_num - 1 thread
    for(; begin != last; std::advance(begin, block_size)) {
        futures.emplace_back(std::async(std::launch::async,
        [begin, &init, block_size, &range_func] {
            return range_func(begin, begin + block_size, init);
        }
        ));
    }
    //last thread
    futures.emplace_back(std::async(std::launch::async,
        [begin, end, &init, &range_func] {
            return range_func(begin, end, init);
    }));

    //get results
    std::vector<ValueType> results;
    results.reserve(thread_num + 1);
    for(auto& fut : futures) {
        results.emplace_back(fut.get());
    }

    return reduce_func(std::begin(results), std::end(results), init);
}

//the reduce function and range function is the same type
//most situations is like this
template<typename Range, typename ReduceFunc> inline
typename std::remove_reference<Range>::type::value_type ParallelReduce(Range&& range,
    typename std::remove_reference<Range>::type::value_type init, ReduceFunc&& reduce_func) {
    return ParallelReduce<Range, ReduceFunc>(std::forward<Range>(range),
        std::move(init),
        std::forward<ReduceFunc>(reduce_func),
        std::forward<ReduceFunc>(reduce_func));
}

/**
 * Parallel sort
 */
template<typename Iterator, typename Pr>
void ParallelSort(Iterator _begin, Iterator _end, Pr pred) {
    size_t len = _end - _begin;
    const size_t kMinInnerLen = 1024; // the max size
    if (len <= kMinInnerLen) {
        std::sort(_begin, _end, pred);
        return;
    }
    int num_threads = std::thread::hardware_concurrency();
    size_t inner_size = (len + num_threads - 1) / num_threads;
    inner_size = std::max(inner_size, kMinInnerLen);
    num_threads = static_cast<int>((len + inner_size - 1) / inner_size);
    ParallelRange(num_threads, [_begin, pred, inner_size, len](int i) {
        size_t left = inner_size * i;
        size_t right = left + inner_size;
        right = std::min(right, len);
        if(right > left) {
            std::sort(_begin + left, _begin + right, pred);
        }
    });
    // set the buffer to merge the result
    std::vector<typename std::iterator_traits<Iterator>::value_type> temp_buf(len);
    auto buf = temp_buf.begin();
    size_t s = inner_size;
    // recursive merge
    while(s < len) {
        int loop_size = static_cast<int>((len + s * 2 - 1) / (s * 2));
        ParallelRange(loop_size, [s, len, _begin, buf, pred](int i) {
            size_t left = i * 2 * s;
            size_t mid = left + s;
            size_t right = std::min(len, mid + s);
            if (mid >= right) { return; }
            std::copy(_begin + left, _begin + mid, buf + left);
            std::merge(buf + left, buf + mid, _begin + mid, _begin + right, _begin + left, pred);
        });
        s *= 2;
    }
}

// the simple version use <
template<typename Iterator>
void ParallelSort(Iterator _begin, Iterator _end) {
    using vt = typename std::iterator_traits<Iterator>::value_type;
    return ParallelSort(_begin, _end, [](const vt& left, const vt& right) { return left < right; });
}
//// parallel sort
} // wzp

#endif /*LIB_PARALLEL_ALGORITHM_HPP*/
