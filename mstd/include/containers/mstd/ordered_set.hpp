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
	template<class T>
	class ordered_set {
	public:
		using value_type			 = remove_cvref_t<T>;
		using iterator				 = _MSTD_TYPENAME17 std::vector<value_type>::iterator;
		using const_iterator		 = _MSTD_TYPENAME17 std::vector<value_type>::const_iterator;
		using reverse_iterator		 = _MSTD_TYPENAME17 std::vector<value_type>::reverse_iterator;
		using const_reverse_iterator = _MSTD_TYPENAME17 std::vector<value_type>::const_reverse_iterator;

	private:
		std::vector<value_type> _orderedElements;
		std::unordered_map<value_type, size_t> _elementsMap;

		_MSTD_CONSTEXPR20 void _update_indexes(const size_t from) {
			static_assert(std::is_copy_constructible_v<value_type>, "Is not copy constructible");
				for (size_t i = from; i != _orderedElements.size(); ++i) { _elementsMap[_orderedElements[i]] = i; }
		}

		template<class U>
		_MSTD_CONSTEXPR20 value_type& _insert(const_iterator where, U&& item) {
				if (!contains(item)) {
					size_t whereOffset =
					  std::clamp<size_t>(std::distance(_orderedElements.cbegin(), where), 0, _orderedElements.size());

					_orderedElements.insert(where, std::forward<U>(item));

					_update_indexes(whereOffset);
					return *std::next(_orderedElements.begin(), whereOffset);
				}

			size_t whereOffset =
			  std::clamp<size_t>(std::distance(_orderedElements.cbegin(), where), 0, _orderedElements.size() - 1);
			const size_t& elementOffset = _elementsMap.at(item);

			// remove element
			_orderedElements.erase(std::next(_orderedElements.cbegin(), elementOffset));

			// insert element where
			_orderedElements.insert(std::next(_orderedElements.cbegin(), whereOffset), std::forward<U>(item));

				// update iterators
				if (whereOffset > elementOffset) { _update_indexes(elementOffset); }
				else { _update_indexes(whereOffset); }

			return *std::next(_orderedElements.begin(), whereOffset);
		}

		template<class U>
		_MSTD_CONSTEXPR20 value_type& _insert_back(U&& item) {
			return insert(cend(), std::forward<U>(item));
		}

		template<class U>
		_MSTD_CONSTEXPR20 void _erase(U&& item) {
				if (!contains(item)) { return; }

			size_t elementOffset = _elementsMap.at(item);

			_orderedElements.erase(std::next(_orderedElements.begin(), elementOffset));
			_elementsMap.erase(std::forward<U>(item));

			_update_indexes(elementOffset);
		}

		template<class U>
		[[nodiscard]] _MSTD_CONSTEXPR20 bool _contains(U&& item) const {
		#if _MSTD_HAS_CXX20
			return _elementsMap.contains(std::forward<U>(item));
		#else
			return _elementsMap.find(std::forward<U>(item)) != _elementsMap.end();
		#endif
		}

		template<class U>
		[[nodiscard]] _MSTD_CONSTEXPR20 iterator _find(U&& item) {
			auto it = _elementsMap.find(std::forward<U>(item));
			return it != _elementsMap.end() ? std::next(_orderedElements.begin(), it->second) : _orderedElements.end();
		}

		template<class U>
		[[nodiscard]] _MSTD_CONSTEXPR20 const_iterator _find(U&& item) const {
			auto it = _elementsMap.find(std::forward<U>(item));
			return it != _elementsMap.end() ? std::next(_orderedElements.begin(), it->second) : _orderedElements.end();
		}

	public:
		_MSTD_CONSTEXPR20 ordered_set()								= default;

		_MSTD_CONSTEXPR20 ordered_set(const ordered_set& other)		= default;
		_MSTD_CONSTEXPR20 ordered_set(ordered_set&& other) noexcept = default;

		_MSTD_CONSTEXPR20 ordered_set(std::initializer_list<value_type> init) { insert_back(init.begin(), init.end()); }

		#if _MSTD_HAS_CXX20
		template<mstd::iterator_of<value_type> Iter>
		#else
		template<class Iter, std::enable_if_t<is_iterator_of_v<Iter, value_type>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 ordered_set(Iter begin, Iter end) {
			insert_back(begin, end);
		}

		_MSTD_CONSTEXPR20 ~ordered_set()									   = default;

		_MSTD_CONSTEXPR20 ordered_set& operator=(const ordered_set& other)	   = default;
		_MSTD_CONSTEXPR20 ordered_set& operator=(ordered_set&& other) noexcept = default;

		#if _MSTD_HAS_CXX20
		template<class... Args>
		#else
		template<class... Args, std::enable_if_t<std::is_constructible_v<value_type, Args...>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 value_type& emplace(const_iterator where,
		  Args&&... args) _MSTD_REQUIRES((std::constructible_from<value_type, Args...>)) {
			const size_t whereOffset =
			  std::clamp<size_t>(std::distance(_orderedElements.cbegin(), where), 0, _orderedElements.size());

			_orderedElements.emplace(where, std::forward<Args>(args)...);

			_update_indexes(whereOffset);

			return *std::next(_orderedElements.begin(), whereOffset);
		}

		#if _MSTD_HAS_CXX20
		template<class... Args>
		#else
		template<class... Args, std::enable_if_t<std::is_constructible_v<value_type, Args...>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 value_type& emplace_back(
		  Args&&... args
		) _MSTD_REQUIRES((std::constructible_from<value_type, Args...>)) {
			return emplace(cend(), std::forward<Args>(args)...);
		}

		_MSTD_CONSTEXPR20 value_type& insert(const_iterator where, value_type&& item) { return _insert(where, std::move(item)); }

		_MSTD_CONSTEXPR20 value_type& insert(const_iterator where, const value_type& item) { return _insert(where, item); }

		#if _MSTD_HAS_CXX20
		template<mstd::iterator_of<value_type> Iter>
		#else
		template<class Iter, std::enable_if_t<is_iterator_of_v<Iter, value_type>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 void insert(const_iterator where, Iter begin, Iter end) {
			size_t currWhereOffset = std::distance(_orderedElements.cbegin(), where);
				for (Iter iter = begin; iter != end; ++iter, ++currWhereOffset) {
					currWhereOffset = std::clamp<size_t>(currWhereOffset, 0, _orderedElements.size());
					insert(std::next(_orderedElements.begin(), currWhereOffset), *iter);
				}
		}

		_MSTD_CONSTEXPR20 void insert(const_iterator where, std::initializer_list<value_type> init) {
			insert(where, init.begin(), init.end());
		}

		_MSTD_CONSTEXPR20 value_type& insert_back(const value_type& item) { return _insert_back(item); }

		_MSTD_CONSTEXPR20 value_type& insert_back(value_type&& item) { return _insert_back(std::move(item)); }

		#if _MSTD_HAS_CXX20
		template<mstd::iterator_of<value_type> Iter>
		#else
		template<class Iter, std::enable_if_t<is_iterator_of_v<Iter, value_type>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 void insert_back(Iter begin, Iter end) {
			insert(cend(), begin, end);
		}

		_MSTD_CONSTEXPR20 void insert_back(std::initializer_list<value_type> init) { insert_back(init.begin(), init.end()); }

		_MSTD_CONSTEXPR20 void erase(const value_type& item) { _erase(item); }

		_MSTD_CONSTEXPR20 void erase(value_type&& item) { _erase(std::move(item)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool contains(const value_type& item) const { return _contains(item); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool contains(value_type&& item) const { return _contains(std::move(item)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 iterator find(const value_type& item) { return _find(item); }

		[[nodiscard]] _MSTD_CONSTEXPR20 iterator find(value_type&& item) { return _find(std::move(item)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const_iterator find(const value_type& item) const { return _find(item); }

		// [[nodiscard]] _MSTD_CONSTEXPR20 const_iterator find(value_type&& item) const {
		// 	return _find(std::move(item));
		// }

		[[nodiscard]] _MSTD_CONSTEXPR20 size_t size() const { return _orderedElements.size(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool empty() const { return _orderedElements.empty(); }

		_MSTD_CONSTEXPR20 void clear() {
			_orderedElements.clear();
			_elementsMap.clear();
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 value_type& front() { return _orderedElements.front(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const value_type& front() const { return _orderedElements.front(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 value_type& back() { return _orderedElements.back(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const value_type& back() const { return _orderedElements.back(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 value_type& at(const size_t idx) { return _orderedElements.at(idx); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const value_type& at(const size_t idx) const { return _orderedElements.at(idx); }

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

		[[nodiscard]] _MSTD_CONSTEXPR20 bool operator==(const ordered_set& other) const {
			return _orderedElements == other._orderedElements;
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 bool operator!=(const ordered_set& other) const { return !(*this == other); }

		[[nodiscard]] _MSTD_CONSTEXPR20 value_type& operator[](const size_t idx) { return _orderedElements[idx]; }

		[[nodiscard]] _MSTD_CONSTEXPR20 const value_type& operator[](const size_t idx) const { return _orderedElements[idx]; }
	};
} // namespace mstd
	#endif
#endif