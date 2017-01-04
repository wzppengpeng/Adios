#ifndef WHERE_ITERATOR_HPP_
#define WHERE_ITERATOR_HPP_


namespace wzp
{

/**
 * Some Traits
 */
template<typename Iterator>
using IteratorType = decltype(**(Iterator*)nullptr);

template<typename Iterator, typename Function>
class WhereIterator {

//self
using Self = WhereIterator<Iterator, Function>;

private:
    Iterator m_begin;
    Iterator m_end;
    Function m_fun;

public:
    /**
     * Constructor, fun is bool function
     */
    WhereIterator(Iterator b, Iterator e, const Function& fun) :
        m_begin(b),
        m_end(e),
        m_fun(fun)
    {
        //need to be the first reach target point
        while(m_begin != m_end && !m_fun(*m_begin)) {
            ++m_begin;
        }
    }

    /**
     * self increament
     */
    Self& operator++() {
        if(m_begin == m_end) return *this;
        ++m_begin;
        while(m_begin != m_end && !m_fun(*m_begin)) {
            ++m_begin;
        }
        return *this;
    }

    /**
     * get data
     */
    IteratorType<Iterator> operator*() {
        return *m_begin;
    }

    /**
     * ==
     */
    bool operator==(const Self& other) const {
        return this->m_begin == other.m_begin;
    }

    /**
     * !=
     */
    bool operator!=(const Self& other) const {
        return this->m_begin != other.m_begin;
    }

};


} //wzp

#endif /*WHERE_ITERATOR_HPP_*/