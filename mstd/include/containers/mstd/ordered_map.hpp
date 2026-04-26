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

namespace mstd {
	template<class Key, class T>
	class ordered_map {
	public:
		using key_type		  = remove_cvref_t<Key>;
		using mapped_type	  = remove_cvref_t<T>;
		using value_type	  = std::pair<key_type, mapped_type>;
		using reference		  = value_type&;
		using const_reference = const value_type&;

	private:
		using _data_type = std::vector<value_type>;

	public:
		using size_type				 = _MSTD_TYPENAME17 _data_type::size_type;
		using difference_type		 = _MSTD_TYPENAME17 _data_type::difference_type;
		using iterator				 = _MSTD_TYPENAME17 _data_type::iterator;
		using const_iterator		 = _MSTD_TYPENAME17 _data_type::const_iterator;
		using reverse_iterator		 = _MSTD_TYPENAME17 _data_type::reverse_iterator;
		using const_reverse_iterator = _MSTD_TYPENAME17 _data_type::const_reverse_iterator;

	private:
		using _map_type = std::unordered_map<key_type, size_type>;

		_data_type _orderedElements;
		_map_type _elementsMap;

		_MSTD_CONSTEXPR20 void _update_indexes(const size_type from) {
			static_assert(std::is_copy_constructible_v<key_type>, "Key is not copy constructible!");
				for (size_type i = from; i != _orderedElements.size(); ++i) { _elementsMap[_orderedElements[i].first] = i; }
		}

		template<class UK, class U>
		_MSTD_CONSTEXPR20 mapped_type& _emplace(const_iterator where, UK&& key, U&& value) {
			return _insert(where, std::make_pair<key_type, mapped_type>(std::forward<UK>(key), std::forward<U>(value)));
		}

		template<class UK, class U>
		_MSTD_CONSTEXPR20 mapped_type& _emplace_back(UK&& key, U&& value) {
			return _emplace(cend(), std::forward<UK>(key), std::forward<U>(value));
		}

		template<class U>
		_MSTD_CONSTEXPR20 mapped_type& _insert(const_iterator where, U&& value) {
			const key_type key = value.first;
				if (!contains(key)) {
					// insert at where and set value (after update iterators in map)
					size_type whereOffset = std::distance(_orderedElements.cbegin(), where);

					_orderedElements.insert(where, std::forward<U>(value));

					_update_indexes(whereOffset);

					return _orderedElements.at(_elementsMap.at(key)).second;
				}

			// move key to where and change value (after update iterators in map)
			size_type whereOffset =
			  std::clamp<size_type>(std::distance(_orderedElements.cbegin(), where), 0, _orderedElements.size() - 1);
			const size_type& elemOffset = _elementsMap.at(key);

			// remove elem
			_orderedElements.erase(std::next(_orderedElements.begin(), elemOffset));

			// insert new elem
			_orderedElements.insert(std::next(_orderedElements.begin(), whereOffset), std::forward<U>(value));

				// update iterators in map
				if (whereOffset > elemOffset) { _update_indexes(elemOffset); }
				else { _update_indexes(whereOffset); }

			return _orderedElements.at(_elementsMap.at(key)).second;
		}

		template<class U>
		_MSTD_CONSTEXPR20 mapped_type& _insert_back(U&& value) {
			return _insert(cend(), std::forward<U>(value));
		}

		template<class UK>
		_MSTD_CONSTEXPR20 void _erase(UK&& key) {
				if (!contains(std::forward<UK>(key))) { return; }

			size_type elementOffset = _elementsMap.at(std::forward<UK>(key));

			_orderedElements.erase(std::next(_orderedElements.begin(), elementOffset));
			_elementsMap.erase(std::forward<UK>(key));

			_update_indexes(elementOffset);
		}

		template<class UK>
		[[nodiscard]] _MSTD_CONSTEXPR20 mapped_type& _at(UK&& key) {
				if _MSTD_CONSTEXPR17 (fmt::is_formattable<key_type>::value) {
					mstd_assert(contains(std::forward<UK>(key)), "Key '{}' not found", std::forward<UK>(key));
				}
				else { mstd_assert(contains(std::forward<UK>(key)), "Key not found"); }
			return _orderedElements.at(_elementsMap.at(std::forward<UK>(key))).second;
		}

		template<class UK>
		[[nodiscard]] _MSTD_CONSTEXPR20 const mapped_type& _at(UK&& key) const {
				if _MSTD_CONSTEXPR17 (fmt::is_formattable<key_type>::value) {
					mstd_assert(contains(std::forward<UK>(key)), "Key '{}' not found", std::forward<UK>(key));
				}
				else { mstd_assert(contains(std::forward<UK>(key)), "Key not found"); }
			return _orderedElements.at(_elementsMap.at(std::forward<UK>(key))).second;
		}

		template<class UK>
		[[nodiscard]] _MSTD_CONSTEXPR20 bool _contains(UK&& key) const {
		#if _MSTD_HAS_CXX20
			return _elementsMap.contains(std::forward<UK>(key));
		#else
			return _elementsMap.find(std::forward<UK>(key)) != _elementsMap.end();
		#endif
		}

		template<class UK>
		[[nodiscard]] _MSTD_CONSTEXPR20 iterator _find(UK&& key) {
			auto it = _elementsMap.find(std::forward<UK>(key));
			return it != _elementsMap.end() ? std::next(_orderedElements.begin(), it->second) : _orderedElements.end();
		}

		template<class UK>
		[[nodiscard]] _MSTD_CONSTEXPR20 const_iterator _find(UK&& key) const {
			auto it = _elementsMap.find(std::forward<UK>(key));
			return it != _elementsMap.end() ? std::next(_orderedElements.begin(), it->second) : _orderedElements.end();
		}

		template<class UK>
		[[nodiscard]] _MSTD_CONSTEXPR20 mapped_type& _at_with_construct(UK&& key) {
			auto it = find(key);
				if (it == end()) { return emplace_back(std::forward<UK>(key), mapped_type()); }
			return it->second;
		}

	public:
		_MSTD_CONSTEXPR20 ordered_map()								= default;

		_MSTD_CONSTEXPR20 ordered_map(const ordered_map& other)		= default;
		_MSTD_CONSTEXPR20 ordered_map(ordered_map&& other) noexcept = default;

		_MSTD_CONSTEXPR20 ordered_map(std::initializer_list<value_type> init) { insert_back(init.begin(), init.end()); }

		#if _MSTD_HAS_CXX20
		template<mstd::iterator_of<value_type> Iter>
		#else
		template<class Iter, std::enable_if_t<is_iterator_of_v<Iter, value_type>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 ordered_map(Iter begin, Iter end) {
			insert_back(begin, end);
		}

		_MSTD_CONSTEXPR20 ~ordered_map()									   = default;

		_MSTD_CONSTEXPR20 ordered_map& operator=(const ordered_map& other)	   = default;
		_MSTD_CONSTEXPR20 ordered_map& operator=(ordered_map&& other) noexcept = default;

		_MSTD_CONSTEXPR20 mapped_type& emplace(const_iterator where, const key_type& key, const mapped_type& value) {
			return _emplace(where, key, value);
		}

		_MSTD_CONSTEXPR20 mapped_type& emplace(const_iterator where, key_type&& key, mapped_type&& value) {
			return _emplace(where, std::move(key), std::move(value));
		}

		_MSTD_CONSTEXPR20 mapped_type& emplace_back(const key_type& key, const mapped_type& value) {
			return _emplace_back(key, value);
		}

		_MSTD_CONSTEXPR20 mapped_type& emplace_back(key_type&& key, mapped_type&& value) {
			return _emplace_back(std::move(key), std::move(value));
		}

		_MSTD_CONSTEXPR20 mapped_type& insert(const_iterator where, const value_type& value) { return _insert(where, value); }

		_MSTD_CONSTEXPR20 mapped_type& insert(const_iterator where, value_type&& value) {
			return _insert(where, std::move(value));
		}

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

		_MSTD_CONSTEXPR20 mapped_type& insert_back(const value_type& value) { return _insert_back(value); }

		_MSTD_CONSTEXPR20 mapped_type& insert_back(value_type&& value) { return _insert_back(std::move(value)); }

		#if _MSTD_HAS_CXX20
		template<mstd::iterator_of<value_type> Iter>
		#else
		template<class Iter, std::enable_if_t<is_iterator_of_v<Iter, value_type>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 void insert_back(Iter begin, Iter end) {
			insert(cend(), begin, end);
		}

		_MSTD_CONSTEXPR20 void insert_back(std::initializer_list<value_type> init) { insert_back(init.begin(), init.end()); }

		_MSTD_CONSTEXPR20 void erase(const key_type& key) { _erase(key); }

		_MSTD_CONSTEXPR20 void erase(key_type&& key) { _erase(std::move(key)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 mapped_type& at(const key_type& key) { return _at(key); }

		[[nodiscard]] _MSTD_CONSTEXPR20 mapped_type& at(key_type&& key) { return _at(std::move(key)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const mapped_type& at(const key_type& key) const { return _at(key); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const mapped_type& at(key_type&& key) const { return _at(std::move(key)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 size_type size() const { return _elementsMap.size(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool empty() const { return _elementsMap.empty(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool contains(const key_type& key) const { return _contains(key); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool contains(key_type&& key) const { return _contains(std::move(key)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 iterator find(const key_type& key) { return _find(key); }

		[[nodiscard]] _MSTD_CONSTEXPR20 iterator find(key_type&& key) { return _find(std::move(key)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const_iterator find(const key_type& key) const { return _find(key); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const_iterator find(key_type&& key) const { return _find(std::move(key)); }

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

		[[nodiscard]] _MSTD_CONSTEXPR20 mapped_type& operator[](const key_type& key) {
				if _MSTD_CONSTEXPR17 (std::is_default_constructible_v<mapped_type>) { return _at_with_construct(key); }
				else { return _at(key); }
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 mapped_type& operator[](key_type&& key) {
				if _MSTD_CONSTEXPR17 (std::is_default_constructible_v<mapped_type>) { return _at_with_construct(std::move(key)); }
				else { return _at(std::move(key)); }
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 const mapped_type& operator[](const key_type& key) const { return _at(key); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const mapped_type& operator[](key_type&& key) const { return _at(std::move(key)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool operator==(const ordered_map& other) const {
			return _orderedElements == other._orderedElements && _elementsMap == other._elementsMap;
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 bool operator!=(const ordered_map& other) const { return !(*this == other); }
	};
} // namespace mstd
	#endif
#endif