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
    template <class Key, class T>
    class ordered_map {
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
        using iterator = typename _data_type::iterator;
        using const_iterator = typename _data_type::const_iterator;
        using reverse_iterator = typename _data_type::reverse_iterator;
        using const_reverse_iterator = typename _data_type::const_reverse_iterator;

    private:
        using _map_type = std::unordered_map<Key, size_type>;

        _data_type _ordered_elements;
        _map_type _elements_map;

        _MSTD_CONSTEXPR20 void _update_indexes(const size_type& from) {
            for (size_type i = from; i != _ordered_elements.size(); ++i) {
                _elements_map[_ordered_elements[i].first] = i;
            }
        }

    public:
        _MSTD_CONSTEXPR20 ordered_map() = default;

        _MSTD_CONSTEXPR20 ordered_map(const ordered_map<Key, T>& other) = default;
        _MSTD_CONSTEXPR20 ordered_map(ordered_map<Key, T>&& other) noexcept = default;

        _MSTD_CONSTEXPR20 ordered_map(const std::initializer_list<value_type>& init) {
            insert_back(init.begin(), init.end());
        }

        template<class _Iter>
        _MSTD_CONSTEXPR20 ordered_map(const _Iter& begin, const _Iter& end) {
            insert_back(begin, end);
        }

        _MSTD_CONSTEXPR20 ordered_map& operator=(const ordered_map<Key, T>& other) = default;
        _MSTD_CONSTEXPR20 ordered_map& operator=(ordered_map<Key, T>&& other) noexcept = default;

        _MSTD_CONSTEXPR20 T& emplace(const const_iterator& where, const Key& key, const T& value) {
            return insert(where, std::make_pair(key, value));
        }

        _MSTD_CONSTEXPR20 T& emplace_back(const Key& key, const T& value) {
            return emplace(cend(), key, value);
        }

        _MSTD_CONSTEXPR20 T& insert(const const_iterator& where, const value_type& value) {
            if (!contains(value.first)) {
                // insert at where and set value (after update iterators in map)
                size_type where_offset = std::distance(_ordered_elements.cbegin(), where);

                _ordered_elements.insert(where, value);

                _update_indexes(where_offset);

                return _ordered_elements.at(_elements_map.at(value.first)).second;
            }

            // move key to where and change value (after update iterators in map)
            size_type where_offset = std::clamp<size_type>(std::distance(_ordered_elements.cbegin(), where), 0, _ordered_elements.size() - 1);
            const size_type& elem_offset = _elements_map.at(value.first);

            // remove elem
            _ordered_elements.erase(std::next(_ordered_elements.begin(), elem_offset));

            // insert new elem
            _ordered_elements.insert(std::next(_ordered_elements.begin(), where_offset), value);

            // update iterators in map
            if (where_offset > elem_offset) _update_indexes(elem_offset);
            else _update_indexes(where_offset);

            return _ordered_elements.at(_elements_map.at(value.first)).second;
        }

        template<class _Iter>
        _MSTD_CONSTEXPR20 void insert(const const_iterator& where, const _Iter& begin, const _Iter& end) {
            size_t curr_where_offset = std::distance(_ordered_elements.cbegin(), where);
            for (_Iter iter = begin; iter != end; ++iter, ++curr_where_offset) {
                curr_where_offset = std::clamp<size_t>(curr_where_offset, 0, _ordered_elements.size());
                insert(std::next(_ordered_elements.begin(), curr_where_offset), *iter);
            }
        }

        _MSTD_CONSTEXPR20 T& insert_back(const value_type& value) {
            return insert(cend(), value);
        }

        template<class _Iter>
        _MSTD_CONSTEXPR20 void insert_back(const _Iter& begin, const _Iter& end) {
            insert(cend(), begin, end);
        }

        _MSTD_CONSTEXPR20 void erase(const Key& key) {
            if (!contains(key)) {
                return;
            }

            size_type element_offset = _elements_map.at(key);

            _ordered_elements.erase(std::next(_ordered_elements.begin(), element_offset));
            _elements_map.erase(key);

            _update_indexes(element_offset);
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 T& at(const Key& key) {
#if _MSTD_HAS_CXX20
            mstd_assert(contains(key), "Key '{}' not found", key);
#else
            mstd_assert(contains(key), "Key not found");
#endif
            return _ordered_elements.at(_elements_map.at(key)).second;
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 const T& at(const Key& key) const {
#if _MSTD_HAS_CXX20
            mstd_assert(contains(key), "Key '{}' not found", key);
#else
            mstd_assert(contains(key), "Key not found");
#endif
            return _ordered_elements.at(_elements_map.at(key)).second;
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 size_type size() const {
            return _elements_map.size();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 bool empty() const {
            return _elements_map.empty();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 bool contains(const Key& key) const {
#if _MSTD_HAS_CXX20
            return _elements_map.contains(key);
#else
            return _elements_map.find(key) != _elements_map.end();
#endif
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 iterator find(const Key& key) {
            auto it = _elements_map.find(key);
            return it != _elements_map.end() ? std::next(_ordered_elements.begin(), it->second) : _ordered_elements.end();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator find(const Key& key) const {
            auto it = _elements_map.find(key);
            return it != _elements_map.end() ? std::next(_ordered_elements.begin(), it->second) : _ordered_elements.end();
        }

        _MSTD_CONSTEXPR20 void clear() {
            _elements_map.clear();
            _ordered_elements.clear();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 iterator begin() { return _ordered_elements.begin(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 iterator end() { return _ordered_elements.end(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator begin() const { return _ordered_elements.cbegin(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator end() const { return _ordered_elements.cend(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator cbegin() const { return _ordered_elements.cbegin(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator cend() const { return _ordered_elements.cend(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 reverse_iterator rbegin() { return _ordered_elements.rbegin(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 reverse_iterator rend() { return _ordered_elements.rend(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_reverse_iterator rbegin() const { return _ordered_elements.crbegin(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_reverse_iterator rend() const { return _ordered_elements.crend(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_reverse_iterator crbegin() const { return _ordered_elements.crbegin(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_reverse_iterator crend() const { return _ordered_elements.crend(); }

        [[nodiscard]] _MSTD_CONSTEXPR20 T& operator[](const Key& key) {
            if (!contains(key)) {
                return emplace_back(key, T());
            }
            return at(key);
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 const T& operator[](const Key& key) const {
            return at(key);
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 bool operator==(const ordered_map<Key, T>& other) const {
            return _ordered_elements == other._ordered_elements && _elements_map == other._elements_map;
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 bool operator!=(const ordered_map<Key, T>& other) const {
            return !(*this == other);
        }
    };
}
#endif