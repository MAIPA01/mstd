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
        using iterator = _MSTD_TYPENAME17 std::vector<T>::iterator;
        using const_iterator = _MSTD_TYPENAME17 std::vector<T>::const_iterator;
        using reverse_iterator = _MSTD_TYPENAME17 std::vector<T>::reverse_iterator;
        using const_reverse_iterator = _MSTD_TYPENAME17 std::vector<T>::const_reverse_iterator;

    private:
        std::vector<T> _orderedElements;
        std::unordered_map<T, size_t> _elementsMap;

        _MSTD_CONSTEXPR20 void _update_indexes(const size_t& from) {
            for (size_t i = from; i != _orderedElements.size(); ++i) {
                _elementsMap[_orderedElements[i]] = i;
            }
        }

    public:
        _MSTD_CONSTEXPR20 ordered_set() = default;

        _MSTD_CONSTEXPR20 ordered_set(const ordered_set& other) = default;
        _MSTD_CONSTEXPR20 ordered_set(ordered_set&& other) noexcept = default;

        _MSTD_CONSTEXPR20 ordered_set(const std::initializer_list<T>& init) {
            insert_back(init.begin(), init.end());
        }

#if _MSTD_HAS_CXX20
        template<mstd::iterator Iter>
#else
        template<class Iter, std::enable_if_t<is_iterator_v<Iter>, bool> = true>
#endif
        _MSTD_CONSTEXPR20 ordered_set(const Iter& begin, const Iter& end) {
            insert_back(begin, end);
        }

        _MSTD_CONSTEXPR20 ~ordered_set() = default;

        _MSTD_CONSTEXPR20 ordered_set& operator=(const ordered_set& other) = default;
        _MSTD_CONSTEXPR20 ordered_set& operator=(ordered_set&& other) noexcept = default;

        template<class... Args>
        _MSTD_CONSTEXPR20 T& emplace(const const_iterator& where, const Args&... args) {
            size_t whereOffset = std::clamp<size_t>(std::distance(_orderedElements.begin(), where), 0, _orderedElements.size());

            _orderedElements.emplace(where, args...);

            _update_indexes(whereOffset);

            return *std::next(_orderedElements.begin(), where);
        }

        template<class... Args>
        _MSTD_CONSTEXPR20 T& emplace_back(const Args&... args) {
            return emplace(cend(), args...);
        }

        _MSTD_CONSTEXPR20 T& insert(const const_iterator& where, const T& item) {
            if (!contains(item)) {
                size_t whereOffset = std::clamp<size_t>(std::distance(_orderedElements.cbegin(), where), 0, _orderedElements.size());

                _orderedElements.insert(where, item);

                _update_indexes(whereOffset);
                return *std::next(_orderedElements.begin(), whereOffset);
            }

            size_t whereOffset = std::clamp<size_t>(std::distance(_orderedElements.cbegin(), where), 0, _orderedElements.size() - 1);
            const size_t& elementOffset = _elementsMap.at(item);

            // remove element
            _orderedElements.erase(std::next(_orderedElements.cbegin(), elementOffset));

            // insert element where
            _orderedElements.insert(std::next(_orderedElements.cbegin(), whereOffset), item);

            // update iterators
            if (whereOffset > elementOffset) _update_indexes(elementOffset);
            else _update_indexes(whereOffset);

            return *std::next(_orderedElements.begin(), whereOffset);
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

        _MSTD_CONSTEXPR20 T& insert_back(const T& item) {
            return insert(cend(), item);
        }

#if _MSTD_HAS_CXX20
        template<mstd::iterator Iter>
#else
        template<class Iter, std::enable_if_t<is_iterator_v<Iter>, bool> = true>
#endif
        _MSTD_CONSTEXPR20 void insert_back(const Iter& begin, const Iter& end) {
            insert(cend(), begin, end);
        }

        _MSTD_CONSTEXPR20 void erase(const T& item) {
            if (!contains(item)) {
                return;
            }

            size_t elementOffset = _elementsMap[item];

            _orderedElements.erase(std::next(_orderedElements.begin(), elementOffset));
            _elementsMap.erase(item);

            _update_indexes(elementOffset);
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 bool contains(const T& item) const {
#if _MSTD_HAS_CXX20
            return _elementsMap.contains(item);
#else
            return _elementsMap.find(item) != _elementsMap.end();
#endif
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 iterator find(const T& item) {
            auto it = _elementsMap.find(item);
            return it != _elementsMap.end() ? std::next(_orderedElements.begin(), it->second) : _orderedElements.end();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator find(const T& item) const {
            auto it = _elementsMap.find(item);
            return it != _elementsMap.end() ? std::next(_orderedElements.begin(), it->second) : _orderedElements.end();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 size_t size() const {
            return _orderedElements.size();
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 bool empty() const {
            return _orderedElements.empty();
        }

        _MSTD_CONSTEXPR20 void clear() {
            _orderedElements.clear();
            _elementsMap.clear();
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

        [[nodiscard]] _MSTD_CONSTEXPR20 bool operator==(const ordered_set<T>& other) const {
            return _orderedElements == other._orderedElements;
        }

        [[nodiscard]] _MSTD_CONSTEXPR20 bool operator!=(const ordered_set<T>& other) const {
            return !(*this == other);
        }
    };
}
#endif
#endif