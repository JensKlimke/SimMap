/*
 * functions.h
 *
 * MIT License
 *
 * Copyright (c) 2019 Jens Klimke <jens.klimke@rwth-aachen.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 *         of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 *         to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *         copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 *         copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *         AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SIMMAP_BASE_SEQUENCE_TYPES_H
#define SIMMAP_BASE_SEQUENCE_TYPES_H


#include <set>
#include <iterator>

namespace base {


    template<typename T>
    struct seq_key_t {
        double position;
        mutable T element;
    };


    template<typename T>
    bool operator< (const base::seq_key_t<T>& lhs, const base::seq_key_t<T>& rhs) { return lhs.position < rhs.position; }

    template<typename T>
    bool operator== (const base::seq_key_t<T>& lhs, const base::seq_key_t<T>& rhs) { return lhs.position == rhs.position; }

    template<typename T>
    bool operator< (const base::seq_key_t<T>& lhs, double rhs) { return lhs.position < rhs; }

    template<typename T>
    bool operator== (const base::seq_key_t<T>& lhs, double rhs) { return lhs.position == rhs; }

    template<typename T>
    bool operator< (double lhs, const base::seq_key_t<T>& rhs) { return lhs < rhs.position; }

    template<typename T>
    bool operator== (double lhs, const base::seq_key_t<T>& rhs) { return lhs == rhs.position; }



    template<typename T>
    struct seq_entry_t {
        double position;
        double length;
        T &element;
    };

    template<typename T>
    struct seq_entry_ptr_t {
        double position;
        double length;
        T *element;
    };

    template<typename T>
    using seq_container_type_t = std::set<seq_key_t<T>>;

    template<typename T>
    using base_iterator_t = std::iterator<std::random_access_iterator_tag, seq_entry_t<T>>;

    template<typename T>
    class seq_iterator : public base_iterator_t<T> {

        typedef seq_container_type_t<T> type_t;

        const type_t *_container;
        const double *_end;
        typename type_t::iterator _it;


    public:


        explicit seq_iterator(const seq_container_type_t<T> *container, const double *length)
            : _container(container), _end(length) { begin(); }

        seq_iterator(const seq_iterator<T>& other)
            : _container(other._container), _end(other._end), _it(other._it) {};

        typename type_t::iterator it() const { return _it; }

        seq_iterator& operator+=(size_t i) { _it = std::next(_it, i); return *this; }
        seq_iterator& operator++() { _it++; return *this; }
        seq_iterator operator++(int) { seq_iterator retval = *this; ++(*this); return retval; }
        bool operator==(seq_iterator other) const { return _container == other._container && _it == other._it; }
        bool operator!=(seq_iterator other) const { return !(*this == other); }

        T& operator-> () const { return _it->element; }
        seq_entry_t<T> operator*() const { return it2entry(_it, _container, *_end); }

        seq_iterator begin() {
            _it = _container->begin();
            return *this;
        }

        seq_iterator end() {
            _it = _container->end();
            return *this;
        }

        static seq_entry_t<T> it2entry(typename type_t::iterator it, const type_t *cont, double len) {

            auto posS = it->position;

            auto itn = std::next(it, 1);
            if(itn != cont->end())
                len = itn->position;

            return seq_entry_t<T>{posS, len - posS, it->element};

        }

    };



} // namespace base

#endif // SIMMAP_BASE_SEQUENCE_TYPES_H
