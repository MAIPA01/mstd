/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#ifndef _MSTD_STABLE_VECTOR_HPP_
	#define _MSTD_STABLE_VECTOR_HPP_

	#include <mstd/config.hpp>

	#if !_MSTD_HAS_CXX17
_MSTD_WARNING("this is only available for c++17 and greater!");
	#else

		#include <mstd/assert.hpp>
		#include <mstd/containers_types.hpp>

namespace mstd {
	template<class T>
	class stable_vector {
	public:
		using value_type	  = remove_cvref_t<T>;
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
		using _to_data_type = std::vector<size_type>;
		using _to_id_type	= std::vector<size_type>;

		_to_data_type _toData;
		_to_id_type _toId;
		_data_type _data;

		_MSTD_CONSTEXPR20 void _append_indexes(size_type count) {
			size_type newSize = _toData.size() + count;

			_toId.reserve(newSize);
			_toData.reserve(newSize);

				for (size_type i = _toData.size(); i < newSize; ++i) {
					_toData.push_back(i);
					_toId.push_back(i);
				}
		}

		template<class U>
		_MSTD_CONSTEXPR20 iterator _insert(U&& value) {
				if (_data.size() == size()) { _append_indexes(1); }
			return _data.insert(_data.cend(), std::forward<U>(value));
		}

		template<class U>
		_MSTD_CONSTEXPR20 iterator _insert_at(const size_type id, U&& value) {
				if (has_value(id)) {
					_data[_toData[id]] = std::forward<U>(value);
					return std::next(_data.begin(), _toData[id]);
				}

			_append_indexes(id + 1 - size());

			// insert data
			auto iter = _data.insert(_data.cend(), std::forward<U>(value));

				if (_data.size() != size()) {
					size_type toSwapId = _toId[_data.size() - 1];

					// swap ids
					std::swap(_toId[_data.size() - 1], _toId[id]);

					// swap indexes
					std::swap(_toData[toSwapId], _toData[id]);
				}

			return iter;
		}

		template<class U>
		_MSTD_CONSTEXPR20 iterator _insert_at(const_iterator pos, U&& value) {
			return _insert_at(_toId[std::distance(_data.cbegin(), pos)], std::forward<U>(value));
		}

		template<class U>
		_MSTD_CONSTEXPR20 void _push_back(U&& value) {
			_insert(std::forward<U>(value));
		}

		template<class U>
		_MSTD_CONSTEXPR20 void _resize(const size_type count, U&& value) {
			_append_indexes(count - size());
			_data.resize(count, std::forward<U>(value));
		}

	public:
		_MSTD_CONSTEXPR20 stable_vector()						  = default;

		_MSTD_CONSTEXPR20 stable_vector(const stable_vector&)	  = default;
		_MSTD_CONSTEXPR20 stable_vector(stable_vector&&) noexcept = default;

		_MSTD_CONSTEXPR20 explicit stable_vector(const size_type count) { resize(count); }

		_MSTD_CONSTEXPR20 stable_vector(const size_type count, const value_type& value) { resize(count, value); }

		_MSTD_CONSTEXPR20 stable_vector(const size_type count, value_type&& value) { resize(count, std::move(value)); }

		_MSTD_CONSTEXPR20 stable_vector(std::initializer_list<value_type> init) : _data(init) { _append_indexes(_data.size()); }

		#if _MSTD_HAS_CXX20
		template<mstd::iterator_of<value_type> Iter>
		#else
		template<class Iter, std::enable_if_t<is_iterator_of_v<Iter, value_type>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 stable_vector(Iter begin, Iter end) : _data(begin, end) {
			_append_indexes(_data.size());
		}

		_MSTD_CONSTEXPR20 ~stable_vector()									 = default;

		_MSTD_CONSTEXPR20 stable_vector& operator=(const stable_vector&)	 = default;
		_MSTD_CONSTEXPR20 stable_vector& operator=(stable_vector&&) noexcept = default;

		#pragma region INSERT_ON_FREE_SPOT

		_MSTD_CONSTEXPR20 iterator insert(const value_type& value) { return _insert<const value_type&>(value); }

		_MSTD_CONSTEXPR20 iterator insert(value_type&& value) { return _insert(std::move(value)); }

		_MSTD_CONSTEXPR20 iterator insert(const value_type& value, const size_type count) {
			_append_indexes(count - (size() - _data.size()));
			return _data.insert(_data.cend(), count, value);
		}

		#if _MSTD_HAS_CXX20
		template<mstd::iterator_of<value_type> Iter>
		#else
		template<class Iter, std::enable_if_t<is_iterator_of_v<Iter, value_type>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 iterator insert(Iter first, Iter last) {
			size_type count = std::distance(first, last);
			_append_indexes(count - (size() - _data.size()));
			return _data.insert(_data.cend(), first, last);
		}

		_MSTD_CONSTEXPR20 iterator insert(std::initializer_list<value_type> init) { return insert(init.begin(), init.end()); }

		#pragma endregion INSERT_ON_FREE_SPOT

		#pragma region INSERT_AT_IDX

		_MSTD_CONSTEXPR20 iterator insert_at(const size_type id, const value_type& value) { return _insert_at(id, value); }

		_MSTD_CONSTEXPR20 iterator insert_at(const size_type id, value_type&& value) { return _insert_at(id, std::move(value)); }

		_MSTD_CONSTEXPR20 iterator insert_at(const size_type id, const value_type& value, const size_type count) {
				for (size_t i = 0; i < count; ++i) { _insert_at(id + i, value); }
			return std::next(_data.begin(), _toData[id]);
		}

		#if _MSTD_HAS_CXX20
		template<mstd::iterator_of<value_type> Iter>
		#else
		template<class Iter, std::enable_if_t<is_iterator_of_v<Iter, value_type>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 iterator insert_at(size_type id, Iter first, Iter last) {
				for (Iter it = first; it != last; ++it, ++id) { insert_at(id, *it); }
			return std::next(_data.begin(), _toData[id]);
		}

		_MSTD_CONSTEXPR20 iterator insert_at(const size_type id, std::initializer_list<value_type> init) {
			return insert_at(id, init.begin(), init.end());
		}

		#pragma endregion INSERT_AT_IDX

		#pragma region INSERT_AT_ITER

		_MSTD_CONSTEXPR20 iterator insert_at(const_iterator pos, const value_type& value) { return _insert_at(pos, value); }

		_MSTD_CONSTEXPR20 iterator insert_at(const_iterator pos, value_type&& value) { return _insert_at(pos, std::move(value)); }

		_MSTD_CONSTEXPR20 iterator insert_at(const_iterator pos, const value_type& value, const size_type count) {
			return _insert_at(_toId[std::distance(_data.cbegin(), pos)], value, count);
		}

		#if _MSTD_HAS_CXX20
		template<mstd::iterator_of<value_type> Iter>
		#else
		template<class Iter, std::enable_if_t<is_iterator_of_v<Iter, value_type>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 iterator insert_at(const_iterator pos, Iter first, Iter last) {
			return insert_at(_toId[std::distance(_data.cbegin(), pos)], first, last);
		}

		_MSTD_CONSTEXPR20 iterator insert_at(const_iterator pos, std::initializer_list<value_type> init) {
			return insert_at(pos, init.begin(), init.end());
		}

		#pragma endregion INSERT_AT_ITER

		#pragma region EMPLACE_ON_FREE_SPOT

		#if _MSTD_HAS_CXX20
		template<class... Args>
		#else
		template<class... Args, std::enable_if_t<std::is_constructible_v<value_type, Args...>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 iterator emplace(Args&&... args) _MSTD_REQUIRES((std::constructible_from<value_type, Args...>)) {
			return insert(value_type(std::forward<Args>(args)...));
		}

		#pragma endregion

		#pragma region EMPLACE_AT_IDX

		#if _MSTD_HAS_CXX20
		template<class... Args>
		#else
		template<class... Args, std::enable_if_t<std::is_constructible_v<value_type, Args...>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 iterator emplace_at(const size_type id,
		  Args&&... args) _MSTD_REQUIRES((std::constructible_from<value_type, Args...>)) {
			return insert_at(id, value_type(std::forward<Args>(args)...));
		}

		#pragma endregion

		#pragma region EMPLACE_AT_ITER

		#if _MSTD_HAS_CXX20
		template<class... Args>
		#else
		template<class... Args, std::enable_if_t<std::is_constructible_v<value_type, Args...>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 iterator emplace_at(const_iterator pos,
		  Args&&... args) _MSTD_REQUIRES((std::constructible_from<value_type, Args...>)) {
			return insert_at(pos, value_type(std::forward<Args>(args)...));
		}

		#pragma endregion

		#if _MSTD_HAS_CXX20
		template<class... Args>
		#else
		template<class... Args, std::enable_if_t<std::is_constructible_v<value_type, Args...>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 value_type& emplace_back(
		  Args&&... args
		) _MSTD_REQUIRES((std::constructible_from<value_type, Args...>)) {
			return *emplace(std::forward<Args>(args)...);
		}

		_MSTD_CONSTEXPR20 void push_back(const value_type& value) { _push_back(value); }

		_MSTD_CONSTEXPR20 void push_back(value_type&& value) { _push_back(std::move(value)); }

		_MSTD_CONSTEXPR20 iterator erase(const size_type id) {
			mstd_assert(id < size(), "Index out of bounds");
				if (!has_value(id)) { return _data.end(); }

			// get data index
			size_t index	 = _toData[id];
			size_t lastIndex = _data.size() - 1;

			// swap data
			std::swap(_data[index], _data[lastIndex]);

			// swap ids
			std::swap(_toId[index], _toId[lastIndex]);

			// erase last item
			auto itr = _data.erase(std::prev(_data.cend()));

			// update indexes
			std::swap(_toData[_toId[index]], _toData[id]);

				// change size if it is possible
				if (_toData[lastIndex] == lastIndex) {
					_toData.erase(std::prev(_toData.cend()));
					_toId.erase(std::prev(_toId.cend()));
				}

			return itr;
		}

		_MSTD_CONSTEXPR20 iterator erase(iterator pos) {
			mstd_assert(pos == _data.end(), "Pos out of bounds");
			return erase(std::distance(_data.begin(), pos));
		}

		_MSTD_CONSTEXPR20 iterator erase(const_iterator pos) {
			mstd_assert(pos == _data.cend(), "Pos out of bounds");
			return erase(std::distance(_data.cbegin(), pos));
		}

		_MSTD_CONSTEXPR20 iterator erase(iterator first, iterator last) {
			iterator iter;
				for (iterator it = first; it != last; ++it) { iter = erase(it); }
			return iter;
		}

		_MSTD_CONSTEXPR20 iterator erase(const_iterator first, const_iterator last) {
			iterator iter;
				for (const_iterator it = first; it != last; ++it) { iter = erase(it); }
			return iter;
		}

		_MSTD_CONSTEXPR20 void reserve(const size_type capacity) {
			_toData.reserve(capacity);
			_toId.reserve(capacity);
			_data.reserve(capacity);
		}

		_MSTD_CONSTEXPR20 void resize(const size_type count) {
			_append_indexes(count - size());
			_data.resize(count);
		}

		_MSTD_CONSTEXPR20 void resize(const size_type count, const value_type& value) { _resize(count, value); }

		_MSTD_CONSTEXPR20 void resize(const size_type count, value_type&& value) { _resize(count, std::move(value)); }

		_MSTD_CONSTEXPR20 void swap(stable_vector& other) noexcept {
			_data.swap(other._data);
			_toId.swap(other._toId);
			_toData.swap(other._toData);
		}

		_MSTD_CONSTEXPR20 void clear() {
			_toData.clear();
			_toId.clear();
			_data.clear();
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 value_type& front() { return _data.front(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const value_type& front() const { return _data.front(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 value_type& back() { return _data.back(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const value_type& back() const { return _data.back(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 iterator get(const size_type id) {
			mstd_assert(id < size(), "Index out of bounds");
			mstd_assert(has_value(id), "Index is a pointer to empty element");

			return std::next(begin(), _toData[id]);
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 const_iterator get(const size_type id) const {
			mstd_assert(id < size(), "Index out of bounds");
			mstd_assert(has_value(id), "Index is a pointer to empty element");

			return std::next(cbegin(), _toData[id]);
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 iterator try_get(const size_type id) {
			mstd_assert(id < size(), "Index out of bounds");
				if (has_value(id)) { return get(id); }
			return end();
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 const_iterator try_get(const size_type id) const {
			mstd_assert(id < size(), "Index out of bounds");
				if (has_value(id)) { return get(id); }
			return cend();
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 value_type& at(const size_type id) {
			mstd_assert(id < size(), "Index out of bounds");
			mstd_assert(has_value(id), "Index is a pointer to empty element");

			return _data[_toData[id]];
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 const value_type& at(const size_type id) const {
			mstd_assert(id < size(), "Index out of bounds");
			mstd_assert(has_value(id), "Index is a pointer to empty element");

			return _data[_toData[id]];
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 value_type* try_at(const size_type id) {
			mstd_assert(id < size(), "Index out of bounds");
				if (has_value(id)) { return &at(id); }
			return nullptr;
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 const value_type* try_at(const size_type id) const {
			mstd_assert(id < size(), "Index out of bounds");
				if (has_value(id)) { return &at(id); }
			return nullptr;
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 size_type front_id() const { return get_id(0); }

		[[nodiscard]] _MSTD_CONSTEXPR20 size_type back_id() const { return get_id(active_slots() - 1); }

		[[nodiscard]] _MSTD_CONSTEXPR20 size_type get_next_id() const {
				if (empty_slots() != 0) { return get_id(active_slots()); }
			return size();
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 size_type get_id(const size_type dataIndex) const {
			mstd_assert(dataIndex < size(), "Data index out of bounds");
			return _toId[dataIndex];
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 size_type get_id(iterator pos) {
			mstd_assert(pos != _data.end(), "Pos out of bounds");
			return get_id(std::distance(_data.begin(), pos));
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 size_type get_id(const_iterator pos) const {
			mstd_assert(pos != _data.cend(), "Pos out of bounds");
			return get_id(std::distance(_data.cbegin(), pos));
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 size_type active_slots() const { return _data.size(); }

		[[nodiscard]] _MSTD_CONSTEXPR17 size_type empty_slots() const { return size() - active_slots(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 size_type size() const { return _toData.size(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 size_type capacity() const { return _toData.capacity(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 size_type max_size() const { return _data.max_size(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool empty() const { return _data.empty(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool has_value(const size_type id) const {
			return id < size() && _toData[id] < _data.size();
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 bool has_value(iterator pos) const { return has_value(get_id(pos)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool has_value(const_iterator pos) const { return has_value(get_id(pos)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 iterator begin() { return _data.begin(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const_iterator begin() const { return _data.begin(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const_iterator cbegin() const { return _data.cbegin(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 iterator end() { return _data.end(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const_iterator end() const { return _data.end(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const_iterator cend() const { return _data.cend(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 reverse_iterator rbegin() { return _data.rbegin(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const_reverse_iterator rbegin() const { return _data.rbegin(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 reverse_iterator rend() { return _data.rend(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const_reverse_iterator rend() const { return _data.rend(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const_reverse_iterator crbegin() const { return _data.crbegin(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const_reverse_iterator crend() const { return _data.crend(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 value_type& operator[](const size_type id) { return at(id); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const value_type& operator[](const size_type id) const { return at(id); }

		[[nodiscard]] value_type* data() { return _data.data(); }

		[[nodiscard]] const value_type* data() const { return _data.data(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool operator==(const stable_vector& other) const {
			return _data == other._data && _toId == other._toId && _toData == other._toData;
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 bool operator!=(const stable_vector& other) const { return !(*this == other); }

		#if _MSTD_HAS_CXX20
		[[nodiscard]] _MSTD_CONSTEXPR20 auto operator<=>(const stable_vector& other) const {
			return _data <=> other._data && _toId <=> other._toId && _toData <=> other._toData;
		}
		#else
		[[nodiscard]] _MSTD_CONSTEXPR20 bool operator<(const stable_vector& other) const {
			return _data < other._data && _toId < other._toId && _toData < other._toData;
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 bool operator<=(const stable_vector& other) const {
			return _data <= other._data && _toId <= other._toId && _toData <= other._toData;
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 bool operator>(const stable_vector& other) const {
			return _data > other._data && _toId > other._toId && _toData > other._toData;
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 bool operator>=(const stable_vector& other) const {
			return _data >= other._data && _toId >= other._toId && _toData >= other._toData;
		}
		#endif
	};
} // namespace mstd

	#endif
#endif