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
    template <class Key, class T, template<class, class, class...> class Map>
    class bimap {
    private:
        using _data_type = std::vector<std::pair<Key, T>>;
        using _map_type = Map<Key, size_t>;
        using _inverted_map_type = Map<T, size_t>;

    public:
        using key_type = Key;
        using value_type = T;
        
        using iterator = _data_type::const_iterator;
        using const_iterator = _data_type::const_iterator;
        using reverse_iterator = _data_type::const_reverse_iterator;
        using const_reverse_iterator = _data_type::const_reverse_iterator;

    private:
        _data_type _data;
        _map_type _map;
        _inverted_map_type _inverted_map;

        void _update_indexes(const size_t& from) {
            for (size_t i = from; i != _data.size(); ++i) {
                _map[_data[i].first] = i;
                _inverted_map[_data[i].second] = i;
            }
        }

    public:
        bimap() = default;

        bimap(const std::initializer_list<std::pair<Key, T>>& init) {
            insert(init.begin(), init.end());
        }

        bimap(const bimap<Key, T, Map>& other) {
            insert(other.begin(), other.end());
        }

        template<class _Iter>
        bimap(const _Iter& begin, const _Iter& end) {
            insert(begin, end);
        }

        void insert(const std::pair<Key, T>& value) {
            if (_map.contains(value.first) && _data[_map.at(value.first)].second == value.second)
                return;

            // add new pair
            _data.insert(_data.end(), value);

            // get maps iterators
            size_t min_idx = _data.size() - 1;
            if (_map.contains(value.first)) {
                T& value_idx = _map.at(value.first);

                // set min idx
                if (value_idx < min_idx) min_idx = value_idx;

                // erase inverted map
                _inverted_map.erase(_data[value_idx].second);

                // erase data
                _data.erase(_data.begin() + value_idx);

                // erase map
                _map.erase(value.first);
            }

            if (_inverted_map.contains(value.second)) {
                Key& key_idx = _inverted_map.at(value.second);

                // set min idx
                if (key_idx < min_idx) min_idx = key_idx;

                // erase map
                _map.erase(_data[key_idx].first);

                // erase data
                _data.erase(_data.begin() + key_idx);

                // erase inverted map
                _inverted_map.erase(value.second);
            }

            _update_indexes(min_idx);
        }

        template<class _Iter>
        void insert(const _Iter& begin, const _Iter& end) {
            for (_Iter iter = begin; iter != end; ++iter) {
                insert(*iter);
            }
        }

        void erase(const Key& key) {
            if (!_map.contains(key)) return;

            size_t element_offset = _map[key];

            const iterator& itr = _data.begin() + element_offset;
            T value = itr->second;
            _data.erase(itr);
            _map.erase(key);
            _inverted_map.erase(value);

            _update_indexes(element_offset);
        }

        void erase_value(const T& value) {
            if (!_inverted_map.contains(value)) return;

            size_t element_offset = _inverted_map[value];

            const iterator& itr = _data.begin() + element_offset;
            Key key = itr->first;
            _data.erase(itr);
            _inverted_map.erase(value);
            _map.erase(key);

            _update_indexes(element_offset);
        }

        const T& at(const Key& key) const {
            if (!_map.contains(key)) {
                throw std::out_of_range("Key not found.");
            }

            return _data.at(_map.at(key)).second;
        }

        const Key& at_value(const T& value) const {
            if (!_inverted_map.contains(value)) {
                throw std::out_of_range("Key not found.");
            }

            return _data.at(_inverted_map.at(value)).first;
        }

        size_t size() const {
            return _data.size();
        }

        bool empty() const {
            return _data.empty();
        }

        bool contains(const Key& key) const {
            return _map.contains(key);
        }

        bool contains_value(const T& value) const {
            return _inverted_map.contains(value);
        }

        iterator find(const Key& key) {
            auto it = _map.find(key);
            return it != _map.end() ? _data.cbegin() + it->second : _data.cend();
        }

        const_iterator find(const Key& key) const {
            auto it = _map.find(key);
            return it != _map.end() ? _data.cbegin() + it->second : _data.cend();
        }

        iterator find_value(const T& value) {
            auto it = _inverted_map.find(value);
            return it != _inverted_map.end() ? _data.cbegin() + it->second : _data.cend();
        }

        const_iterator find_value(const T& value) const {
            auto it = _inverted_map.find(value);
            return it != _inverted_map.end() ? _data.cbegin() + it->second : _data.cend();
        }

        const T& set_value(const Key& key, const T& value) {
            insert(std::pair<Key, T>{ key, value });
            return at(key);
        }

        const T& set_value(const const_iterator& where, const T& value) {
            return set_value(where->first, value);
        }

        const Key& set_key(const T& value, const Key& key) {
            insert(std::pair<Key, T>{ key, value });
            return at_value(value);
        }

        const Key& set_key(const const_iterator& where, const Key& key) {
            return set_key(where->second, key);
        }

        void clear() {
            _map.clear();
            _inverted_map.clear();
        }

        iterator begin() { return _map.cbegin(); }
        iterator end() { return _map.cend(); }
        const_iterator begin() const { return _map.cbegin(); }
        const_iterator end() const { return _map.cend(); }
        const_iterator cbegin() const { return _map.cbegin(); }
        const_iterator cend() const { return _map.cend(); }
        reverse_iterator rbegin() { return _map.crbegin(); }
        reverse_iterator rend() { return _map.crend(); }
        const_reverse_iterator rbegin() const { return _map.crbegin(); }
        const_reverse_iterator rend() const { return _map.crend(); }
        const_reverse_iterator crbegin() const { return _map.crbegin(); }
        const_reverse_iterator crend() const { return _map.crend(); }

        const T& operator[](const Key& key) const {
            return at(key);
        }

        bool operator==(const bimap<Key, T, Map>& other) const {
            return _map == other._map && _inverted_map == other._inverted_map;
        }

        bool operator!=(const bimap<Key, T, Map>& other) const {
            return _map != other._map || _inverted_map != other._inverted_map;
        }
    };
}