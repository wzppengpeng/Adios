#ifndef TAKE_ITERATOR_HPP_
#define TAKE_ITERATOR_HPP_

namespace wzp
{

/**
 * take first count element of a range
 */
template<typename Iterator>
class TakeIterator {
using Self = TakeIterator<Iterator>;

private:
    Iterator m_iterator;
    Iterator m_end;
    size_t m_current;
    size_t m_count; // the number of element need to take

public:
    /**
     * Constructor
     */
    TakeIterator(Iterator iter, Iterator end_, size_t count) :
        m_iterator(iter),
        m_end(end_),
        m_current(0),
        m_count(count)
    {
        if(m_current == m_count) {
            auto temp = Iterator(m_end);
            m_iterator = std::move(temp);
        }
    }

    /**
     * Self increament
     */
    Self& operator++ () {
        if(++m_current == m_count) {
            auto temp = Iterator(m_end);
            m_iterator = std::move(temp);
        }
        else {
            ++m_iterator;
        }
        return *this;
    }

    /**
     * get element
     */
    auto operator*() -> decltype(*m_iterator) {
        return *m_iterator;
    }

    /**
     * ==
     */
    bool operator== (const Self& other) const {
        return this->m_iterator == other.m_iterator;
    }

    /**
     * !=
     */
    bool operator!= (const Self& other) const {
        return this->m_iterator != other.m_iterator;
    }

};

} //wzp

#endif /*TAKE_ITERATOR_HPP_*/