/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

#include <mstd/mstd_config.hpp>

 // define USE_FOR_EACH_MACROS or USE_ENUMS_MACROS or USE_CLONE_FUNC_MACROS to use macros in for each region
#pragma region FOR_EACH

#if defined(USE_FOR_EACH_MACROS) || defined(USE_ENUMS_MACROS) || defined(USE_CLONE_FUNC_MACROS)

#define _RESCAN(...) _RESCAN1(_RESCAN1(_RESCAN1(_RESCAN1(__VA_ARGS__))))
#define _RESCAN1(...) _RESCAN2(_RESCAN2(_RESCAN2(_RESCAN2(__VA_ARGS__))))
#define _RESCAN2(...) _RESCAN3(_RESCAN3(_RESCAN3(_RESCAN3(__VA_ARGS__))))
#define _RESCAN3(...) _RESCAN4(_RESCAN4(_RESCAN4(_RESCAN4(__VA_ARGS__))))
#define _RESCAN4(...) __VA_ARGS__

#define _INVOKE(macro, args) macro args

#pragma region TUPLES

#define MAKE_TUPLE(...) (__VA_ARGS__)

#define _BREAK_TUPLE_HELPER(...) __VA_ARGS__
#define BREAK_TUPLE(tuple) _BREAK_TUPLE_HELPER tuple

#define APPEND_TUPLE_FRONT(tuple, value) (value, BREAK_TUPLE(tuple))
#define APPEND_TUPLE_BACK(tuple, value) (BREAK_TUPLE(tuple), value)

#define _POP_TUPLE_FRONT_HELPER(value, ...) MAKE_TUPLE(__VA_ARGS__)
#define POP_TUPLE_FRONT(tuple) _INVOKE(_POP_TUPLE_FRONT_HELPER, (BREAK_TUPLE(tuple)))

#pragma endregion

#pragma region FOR_EACH

#define _PARENS ()
#define _NEXT_ELEM ,

#define _FOR_EACH(func, helper, value, ...)\
	_RESCAN(helper(func, value __VA_OPT__(, __VA_ARGS__)))
#define _FOR_EACH_HELPER(func, again, separator, value, ...)\
	func(value)\
	__VA_OPT__(separator again _PARENS (func, __VA_ARGS__))

#define DO_FOR_EACH(func, value, ...) _FOR_EACH(func, _DO_FOR_EACH_HELPER, value __VA_OPT__(, __VA_ARGS__))
#define _DO_FOR_EACH_HELPER(func, value, ...) _FOR_EACH_HELPER(func, _DO_FOR_EACH_AGAIN, , value __VA_OPT__(, __VA_ARGS__))
#define _DO_FOR_EACH_AGAIN() _DO_FOR_EACH_HELPER

#define DO_FOR_EACH_WITH_CONST(func, const_value, value, ...) _FOR_EACH(func, _DO_FOR_EACH_WITH_CONST_HELPER, const_value, value __VA_OPT__(, __VA_ARGS__))
#define _DO_FOR_EACH_WITH_CONST_HELPER(func, const_value, value, ...)\
	func(const_value, value)\
	__VA_OPT__(_DO_FOR_EACH_WITH_CONST_AGAIN _PARENS (func, const_value, __VA_ARGS__))
#define _DO_FOR_EACH_WITH_CONST_AGAIN() _DO_FOR_EACH_WITH_CONST_HELPER

#define LIST_DO_FOR_EACH(func, value, ...) _FOR_EACH(func, _LIST_DO_FOR_EACH_HELPER, value __VA_OPT__(, __VA_ARGS__))
#define _LIST_DO_FOR_EACH_HELPER(func, value, ...) _FOR_EACH_HELPER(func, _LIST_DO_FOR_EACH_AGAIN, _NEXT_ELEM, value __VA_OPT__(, __VA_ARGS__))
#define _LIST_DO_FOR_EACH_AGAIN() _LIST_DO_FOR_EACH_HELPER

#pragma endregion

#endif

#pragma endregion

// define USE_ENUMS_MACROS to use macros in enums region
#pragma region ENUMS

#ifdef USE_ENUMS_MACROS

#define _ENUM_ELEMENT_COUNT(tuple) + 1ull
#define _ENUM_TO_STRING_CASE_HELPER(enum_name, name, ...) case enum_name::name: return #name;
#define _ENUM_TO_STRING_CASE(enum_name, tuple) _INVOKE(_ENUM_TO_STRING_CASE_HELPER, (enum_name, BREAK_TUPLE(tuple)))
#define _ENUM_FROM_STRING_IF_HELPER(enum_name, name, ...) if (s == #name) return enum_name::name;
#define _ENUM_FROM_STRING_IF(enum_name, tuple) _INVOKE(_ENUM_FROM_STRING_IF_HELPER, (enum_name, BREAK_TUPLE(tuple)))

#define _ENUM_DECLARATION(type, name, base, elem_func, ...)\
type name base { LIST_DO_FOR_EACH(elem_func, __VA_ARGS__) };

#define _ENUM_SIZE_FUNC(name, ...)\
template<class T> _MSTD_CONSTEXPR20 size_t size() noexcept;\
template<> _MSTD_CONSTEXPR20 size_t size<name>() noexcept {\
	return DO_FOR_EACH(_ENUM_ELEMENT_COUNT, __VA_ARGS__);\
}

#define _ENUM_TO_STRING_FUNC(name, case_func, ...)\
inline const ::std::string to_string(const name& v) {\
	switch(v) {\
	DO_FOR_EACH_WITH_CONST(case_func, name, __VA_ARGS__)\
	default:\
		return "UNKNOWN";\
	}\
}

#define _ENUM_STREAM_OPERATOR(name)\
inline ::std::ostream& operator<<(::std::ostream& os, const name& v) {\
    return os << to_string(v);\
}

#define _ENUM_FROM_STRING_FUNC(name, case_func, ...)\
template<class T> inline T from_string(const std::string&);\
template<> inline name from_string<name>(const std::string& s) {\
	DO_FOR_EACH_WITH_CONST(case_func, name, __VA_ARGS__)\
	throw std::invalid_argument("Unknown enum string: " + s);\
}

#define _ENUM_TEMPLATE(type, name, base, declaration_func, size_func, to_string_func, from_string_func, ...)\
	declaration_func(type, name, base, __VA_ARGS__)\
	size_func(name, __VA_ARGS__)\
	to_string_func(name, __VA_ARGS__)\
	_ENUM_STREAM_OPERATOR(name)\
	from_string_func(name, __VA_ARGS__)

#pragma region STANDARD_ENUMS

#define _STANDARD_ENUM_ELEMENT_HELPER(name, ...) name
#define _STANDARD_ENUM_ELEMENT(tuple) _INVOKE(_STANDARD_ENUM_ELEMENT_HELPER, (BREAK_TUPLE(tuple)))
#define _STANDARD_ENUM_STRING_TO_STRING_CASE_HELPER(enum_name, name, string) case enum_name::name: return string;
#define _STANDARD_ENUM_STRING_TO_STRING_CASE(enum_name, tuple) _INVOKE(_STANDARD_ENUM_STRING_TO_STRING_CASE_HELPER, (enum_name, BREAK_TUPLE(tuple))) 
#define _STANDARD_ENUM_STRING_FROM_STRING_IF_HELPER(enum_name, name, string) if (s == string) return enum_name::name;
#define _STANDARD_ENUM_STRING_FROM_STRING_IF(enum_name, tuple) _INVOKE(_STANDARD_ENUM_STRING_FROM_STRING_IF_HELPER, (enum_name, BREAK_TUPLE(tuple))) 

#define _STANDARD_ENUM_DECLARATION(type, name, base, ...)\
_ENUM_DECLARATION(type, name, base, _STANDARD_ENUM_ELEMENT, __VA_ARGS__)

#define _STANDARD_ENUM_SIZE_FUNC(name, ...)\
_ENUM_SIZE_FUNC(name, __VA_ARGS__)

#define _STANDARD_ENUM_TO_STRING_FUNC(name, ...)\
_ENUM_TO_STRING_FUNC(name, _ENUM_TO_STRING_CASE, __VA_ARGS__)

#define _STANDARD_ENUM_STRING_TO_STRING_FUNC(name, ...)\
_ENUM_TO_STRING_FUNC(name, _STANDARD_ENUM_STRING_TO_STRING_CASE, __VA_ARGS__)

#define _STANDARD_ENUM_FROM_STRING_FUNC(name, ...)\
_ENUM_FROM_STRING_FUNC(name, _ENUM_FROM_STRING_IF, __VA_ARGS__)

#define _STANDARD_ENUM_STRING_FROM_STRING_FUNC(name, ...)\
_ENUM_FROM_STRING_FUNC(name, _STANDARD_ENUM_STRING_FROM_STRING_IF, __VA_ARGS__)

#define _STANDARD_ENUM_FUNC_TEMPLATE(type, name, base, declaration_func, size_func, to_string_func, from_string_func, ...)\
_ENUM_TEMPLATE(type, name, base, declaration_func, size_func, to_string_func, from_string_func, __VA_ARGS__)

#define _STANDARD_ENUM_TEMPLATE(type, name, base, ...)\
_STANDARD_ENUM_FUNC_TEMPLATE(type, name, base, _STANDARD_ENUM_DECLARATION, _STANDARD_ENUM_SIZE_FUNC, _STANDARD_ENUM_TO_STRING_FUNC, _STANDARD_ENUM_FROM_STRING_FUNC, __VA_ARGS__)
#define _STANDARD_ENUM_STRING_TEMPLATE(type, name, base, ...)\
_STANDARD_ENUM_FUNC_TEMPLATE(type, name, base, _STANDARD_ENUM_DECLARATION, _STANDARD_ENUM_SIZE_FUNC, _STANDARD_ENUM_STRING_TO_STRING_FUNC, _STANDARD_ENUM_STRING_FROM_STRING_FUNC, __VA_ARGS__)

#define _STANDARD_ENUM(name, base, ...) _STANDARD_ENUM_TEMPLATE(enum, name, base, __VA_ARGS__)
#define _STANDARD_ENUM_CLASS(name, base, ...) _STANDARD_ENUM_TEMPLATE(enum class, name, base, __VA_ARGS__)

#define _STANDARD_ENUM_STRING(name, base, ...) _STANDARD_ENUM_STRING_TEMPLATE(enum, name, base, __VA_ARGS__)
#define _STANDARD_ENUM_CLASS_STRING(name, base, ...) _STANDARD_ENUM_STRING_TEMPLATE(enum class, name, base, __VA_ARGS__)

#define ENUM(name, ...) _STANDARD_ENUM(name, , __VA_ARGS__)
#define ENUM_BASE(name, base, ...) _STANDARD_ENUM(name, : base, __VA_ARGS__)
#define ENUM_CLASS(name, ...) _STANDARD_ENUM_CLASS(name, , __VA_ARGS__)
#define ENUM_CLASS_BASE(name, base, ...) _STANDARD_ENUM_CLASS(name, : base, __VA_ARGS__)

#define ENUM_STRING(name, ...) _STANDARD_ENUM_STRING(name, , __VA_ARGS__)
#define ENUM_BASE_STRING(name, base, ...) _STANDARD_ENUM_STRING(name, : base, __VA_ARGS__)
#define ENUM_CLASS_STRING(name, ...) _STANDARD_ENUM_CLASS_STRING(name, , __VA_ARGS__)
#define ENUM_CLASS_BASE_STRING(name, base, ...) _STANDARD_ENUM_CLASS_STRING(name, : base, __VA_ARGS__)

#pragma endregion

#pragma region ENUMS_WITH_VALUE

#define _VALUE_ENUM_ELEMENT_HELPER(name, value, ...) name = value
#define _VALUE_ENUM_ELEMENT(tuple) _INVOKE(_VALUE_ENUM_ELEMENT_HELPER, (BREAK_TUPLE(tuple)))
#define _VALUE_ENUM_STRING_TO_STRING_CASE_HELPER(enum_name, name, value, string) case enum_name::name: return string;
#define _VALUE_ENUM_STRING_TO_STRING_CASE(enum_name, tuple) _INVOKE(_VALUE_ENUM_STRING_TO_STRING_CASE_HELPER, (enum_name, BREAK_TUPLE(tuple)))
#define _VALUE_ENUM_STRING_FROM_STRING_IF_HELPER(enum_name, name, value, string) if (s == string) return enum_name::name;
#define _VALUE_ENUM_STRING_FROM_STRING_IF(enum_name, tuple) _INVOKE(_VALUE_ENUM_STRING_FROM_STRING_IF_HELPER, (enum_name, BREAK_TUPLE(tuple))) 

#define _VALUE_ENUM_DECLARATION(type, name, base, ...)\
_ENUM_DECLARATION(type, name, base, _VALUE_ENUM_ELEMENT, __VA_ARGS__)

#define _VALUE_ENUM_SIZE_FUNC(name, ...)\
_ENUM_SIZE_FUNC(name, __VA_ARGS__)

#define _VALUE_ENUM_TO_STRING_FUNC(name, ...)\
_ENUM_TO_STRING_FUNC(name, _ENUM_TO_STRING_CASE, __VA_ARGS__)
#define _VALUE_ENUM_STRING_TO_STRING_FUNC(name, ...)\
_ENUM_TO_STRING_FUNC(name, _VALUE_ENUM_STRING_TO_STRING_CASE, __VA_ARGS__)

#define _VALUE_ENUM_FROM_STRING_FUNC(name, ...)\
_ENUM_FROM_STRING_FUNC(name, _ENUM_FROM_STRING_IF, __VA_ARGS__)
#define _VALUE_ENUM_STRING_FROM_STRING_FUNC(name, ...)\
_ENUM_FROM_STRING_FUNC(name, _VALUE_ENUM_STRING_FROM_STRING_IF, __VA_ARGS__)

#define _VALUE_ENUM_FUNC_TEMPLATE(type, name, base, declaration, size_func, to_string_func, from_string_func, ...)\
_ENUM_TEMPLATE(type, name, base, declaration, size_func, to_string_func, from_string_func, __VA_ARGS__)

#define _VALUE_ENUM_TEMPLATE(type, name, base, ...)\
_VALUE_ENUM_FUNC_TEMPLATE(type, name, base, _VALUE_ENUM_DECLARATION, _VALUE_ENUM_SIZE_FUNC, _VALUE_ENUM_TO_STRING_FUNC, _VALUE_ENUM_FROM_STRING_FUNC, __VA_ARGS__)
#define _VALUE_ENUM_STRING_TEMPLATE(type, name, base, ...)\
_VALUE_ENUM_FUNC_TEMPLATE(type, name, base, _VALUE_ENUM_DECLARATION, _VALUE_ENUM_SIZE_FUNC, _VALUE_ENUM_STRING_TO_STRING_FUNC, _VALUE_ENUM_STRING_FROM_STRING_FUNC, __VA_ARGS__)

#define _VALUE_ENUM(name, base, ...) _VALUE_ENUM_TEMPLATE(enum, name, base, __VA_ARGS__)
#define _VALUE_ENUM_CLASS(name, base, ...) _VALUE_ENUM_TEMPLATE(enum class, name, base, __VA_ARGS__)

#define _VALUE_ENUM_STRING(name, base, ...) _VALUE_ENUM_STRING_TEMPLATE(enum, name, base, __VA_ARGS__)
#define _VALUE_ENUM_CLASS_STRING(name, base, ...) _VALUE_ENUM_STRING_TEMPLATE(enum class, name, base, __VA_ARGS__)

#define ENUM_VALUE(name, ...) _VALUE_ENUM(name, , __VA_ARGS__)
#define ENUM_BASE_VALUE(name, base, ...) _VALUE_ENUM(name, : base, __VA_ARGS__)
#define ENUM_CLASS_VALUE(name, ...) _VALUE_ENUM_CLASS(name, , __VA_ARGS__)
#define ENUM_CLASS_BASE_VALUE(name, base, ...) _VALUE_ENUM_CLASS(name, : base, __VA_ARGS__)

#define ENUM_VALUE_STRING(name, ...) _VALUE_ENUM_STRING(name, , __VA_ARGS__)
#define ENUM_BASE_VALUE_STRING(name, base, ...) _VALUE_ENUM_STRING(name, : base, __VA_ARGS__)
#define ENUM_CLASS_VALUE_STRING(name, ...) _VALUE_ENUM_CLASS_STRING(name, , __VA_ARGS__)
#define ENUM_CLASS_BASE_VALUE_STRING(name, base, ...) _VALUE_ENUM_CLASS_STRING(name, : base, __VA_ARGS__)

#pragma endregion

#endif

#pragma endregion

// define USE_CLONE_FUNC_MACROS to use macros in clone func region
#pragma region CLONE_FUNC

#ifdef USE_CLONE_FUNC_MACROS

#pragma region CLONE_FUNC_DECLARATION

#define _DECLARE_CLONE_FUNC_TEMPLATE(class_name, base_class_operator)\
virtual class_name* Clone() const base_class_operator;\
void CloneTo(class_name* cloned) const;

#define DECLARE_CLONE_FUNC(class_name) _DECLARE_CLONE_FUNC_TEMPLATE(class_name, )
#define DECLARE_OVERRIDED_CLONE_FUNC(class_name) _DECLARE_CLONE_FUNC_TEMPLATE(class_name, override)
#define DECLARE_ABSTRACT_CLONE_FUNC(class_name) _DECLARE_CLONE_FUNC_TEMPLATE(class_name, = 0)

#pragma endregion

#pragma region CLONE_FUNC_DECLARATION_WITH_DEFINITION

#define _FIELD_CLONE_HELPER(fieldName, value) cloned->fieldName = value;
#define FIELD_CLONE(tuple) _INVOKE(_FIELD_CLONE_HELPER, (BREAK_TUPLE(tuple)))

#define _DECLARE_CLONE_FUNC_WITH_DEFINITION_TEMPLATE(class_name, base_class_func, base_class_operator, ...)\
virtual class_name* Clone() const base_class_operator\
{\
    class_name* cloned = new class_name();\
    CloneTo(cloned);\
    return cloned;\
}\
void CloneTo(class_name* cloned) const\
{\
    base_class_func\
    __VA_OPT__(DO_FOR_EACH(FIELD_CLONE, __VA_ARGS__))\
}

#define STANDARD_CLONE(fieldName) MAKE_TUPLE(fieldName, fieldName)
#define POINTER_DEEP_CLONE(fieldName, valueType) MAKE_TUPLE(fieldName, new valueType(*fieldName))

#define DECLARE_CLONE_FUNC_WITH_DEFINITION_ADVANCED(class_name, ...)\
_DECLARE_CLONE_FUNC_WITH_DEFINITION_TEMPLATE(class_name, , __VA_OPT__(, __VA_ARGS__))
#define DECLARE_CLONE_BASE_FUNC_WITH_DEFINITION_ADVANCED(class_name, base_class_name, ...)\
_DECLARE_CLONE_FUNC_WITH_DEFINITION_TEMPLATE(class_name, base_class_name::CloneTo(cloned);, override __VA_OPT__(, __VA_ARGS__))

#define DECLARE_CLONE_FUNC_WITH_DEFINITION(class_name, ...)\
DECLARE_CLONE_FUNC_WITH_DEFINITION_ADVANCED(class_name __VA_OPT__(, LIST_DO_FOR_EACH(STANDARD_CLONE, __VA_ARGS__)))
#define DECLARE_CLONE_BASE_FUNC_WITH_DEFINITION(class_name, base_class_name, ...)\
DECLARE_CLONE_BASE_FUNC_WITH_DEFINITION_ADVANCED(class_name, base_class_name __VA_OPT__(, LIST_DO_FOR_EACH(STANDARD_CLONE, __VA_ARGS__)))

#pragma endregion

#pragma region CLONE_FUNC_DEFINITION

#define _CLONE_FUNC_DEFINITION_TEMPLATE(class_name, base_class_func, ...)\
class_name* class_name::Clone() const\
{\
	class_name* cloned = new class_name();\
	CloneTo(cloned);\
	return cloned;\
}\
void class_name::CloneTo(class_name* cloned) const\
{\
	base_class_func\
	__VA_OPT__(DO_FOR_EACH(FIELD_CLONE, __VA_ARGS__))\
}

#define CLONE_FUNC_DEFINITION_ADVANCED(class_name, ...)\
_CLONE_FUNC_DEFINITION_TEMPLATE(class_name, __VA_OPT__(, __VA_ARGS__))
#define CLONE_BASE_FUNC_DEFINITION_ADVANCED(class_name, base_class_name, ...)\
_CLONE_FUNC_DEFINITION_TEMPLATE(class_name, base_class_name::CloneTo(cloned); __VA_OPT__(, __VA_ARGS__))

#define CLONE_FUNC_DEFINITION(class_name, ...)\
CLONE_FUNC_DEFINITION_ADVANCED(class_name __VA_OPT__(, LIST_DO_FOR_EACH(STANDARD_CLONE, __VA_ARGS__)))
#define CLONE_BASE_FUNC_DEFINITION(class_name, base_class_name, ...)\
CLONE_BASE_FUNC_DEFINITION_ADVANCED(class_name, base_class_name __VA_OPT__(, LIST_DO_FOR_EACH(STANDARD_CLONE, __VA_ARGS__)))

#pragma endregion

#endif

#pragma endregion

// define USE_EXTRA_MACROS to use macros in extra region
#pragma region EXTRA

#ifdef USE_EXTRA_MACROS

#define VAR_TO_STRING(var) #var

#endif

#pragma endregion