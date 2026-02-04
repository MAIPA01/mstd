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
    template <class Key, class T>
    class ordered_map {
    private:
        using _data_type = std::vector<std::pair<Key, T>>;

    public:
        using key_type = Key;
        using value_type = T;
        using iterator = _data_type::iterator;
        using const_iterator = _data_type::const_iterator;
        using reverse_iterator = _data_type::reverse_iterator;
        using const_reverse_iterator = _data_type::const_reverse_iterator;

    private:
        _data_type _ordered_elements;
        std::unordered_map<Key, size_t> _elements_map;

        _MSTD_CONSTEXPR20 void _update_indexes(const size_t& from) {
            for (size_t i = from; i != _ordered_elements.size(); ++i) {
                _elements_map[_ordered_elements[i].first] = i;
            }
        }

    public:
        _MSTD_CONSTEXPR20 ordered_map() = default;

        _MSTD_CONSTEXPR20 ordered_map(const std::initializer_list<std::pair<Key, T>>& init) {
            insert(this->end(), init.begin(), init.end());
        }

        _MSTD_CONSTEXPR20 ordered_map(const ordered_map<Key, T>& other) {
            insert(this->end(), other.begin(), other.end());
        }

        template<class _Iter>
        _MSTD_CONSTEXPR20 ordered_map(const _Iter& begin, const _Iter& end) {
            insert(this->end(), begin, end);
        }

        _MSTD_CONSTEXPR20 void insert(const const_iterator& where, const std::pair<Key, T>& value) {
            if (_elements_map.contains(value.first)) {
                // move key to where and change value (after update iterators in map)
                size_t where_offset = std::clamp<size_t>(where - _ordered_elements.begin(), 0, _ordered_elements.size() - 1);
                const size_t& elem_offset = _elements_map[value.first];

                // remove elem
                _ordered_elements.erase(_ordered_elements.begin() + elem_offset);

                // insert new elem
                _ordered_elements.insert(_ordered_elements.begin() + where_offset, value);

                // update iterators in map
                if (where_offset > elem_offset) _update_indexes(elem_offset);
                else _update_indexes(where_offset);
            }
            else {
                // insert at where and set value (after update iterators in map)
                size_t where_offset = where - _ordered_elements.begin();

                _ordered_elements.insert(where, value);

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

        _MSTD_CONSTEXPR20 void erase(const Key& key) {
            if (_elements_map.contains(key)) {
                size_t element_offset = _elements_map[key];

                _ordered_elements.erase(_ordered_elements.begin() + element_offset);
                _elements_map.erase(key);

                _update_indexes(element_offset);
            }
        }

        _MSTD_CONSTEXPR20 T& at(const Key& key) {
            if (!_elements_map.contains(key)) {
                insert(this->end(), { key, T() });
            }
            return _ordered_elements[_elements_map[key]].second;
        }

        _MSTD_CONSTEXPR20 const T& at(const Key& key) const {
            if (_elements_map.contains(key)) {
                return _ordered_elements.at(_elements_map.at(key)).second;
            }
            else {
                throw std::out_of_range("Key not found.");
            }
        }

        _MSTD_CONSTEXPR20 size_t size() const {
            return _elements_map.size();
        }

        _MSTD_CONSTEXPR20 bool empty() const {
            return _elements_map.empty();
        }

        _MSTD_CONSTEXPR20 bool contains(const Key& key) const {
            return _elements_map.contains(key);
        }

        _MSTD_CONSTEXPR20 iterator find(const Key& key) {
            auto it = _elements_map.find(key);
            return it != _elements_map.end() ? _ordered_elements.begin() + it->second : _ordered_elements.end();
        }

        _MSTD_CONSTEXPR20 const_iterator find(const Key& key) const {
            auto it = _elements_map.find(key);
            return it != _elements_map.end() ? _ordered_elements.begin() + it->second : _ordered_elements.end();
        }

        _MSTD_CONSTEXPR20 void clear() {
            _elements_map.clear();
            _ordered_elements.clear();
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

        _MSTD_CONSTEXPR20 T& operator[](const Key& key) {
            return at(key);
        }

        _MSTD_CONSTEXPR20 const T& operator[](const Key& key) const {
            return at(key);
        }

        _MSTD_CONSTEXPR20 bool operator==(const ordered_map<Key, T>& other) const {
            return _ordered_elements == other._ordered_elements && _elements_map == other._elements_map;
        }

        _MSTD_CONSTEXPR20 bool operator!=(const ordered_map<Key, T>& other) const {
            return !(*this == other);
        }
    };
}