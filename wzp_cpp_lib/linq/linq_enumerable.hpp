#ifndef LINQ_ENUMERABLE_HPP
#define LINQ_ENUMERABLE_HPP

namespace  wzp
{

/**
 * the Class LinqEnumerable is a pair of Iterator of STL Container
 */
template<typename Iterator>
class LinqEnumerable {
public:
    /**
     * the constructor
     */
    LinqEnumerable(Iterator begin_, Iterator end_) : m_begin(begin_), m_end(end_)
    {}

    /**
     * getter functions
     */

    /**
     * begin function to get the m_begin
     * @return begin Iterator
     */
    inline Iterator begin() const { return m_begin; }

    /**
     * [end function to get the end Iterator]
     * @return end iterator
     */
    inline Iterator end() const { return m_end; }


private:
    Iterator m_begin;
    Iterator m_end;
};

} //wzp

#endif /*LINQ_ENUMERABLE_HPP*/