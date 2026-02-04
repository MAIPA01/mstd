/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include "containers_types.hpp"

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

        _MSTD_CONSTEXPR20 void _update_indexes(const size_t& from) {
            for (size_t i = from; i != _ordered_elements.size(); ++i) {
                _elements_map[_ordered_elements[i]] = i;
            }
        }

    public:
        _MSTD_CONSTEXPR20 ordered_set() = default;

        _MSTD_CONSTEXPR20 ordered_set(const std::initializer_list<T>& init) {
            insert(this->end(), init.begin(), init.end());
        }

        template<class _Iter>
        _MSTD_CONSTEXPR20 ordered_set(const _Iter& begin, const _Iter& end) {
            insert(this->end(), begin, end);
        }

        _MSTD_CONSTEXPR20 ordered_set(const ordered_set& other) {
            insert(this->end(), other.begin(), other.end());
        }

        _MSTD_CONSTEXPR20 void insert(const const_iterator& where, const T& item) {
            if (_elements_map.contains(item)) {
                size_t where_offset = std::clamp<size_t>(where - _ordered_elements.begin(), 0, _ordered_elements.size() - 1);
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
                size_t where_offset = std::clamp<size_t>(where - _ordered_elements.begin(), 0, _ordered_elements.size());

                _ordered_elements.insert(where, item);

                _update_indexes(where_offset);
            }
        }

        template<class _Iter>
        _MSTD_CONSTEXPR20 void insert(const const_iterator& where, const _Iter& begin, const _Iter& end) {
            size_t curr_where_offset = where - _ordered_elements.begin();
            for (_Iter iter = begin; iter != end; ++iter, ++curr_where_offset) {
                curr_where_offset = std::clamp<size_t>(curr_where_offset, 0, _ordered_elements.size());
                insert(_ordered_elements.begin() + curr_where_offset, *iter);
            }
        }

        _MSTD_CONSTEXPR20 void erase(const T& item) {
            if (_elements_map.contains(item)) {
                size_t element_offset = _elements_map[item];

                _ordered_elements.erase(_ordered_elements.begin() + element_offset);
                _elements_map.erase(item);

                _update_indexes(element_offset);
            }
        }

        _MSTD_CONSTEXPR20 bool contains(const T& item) const {
            return _elements_map.contains(item);
        }

        _MSTD_CONSTEXPR20 iterator find(const T& item) {
            auto it = _elements_map.find(item);
            return it != _elements_map.end() ? _ordered_elements.begin() + it->second : _ordered_elements.end();
        }

        _MSTD_CONSTEXPR20 const_iterator find(const T& item) const {
            auto it = _elements_map.find(item);
            return it != _elements_map.end() ? _ordered_elements.begin() + it->second : _ordered_elements.end();
        }

        _MSTD_CONSTEXPR20 size_t size() const {
            return _ordered_elements.size();
        }

        _MSTD_CONSTEXPR20 bool empty() const {
            return _ordered_elements.empty();
        }

        _MSTD_CONSTEXPR20 void clear() {
            _ordered_elements.clear();
            _elements_map.clear();
        }

        _MSTD_CONSTEXPR20 iterator begin() { return _ordered_elements.begin(); }
        _MSTD_CONSTEXPR20 iterator end() { return _ordered_elements.end(); }
        _MSTD_CONSTEXPR20 const_iterator begin() const { return _ordered_elements.cbegin(); }
        _MSTD_CONSTEXPR20 const_iterator end() const { return _ordered_elements.cend(); }
        _MSTD_CONSTEXPR20 const_iterator cbegin() const { return _ordered_elements.cbegin(); }
        _MSTD_CONSTEXPR20 const_iterator cend() const { return _ordered_elements.cend(); }
        _MSTD_CONSTEXPR20 reverse_iterator rbegin() { return _ordered_elements.rbegin(); }
        _MSTD_CONSTEXPR20 reverse_iterator rend() { return _ordered_elements.rend(); }
        _MSTD_CONSTEXPR20 const_reverse_iterator rbegin() const { return _ordered_elements.crbegin(); }
        _MSTD_CONSTEXPR20 const_reverse_iterator rend() const { return _ordered_elements.crend(); }
        _MSTD_CONSTEXPR20 const_reverse_iterator crbegin() const { return _ordered_elements.crbegin(); }
        _MSTD_CONSTEXPR20 const_reverse_iterator crend() const { return _ordered_elements.crend(); }

        _MSTD_CONSTEXPR20 bool operator==(const ordered_set<T>& other) const {
            return _ordered_elements == other._ordered_elements;
        }

        _MSTD_CONSTEXPR20 bool operator!=(const ordered_set<T>& other) const {
            return !(*this == other);
        }
    };
}