/**
 * Design and implement a data structure for a compressed string iterator. It should support the following operations: next and hasNext.

The given compressed string will be in the form of each letter followed by a positive integer representing the number of this letter existing in the original uncompressed string.

next() - if the original string still has uncompressed characters, return the next letter; Otherwise return a white space.
hasNext() - Judge whether there is any letter needs to be uncompressed.
 */


/**
 * 需要注意两点，复位
 * 二是可能数字有多位，因此先找不是数字的下一位
 */

class StringIterator {
public:
    StringIterator(string compressedString) : m_str(compressedString)
    {
        clear();
        parse_new();
    }

    char next() {
        if(idx >= n) return ' ';
        char r = m_str[idx];
        if(--num == 0) {
            idx = idx_next;
            parse_new();
        }
        return r;
    }

    bool hasNext() {
        return idx < n;
    }

private:
    int idx;
    int idx_next;
    int num;
    int n;
    string m_str;

private:
    //the private method
    inline void clear() {
        idx = 0;
        n = m_str.size();
        num = 0;
        idx_next = 0;
    }

    void parse_new() {
        int i = idx + 1;
        while(i < n && m_str[i] >= '0' && m_str[i] <= '9') ++i;
        if(i > idx + 1) {
            num = std::stoi(m_str.substr(idx + 1, i - idx - 1));
            idx_next = i;
        }
    }

};

/**
 * Your StringIterator object will be instantiated and called as such:
 * StringIterator obj = new StringIterator(compressedString);
 * char param_1 = obj.next();
 * bool param_2 = obj.hasNext();
 */