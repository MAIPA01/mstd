/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_BIMAP_HPP_
#define _MSTD_BIMAP_HPP_

#include <mstd/config.hpp>

#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
#else

#include <mstd/containers_types.hpp>
#include <mstd/assert.hpp>

namespace mstd {
    template <class Key, class T, template<class, class, class...> class Map>
    class bimap {
    public:
        using key_type = Key;
        using mapped_type = T;
        using value_type = std::pair<Key, T>;
        using reference = value_type&;
        using const_reference = const value_type&;
        
    private:
        using _data_type = std::vector<value_type>;

    public:
        using size_type = _MSTD_TYPENAME17 _data_type::size_type;
        using difference_type = _MSTD_TYPENAME17 _data_type::difference_type;
        using iterator = _MSTD_TYPENAME17 _data_type::const_iterator;
        using const_iterator = _MSTD_TYPENAME17 _data_type::const_iterator;
        using reverse_iterator = _MSTD_TYPENAME17 _data_type::const_reverse_iterator;
        using const_reverse_iterator = _MSTD_TYPENAME17 _data_type::const_reverse_iterator;

    private:
        using _map_type = Map<Key, size_type>;
        using _inverted_map_type = Map<T, size_type>;

        _data_type _data;
        _map_type _map;
        _inverted_map_type _invertedMap;

        _MSTD_CONSTEXPR20 void _update_indexes(const size_type& from) {
            for (size_type i = from; i != _data.size(); ++i) {
                _map[_data[i].first] = i;
                _invertedMap[_data[i].second] = i;
            }
        }

    public:
        _MSTD_CONSTEXPR20 bimap() = default;

        _MSTD_CONSTEXPR20 bimap(const std::initializer_list<value_type>& init) {
            insert(init.begin(), init.end());
        }

        _MSTD_CONSTEXPR20 bimap(const bimap<Key, T, Map>& other) = default;
        _MSTD_CONSTEXPR20 bimap(bimap<Key, T, Map>&& other) noexcept = default;

#if _MSTD_HAS_CXX20
        template<mstd::iterator Iter>
#else
        template<class Iter, std::enable_if_t<is_iterator_v<Iter>, bool> = true>
#endif
        _MSTD_CONSTEXPR20 bimap(const Iter& begin, const Iter& end) {
            insert(begin, end);
        }

        _MSTD_CONSTEXPR20 ~bimap() = default;

        _MSTD_CONSTEXPR20 bimap& operator=(const bimap& other) = default;
        _MSTD_CONSTEXPR20 bimap& operator=(bimap&& other) noexcept = default;

        _MSTD_CONSTEXPR20 T& emplace(const Key& key, const T& value) {
            return insert(std::make_pair(key, value));
        }

        _MSTD_CONSTEXPR20 T& insert(const value_type& value) {
            if (contains(value.first) && _data[_map.at(value.first)].second == value.second)
                return _data[_map.at(value.first)].second;

            // add new pair
            _data.push_back(value);

            // get maps iterators
            size_t minIdx = _data.size() - 1;
            if (contains(value.first)) {
                size_t valueIdx = _map.at(value.first);

                // set min idx
                minIdx = std::min(minIdx, valueIdx);

                // erase inverted map
                _invertedMap.erase(_data[valueIdx].second);

                // erase data
                _data.erase(std::next(_data.begin(), valueIdx));

                // erase map
                _map.erase(value.first);
            }

            if (contains_value(value.second)) {
                size_t keyIdx = _invertedMap.at(value.second);

                // set min idx
                minIdx = std::min(keyIdx, minIdx);

                // erase map
                _map.erase(_data[keyIdx].first);

                // erase data
                _data.erase(std::next(_data.begin(), keyIdx));

                // erase inverted map
                _invertedMap.erase(value.second);
            }

            _update_indexes(minIdx);

            return _data[_map.at(value.first)].second;
        }

#if _MSTD_HAS_CXX20
        template<mstd::iterator Iter>
#else
        template<class Iter, std::enable_if_t<is_iterator_v<Iter>, bool> = true>
#endif
        _MSTD_CONSTEXPR20 void insert(const Iter& begin, const Iter& end) {
            for (Iter iter = begin; iter != end; ++iter) {
                insert(*iter);
            }
        }

        _MSTD_CONSTEXPR20 void erase(const Key& key) {
            if (!contains(key)) return;

            size_t elementOffset = _map.at(key);

            const iterator& itr = std::next(_data.begin(), elementOffset);
            T value = itr->second;
            _data.erase(itr);
            _map.erase(key);
            _invertedMap.erase(value);

            _update_indexes(elementOffset);
        }

        _MSTD_CONSTEXPR20 void erase_value(const T& value) {
            if (!contains_value(value)) return;

            size_t elementOffset = _invertedMap.at(value);

            const iterator& itr = std::next(_data.begin(), elementOffset);
            Key key = itr->first;
            _data.erase(itr);
            _invertedMap.erase(value);
            _map.erase(key);

            _update_indexes(elementOffset);
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 T& at(const Key& key) {
#if _MSTD_HAS_CXX20
            mstd_assert(contains(key), "Key '{}' not found", key);
#else
            mstd_assert(contains(key), "Key not found");
#endif
            return _data.at(_map.at(key)).second;
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 const T& at(const Key& key) const {
#if _MSTD_HAS_CXX20
            mstd_assert(contains(key), "Key '{}' not found", key);
#else
            mstd_assert(contains(key), "Key not found");
#endif
            return _data.at(_map.at(key)).second;
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 Key& at_value(const T& value) {
#if _MSTD_HAS_CXX20
            mstd_assert(contains_value(value), "Value '{}' not found", value);
#else
            mstd_assert(contains_value(value), "Value not found");
#endif
            return _data.at(_invertedMap.at(value)).first;
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 const Key& at_value(const T& value) const {
#if _MSTD_HAS_CXX20
            mstd_assert(contains_value(value), "Value '{}' not found", value);
#else
            mstd_assert(contains_value(value), "Value not found");
#endif
            return _data.at(_invertedMap.at(value)).first;
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 size_t size() const {
            return _data.size();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 bool empty() const {
            return _data.empty();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 bool contains(const Key& key) const {
#if _MSTD_HAS_CXX20
            return _map.contains(key);
#else
            return _map.find(key) != _map.end();
#endif
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 bool contains_value(const T& value) const {
#if _MSTD_HAS_CXX20
            return _invertedMap.contains(value);
#else
            return _invertedMap.find(value) != _invertedMap.end();
#endif
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 iterator find(const Key& key) {
            auto it = _map.find(key);
            return it != _map.end() ? std::next(_data.begin(), it->second) : _data.end();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator find(const Key& key) const {
            auto it = _map.find(key);
            return it != _map.end() ? std::next(_data.cbegin(), it->second) : _data.cend();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 iterator find_value(const T& value) {
            auto it = _invertedMap.find(value);
            return it != _invertedMap.end() ? std::next(_data.begin(), it->second) : _data.end();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator find_value(const T& value) const {
            auto it = _invertedMap.find(value);
            return it != _invertedMap.end() ? std::next(_data.cbegin(), it->second) : _data.cend();
        }

        _MSTD_CONSTEXPR20 void clear() {
            _data.clear();
            _map.clear();
            _invertedMap.clear();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 iterator begin() { return _data.begin(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 iterator end() { return _data.end(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator begin() const { return _data.cbegin(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator end() const { return _data.cend(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator cbegin() const { return _data.cbegin(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator cend() const { return _data.cend(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 reverse_iterator rbegin() { return _data.rbegin(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 reverse_iterator rend() { return _data.rend(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_reverse_iterator rbegin() const { return _data.crbegin(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_reverse_iterator rend() const { return _data.crend(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_reverse_iterator crbegin() const { return _data.crbegin(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_reverse_iterator crend() const { return _data.crend(); }

        [[nodiscard]] _MSTD_CONSTEXPR20 T& operator[](const Key& key) {
            if (!contains(key)) {
                return emplace(key, T());
            }
            return at(key);
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 const T& operator[](const Key& key) const {
            return at(key);
        }

        _MSTD_CONSTEXPR20 bool operator==(const bimap<Key, T, Map>& other) const {
            return _map == other._map && _invertedMap == other._invertedMap && _data == other._data;
        }

        _MSTD_CONSTEXPR20 bool operator!=(const bimap<Key, T, Map>& other) const {
            return !(*this == other);
        }
    };
}

#endif
#endif