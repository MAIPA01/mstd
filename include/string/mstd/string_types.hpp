/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#pragma once
#include "string_libs.hpp"

namespace mstd {
    namespace mstd {
        template<typename T, typename = void>
        struct string_type_info;

        template<typename T>
        struct string_type_info<T, std::enable_if_t<
            std::is_same_v<std::remove_cv_t<T>, char> ||
            std::is_same_v<std::remove_cv_t<T>, signed char> ||
            std::is_same_v<std::remove_cv_t<T>, unsigned char>>> {
            static constexpr size_t size(const T&) { return 1; }
        };

        template<typename T, size_t N>
        struct string_type_info<T[N], std::enable_if_t<
            std::is_same_v<std::remove_cv_t<T>, char> ||
            std::is_same_v<std::remove_cv_t<T>, signed char> ||
            std::is_same_v<std::remove_cv_t<T>, unsigned char>>> {
            static constexpr size_t size(const T(&)[N]) { return N - 1; }
        };

        template<typename T>
        struct string_type_info<T, std::enable_if_t<
            std::is_pointer_v<T> &&
            (std::is_same_v<std::remove_cv_t<std::remove_pointer_t<T>>, char> ||
                std::is_same_v<std::remove_cv_t<std::remove_pointer_t<T>>, signed char> ||
                std::is_same_v<std::remove_cv_t<std::remove_pointer_t<T>>, unsigned char>)>> {
            static size_t size(T s) {
                return s ? std::strlen(reinterpret_cast<const char*>(s)) : 0;
            }
        };

        template<typename T>
        struct string_type_info<T, std::enable_if_t<std::is_same_v<std::remove_cv_t<T>, std::string>>> {
            static size_t size(const std::string& s) { return s.size(); }
        };

        template<class _String>
        static constexpr size_t string_size(_String&& s) {
            using string_t = std::remove_reference_t<_String>;
            return string_type_info<string_t>::size(s);
        }
    }
}