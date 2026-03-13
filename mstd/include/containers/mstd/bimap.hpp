/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include <mstd/config.hpp>

#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
#else

#include "containers_types.hpp"
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
        using size_type = typename _data_type::size_type;
        using difference_type = typename _data_type::difference_type;
        using iterator = typename _data_type::const_iterator;
        using const_iterator = typename _data_type::const_iterator;
        using reverse_iterator = typename _data_type::const_reverse_iterator;
        using const_reverse_iterator = typename _data_type::const_reverse_iterator;

    private:
        using _map_type = Map<Key, size_type>;
        using _inverted_map_type = Map<T, size_type>;

    private:
        _data_type _data;
        _map_type _map;
        _inverted_map_type _inverted_map;

        _MSTD_CONSTEXPR20 void _update_indexes(const size_type& from) {
            for (size_type i = from; i != _data.size(); ++i) {
                _map[_data[i].first] = i;
                _inverted_map[_data[i].second] = i;
            }
        }

    public:
        _MSTD_CONSTEXPR20 bimap() = default;

        _MSTD_CONSTEXPR20 bimap(const std::initializer_list<value_type>& init) {
            insert(init.begin(), init.end());
        }

        _MSTD_CONSTEXPR20 bimap(const bimap<Key, T, Map>& other) = default;
        _MSTD_CONSTEXPR20 bimap(bimap<Key, T, Map>&& other) noexcept = default;

        template<class _Iter, std::enable_if_t<std::_Is_iterator_v<_Iter>, bool> = true>
        _MSTD_CONSTEXPR20 bimap(const _Iter& begin, const _Iter& end) {
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
            size_t min_idx = _data.size() - 1;
            if (contains(value.first)) {
                size_t value_idx = _map.at(value.first);

                // set min idx
                if (value_idx < min_idx) min_idx = value_idx;

                // erase inverted map
                _inverted_map.erase(_data[value_idx].second);

                // erase data
                _data.erase(std::next(_data.begin(), value_idx));

                // erase map
                _map.erase(value.first);
            }

            if (contains_value(value.second)) {
                size_t key_idx = _inverted_map.at(value.second);

                // set min idx
                if (key_idx < min_idx) min_idx = key_idx;

                // erase map
                _map.erase(_data[key_idx].first);

                // erase data
                _data.erase(std::next(_data.begin(), key_idx));

                // erase inverted map
                _inverted_map.erase(value.second);
            }

            _update_indexes(min_idx);

            return _data[_map.at(value.first)].second;
        }

        template<class _Iter, std::enable_if_t<std::_Is_iterator_v<_Iter>, bool> = true>
        _MSTD_CONSTEXPR20 void insert(const _Iter& begin, const _Iter& end) {
            for (_Iter iter = begin; iter != end; ++iter) {
                insert(*iter);
            }
        }

        _MSTD_CONSTEXPR20 void erase(const Key& key) {
            if (!contains(key)) return;

            size_t element_offset = _map.at(key);

            const iterator& itr = std::next(_data.begin(), element_offset);
            T value = itr->second;
            _data.erase(itr);
            _map.erase(key);
            _inverted_map.erase(value);

            _update_indexes(element_offset);
        }

        _MSTD_CONSTEXPR20 void erase_value(const T& value) {
            if (!contains_value(value)) return;

            size_t element_offset = _inverted_map.at(value);

            const iterator& itr = std::next(_data.begin(), element_offset);
            Key key = itr->first;
            _data.erase(itr);
            _inverted_map.erase(value);
            _map.erase(key);

            _update_indexes(element_offset);
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
            return _data.at(_inverted_map.at(value)).first;
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 const Key& at_value(const T& value) const {
#if _MSTD_HAS_CXX20
            mstd_assert(contains_value(value), "Value '{}' not found", value);
#else
            mstd_assert(contains_value(value), "Value not found");
#endif
            return _data.at(_inverted_map.at(value)).first;
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
            return _inverted_map.contains(value);
#else
            return _inverted_map.find(value) != _inverted_map.end();
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
            auto it = _inverted_map.find(value);
            return it != _inverted_map.end() ? std::next(_data.begin(), it->second) : _data.end();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator find_value(const T& value) const {
            auto it = _inverted_map.find(value);
            return it != _inverted_map.end() ? std::next(_data.cbegin(), it->second) : _data.cend();
        }

        _MSTD_CONSTEXPR20 void clear() {
            _data.clear();
            _map.clear();
            _inverted_map.clear();
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
            return _map == other._map && _inverted_map == other._inverted_map && _data == other._data;
        }

        _MSTD_CONSTEXPR20 bool operator!=(const bimap<Key, T, Map>& other) const {
            return !(*this == other);
        }
    };
}

#endif
