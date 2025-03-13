/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include "containers_libs.hpp"

namespace mstd {
    template <class Key, class T>
    class ordered_map {
    public:
        using key_type = Key;
        using value_type = T;
        using iterator = std::vector<std::pair<Key, T>>::iterator;
        using const_iterator = std::vector<std::pair<Key, T>>::const_iterator;
        using reverse_iterator = std::vector<std::pair<Key, T>>::reverse_iterator;
        using const_reverse_iterator = std::vector<std::pair<Key, T>>::const_reverse_iterator;

    private:
        std::vector<std::pair<Key, T>> _ordered_elements;
        std::unordered_map<Key, size_t> _elements_map;

        void _update_indexes(const size_t& from) {
            for (size_t i = from; i != _ordered_elements.size(); ++i) {
                _elements_map[_ordered_elements[i].first] = i;
            }
        }

    public:
        ordered_map() = default;

        ordered_map(const std::initializer_list<std::pair<Key, T>>& init) {
            insert(this->end(), init.begin(), init.end());
        }

        ordered_map(const ordered_map& other) {
            insert(this->end(), other.begin(), other.end());
        }

        template<class _Iter>
        ordered_map(const _Iter& begin, const _Iter& end) {
            insert(this->end(), begin, end);
        }

        void insert(const const_iterator& where, const std::pair<Key, T>& value) {
            if (_elements_map.contains(value.first)) {
                // move key to where and change value (after update iterators in map)
                
                size_t where_offset = where - _ordered_elements.begin();
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
        void insert(const const_iterator& where, const _Iter& begin, const _Iter& end) {
            size_t curr_where_offset = where - _ordered_elements.begin();
            for (_Iter iter = begin; iter != end; ++iter, ++curr_where_offset) {
                insert(_ordered_elements.begin() + curr_where_offset, *iter);
            }
        }

        void erase(const Key& key) {
            if (_elements_map.contains(key)) {
                size_t element_offset = _elements_map[key];

                _ordered_elements.erase(_ordered_elements.begin() + element_offset);
                _elements_map.erase(key);

                _update_indexes(element_offset);
            }
        }

        T& at(const Key& key) {
            if (!_elements_map.contains(key)) {
                insert(this->end(), { key, T() });
            }
            return _ordered_elements[_elements_map[key]].second;
        }

        const T& at(const Key& key) const {
            if (_elements_map.contains(key)) {
                return _ordered_elements.at(_elements_map.at(key)).second;
            }
            else {
                throw std::out_of_range("Key not found.");
            }
        }

        size_t size() const {
            return _elements_map.size();
        }

        bool empty() const {
            return _elements_map.empty();
        }

        bool contains(const Key& key) const {
            return _elements_map.contains(key);
        }

        iterator find(const Key& key) {
            auto it = _elements_map.find(key);
            return it != _elements_map.end() ? _ordered_elements.begin() + it->second : _ordered_elements.end();
        }

        const_iterator find(const Key& key) const {
            auto it = _elements_map.find(key);
            return it != _elements_map.end() ? _ordered_elements.begin() + it->second : _ordered_elements.end();
        }

        void clear() {
            _elements_map.clear();
            _ordered_elements.clear();
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

        T& operator[](const Key& key) {
            return at(key);
        }

        const T& operator[](const Key& key) const {
            return at(key);
        }

        bool operator==(const ordered_map<Key, T>& other) const {
            /*if (this->size() != other.size()) {
                return false;
            }
            for (const std::pair<Key, T>& element : *this) {
                if (!other.contains(element.first)) {
                    return false;
                }
                if (other[element.first] != element.second) {
                    return false;
                }
            }
            return true;*/
            return _ordered_elements == other._ordered_elements && _elements_map == other._elements_map;
        }

        bool operator!=(const ordered_map<Key, T>& other) const {
            return _ordered_elements != other._ordered_elements || _elements_map != other._elements_map;
        }
    };
}