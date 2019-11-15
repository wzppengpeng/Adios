#ifndef WZP_CPP_LIB_TIL_SERIALIZER_HPP_
#define WZP_CPP_LIB_TIL_SERIALIZER_HPP_

/**
 * 使用模板对不同的类型进行序列化到buffer/stream中
 * 针对不同的类型，使用模板特化进行区分
 * 针对不同的输出方式，使用模板参数-策略模式进行切换，保证相同的read / write 接口即可
 * 可以序列化POD类型以及各种常见STL容器与POD类型的相会嵌套，对于其他特殊的自定义类型
 * 需要侵入式地在类型中自己定义Load(Stream*)和Dump(Stream*)的接口
 */

#include <cstdint>

#include <vector>
#include <string>
#include <map>
#include <set>
#include <list>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <type_traits>


namespace wzp
{

namespace serializer
{

namespace meta
{

// defines the macro some meta programing functions to check for member functions
#ifndef HAS_MEMBRT_FUN
#define HAS_MEMBRT_FUN(Member) \
    template<typename T, typename... Args> \
    struct HasMember##Member { \
    private: \
           template<typename U> static auto check(int) \
                -> decltype(std::declval<U>().Member(std::declval<Args>()...), std::true_type()); \
    \
           template<typename U> static std::false_type check(...); \
    public: \
           enum { value = std::is_same<decltype(check<T>(0)), std::true_type>::value }; \
    };
#endif /*HAS_MEMBRT_FUN*/

// define the Load checker
HAS_MEMBRT_FUN(Load)
HAS_MEMBRT_FUN(Dump)

// check if the T has member function of Load and Dump for Stream*
template<typename T, typename Stream>
struct CheckTypeHasLoadDumpWithStream {
    constexpr static const bool value
        = HasMemberLoad<T, Stream*>::value
          && HasMemberDump<T, Stream*>::value;
};

} // meta

// define the class of Serializer Helper
// each type of serilizer has write and read function to get in the stream
template<typename T, typename Stream>
struct Serializer;

// to serialize the native pod types
template<typename T, typename Stream>
struct NativePODSerializer {
    // write into stream/buffer
    inline static void Write(Stream* stream, const T& val) {
        stream->Write(val);
    }

    // read from the stream
    inline static bool Read(Stream* stream, T* val) {
        return stream->Read(val);
    }

};

// to serialize the type with Load & Dump interface
template<typename T, typename Stream>
struct LoadAndDumpSerializer {
    // write into stream/buffer
    inline static void Write(Stream* stream, const T& val) {
        T& tmp_val = const_cast<T&>(val);
        tmp_val.Dump(stream);
    }

    // read from the stream
    inline static bool Read(Stream* stream, T* val) {
        return val->Load(stream);
    }
};

// dummy class for undefined serialization.
template<typename T, typename Stream>
struct UndefinedSerializer {
};


// the basic type of serialize
template<typename T, typename Stream>
struct Serializer {
    using SerializerHelper
        = typename std::conditional<
            std::is_pod<T>::value,
            NativePODSerializer<T, Stream>,
            typename std::conditional<
                meta::CheckTypeHasLoadDumpWithStream<T, Stream>::value,
                LoadAndDumpSerializer<T, Stream>,
                UndefinedSerializer<T, Stream>
            >::type
        >::type;

    inline static void Write(Stream* stream, const T& val) {
        SerializerHelper::Write(stream, val);
    }

    // read from the stream
    inline static bool Read(Stream* stream, T* val) {
        return SerializerHelper::Read(stream, val);
    }

};

// to serialize types for vector and string
template<typename T, typename Stream, template<typename...> class Array>
struct ArrayLikeSerializer {
    // write the array info into stream
    inline static void Write(Stream* stream, const Array<T>& arr) {
        uint64_t sz = static_cast<uint64_t>(arr.size());
        Serializer<uint64_t, Stream>::Write(stream, sz);
        for (const T& t : arr) {
            Serializer<T, Stream>::Write(stream, t);
        }
    }

    // read the info from stream to array
    inline static bool Read(Stream* stream, Array<T>* arr) {
        uint64_t sz;
        if (!Serializer<uint64_t, Stream>::Read(stream, &sz))
            return false;
        arr->resize(sz);
        for (auto it = arr->begin(); it != arr->end(); ++it) {
            T* val_ptr = &(*it);
            if (!Serializer<T, Stream>::Read(stream, val_ptr))
                return false;
        }
        return true;
    }
};

// the pair type serializer
template<typename TA, typename TB, typename Stream>
struct PairSerilizer {
    inline static void Write(Stream* stream, const std::pair<TA, TB>& val) {
        Serializer<TA, Stream>::Write(stream, val.first);
        Serializer<TB, Stream>::Write(stream, val.second);
    }

    // read from the stream
    inline static bool Read(Stream* stream, std::pair<TA, TB>* val) {
        return Serializer<TA, Stream>::Read(stream, &(val->first))
            && Serializer<TB, Stream>::Read(stream, &(val->second));
    }
};

// the collecttion serializer (map/dict), has insert, begin, end interface
template<typename Container, typename T, typename Stream>
struct CollectionSerializer {
    // collection has no resize interface
    inline static void Write(Stream* stream, const Container& data) {
        std::vector<T> virtual_data(data.begin(), data.end());
        Serializer<std::vector<T>, Stream>::Write(stream, virtual_data);
    }

    inline static bool Read(Stream* stream, Container* data) {
        std::vector<T> virtual_data;
        if (!Serializer<std::vector<T>, Stream>::Read(stream, &virtual_data))
            return false;
        data->clear();
        data->insert(virtual_data.begin(), virtual_data.end());
        return true;
    }
};

template<typename T, typename Stream>
struct Serializer<std::vector<T>, Stream> {
    inline static void Write(Stream* stream, const std::vector<T>& val) {
        ArrayLikeSerializer<T, Stream, std::vector>::Write(stream, val);
    }

    // read from the stream
    inline static bool Read(Stream* stream, std::vector<T>* val) {
        return ArrayLikeSerializer<T, Stream, std::vector>::Read(stream, val);
    }
};

template<typename T, typename Stream>
struct Serializer<std::basic_string<T>, Stream> {
    inline static void Write(Stream* stream, const std::basic_string<T>& val) {
        ArrayLikeSerializer<T, Stream, std::basic_string>::Write(stream, val);
    }

    // read from the stream
    inline static bool Read(Stream* stream, std::basic_string<T>* val) {
        return ArrayLikeSerializer<T, Stream, std::basic_string>::Read(stream, val);
    }
};

template<typename T, typename Stream>
struct Serializer<std::list<T>, Stream> {
    inline static void Write(Stream* stream, const std::list<T>& val) {
        ArrayLikeSerializer<T, Stream, std::list>::Write(stream, val);
    }

    // read from the stream
    inline static bool Read(Stream* stream, std::list<T>* val) {
        return ArrayLikeSerializer<T, Stream, std::list>::Read(stream, val);
    }
};

template<typename T, typename Stream>
struct Serializer<std::deque<T>, Stream> {
    inline static void Write(Stream* stream, const std::deque<T>& val) {
        ArrayLikeSerializer<T, Stream, std::deque>::Write(stream, val);
    }

    // read from the stream
    inline static bool Read(Stream* stream, std::deque<T>* val) {
        return ArrayLikeSerializer<T, Stream, std::deque>::Read(stream, val);
    }
};

template<typename TA, typename TB, typename Stream>
struct Serializer<std::pair<TA, TB>, Stream> {
    inline static void Write(Stream* stream, const std::pair<TA, TB>& val) {
        PairSerilizer<TA, TB, Stream>::Write(stream, val);
    }

    // read from the stream
    inline static bool Read(Stream* stream, std::pair<TA, TB>* val) {
        return PairSerilizer<TA, TB, Stream>::Read(stream, val);
    }
};

// --- the part of collections
template<typename K, typename V, typename Stream>
struct Serializer<std::map<K, V>, Stream>
    : public CollectionSerializer<std::map<K, V>, std::pair<K, V>, Stream> {
};

template<typename K, typename V, typename Stream>
struct Serializer<std::multimap<K, V>, Stream>
    : public CollectionSerializer<std::multimap<K, V>, std::pair<K, V>, Stream> {
};

template<typename K, typename V, typename Stream>
struct Serializer<std::unordered_map<K, V>, Stream>
    : public CollectionSerializer<std::unordered_map<K, V>, std::pair<K, V>, Stream> {
};

template<typename K, typename V, typename Stream>
struct Serializer<std::unordered_multimap<K, V>, Stream>
    : public CollectionSerializer<std::unordered_multimap<K, V>, std::pair<K, V>, Stream> {
};

template<typename T, typename Stream>
struct Serializer<std::set<T>, Stream>
    : public CollectionSerializer<std::set<T>, T, Stream> {
};

template<typename T, typename Stream>
struct Serializer<std::multiset<T>, Stream>
    : public CollectionSerializer<std::multiset<T>, T, Stream> {
};

template<typename T, typename Stream>
struct Serializer<std::unordered_set<T>, Stream>
    : public CollectionSerializer<std::unordered_set<T>, T, Stream> {
};

template<typename T, typename Stream>
struct Serializer<std::unordered_multiset<T>, Stream>
    : public CollectionSerializer<std::unordered_multiset<T>, T, Stream> {
};

// ------- help functions -------
// define the help function to get the T and Stream
template<typename T, typename Stream>
inline void Serialize(Stream* stream, const T& val) {
    Serializer<T, Stream>::Write(stream, val);
}

// help function to read from stream or buffer
template<typename T, typename Stream>
inline bool Deserialize(Stream* stream, T* val) {
    return Serializer<T, Stream>::Read(stream, val);
}

} //serializer

} //wzp

#endif /*WZP_CPP_LIB_TIL_SERIALIZER_HPP_*/