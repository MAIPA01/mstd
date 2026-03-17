/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_ORDERED_SET_HPP_
#define _MSTD_ORDERED_SET_HPP_

#include <mstd/config.hpp>

#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
#else

#include <mstd/containers_types.hpp>

namespace mstd {
    template <class T>
    class ordered_set {
    public:
        using value_type = T;
        using iterator = typename std::vector<T>::iterator;
        using const_iterator = typename std::vector<T>::const_iterator;
        using reverse_iterator = typename std::vector<T>::reverse_iterator;
        using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator;

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

        _MSTD_CONSTEXPR20 ordered_set(const ordered_set& other) = default;
        _MSTD_CONSTEXPR20 ordered_set(ordered_set&& other) noexcept = default;

        _MSTD_CONSTEXPR20 ordered_set(const std::initializer_list<T>& init) {
            insert_back(init.begin(), init.end());
        }

        template<class _Iter, std::enable_if_t<std::_Is_iterator_v<_Iter>, bool> = true>
        _MSTD_CONSTEXPR20 ordered_set(const _Iter& begin, const _Iter& end) {
            insert_back(begin, end);
        }

        _MSTD_CONSTEXPR20 ~ordered_set() = default;

        _MSTD_CONSTEXPR20 ordered_set& operator=(const ordered_set& other) = default;
        _MSTD_CONSTEXPR20 ordered_set& operator=(ordered_set&& other) noexcept = default;

        template<class... Args>
        _MSTD_CONSTEXPR20 T& emplace(const const_iterator& where, const Args&... args) {
            size_t where_offset = std::clamp<size_t>(std::distance( _ordered_elements.begin(), where), 0, _ordered_elements.size());

            _ordered_elements.emplace(where, args...);

            _update_indexes(where_offset);

            return *std::next(_ordered_elements.begin(), where);
        }

        template<class... Args>
        _MSTD_CONSTEXPR20 T& emplace_back(const Args&... args) {
            return emplace(cend(), args...);
        }

        _MSTD_CONSTEXPR20 T& insert(const const_iterator& where, const T& item) {
            if (!contains(item)) {
                size_t where_offset = std::clamp<size_t>(std::distance(_ordered_elements.cbegin(), where), 0, _ordered_elements.size());

                _ordered_elements.insert(where, item);

                _update_indexes(where_offset);
                return *std::next(_ordered_elements.begin(), where_offset);
            }

            size_t where_offset = std::clamp<size_t>(std::distance(_ordered_elements.cbegin(), where), 0, _ordered_elements.size() - 1);
            const size_t& element_offset = _elements_map.at(item);

            // remove element
            _ordered_elements.erase(std::next(_ordered_elements.cbegin(), element_offset));

            // insert element where
            _ordered_elements.insert(std::next(_ordered_elements.cbegin(), where_offset), item);

            // update iterators
            if (where_offset > element_offset) _update_indexes(element_offset);
            else _update_indexes(where_offset);

            return *std::next(_ordered_elements.begin(), where_offset);
        }

        template<class _Iter, std::enable_if_t<std::_Is_iterator_v<_Iter>, bool> = true>
        _MSTD_CONSTEXPR20 void insert(const const_iterator& where, const _Iter& begin, const _Iter& end) {
            size_t curr_where_offset = std::distance(_ordered_elements.cbegin(), where);
            for (_Iter iter = begin; iter != end; ++iter, ++curr_where_offset) {
                curr_where_offset = std::clamp<size_t>(curr_where_offset, 0, _ordered_elements.size());
                insert(std::next(_ordered_elements.begin(), curr_where_offset), *iter);
            }
        }

        _MSTD_CONSTEXPR20 T& insert_back(const T& item) {
            return insert(cend(), item);
        }

        template<class _Iter, std::enable_if_t<std::_Is_iterator_v<_Iter>, bool> = true>
        _MSTD_CONSTEXPR20 void insert_back(const _Iter& begin, const _Iter& end) {
            insert(cend(), begin, end);
        }

        _MSTD_CONSTEXPR20 void erase(const T& item) {
            if (!contains(item)) {
                return;
            }

            size_t element_offset = _elements_map[item];

            _ordered_elements.erase(std::next(_ordered_elements.begin(), element_offset));
            _elements_map.erase(item);

            _update_indexes(element_offset);
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 bool contains(const T& item) const {
#if _MSTD_HAS_CXX20
            return _elements_map.contains(item);
#else
            return _elements_map.find(item) != _elements_map.end();
#endif
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 iterator find(const T& item) {
            auto it = _elements_map.find(item);
            return it != _elements_map.end() ? std::next(_ordered_elements.begin(), it->second) : _ordered_elements.end();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator find(const T& item) const {
            auto it = _elements_map.find(item);
            return it != _elements_map.end() ? std::next(_ordered_elements.begin(), it->second) : _ordered_elements.end();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 size_t size() const {
            return _ordered_elements.size();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 bool empty() const {
            return _ordered_elements.empty();
        }

        _MSTD_CONSTEXPR20 void clear() {
            _ordered_elements.clear();
            _elements_map.clear();
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

        [[nodiscard]] _MSTD_CONSTEXPR20 bool operator==(const ordered_set<T>& other) const {
            return _ordered_elements == other._ordered_elements;
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 bool operator!=(const ordered_set<T>& other) const {
            return !(*this == other);
        }
    };
}
#endif
#endif