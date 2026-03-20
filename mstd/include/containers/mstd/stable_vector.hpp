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
		using value_type	  = T;
		using reference		  = T&;
		using const_reference = T const&;

	private:
		using _data_type = std::vector<T>;

	public:
		using size_type				 = _MSTD_TYPENAME17 _data_type::size_type;
		using difference_type		 = _MSTD_TYPENAME17 _data_type::difference_type;
		using iterator				 = _MSTD_TYPENAME17 _data_type::iterator;
		using const_iterator		 = _MSTD_TYPENAME17 _data_type::const_iterator;
		using reverse_iterator		 = _MSTD_TYPENAME17 _data_type::reverse_iterator;
		using const_reverse_iterator = _MSTD_TYPENAME17 _data_type::const_reverse_iterator;

	private:
		using _data_index_type = std::vector<size_type>;
		using _id_type		   = std::vector<size_type>;

		_data_index_type _dataIndex;
		_id_type _id;
		_data_type _data;

		_MSTD_CONSTEXPR20 void _append_indexes(size_type count) {
			size_type newSize = _dataIndex.size() + count;

			_id.reserve(newSize);
			_dataIndex.reserve(newSize);

				for (size_type i = _dataIndex.size(); i < newSize; ++i) {
					_dataIndex.push_back(i);
					_id.push_back(i);
				}
		}

	public:
		_MSTD_CONSTEXPR20 stable_vector()						  = default;

		_MSTD_CONSTEXPR20 stable_vector(stable_vector const&)	  = default;
		_MSTD_CONSTEXPR20 stable_vector(stable_vector&&) noexcept = default;

		_MSTD_CONSTEXPR20 stable_vector(size_type count) { resize(count); }

		_MSTD_CONSTEXPR20 stable_vector(size_type count, T const& value) { resize(count, value); }

		_MSTD_CONSTEXPR20 stable_vector(std::initializer_list<T> const& init) : _data(init) {
			_append_indexes(_data.size());
		}

		#if _MSTD_HAS_CXX20
		template<mstd::iterator Iter>
		#else
		template<class Iter, std::enable_if_t<is_iterator_v<Iter>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 stable_vector(const Iter& begin, const Iter& end) : _data(begin, end) {
			_append_indexes(_data.size());
		}

		_MSTD_CONSTEXPR20 ~stable_vector()									 = default;

		_MSTD_CONSTEXPR20 stable_vector& operator=(stable_vector const&)	 = default;
		_MSTD_CONSTEXPR20 stable_vector& operator=(stable_vector&&) noexcept = default;

		#pragma region INSERT_ON_FREE_SPOT

		_MSTD_CONSTEXPR20 iterator insert(T const& value) {
				if (_data.size() == size()) { _append_indexes(1); }
			return _data.insert(_data.cend(), value);
		}

		_MSTD_CONSTEXPR20 iterator insert(T&& value) {
				if (_data.size() == size()) { _append_indexes(1); }
			return _data.insert(_data.cend(), value);
		}

		_MSTD_CONSTEXPR20 iterator insert(T const& value, size_type count) {
			_append_indexes(count - (size() - _data.size()));
			return _data.insert(_data.cend(), count, value);
		}

		#if _MSTD_HAS_CXX20
		template<mstd::iterator Iter>
		#else
		template<class Iter, std::enable_if_t<is_iterator_v<Iter>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 iterator insert(Iter first, Iter last) {
			size_type count = std::distance(first, last);
			_append_indexes(count - (size() - _data.size()));
			return _data.insert(_data.cend(), first, last);
		}

		_MSTD_CONSTEXPR20 iterator insert(std::initializer_list<T> init) {
			return insert(init.begin(), init.end());
		}

		#pragma endregion INSERT_ON_FREE_SPOT

		#pragma region INSERT_AT_IDX

		_MSTD_CONSTEXPR20 iterator insert_at(size_type id, T const& value) {
				if (has_value(id)) {
					_data[_dataIndex[id]] = value;
					return std::next(_data.begin() + _dataIndex[id]);
				}

			_append_indexes(id + 1 - size());

			// insert data
			auto iter = _data.insert(_data.cend(), value);

				if (_data.size() != size()) {
					size_type toSwapId = _id[_data.size() - 1];

					// swap ids
					std::swap(_id[_data.size() - 1], _id[id]);

					// swap indexes
					std::swap(_dataIndex[toSwapId], _dataIndex[id]);
				}

			return iter;
		}

		_MSTD_CONSTEXPR20 iterator insert_at(size_type id, T&& value) {
				if (has_value(id)) {
					_data[_dataIndex[id]] = value;
					return std::next(_data.begin() + _dataIndex[id]);
				}

			_append_indexes(id + 1 - size());

			// insert data
			auto iter = _data.insert(_data.cend(), value);

				if (_data.size() != size()) {
					size_type toSwapId = _id[_data.size() - 1];

					// swap ids
					std::swap(_id[_data.size() - 1], _id[id]);

					// swap indexes
					std::swap(_dataIndex[toSwapId], _dataIndex[id]);
				}

			return iter;
		}

		_MSTD_CONSTEXPR20 iterator insert_at(size_type id, T const& value, size_type count) {
			iterator iter = insert(id, value);
				for (size_t i = 1; i < count; ++i) { insert(id + i, value); }
			return iter;
		}

		#if _MSTD_HAS_CXX20
		template<mstd::iterator Iter>
		#else
		template<class Iter, std::enable_if_t<is_iterator_v<Iter>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 iterator insert_at(size_type id, Iter first, Iter last) {
			iterator iter = insert(id, *first);
			++id;
				for (Iter it = std::next(first); it != last; ++it, ++id) { insert(id, *it); }
			return iter;
		}

		_MSTD_CONSTEXPR20 iterator insert_at(size_type id, std::initializer_list<T> init) {
			return insert(id, init.begin(), init.end());
		}

		#pragma endregion INSERT_AT_IDX

		#pragma region INSERT_AT_ITER

		_MSTD_CONSTEXPR20 iterator insert_at(const_iterator pos, T const& value) {
			return insert(_id[std::distance(_data.begin(), pos)], value);
		}

		_MSTD_CONSTEXPR20 iterator insert_at(const_iterator pos, T&& value) {
			return insert(_id[std::distance(_data.begin(), pos)], value);
		}

		_MSTD_CONSTEXPR20 iterator insert_at(const_iterator pos, T const& value, size_type count) {
			return insert(_id[std::distance(_data.begin(), pos)], count, value);
		}

		#if _MSTD_HAS_CXX20
		template<mstd::iterator Iter>
		#else
		template<class Iter, std::enable_if_t<is_iterator_v<Iter>, bool> = true>
		#endif
		_MSTD_CONSTEXPR20 iterator insert_at(const_iterator pos, Iter first, Iter last) {
			return insert(_id[std::distance(_data.begin(), pos)], first, last);
		}

		_MSTD_CONSTEXPR20 iterator insert_at(const_iterator pos, std::initializer_list<T> init) {
			return insert(pos, init.begin(), init.end());
		}

		#pragma endregion INSERT_AT_ITER

		#pragma region EMPLACE_ON_FREE_SPOT

		template<class... Args>
		_MSTD_CONSTEXPR20 iterator emplace(Args&&... args) {
			return insert(T(std::forward<Args>(args)...));
		}

		#pragma endregion

		#pragma region EMPLACE_AT_IDX

		template<class... Args>
		_MSTD_CONSTEXPR20 iterator emplace_at(size_type id, Args&&... args) {
			return insert_at(id, T(std::forward<Args>(args)...));
		}

		#pragma endregion

		#pragma region EMPLACE_AT_ITER

		template<class... Args>
		_MSTD_CONSTEXPR20 iterator emplace_at(const_iterator pos, Args&&... args) {
			return insert_at(pos, T(std::forward<Args>(args)...));
		}

		#pragma endregion

		template<class... Args>
		_MSTD_CONSTEXPR20 T& emplace_back(Args&&... args) {
			return *emplace_at(size(), std::forward<Args>(args)...);
		}

		_MSTD_CONSTEXPR20 void push_back(T const& value) { insert_at(size(), value); }

		_MSTD_CONSTEXPR20 void push_back(T&& value) { insert_at(size(), value); }

		_MSTD_CONSTEXPR20 iterator erase(size_type id) {
			mstd_assert(id < size(), "Index out of bounds");
				if (!has_value(id)) { return _data.end(); }

			// get data index
			size_t index	 = _dataIndex[id];
			size_t lastIndex = _data.size() - 1;

			// swap data
			std::swap(_data[index], _data[lastIndex]);

			// swap ids
			std::swap(_id[index], _id[lastIndex]);

			// erase last item
			auto itr = _data.erase(std::prev(_data.cend()));

			// update indexes
			std::swap(_dataIndex[_id[index]], _dataIndex[id]);

				// change size if it is possible
				if (_dataIndex[lastIndex] == lastIndex) {
					_dataIndex.erase(std::prev(_dataIndex.cend()));
					_id.erase(std::prev(_id.cend()));
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

		_MSTD_CONSTEXPR20 void pop_back() { erase(size()); }

		_MSTD_CONSTEXPR20 void reserve(size_type capacity) {
			_dataIndex.reserve(capacity);
			_id.reserve(capacity);
			_data.reserve(capacity);
		}

		_MSTD_CONSTEXPR20 void resize(size_type count) {
			_append_indexes(count - size());
			_data.resize(count);
		}

		_MSTD_CONSTEXPR20 void resize(size_type count, T const& value) {
			_append_indexes(count - size());
			_data.resize(count, value);
		}

		_MSTD_CONSTEXPR20 void swap(stable_vector& other) noexcept {
			_data.swap(other._data);
			_id.swap(other._id);
			_dataIndex.swap(other._dataIndex);
		}

		_MSTD_CONSTEXPR20 void clear() {
			_dataIndex.clear();
			_id.clear();
			_data.clear();
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 T& front() {
			mstd_assert(has_value(0), "Index is a pointer to empty element");
			return _data[_dataIndex.front()];
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 const T& front() const {
			mstd_assert(has_value(0), "Index is a pointer to empty element");
			return _data[_dataIndex.front()];
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 T& back() {
			mstd_assert(has_value(size() - 1), "Index is a pointer to empty element");
			return _data[_dataIndex.back()];
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 const T& back() const {
			mstd_assert(has_value(size() - 1), "Index is a pointer to empty element");
			return _data[_dataIndex.back()];
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 iterator get(size_type id) {
			mstd_assert(id < size(), "Index out of bounds");
			mstd_assert(has_value(id), "Index is a pointer to empty element");

			return std::next(begin(), _dataIndex[id]);
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 const_iterator get(size_type id) const {
			mstd_assert(id < size(), "Index out of bounds");
			mstd_assert(has_value(id), "Index is a pointer to empty element");

			return std::next(cbegin(), _dataIndex[id]);
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 iterator try_get(size_type id) {
			mstd_assert(id < size(), "Index out of bounds");
				if (has_value(id)) { return get(id); }
			return end();
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 const_iterator try_get(size_type id) const {
			mstd_assert(id < size(), "Index out of bounds");
				if (has_value(id)) { return get(id); }
			return cend();
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 T& at(size_type id) {
			mstd_assert(id < size(), "Index out of bounds");
			mstd_assert(has_value(id), "Index is a pointer to empty element");

			return _data[_dataIndex[id]];
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 const T& at(size_type id) const {
			mstd_assert(id < size(), "Index out of bounds");
			mstd_assert(has_value(id), "Index is a pointer to empty element");

			return _data[_dataIndex[id]];
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 T* try_at(size_type id) {
			mstd_assert(id < size(), "Index out of bounds");
				if (has_value(id)) { return &at(id); }
			return nullptr;
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 const T* try_at(size_type id) const {
			mstd_assert(id < size(), "Index out of bounds");
				if (has_value(id)) { return &at(id); }
			return nullptr;
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 size_type get_id(iterator pos) {
			mstd_assert(pos != _data.end(), "Pos out of bounds");
			return _id[std::distance(_data.begin(), pos)];
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 size_type get_id(const_iterator pos) const {
			mstd_assert(pos != _data.cend(), "Pos out of bounds");
			return _id[std::distance(_data.cbegin(), pos)];
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 size_type active_slots() const { return _data.size(); }

		[[nodiscard]] _MSTD_CONSTEXPR17 size_type empty_slots() const { return size() - active_slots(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 size_type size() const { return _dataIndex.size(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 size_type capacity() const { return _dataIndex.capacity(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 size_type max_size() const { return _data.max_size(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool empty() const { return _data.empty(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool has_value(size_type id) const {
			return id < size() && _dataIndex[id] < _data.size();
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 bool has_value(iterator pos) const { return has_value(get_id(pos)); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool has_value(const_iterator pos) const {
			return has_value(get_id(pos));
		}

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

		[[nodiscard]] _MSTD_CONSTEXPR20 T& operator[](size_type id) { return at(id); }

		[[nodiscard]] _MSTD_CONSTEXPR20 const T& operator[](size_type id) const { return at(id); }

		[[nodiscard]] T* data() { return _data.data(); }

		[[nodiscard]] T const* data() const { return _data.data(); }

		[[nodiscard]] _MSTD_CONSTEXPR20 bool operator==(stable_vector const& other) const {
			return _data == other._data && _id == other._id && _dataIndex == other._dataIndex;
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 bool operator!=(stable_vector const& other) const {
			return !(*this == other);
		}

		#if _MSTD_HAS_CXX20
		[[nodiscard]] _MSTD_CONSTEXPR20 auto operator<=>(stable_vector const& other) const {
			return _data <=> other._data && _id <=> other._id && _dataIndex <=> other._dataIndex;
		}
		#else
		[[nodiscard]] _MSTD_CONSTEXPR20 bool operator<(stable_vector const& other) const {
			return _data < other._data && _id < other._id && _dataIndex < other._dataIndex;
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 bool operator<=(stable_vector const& other) const {
			return _data <= other._data && _id <= other._id && _dataIndex <= other._dataIndex;
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 bool operator>(stable_vector const& other) const {
			return _data > other._data && _id > other._id && _dataIndex > other._dataIndex;
		}

		[[nodiscard]] _MSTD_CONSTEXPR20 bool operator>=(stable_vector const& other) const {
			return _data >= other._data && _id >= other._id && _dataIndex >= other._dataIndex;
		}
		#endif
	};
} // namespace mstd

	#endif
#endif