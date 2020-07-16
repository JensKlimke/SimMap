/*
 * functions.h
 *
 * MIT License
 *
 * Copyright (c) 2019 Jens Klimke <jens.klimke@rwth-aachen.de>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef SIMMAP_BASE_SEQUENCE_H
#define SIMMAP_BASE_SEQUENCE_H

#include <algorithm>
#include <vector>
#include "definitions.h"
#include "sequence_types.h"


namespace base {


    /**
     * This class represents a sequence of objects along a one dimensional axis with unique position. Each element has a
     * length defined by its start and the start of the next element. The class provides multiple functions to oragnize and
     * access the elements
     * @tparam T Class to be organized in the sequence
     */
    template<typename T>
    class sequence {

        typedef seq_container_type_t<T> cont_type_t;
        typedef seq_key_t<T> entry_type_t;

        cont_type_t _entries;
        double _end = INFINITY;


    public:

        typedef seq_iterator<T> iterator;
        typedef seq_entry_t<T> Entry;
        typedef seq_entry_ptr_t<T> EntryPtr;


        /**
         * Empties the container
         */
        void clear() {

            _entries.clear();
            _end = INFINITY;

        }


        /**
         * Returns a flag if the container is empty
         * @return
         */
        bool empty() const {

            return _entries.empty();

        }


        /**
         * Returns the number of elements in the container
         * @return No of elements
         */
        size_t size() const {

            return _entries.size();

        }


        /**
         * Sets the length of the sequence. This function simply updates the last position in the s vector.
         * @param len Length to be set
         */
        virtual void length(double len) {

            if(empty()) {
                _end = len;
                return;
            }

            _end = len + _entries.begin()->position;

        }


        /**
         * Returns the length of the sequence
         * @return Length of the sequence
         */
        virtual double length() const {

            if(this->empty())
                return 0.0;

            return _end - _entries.begin()->position;

        }


        /**
         * Inserts an element at the given position.
         * @param s Position to be added
         * @param e Entry to be added
         * @return A pointer of the created element
         */
        T* emplace(double s, T &&e) {

            if (s > _end)
                throw std::invalid_argument("s is out of bounds");

            auto res = _entries.insert(entry_type_t{s, std::move(e)});
            return &res.first->element;

        }


        /**
         * Inserts an element at the given position.
         * @param s Position to be added
         * @param e Entry to be added
         */
        T* push(double s, const T& e) {

            if (s > _end)
                throw std::invalid_argument("s is out of bounds");

            auto res = _entries.insert(entry_type_t{s, e});

            if(!res.second)
                return nullptr;
            else
                return &res.first->element;

        }


        /**
         * Adds an element to the end of the sequence
         * @param ds Length of the added element
         * @param e Element to be added
         */
        T* emplace_back(double ds, T &&e) {

            auto res = _entries.insert(entry_type_t{_end, std::move(e)});
            _end += ds;

            return &res.first->element;

        }


        /**
         * Adds a copy of the given element
         * @param ds Length of the added element
         * @param e Element to be added
         */
        void append(double ds, const T& e) {

            _entries.insert(entry_type_t{_end, e});
            _end += ds;

        }


        /**
         * Returns the begin iterator
         * @return Begin iterator
         */
        iterator begin() const {

            return iterator(&_entries, &_end).begin();

        }


        /**
         * Returns the begin iterator
         * @return Begin iterator
         */
        iterator end() const {

            return iterator(&_entries, &_end).end();

        }


        /**
         * Deletes the given entry
         * @param it Itertor to entry which shall be deleted
         */
        void erase(iterator it) {

            _entries.erase(it.it());

        }


        /**
         * Returns the entry at the front position
         * @return Entry at front position
         */
        Entry front() const {

            if (empty())
                throw std::runtime_error("sequence is empty");

            return *begin();

        }


        /**
         * Returns the entry at the back position
         * @return Entry at back position
         */
        Entry back() const {

            if (empty())
                throw std::runtime_error("sequence is empty");

            // get last element and update position to end position
            auto e = *std::next(begin(), _entries.size() - 1);
            e.position = _end;

            // return entry
            return e;

        }


        /**
         * Returns whether an entry exists at the given position
         * @param s Position
         * @param eps Tolerance
         * @return Flag of existence
         */
        bool exists(double s, double eps = base::EPS_DISTANCE) const {

            return !empty() && s >= front().position - eps && s <= _end + eps;

        }


        /**
         * Checks if a entry exists
         * @param s Position
         * @param element Element
         * @param eps Tolerance for position
         * @return Flag of existence
         */
        bool exists(double s, const T& element) const {

            return exists(s) && atPos(s).element == element;

        }


        /**
         * Returns the entry at the given position
         * @param s Position
         * @return Entry
         */
        Entry atPos(double s) const {

            // TODO: define eps (in template?) to avoid numerical error at beginning or end

            if (empty())
                throw std::runtime_error("sequence is empty");

            if (s > _end || s < startPosition())
                throw std::invalid_argument("s is out of range");

            // find element
            auto it = std::upper_bound(_entries.begin(), _entries.end(), s);

            // if not first one, move one back
            if(it != _entries.begin())
                it = std::next(it, -1);

            // create entry and calculate local position
            auto entry = iterator::it2entry(it, &_entries, _end);
            entry.position = s - entry.position;

            // create entry
            return entry;

        }


        /**
         * Returns the start position of the sequence
         * @return Start position
         */
        double startPosition() const {

            if(empty())
                return _end;

            return _entries.begin()->position;

        }


        /**
         * Returns the end position of the sequence
         * @return Start position
         */
        double endPosition() const {

            if(empty())
                return _end;

            return _end;

        }


        /**
         * Returns the list of the grid points of the sequence.
         * @return Vector of points
         */
        std::vector<double> points() const {

            // create output container
            std::vector<double> ret;
            ret.reserve(this->size() + 1);

            // copy positions from entries
            size_t i = 0;
            for(auto &e : _entries)
                ret.push_back(e.position);

            // set end point
            ret.push_back(_end);

            return ret;

        }


        /**
         * Returns a list of entries of the container
         * @return Vector of entries
         */
        std::vector<EntryPtr> entries() const {

            // create output container
            std::vector<EntryPtr> ret(this->size());

            // copy positions from entries
            size_t i = 0;
            for(auto const &e : *this)
                ret[i++] = EntryPtr{e.position, e.length, &e.element};

            return ret;

        }


    };

}

#endif // SIMMAP_BASE_SEQUENCE_H
