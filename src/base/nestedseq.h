/*
 * nestedseq.h
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

#ifndef SIMMAP_BASE_NESTEDSEQ_H
#define SIMMAP_BASE_NESTEDSEQ_H


#include "sequence.h"
#include <algorithm>


namespace base {


    template<typename T>
    struct SubSequence {

        sequence<T> first{};
        sequence<T> second{};

    };


    template <typename T>
    using seqtype_t = sequence<SubSequence<T>>;

    template<typename T>
    class nestedseq {


        seqtype_t<T> _sections{};


    public:

        enum class Side {FIRST, SECOND};

        struct Entry : public sequence<T>::EntryPtr {

            Side side{};

            Entry() = default;

            Entry(const typename sequence<T>::EntryPtr &e, Side sd) : sequence<T>::EntryPtr(e), side(sd) {}

            Entry(double pos, double len, T* e, Side sd) : sequence<T>::EntryPtr(), side(sd) {
                this->position = pos;
                this->length   = len;
                this->element  = e;
            }

            Entry(const typename sequence<T>::Entry &e, Side sd) : sequence<T>::EntryPtr(), side(sd) {
                this->position = e.position;
                this->length   = e.length;
                this->element  = &e.element;
            }

        };

        struct EntryPair {

            Entry first{};
            Entry second{};

        };



        /**
         * Creates an element at the given position
         * @param s Position
         */
        SubSequence<T> *create(double s) {

            // create element
            return _sections.emplace(s, SubSequence<T>{});

        }


        /**
         * Updates the length and the sub-sequence lengths of the sequence
         * @param length Length to be set
         */
        virtual void length(double length) {

            _sections.length(length);

            // iterate over elements and set length
            for(auto const &e : _sections) {

                // update length for first element
                if(!e.element.first.empty())
                    e.element.first.length(e.length);
                else
                    e.element.first.length(INFINITY);

                // update length for second element
                if(!e.element.second.empty())
                    e.element.second.length(e.length);
                else
                    e.element.second.length(INFINITY);

            }

        }


        /**
         * Returns the length of the nested sequence
         * @return Length
         */
        virtual double length() const {

            return _sections.length();

        }


        /**
         * Returns the entries at the given position
         * @param s Position
         * @return cross section entry element
         */
        EntryPair at(double s) const {

            // get first level element
            auto ep = _entry(_sections.atPos(s), s);
            return ep;

        }


        /**
         * Returns the entries at the front of the sequence
         * @return cross section entry element
         */
        EntryPair front() const {

            // get first level element
            auto e = _sections.front();
            return _entry(e, e.position);

        }


        /**
         * Returns the entries at the front of the sequence
         * @return cross section entry element
         */
        EntryPair back() const {

            // get first level element
            auto e = _sections.back();
            return _entry(e, e.position);

        }


        /**
         * Returns a list of the entries in the sub-sequences
         * @return A sorted list of sub-entires
         */
        std::vector<Entry> entries() const {

            // create return container
            std::vector<Entry> ret;

            // iterate over elements
            for(auto e : _sections) {

                // get sub-elements
                for(auto entry : e.element.first.entries())
                    ret.push_back(Entry(entry, Side::FIRST));

                // get sub-elements
                for(auto entry : e.element.second.entries())
                    ret.push_back(Entry(entry, Side::SECOND));

            }

            // sort results
            std::sort(ret.begin(), ret.end(),
                 [](const Entry& a, const Entry& b) -> bool {
                     return a.position < b.position;
                 });

            return ret;

        }


        /**
         * Returns the sequence
         * @return Vector of entries
         */
        const seqtype_t<T> &sections() const {

            return _sections;

        }


    private:

        /**
         * Creates an entry pair by the given entry and position
         * @param entry Entry
         * @param s Position
         * @return Entry pair
         */
        static EntryPair _entry(const typename seqtype_t<T>::Entry& entry, double s) {

            // create cross section
            EntryPair cs{};

            // set first element
            if(!entry.element.first.empty())
                cs.first = Entry(entry.element.first.atPos(s), Side::FIRST);
            else
                cs.first = Entry(INFINITY, INFINITY, nullptr, Side::FIRST);

            // set second element
            if(!entry.element.second.empty())
                cs.second = Entry(entry.element.second.atPos(s), Side::SECOND);
            else
                cs.second = Entry(INFINITY, INFINITY, nullptr, Side::SECOND);

            return cs;

        }

    };

}


#endif // SIMMAP_BASE_NESTEDSEQ_H
