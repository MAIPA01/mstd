/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_ORDERED_MAP_HPP_
#define _MSTD_ORDERED_MAP_HPP_

#include <mstd/config.hpp>

#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
#else

#include <mstd/containers_types.hpp>
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
        using size_type = _MSTD_TYPENAME17 _data_type::size_type;
        using difference_type = _MSTD_TYPENAME17 _data_type::difference_type;
        using iterator = _MSTD_TYPENAME17 _data_type::iterator;
        using const_iterator = _MSTD_TYPENAME17 _data_type::const_iterator;
        using reverse_iterator = _MSTD_TYPENAME17 _data_type::reverse_iterator;
        using const_reverse_iterator = _MSTD_TYPENAME17 _data_type::const_reverse_iterator;

    private:
        using _map_type = std::unordered_map<Key, size_type>;

        _data_type _orderedElements;
        _map_type _elementsMap;

        _MSTD_CONSTEXPR20 void _update_indexes(const size_type& from) {
            for (size_type i = from; i != _orderedElements.size(); ++i) {
                _elementsMap[_orderedElements[i].first] = i;
            }
        }

    public:
        _MSTD_CONSTEXPR20 ordered_map() = default;

        _MSTD_CONSTEXPR20 ordered_map(const ordered_map<Key, T>& other) = default;
        _MSTD_CONSTEXPR20 ordered_map(ordered_map<Key, T>&& other) noexcept = default;

        _MSTD_CONSTEXPR20 ordered_map(const std::initializer_list<value_type>& init) {
            insert_back(init.begin(), init.end());
        }

#if _MSTD_HAS_CXX20
        template<mstd::iterator Iter>
#else
        template<class Iter, std::enable_if_t<is_iterator_v<Iter>, bool> = true>
#endif
        _MSTD_CONSTEXPR20 ordered_map(const Iter& begin, const Iter& end) {
            insert_back(begin, end);
        }

        _MSTD_CONSTEXPR20 ~ordered_map() = default;

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
                size_type whereOffset = std::distance(_orderedElements.cbegin(), where);

                _orderedElements.insert(where, value);

                _update_indexes(whereOffset);

                return _orderedElements.at(_elementsMap.at(value.first)).second;
            }

            // move key to where and change value (after update iterators in map)
            size_type whereOffset = std::clamp<size_type>(std::distance(_orderedElements.cbegin(), where), 0, _orderedElements.size() - 1);
            const size_type& elemOffset = _elementsMap.at(value.first);

            // remove elem
            _orderedElements.erase(std::next(_orderedElements.begin(), elemOffset));

            // insert new elem
            _orderedElements.insert(std::next(_orderedElements.begin(), whereOffset), value);

            // update iterators in map
            if (whereOffset > elemOffset) _update_indexes(elemOffset);
            else _update_indexes(whereOffset);

            return _orderedElements.at(_elementsMap.at(value.first)).second;
        }

#if _MSTD_HAS_CXX20
        template<mstd::iterator Iter>
#else
        template<class Iter, std::enable_if_t<is_iterator_v<Iter>, bool> = true>
#endif
        _MSTD_CONSTEXPR20 void insert(const const_iterator& where, const Iter& begin, const Iter& end) {
            size_t currWhereOffset = std::distance(_orderedElements.cbegin(), where);
            for (Iter iter = begin; iter != end; ++iter, ++currWhereOffset) {
                currWhereOffset = std::clamp<size_t>(currWhereOffset, 0, _orderedElements.size());
                insert(std::next(_orderedElements.begin(), currWhereOffset), *iter);
            }
        }

        _MSTD_CONSTEXPR20 T& insert_back(const value_type& value) {
            return insert(cend(), value);
        }

#if _MSTD_HAS_CXX20
        template<mstd::iterator Iter>
#else
        template<class Iter, std::enable_if_t<is_iterator_v<Iter>, bool> = true>
#endif
        _MSTD_CONSTEXPR20 void insert_back(const Iter& begin, const Iter& end) {
            insert(cend(), begin, end);
        }

        _MSTD_CONSTEXPR20 void erase(const Key& key) {
            if (!contains(key)) {
                return;
            }

            size_type elementOffset = _elementsMap.at(key);

            _orderedElements.erase(std::next(_orderedElements.begin(), elementOffset));
            _elementsMap.erase(key);

            _update_indexes(elementOffset);
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 T& at(const Key& key) {
#if _MSTD_HAS_CXX20
            mstd_assert(contains(key), "Key '{}' not found", key);
#else
            mstd_assert(contains(key), "Key not found");
#endif
            return _orderedElements.at(_elementsMap.at(key)).second;
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 const T& at(const Key& key) const {
#if _MSTD_HAS_CXX20
            mstd_assert(contains(key), "Key '{}' not found", key);
#else
            mstd_assert(contains(key), "Key not found");
#endif
            return _orderedElements.at(_elementsMap.at(key)).second;
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 size_type size() const {
            return _elementsMap.size();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 bool empty() const {
            return _elementsMap.empty();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 bool contains(const Key& key) const {
#if _MSTD_HAS_CXX20
            return _elementsMap.contains(key);
#else
            return _elementsMap.find(key) != _elementsMap.end();
#endif
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 iterator find(const Key& key) {
            auto it = _elementsMap.find(key);
            return it != _elementsMap.end() ? std::next(_orderedElements.begin(), it->second) : _orderedElements.end();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator find(const Key& key) const {
            auto it = _elementsMap.find(key);
            return it != _elementsMap.end() ? std::next(_orderedElements.begin(), it->second) : _orderedElements.end();
        }

        _MSTD_CONSTEXPR20 void clear() {
            _elementsMap.clear();
            _orderedElements.clear();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 iterator begin() { return _orderedElements.begin(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 iterator end() { return _orderedElements.end(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator begin() const { return _orderedElements.cbegin(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator end() const { return _orderedElements.cend(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator cbegin() const { return _orderedElements.cbegin(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator cend() const { return _orderedElements.cend(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 reverse_iterator rbegin() { return _orderedElements.rbegin(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 reverse_iterator rend() { return _orderedElements.rend(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_reverse_iterator rbegin() const { return _orderedElements.crbegin(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_reverse_iterator rend() const { return _orderedElements.crend(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_reverse_iterator crbegin() const { return _orderedElements.crbegin(); }
        [[nodiscard]] _MSTD_CONSTEXPR20 const_reverse_iterator crend() const { return _orderedElements.crend(); }

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
            return _orderedElements == other._orderedElements && _elementsMap == other._elementsMap;
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 bool operator!=(const ordered_map<Key, T>& other) const {
            return !(*this == other);
        }
    };
}
#endif
#endif