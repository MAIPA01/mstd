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

		#include <mstd/assert.hpp>
		#include <mstd/containers_types.hpp>

namespace mstd {
	template<class Key, class T, template<class, class, class...> class Map>
	class bimap {
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
		using iterator				 = _MSTD_TYPENAME17 _data_type::const_iterator;
		using const_iterator		 = _MSTD_TYPENAME17 _data_type::const_iterator;
		using reverse_iterator		 = _MSTD_TYPENAME17 _data_type::const_reverse_iterator;
		using const_reverse_iterator = _MSTD_TYPENAME17 _data_type::const_reverse_iterator;

	private:
		using _map_type			 = Map<key_type, size_type>;
		using _inverted_map_type = Map<mapped_type, size_type>;

		_data_type _data;
		_map_type _map;
		_inverted_map_type _invertedMap;

		_MSTD_CONSTEXPR20 void _update_indexes(const size_type from) {
				for (size_type i = from; i != _data.size(); ++i) {
					_map[_data[i].first]		  = i;
					_invertedMap[_data[i].second] = i;
				}
		}

		template<class UK, class U>
		_MSTD_CONSTEXPR20 mapped_type& _emplace(UK&& key, U&& value) {
			return _insert(std::make_pair(std::forward<UK>(key), std::forward<U>(value)));
		}

		template<class U>
		_MSTD_CONSTEXPR20 mapped_type& _insert(U&& value) {
			const key_type key			  = value.first;
			const mapped_type mappedValue = value.second;

				if (contains(key) && _data[_map.at(key)].second == mappedValue) { return _data[_map.at(key)].second; }

			// add new pair
			_data.push_back(std::forward<U>(value));

			// get maps iterators
			size_t minIdx = _data.size() - 1;
				if (contains(key)) {
					size_t valueIdx = _map.at(key);

					// set min idx
					minIdx			= std::min(minIdx, valueIdx);

					// erase inverted map
					_invertedMap.erase(_data[valueIdx].second);

					// erase data
					_data.erase(std::next(_data.begin(), valueIdx));

					// erase map
					_map.erase(key);
				}

				if (contains_value(mappedValue)) {
					size_t keyIdx = _invertedMap.at(mappedValue);

					// set min idx
					minIdx		  = std::min(keyIdx, minIdx);

					// erase map
					_map.erase(_data[keyIdx].first);

					// erase data
					_data.erase(std::next(_data.begin(), keyIdx));

					// erase inverted map
					_invertedMap.erase(mappedValue);
				}

			_update_indexes(minIdx);

			return _data[_map.at(key)].second;
		}

		template<class UK>
		_MSTD_CONSTEXPR20 void _erase(UK&& key) {
				if (!contains(key)) { return; }

			size_t elementOffset = _map.at(key);

			const iterator& itr	 = std::next(_data.begin(), elementOffset);
			mapped_type value	 = itr->second;
			_data.erase(itr);
			_map.erase(std::forward<UK>(key));
			_invertedMap.erase(value);

			_update_indexes(elementOffset);
		}

		template<class U>
		_MSTD_CONSTEXPR20 void _erase_value(U&& value) {
				if (!contains_value(value)) { return; }

			size_t elementOffset = _invertedMap.at(value);

			const iterator& itr	 = std::next(_data.begin(), elementOffset);
			key_type key		 = itr->first;
			_data.erase(itr);
			_invertedMap.erase(std::forward<U>(value));
			_map.erase(key);

			_update_indexes(elementOffset);
		}

		template<class UK>
		[[nodiscard]] _MSTD_CONSTEXPR20 mapped_type& _at(UK&& key) {
				if _MSTD_CONSTEXPR17 (fmt::is_formattable<key_type>::value) {
					mstd_assert(contains(key), "Key '{}' not found", key);
				}
				else { mstd_assert(contains(key), "Key not found"); }
			return _data.at(_map.at(std::forward<UK>(key))).second;
		}

		template<class UK>
		[[nodiscard]] _MSTD_CONSTEXPR20 const mapped_type& _at(UK&& key) const {
				if _MSTD_CONSTEXPR17 (fmt::is_formattable<key_type>::value) {
					mstd_assert(contains(key), "Key '{}' not found", key);
				}
				else { mstd_assert(contains(key), "Key not found"); }
			return _data.at(_map.at(std::forward<UK>(key))).second;
		}

		template<class U>
		[[nodiscard]] _MSTD_CONSTEXPR20 key_type& _at_value(U&& value) {
				if _MSTD_CONSTEXPR17 (fmt::is_formattable<mapped_type>::value) {
					mstd_assert(contains_value(value), "Value '{}' not found", value);
				}
				else { mstd_assert(contains_value(value), "Value not found"); }
			return _data.at(_invertedMap.at(std::forward<U>(value))).first;
		}

		template<class U>
		[[nodiscard]] _MSTD_CONSTEXPR20 const key_type& _at_value(U&& value) const {
				if _MSTD_CONSTEXPR17 (fmt::is_formattable<mapped_type>::value) {
					mstd_assert(contains_value(value), "Value '{}' not found", value);
				}
				else { mstd_assert(contains_value(value), "Value not found"); }
			return _data.at(_invertedMap.at(std::forward<U>(value))).first;
		}

		template<class UK>
		[[nodiscard]] _MSTD_CONSTEXPR20 bool _contains(UK&& key) const {
		#if _MSTD_HAS_CXX20
			return _map.contains(std::forward<UK>(key));
		#else
			return _map.find(std::forward<UK>(key)) != _map.end();
		#endif
		}

		template<class U>
		[[nodiscard]] _MSTD_CONSTEXPR20 bool _contains_value(U&& value) const {
		#if _MSTD_HAS_CXX20
			return _invertedMap.contains(std::forward<U>(value));
		#else
			return _invertedMap.find(std::forward<U>(value)) != _invertedMap.end();
		#endif
		}

		template<class UK>
		[[nodiscard]] _MSTD_CONSTEXPR20 key_type& _at_with_construct(UK&& key) {
				if (!contains(key)) { return _emplace(std::forward<UK>(key), mapped_type()); }
			return _at(std::forward<UK>(key));
		}

		template<class UK>
		[[nodiscard]] _MSTD_CONSTEXPR20 iterator _find(UK&& key) {
			auto it = _map.find(std::forward<UK>(key));
			return it != _map.end() ? std::next(_data.begin(), it->second) : _data.end();
		}

		template<class UK>
		[[nodiscard]] _MSTD_CONSTEXPR20 const_iterator _find(UK&& key) const {
			auto it = _map.find(std::forward<UK>(key));
			return it != _map.end() ? std::next(_data.cbegin(), it->second) : _data.cend();
		}

		template<class U>
		[[nodiscard]] _MSTD_CONSTEXPR20 iterator _find_value(U&& value) {
			auto it = _invertedMap.find(std::forward<U>(value));
			return it != _invertedMap.end() ? std::next(_data.begin(), it->second) : _data.end();
		}

		template<class U>
		[[nodiscard]] _MSTD_CONSTEXPR20 const_iterator _find_value(U&& value) const {
			auto it = _invertedMap.find(std::forward<U>(value));
			return it != _invertedMap.end() ? std::next(_data.cbegin(), it->second) : _data.cend();
		}

	public:
		_MSTD_CONSTEXPR20 bimap() = default;

		_MSTD_CONSTEXPR20 bimap(std::initializer_list<value_type> init) { insert(init.begin(), init.end()); }

		_MSTD_CONSTEXPR20 bimap(const bimap& other)		= default;
		_MSTD_CONSTEXPR20 bimap(bimap&& other) noexcept = default;

		#if _MSTD_HAS_CXX20
		template<mstd::iterator_of<value_type> Iter>
		#else
		template<class Iter, std::enable_if_t<is_iterator_of_v<Iter, value_type>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 bimap(Iter begin, Iter end) {
			insert(begin, end);
		}

		_MSTD_CONSTEXPR20 ~bimap()								   = default;

		_MSTD_CONSTEXPR20 bimap& operator=(const bimap& other)	   = default;
		_MSTD_CONSTEXPR20 bimap& operator=(bimap&& other) noexcept = default;

		_MSTD_CONSTEXPR20 mapped_type& emplace(const key_type& key, const mapped_type& value) { return _emplace(key, value); }

		_MSTD_CONSTEXPR20 mapped_type& emplace(key_type&& key, mapped_type&& value) {
			return _emplace(std::move(key), std::move(value));
		}

		_MSTD_CONSTEXPR20 mapped_type& insert(const value_type& value) { return _insert(value); }

		_MSTD_CONSTEXPR20 mapped_type& insert(value_type&& value) { return _insert(std::move(value)); }

		#if _MSTD_HAS_CXX20
		template<mstd::iterator_of<value_type> Iter>
		#else
		template<class Iter, std::enable_if_t<is_iterator_of_v<Iter, value_type>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 void insert(Iter begin, Iter end) {
				for (Iter iter = begin; iter != end; ++iter) { insert(*iter); }
		}

		_MSTD_CONSTEXPR20 void erase(const key_type& key) { _erase(key); }

		_MSTD_CONSTEXPR20 void erase(key_type&& key) { _erase(std::move(key)); }

		_MSTD_CONSTEXPR20 void erase_value(const mapped_type& value) { _erase_value(value); }

		_MSTD_CONSTEXPR20 void erase_value(mapped_type&& value) { _erase_value(std::move(value)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 mapped_type& at(const key_type& key) { return _at(key); }

		[[nodiscard]] _MSTD_CONSTEXPR20 mapped_type& at(key_type&& key) { return _at(std::move(key)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const mapped_type& at(const key_type& key) const { return _at(key); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const mapped_type& at(key_type&& key) const { return _at(std::move(key)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 key_type& at_value(const mapped_type& value) { return _at_value(value); }

		[[nodiscard]] _MSTD_CONSTEXPR20 key_type& at_value(mapped_type&& value) { return _at_value(std::move(value)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const key_type& at_value(const mapped_type& value) const { return _at_value(value); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const key_type& at_value(mapped_type&& value) const {
			return _at_value(std::move(value));
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 size_t size() const { return _data.size(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool empty() const { return _data.empty(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool contains(const key_type& key) const { return _contains(key); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool contains(key_type&& key) const { return _contains(std::move(key)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool contains_value(const mapped_type& value) const { return _contains_value(value); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool contains_value(mapped_type&& value) const {
			return _contains_value(std::move(value));
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 iterator find(const key_type& key) { return _find(key); }

		[[nodiscard]] _MSTD_CONSTEXPR20 iterator find(key_type&& key) { return _find(std::move(key)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const_iterator find(const key_type& key) const { return _find(key); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const_iterator find(key_type&& key) const { return _find(std::move(key)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 iterator find_value(const mapped_type& value) { return _find_value(value); }

		[[nodiscard]] _MSTD_CONSTEXPR20 iterator find_value(mapped_type&& value) { return _find_value(std::move(value)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const_iterator find_value(const mapped_type& value) const { return _find_value(value); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const_iterator find_value(mapped_type&& value) const {
			return _find_value(std::move(value));
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

		_MSTD_CONSTEXPR20 bool operator==(const bimap& other) const {
			return _map == other._map && _invertedMap == other._invertedMap && _data == other._data;
		}

		_MSTD_CONSTEXPR20 bool operator!=(const bimap& other) const { return !(*this == other); }
	};
} // namespace mstd

	#endif
#endif