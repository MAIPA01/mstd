/*
 * mstd - Maipa's Standard Library
 *
 * BSD 3-Clause License with Attribution Requirement
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions, and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software must
 *    display the following acknowledgment:
 *    - "This product includes software developed by Patryk Antosik (MAIPA01) as
 *      part of the mstd project."
 * 4. If you modify this software and distribute it, you must include a notice
 *    stating that the original work was created by Patryk Antosik (MAIPA01) as
 *    part of the mstd project.
 * 5. Neither the name of the author nor the name mstd may be used to endorse or
 *    promote products derived from this software without specific prior written
 *    permission.
 * 6. Sale of unmodified code: The unmodified version of this software may not be
 *    sold without the explicit permission of the author.
 * 7. Sale of modified code: Modified versions of the code may be sold, provided that:
 *    - Proper attribution to Patryk Antosik (MAIPA01) as the original author is maintained.
 *    - The modified code is clearly marked as a modified version.
 *    - The modified code must be accompanied by documentation that includes
 *      references to the original version and its author.
 * 8. Exception for large projects: If this code is used as part of a larger project
 *    (e.g., an application, framework, or other libraries), permission is not required
 *    for the sale of the unmodified version, as long as:
 *    - The project is more than just a wrapper for the original code.
 *    - The code is an integral part of the larger project, and the project does not
 *      merely involve selling the original code.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once
#include "containers_libs.hpp"

namespace mstd {
    template <class T>
    class ordered_set {
    public:
        using value_type = T;
        using iterator = std::vector<T>::iterator;
        using const_iterator = std::vector<T>::const_iterator;
        using reverse_iterator = std::vector<T>::reverse_iterator;
        using const_reverse_iterator = std::vector<T>::const_reverse_iterator;

    private:
        std::vector<T> _ordered_elements;
        std::unordered_map<T, size_t> _elements_map;

        void _update_indexes(const size_t& from) {
            for (size_t i = from; i != _ordered_elements.size(); ++i) {
                _elements_map[_ordered_elements[i]] = i;
            }
        }

    public:
        ordered_set() = default;

        ordered_set(const std::initializer_list<T>& init) {
            insert(this->end(), init.begin(), init.end());
        }

        template<class _Iter>
        ordered_set(const _Iter& begin, const _Iter& end) {
            insert(this->end(), begin, end);
        }

        ordered_set(const ordered_set& other) {
            insert(this->end(), other.begin(), other.end());
        }

        void insert(const const_iterator& where, const T& item) {
            if (_elements_map.contains(item)) {
                size_t where_offset = where - _ordered_elements.begin();
                const size_t& element_offset = _elements_map[item];

                // remove element
                _ordered_elements.erase(_ordered_elements.begin() + element_offset);

                // insert element where
                _ordered_elements.insert(_ordered_elements.begin() + where_offset, item);

                // update iterators
                if (where_offset > element_offset) _update_indexes(element_offset);
                else _update_indexes(where_offset);
            }
            else {
                size_t where_offset = where - _ordered_elements.begin();

                _ordered_elements.insert(where, item);

                _update_indexes(where_offset);
            }
        }

        template<class _Iter>
        void insert(const const_iterator& where, const _Iter& begin, const _Iter& end) {
            size_t curr_where_offset = where - _ordered_elements.begin();
            for (_Iter iter = begin; iter != end; ++iter, ++curr_where_offset) {
                insert(_ordered_elements.begin() + curr_where_offset, *iter);
            }
        }

        void erase(const T& item) {
            if (_elements_map.contains(item)) {
                size_t element_offset = _elements_map[item];

                _ordered_elements.erase(_ordered_elements.begin() + element_offset);
                _elements_map.erase(item);

                _update_indexes(element_offset);
            }
        }

        bool contains(const T& item) const {
            return _elements_map.contains(item);
        }

        iterator find(const T& item) {
            auto it = _elements_map.find(item);
            return it != _elements_map.end() ? it->second : _ordered_elements.end();
        }

        const_iterator find(const T& item) const {
            auto it = _elements_map.find(item);
            return it != _elements_map.end() ? it->second : _ordered_elements.end();
        }

        size_t size() const {
            return _ordered_elements.size();
        }

        bool empty() const {
            return _ordered_elements.empty();
        }

        void clear() {
            _ordered_elements.clear();
            _elements_map.clear();
        }

        iterator begin() { return _ordered_elements.begin(); }
        iterator end() { return _ordered_elements.end(); }
        const_iterator begin() const { return _ordered_elements.cbegin(); }
        const_iterator end() const { return _ordered_elements.cend(); }
        const_iterator cbegin() const { return _ordered_elements.cbegin(); }
        const_iterator cend() const { return _ordered_elements.cend(); }
        reverse_iterator rbegin() { return _ordered_elements.rbegin(); }
        reverse_iterator rend() { return _ordered_elements.rend(); }
        const_reverse_iterator rbegin() const { return _ordered_elements.crbegin(); }
        const_reverse_iterator rend() const { return _ordered_elements.crend(); }
        const_reverse_iterator crbegin() const { return _ordered_elements.crbegin(); }
        const_reverse_iterator crend() const { return _ordered_elements.crend(); }

        bool operator==(const ordered_set<T>& other) const {
            return _ordered_elements == other._ordered_elements;
        }

        bool operator!=(const ordered_set<T>& other) const {
            return _ordered_elements != other._ordered_elements;
        }
    };
}