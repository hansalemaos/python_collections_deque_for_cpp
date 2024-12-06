#ifndef PYDEQUE_HPP
#define PYDEQUE_HPP
#include <iostream>
#include <map>
#include <unordered_map>
#include <vector>

namespace pyindex
{
using PY_RANGE = std::pair<int, int>;
using PY_INT_VEC = std::vector<int>;
using PY_SIZE_T = size_t;
using PY_CHR_PTR_LIST = std::pair<PY_INT_VEC, std::string>;
using PY_CHAR_VEC = std::vector<char>;
using PY_TRANSLATION_TABLE = std::unordered_map<unsigned char, unsigned char>;
using PY_STRING_VEC = std::vector<std::string>;
using PY_STRING_DICT_UNORDERED = std::unordered_map<std::string, std::string>;
using PY_STRING_DICT = std::map<std::string, std::string>;
struct PY_SLICE
{
    int start;
    int end;
    int step;
};

} // namespace pyindex

namespace dequehelpers
{
size_t _get_loop_size(size_t max_size, size_t size_other)
{
    if (max_size > size_other)
    {
        return size_other;
    }
    return max_size;
}
template <typename T> void _fill_vec_with_elements(std::vector<T> &v, std::vector<T> &vec_other, size_t loop_size)
{
    for (size_t i{}; i < loop_size; i++)
    {
        v.push_back(vec_other[i]);
    }
}
} // namespace dequehelpers

template <typename T> class SliceObject
{
  public:
    SliceObject(std::vector<T *> slice) : _slice(slice), mysize(slice.size())
    {
    }
    std::vector<T *> _slice;
    size_t mysize;

    SliceObject &operator=(std::vector<T> other)
    {
        if (other.size() == 0)
        {
            throw std::invalid_argument("Other is empty");
        }
        for (size_t i{}; i < mysize; i++)
        {
            *(this->_slice[i]) = (other[i % other.size()]);
        }
        return *this;
    }
    SliceObject &operator=(T other)
    {
        return this->operator=(std::vector<T>{other});
    }
};

template <typename T> class PyDeque
{
  public:
    std::vector<T> v;
    size_t maxlen;
    PyDeque(std::vector<T> value, size_t maxlen) : maxlen(maxlen)
    {
        v.reserve(maxlen);
        dequehelpers::_fill_vec_with_elements(v, value, dequehelpers::_get_loop_size(maxlen, value.size()));
    }
    std::vector<T>::iterator begin()
    {
        return v.begin();
    }
    std::vector<T>::iterator end()
    {
        return v.end();
    }
    std::vector<T>::const_iterator cbegin()
    {
        return v.begin();
    }
    std::vector<T>::const_iterator cend()
    {
        return v.end();
    }
    PyDeque<T> operator+(const T other)
    {
        PyDeque<T> tmpdeque{v, maxlen};
        tmpdeque.append(other);
        return tmpdeque;
    }
    PyDeque<T> operator+(const PyDeque &other)
    {
        PyDeque<T> tmpdeque{v, maxlen};
        tmpdeque.extend(other.v);
        return tmpdeque;
    }
    PyDeque<T> operator+(const std::vector<T> &other)
    {
        PyDeque<T> tmpdeque{v, maxlen};

        tmpdeque.extend(other);
        return tmpdeque;
    }

    PyDeque<T> operator*(size_t other)

    {
        if (other == 0)
        {
            throw std::invalid_argument("Other is 0");
        }
        PyDeque<T> tmpdeque{v, maxlen};
        for (size_t i{}; i < other - 1; i++)
        {
            tmpdeque.extend(v);
        }
        return tmpdeque;
    }
    PyDeque<T> &operator*=(size_t other)
    {
        if (other == 0)
        {
            throw std::invalid_argument("Other is 0");
        }
        PyDeque<T> tmpdeque{v, maxlen};
        for (size_t i{}; i < other - 1; i++)
        {
            tmpdeque.extend(v);
        }
        this->operator=(tmpdeque);
        return *this;
    }

    PyDeque<T> &operator+=(const T other)
    {
        PyDeque<T> tmpdeque{v, maxlen};
        tmpdeque.append(other);
        this->operator=(tmpdeque);
        return *this;
    }
    PyDeque<T> &operator+=(const PyDeque &other)
    {
        PyDeque<T> tmpdeque{v, maxlen};
        tmpdeque.extend(other.v);
        this->operator=(tmpdeque);
        return *this;
    }
    PyDeque<T> &operator+=(const std::vector<T> &other)
    {
        PyDeque<T> tmpdeque{v, maxlen};

        tmpdeque.extend(other);
        this->operator=(tmpdeque);
        return *this;
    }
    PyDeque<T> &operator=(PyDeque<T> other)
    {
        this->v = other.v;
        this->maxlen = other.maxlen;
        return *this;
    }

    PyDeque<T> &operator=(std::vector<T> other)
    {
        this->v = other;
        this->maxlen = other.size();
        return *this;
    }

    void push_back(const T &value)
    {
        if (v.size() < maxlen)
        {
            v.push_back(value);
        }
        else
        {
            v.erase(v.begin());
            v.push_back(value);
        }
    }
    void emplace_back(const T &value)
    {
        if (v.size() < maxlen)
        {
            v.emplace_back(value);
        }
        else
        {
            v.erase(v.begin());
            v.emplace_back(value);
        }
    }
    void extend(const PyDeque &other)
    {
        extend(other.v);
    }
    void extend(const std::vector<T> &other)
    {
        auto it{other.begin()};
        while (it != other.end())
        {
            push_back(*it++);
        }
    }
    void extend_left(const PyDeque &other)
    {
        extend(other.v);
    }

    void extend_left(const std::vector<T> &other)
    {
        auto it{other.begin()};
        while (it != other.end())
        {
            append_left(*it++);
        }
    }
    void append(const T value)
    {
        push_back(value);
    }
    void append_left(const T value)
    {
        _erase_last_if_deque_size_minus_one();
        insert(0, value);
    }
    void insert(const size_t index, const T value)
    {
        _erase_first_if_deque_size_minus_one();
        v.insert(v.begin() + index, value);
    }
    void clear()
    {
        v.clear();
    }
    PyDeque<T> copy()
    {
        return PyDeque<T>(v, maxlen);
    }
    size_t count(const T &value)
    {
        return std::count(v.begin(), v.end(), value);
    }
    operator std::vector<T>()
    {
        return v;
    }

    size_t index(const T &value)
    {
        _raise_if_empty();
        auto it = v.begin();
        while (it != v.end())
        {
            if (*it == value)
            {
                return std::distance(v.begin(), it);
            }
            it++;
        }
        throw std::runtime_error("Value not found");
    }

    std::vector<size_t> get_all_indices(const T &value)
    {
        std::vector<size_t> indices;
        if (v.size() == 0)
        {
            return indices;
        }
        auto it = v.begin();
        while (it != v.end())
        {
            if (*it == value)
            {
                indices.emplace_back(std::distance(v.begin(), it));
            }
            it++;
        }
        return indices;
    }

    T pop()
    {
        _raise_if_empty();
        T value{v.back()};
        v.pop_back();
        return value;
    }
    T pop_left()
    {
        _raise_if_empty();
        T value{v.front()};
        v.erase(v.begin());
        return value;
    }
    void reverse()
    {
        std::reverse(v.begin(), v.end());
    }
    void rotate(size_t n)

    {
        if (v.size() == 0)
        {
            return;
        }
        if (n > v.size())
        {
            n %= v.size();
        }
        std::rotate(v.begin(), v.begin() + n, v.end());
    }
    void change_max_size(const size_t new_max_size)
    {
        maxlen = new_max_size;
        if (v.size() > maxlen)
        {
            v.resize(maxlen);
        }
    }

    T generator()
    {
        _raise_if_empty();
        if (_generator_index >= v.size())
        {
            _generator_index = 0;
        }
        T returnval = v[_generator_index++];
        return returnval;
    }

    std::pair<size_t, T> generator_enumerate()
    {
        _raise_if_empty();
        if (_generator_index >= v.size())
        {
            _generator_index = 0;
        }
        size_t returnindex = _generator_index;
        T returnval = v[_generator_index++];
        return std::make_pair(returnindex, returnval);
    }
    void set_generator_index(const size_t index)
    {
        if (index >= v.size())
        {
            throw std::runtime_error("Index out of range");
        }
        _generator_index = index;
    }
    T &operator[](int index)
    {
        return v[_get_real_index_or_raise(index)];
    }
    SliceObject<T> operator[](pyindex::PY_SLICE &&pyslice)
    {
        int start{_get_real_index_or_raise(pyslice.start)};
        int end{_get_real_index_or_raise(pyslice.end)};
        int step{pyslice.step};
        if ((start >= end) && (step > 0))
        {
            throw std::out_of_range("Start index cannot be greater than end index, "
                                    "when step is positive");
        }
        if ((end >= start) && (step < 0))
        {
            throw std::out_of_range("End index cannot be greater than start index, "
                                    "when step is negative");
        }
        if (step == 0)
        {
            step = 1;
        }
        std::vector<T *> ptrvec;
        if (start < end)
        {

            for (int i{start}; i < end; i += step)
            {
                ptrvec.push_back((T *)&v[i]);
            }
        }
        else
        {
            for (int i{start}; i > end; i -= ::abs(step))
            {
                ptrvec.push_back((T *)&v[i]);
            }
        }
        return SliceObject(ptrvec);
    }

  private:
    size_t _generator_index{};
    void _raise_if_empty()
    {
        if (v.size() == 0)
        {
            throw std::runtime_error("Deque is empty");
        }
    }
    void _erase_first_if_deque_size_minus_one()
    {
        if (v.size() > maxlen - 1)
        {
            v.erase(v.begin());
        }
    }
    void _erase_last_if_deque_size_minus_one()
    {
        if (v.size() > maxlen - 1)
        {
            v.erase(v.end() - 1);
        }
    }
    void _check_if_start_end_valid(int start, int end)
    {
        if (start > end)
        {
            throw std::out_of_range("Start index cannot be greater than end index");
        }
    }
    int _get_real_index_or_raise(int index)
    {
        if (index < 0)
        {
            index = (int)v.size() + index;
        }
        if (index > (int)v.size() - 1)
        {
            throw std::out_of_range("Index out of range");
        }
        return index;
    }
};

template <typename T> std::ostream &operator<<(std::ostream &os, std::vector<T> &v)
{
    if (v.size() == 0)
    {
        os << "[]";
        return os;
    }
    auto it{v.begin()};

    os << '[';
    while (it != v.end() - 1)
    {
        os << *it;
        os << ", ";
        it++;
    }
    os << *it;
    os << ']';

    return os;
}

template <typename T> std::ostream &operator<<(std::ostream &os, PyDeque<T> &thepydeque)
{
    return os << thepydeque.v;
}

#endif
