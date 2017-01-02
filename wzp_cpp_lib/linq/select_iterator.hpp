#ifndef SELECT_ITERATOR_HPP
#define SELECT_ITERATOR_HPP

namespace wzp
{

template<typename Iterator, typename Function>
class SelectIterator {

using Self = SelectIterator<Iterator, Function>;

private:
    Iterator m_iterator; //the iterator of STL container
    Function m_fun; // the Function object to store the select function(best is Lambda function or std::function)

public:
    /**
     * COnstructor
     */
    SelectIterator(const Iterator& iter, const Function& f) : m_iterator(iter), m_fun(f)
    {}

    /**
     * self increament operration
     */
    Self& operator++() {
        ++m_iterator;
        return *this;
    }

    /**
     * get the reference of iterator data
     */
    auto operator* () -> decltype(m_fun(*m_iterator)) {
        return m_fun(*m_iterator);
    }

    /**
     * the operator of ==
     */
    bool operator== (const Self& other) const {
        return this->m_iterator == other.m_iterator;
    }

    /**
     * the operator of !=
     */
    bool operator!= (const Self& other) const {
        return this->m_iterator != other.m_iterator;
    }

};


} //wzp

#endif /*SELECT_ITERATOR_HPP*/