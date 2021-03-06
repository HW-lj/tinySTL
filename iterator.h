#ifndef MYSTL_ITERATOR_H_
#define MYSTL_ITERATOR_H_

#include "type_traits"
#include <cstddef>

namespace mystl {

// 五种迭代器类型
struct input_ietrator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_ietrator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

//input
template <class T, class Distance>
struct input_ietrator {
    typedef input_ietrator_tag  iterator_category;
    typedef T                   value_type;
    typedef Distance            difference_type;
    typedef T*                  pointer;
    typedef T&                  reference;
};

//output
struct output_ietrator {
    typedef output_ietrator_tag     iterator_category;
    typedef void                    value_type;
    typedef void                    difference_type;
    typedef void                    pointer;
    typedef void                    reference;
};

//forward
template <class T, class Distance>
struct forward_ietrator {
    typedef forward_ietrator_tag    iterator_category;
    typedef T                       value_type;
    typedef Distance                difference_type;
    typedef T*                      pointer;
    typedef T&                      reference;
};

//bidirectional
template <class T, class Distance>
struct bidirectional_ietrator {
    typedef bidirectional_ietrator_tag      iterator_category;
    typedef T                               value_type;
    typedef Distance                        difference_type;
    typedef T*                              pointer;
    typedef T&                              reference;
};

//random_access
template <class T, class Distance>
struct random_access_ietrator {
    typedef random_access_ietrator_tag      iterator_category;
    typedef T                               value_type;
    typedef Distance                        difference_type;
    typedef T*                              pointer;
    typedef T&                              reference;
};

//iterator
template <class Category, class T, class Distance = ptrdiff_t,
          class Pointer = T*, class Reference = T&>
struct iterator {
    typedef Category    iterator_category;
    typedef T           value_type;
    typedef Distance    difference_type;
    typedef Pointer     pointer;
    typedef Reference   reference;
};

// 通过迭代器萃取对应类型
//泛化版本
template <class Iterator>
struct iterator_traits {
    typedef typename Iterator::iterator_category    iterator_category;
    typedef typename Iterator::value_type           value_type;
    typedef typename Iterator::difference_type      difference_type;
    typedef typename Iterator::pointer              pointer;
    typedef typename Iterator::reference            reference;
};

//偏特化，主要是对指针和常指针
template <class T>
struct iterator_traits <T*> {
    typedef random_access_iterator_tag     iterator_category;
    typedef T                              value_type;
    typedef ptrdiff_t                      difference_type;
    typedef T*                             pointer;
    typedef T&                             reference;
};

template <class T>
struct iterator_traits <const T*> {
    typedef random_access_iterator_tag     iterator_category;
    typedef T                              value_type;
    typedef ptrdiff_t                      difference_type;
    typedef const T*                       pointer;
    typedef const T&                       reference;
};

//获取迭代器category
template <class Iter>
inline typename iterator_traits<Iter>::iterator_category
iteraor_category(const Iter&)
{
    typedef typename iterator_traits<Iter>::iterator_category Category;
    return Category();
}

//获取迭代器 distance type
template <class Iter>
inline typename iterator_traits<Iter>::difference_type *
distance_type(const Iter&)
{
    return static_cast<typename iterator_traits<Iter>::difference_type *>(0);
}

//获取迭代器 value type
template<class Iter>
inline typename iterator_traits<Iter>::value_type *
value_type(const Iter&)
{
    return static_cast<typename iterator_traits<Iter>::value_type *>(0);
}

template <class InputIterator>
inline typename iterator_traits<InputIterator>::distance_type
distance(InputIterator first, InputIterator last) {
    typedef typename iterator_traits<InputIterator>::iteraor_category Category;
    return __distance(first, last, Category());
}

template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
__distance(InputIterator first, InputIterator last, input_ietrator_tag) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last) {
        ++first;
        ++n;
    }
    return n;
}

template <class RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type
__distance(RandomAccessIterator first, RandomAccessIterator last,
           random_access_ietrator_tag) {
    return last - first;
}

template <class InputIterator, class Distance>
inline void advance(InputIterator& i, Distance n) {
    __advance(i, n, iteraor_category(i));
}

template <class InputIterator, class Distance>
inline void __advance(InputIterator& i, Distance n, input_ietrator_tag) {
    while (n--)
        ++i;
}

template <class BidrectionalIterator, class Distance>
inline void __advance(BidrectionalIterator& i, Distance n, bidirectional_ietrator_tag) {
    if (n >= 0)
        while (n--)
            ++i;
    else
        while (n++)
            --i;
}

template <class RandomAccessIterator, class Distance>
inline void __advance(RandomAccessIterator&i, Distance n, random_access_iterator_tag) {
    i += n;
}

template <class Iterator>
class reverse_iterator {
protected:
    Iterator current;
public:
    typedef typename iterator_traits<Iterator>::iteraor_category    iteraor_category;
    typedef typename iterator_traits<Iterator>::value_type          value_type;
    typedef typename iterator_traits<Iterator>::difference_type     difference_type;
    typedef typename iterator_traits<Iterator>::pointer             pointer;
    typedef typename iterator_traits<Iterator>::reference           reference;
    
    typedef Iterator iterator_type;
    typedef reverse_iterator<Iterator> Self;
    
public:
    reverse_iterator() {}
    explicit reverse_iterator(iterator_type x) : current(x) {}
    
    reverse_iterator(const Self& x) : current(x.current) {};
    
    iterator_type base() const { return current; } //取出对应的正向迭代器
    reference operator*() const { //实际对应正向迭代器的前一个位置
        Iterator tmp = current;
        return *--tmp;
    }
    
    pointer operator->() const { return &(operator*()); }
    
    Self& operator++() { //++riter
        --current;
        return *this;
    }
    
    Self operator++(int) { //riter++
        Self tmp = *this;
        --current;
        return tmp;
    }
    
    Self& operator--() {
        ++current;
        return *this;
    }
    
    Self operator--(int) {
        Self tmp = *this;
        ++current;
        return tmp;
    }
    
    Self operator+(difference_type n) const {
        return Self(current - n);
    }
    
    Self& operator+(difference_type n) {
        current -= n;
        return *this;
    }
    
    Self operator-(difference_type n) const {
        return Self(current + n);
    }
    
    Self& operator-(difference_type n) {
        current += n;
        return *this;
    }
    
    reference operator[](difference_type n) const { return *(*this + n); }
};

template <class InputIterator, class Distance>
inline void distance(InputIterator first, InputIterator last, Distance& n) {
    __distance(first, last, n, iteraor_category(first));
}

template <class RandomAccessIterator, class Distance>
inline void __distance(RandomAccessIterator first, RandomAccessIterator last,
                       Distance& n, random_access_ietrator_tag) {
    n += last - first;
}

template <class InputIterator, class Distance>
inline void __distance(InputIterator first, InputIterator last,
                       Distance& n, input_ietrator_tag) {
    while (first != last) {
        ++first;
        ++n;
    }
}

}
#endif
