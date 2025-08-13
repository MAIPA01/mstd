/*
 * mstd - Maipa's Standard Library
 *
 * Licensed under the BSD 3-Clause License with Attribution Requirement.
 * See the LICENSE file for details: https://github.com/MAIPA01/mstd/blob/main/LICENSE
 *
 * Copyright (c) 2025, Patryk Antosik (MAIPA01)
 */

 // define USE_FOR_EACH_MACROS or USE_ENUMS_MACROS or USE_CLONE_FUNC_MACROS to use macros in for each region
#pragma region FOR_EACH

#if defined(USE_FOR_EACH_MACROS) || defined(USE_ENUMS_MACROS) || defined(USE_CLONE_FUNC_MACROS)

#define _PARENS ()
#define _NEXT_ELEM ,

#define _RESCAN(...) _RESCAN1(_RESCAN1(_RESCAN1(_RESCAN1(__VA_ARGS__))))
#define _RESCAN1(...) _RESCAN2(_RESCAN2(_RESCAN2(_RESCAN2(__VA_ARGS__))))
#define _RESCAN2(...) _RESCAN3(_RESCAN3(_RESCAN3(_RESCAN3(__VA_ARGS__))))
#define _RESCAN3(...) _RESCAN4(_RESCAN4(_RESCAN4(_RESCAN4(__VA_ARGS__))))
#define _RESCAN4(...) __VA_ARGS__

#pragma region SIGNLE_FOR_EACH

#define _SINGLE_FOR_EACH(func, helper, value, ...)\
	_RESCAN(helper(func, value __VA_OPT__(, __VA_ARGS__)))
#define _SINGLE_FOR_EACH_HELPER(func, again, separator, value, ...)\
	func(value)\
	__VA_OPT__(separator again _PARENS (func, __VA_ARGS__))

#define DO_FOR_EACH(func, value, ...) _SINGLE_FOR_EACH(func, DO_FOR_EACH_HELPER, value __VA_OPT__(, __VA_ARGS__))
#define DO_FOR_EACH_HELPER(func, value, ...) _SINGLE_FOR_EACH_HELPER(func, , DO_FOR_EACH_AGAIN, value __VA_OPT__(, __VA_ARGS__)
#define DO_FOR_EACH_AGAIN() DO_FOR_EACH_HELPER

#define DO_FOR_EACH_WITH_CONST(func, const_value, value, ...) _SINGLE_FOR_EACH(func, DO_FOR_EACH_WITH_CONST_HELPER, const_value, value __VA_OPT__(, __VA_ARGS__))
#define DO_FOR_EACH_WITH_CONST_HELPER(func, const_value, value, ...)\
	func(const_value, value)\
	__VA_OPT__(DO_FOR_EACH_WITH_CONST_AGAIN _PARENS (func, const_value, __VA_ARGS__))
#define DO_FOR_EACH_WITH_CONST_AGAIN() DO_FOR_EACH_WITH_CONST_HELPER

#define LIST_DO_FOR_EACH(func, value, ...) _SINGLE_FOR_EACH(func, LIST_DO_FOR_EACH_HELPER, value __VA_OPT__(, __VA_ARGS__))
#define LIST_DO_FOR_EACH_HELPER(func, value, ...) _SINGLE_FOR_EACH_HELPER(func, LIST_DO_FOR_EACH_AGAIN, _NEXT_ELEM, value __VA_OPT__(, __VA_ARGS__))
#define LIST_DO_FOR_EACH_AGAIN() LIST_DO_FOR_EACH_HELPER

#pragma endregion

#pragma region PAIR_FOR_EACH

#define _PAIR_FOR_EACH(func, helper, value0, value1, ...)\
	_RESCAN(helper(func, value0, value1 __VA_OPT__(, __VA_ARGS__)))
#define _PAIR_FOR_EACH_HELPER(func, again, separator, value0, value1, ...)\
	func(value0, value1)\
	__VA_OPT__(separator again _PARENS (func, __VA_ARGS__))

#define DO_FOR_EACH_PAIR(func, value0, value1, ...) _PAIR_FOR_EACH(func, DO_FOR_EACH_PAIR_HELPER, value0, value1 __VA_OPT__(, __VA_ARGS__))
#define DO_FOR_EACH_PAIR_HELPER(func, value0, value1, ...) _PAIR_FOR_EACH_HELPER(func, DO_FOR_EACH_PAIR_AGAIN, , value0, value1 __VA_OPT__(, __VA_ARGS__)) 
#define DO_FOR_EACH_PAIR_AGAIN() DO_FOR_EACH_PAIR_HELPER

#define DO_FOR_EACH_PAIR_WITH_CONST(func, const_value, value0, value1, ...) _PAIR_FOR_EACH(func, DO_FOR_EACH_PAIR_WITH_CONST_HELPER, const_value, value0, value1 __VA_OPT__(, __VA_ARGS__))
#define DO_FOR_EACH_PAIR_WITH_CONST_HELPER(func, const_value, value0, value1, ...)\
	func(const_value, value0, value1)\
	__VA_OPT__(DO_FOR_EACH_PAIR_WITH_CONST_AGAIN _PARENS (func, const_value, __VA_ARGS__))
#define DO_FOR_EACH_PAIR_WITH_CONST_AGAIN() DO_FOR_EACH_PAIR_WITH_CONST_HELPER

#define LIST_DO_FOR_EACH_PAIR(func, value0, value1, ...) _PAIR_FOR_EACH(func, LIST_DO_FOR_EACH_PAIR_HELPER, value0, value1 __VA_OPT__(, __VA_ARGS__))
#define LIST_DO_FOR_EACH_PAIR_HELPER(func, value0, value1, ...) _PAIR_FOR_EACH_HELPER(func, LIST_DO_FOR_EACH_PAIR_AGAIN, _NEXT_ELEM, value0, value1 __VA_OPT__(, __VA_ARGS__))
#define LIST_DO_FOR_EACH_PAIR_AGAIN() LIST_DO_FOR_EACH_PAIR_HELPER

#pragma endregion

#pragma region THREE_FOR_EACH

#define _THREE_FOR_EACH(func, helper, value0, value1, value2, ...)\
	_RESCAN(helper(func, value0, value1, value2 __VA_OPT__(, __VA_ARGS__)))
#define _THREE_FOR_EACH_HELPER(func, again, separator, value0, value1, value2, ...)\
	func(value0, value1, value2)\
	__VA_OPT__(separator again _PARENS (func, __VA_ARGS__))

#define DO_FOR_EACH_THREE(func, value0, value1, value2, ...) _THREE_FOR_EACH(func, DO_FOR_EACH_THREE_HELPER, value0, value1, value2 __VA_OPT__(, __VA_ARGS__))
#define DO_FOR_EACH_THREE_HELPER(func, value0, value1, value2, ...) _THREE_FOR_EACH_HELPER(func, DO_FOR_EACH_THREE_AGAIN, , value0, value1, value2 __VA_OPT__(, __VA_ARGS__))
#define DO_FOR_EACH_THREE_AGAIN() DO_FOR_EACH_THREE_HELPER

#define DO_FOR_EACH_THREE_WITH_CONST(func, const_value, value0, value1, value2, ...) _THREE_FOR_EACH(func, DO_FOR_EACH_THREE_WITH_CONST_HELPER, const_value, value0, value1, value2 __VA_OPT__(, __VA_ARGS__))
#define DO_FOR_EACH_THREE_WITH_CONST_HELPER(func, const_value, value0, value1, value2, ...)\
	func(const_value, value0, value1, value2)\
	__VA_OPT__(DO_FOR_EACH_THREE_WITH_CONST_AGAIN _PARENS (func, const_value, __VA_ARGS__))
#define DO_FOR_EACH_THREE_WITH_CONST_AGAIN() DO_FOR_EACH_THREE_WITH_CONST_HELPER

#define LIST_DO_FOR_EACH_THREE(func, value0, value1, value2, ...) _THREE_FOR_EACH(func, LIST_DO_FOR_EACH_THREE_HELPER, value0, value1, value2 __VA_OPT__(, __VA_ARGS__))
#define LIST_DO_FOR_EACH_THREE_HELPER(func, value0, value1, value2, ...) _THREE_FOR_EACH_HELPER(func, LIST_DO_FOR_EACH_THREE_AGAIN, _NEXT_ELEM, value0, value1, value2 __VA_OPT__(, __VA_ARGS__))
#define LIST_DO_FOR_EACH_THREE_AGAIN() LIST_DO_FOR_EACH_THREE_HELPER

#pragma endregion

#endif

#pragma endregion

// define USE_ENUMS_MACROS to use macros in enums region
#pragma region ENUMS

#ifdef USE_ENUMS_MACROS

#define _ENUM_ELEMENT_COUNT(...) + 1ull
#define _ENUM_CASE(enum_name, name, ...) case enum_name::name: return #name;

#define _ENUM_DECLARATION(type, name, base, list_for_each, elem_func, ...)\
type name base { list_for_each(elem_func, __VA_ARGS__) };

#define _ENUM_SIZE_FUNC(name, for_each, ...)\
template<class T> static size_t size();\
template<> static constexpr size_t size<name>() {\
	return for_each(_ENUM_ELEMENT_COUNT, __VA_ARGS__);\
}

#define _ENUM_TO_STRING_FUNC(name, for_each, case_func, ...)\
static const std::string to_string(const name& v) {\
	switch(v) {\
	for_each(case_func, __VA_ARGS__)\
	default:\
		return "UNKNOWN";\
	}\
}

#define _ENUM_TEMPLATE(type, name, base, declaration_func, size_func, string_func, ...)\
	declaration_func(type, name, base, __VA_ARGS__)\
	size_func(name, __VA_ARGS__)\
	string_func(name, __VA_ARGS__)

#pragma region STANDARD_ENUMS

#define _STANDARD_ENUM_ELEMENT(name, ...) name
#define _STANDARD_ENUM_CASE_STRING(enum_name, name, string) case enum_name::name: return string;

#define _STANDARD_ENUM_DECLARATION(type, name, base, ...)\
_ENUM_DECLARATION(type, name, base, LIST_DO_FOR_EACH, _STANDARD_ENUM_ELEMENT, __VA_ARGS__)

#define _STANDARD_ENUM_STRING_DECLARATION(type, name, base, ...)\
_ENUM_DECLARATION(type, name, base, LIST_DO_FOR_EACH_PAIR, _STANDARD_ENUM_ELEMENT, __VA_ARGS__)

#define _STANDARD_ENUM_SIZE_FUNC(name, ...)\
_ENUM_SIZE_FUNC(name, DO_FOR_EACH, __VA_ARGS__)

#define _STANDARD_ENUM_STRING_SIZE_FUNC(name, ...)\
_ENUM_SIZE_FUNC(name, DO_FOR_EACH_PAIR, __VA_ARGS__)

#define _STANDARD_ENUM_TO_STRING_FUNC(name, ...)\
_ENUM_TO_STRING_FUNC(name, DO_FOR_EACH_WITH_CONST, _ENUM_CASE, name, __VA_ARGS__)

#define _STANDARD_ENUM_STRING_TO_STRING_FUNC(name, ...)\
_ENUM_TO_STRING_FUNC(name, DO_FOR_EACH_PAIR_WITH_CONST, _STANDARD_ENUM_CASE_STRING, name, __VA_ARGS__)

#define _STANDARD_ENUM_FUNC_TEMPLATE(type, name, base, declaration, string_func, size_func, ...)\
_ENUM_TEMPLATE(type, name, base, declaration, size_func, string_func, __VA_ARGS__)

#define _STANDARD_ENUM_TEMPLATE(type, name, base, ...)\
_STANDARD_ENUM_FUNC_TEMPLATE(type, name, base, _STANDARD_ENUM_DECLARATION, _STANDARD_ENUM_TO_STRING_FUNC, _STANDARD_ENUM_SIZE_FUNC, __VA_ARGS__)
#define _STANDARD_ENUM_STRING_TEMPLATE(type, name, base, ...)\
_STANDARD_ENUM_FUNC_TEMPLATE(type, name, base, _STANDARD_ENUM_STRING_DECLARATION, _STANDARD_ENUM_STRING_TO_STRING_FUNC, _STANDARD_ENUM_STRING_SIZE_FUNC, __VA_ARGS__)

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

#define _VALUE_ENUM_ELEMENT(name, value, ...) name = value
#define _VALUE_ENUM_CASE_STRING(enum_name, name, value, string) case enum_name::name: return string;

#define _VALUE_ENUM_DECLARATION(type, name, base, ...)\
_ENUM_DECLARATION(type, name, base, LIST_DO_FOR_EACH_PAIR, _VALUE_ENUM_ELEMENT, __VA_ARGS__)
#define _VALUE_ENUM_STRING_DECLARATION(type, name, base, ...)\
_ENUM_DECLARATION(type, name, base, LIST_DO_FOR_EACH_THREE, _VALUE_ENUM_ELEMENT, __VA_ARGS__)

#define _VALUE_ENUM_SIZE_FUNC(name, ...)\
_ENUM_SIZE_FUNC(name, DO_FOR_EACH_PAIR, __VA_ARGS__)
#define _VALUE_ENUM_STRING_SIZE_FUNC(name, ...)\
_ENUM_SIZE_FUNC(name, DO_FOR_EACH_THREE, __VA_ARGS__)

#define _VALUE_ENUM_TO_STRING_FUNC(name, ...)\
_ENUM_TO_STRING_FUNC(name, DO_FOR_EACH_PAIR_WITH_CONST, _ENUM_CASE, name, __VA_ARGS__)
#define _VALUE_ENUM_STRING_TO_STRING_FUNC(name, ...)\
_ENUM_TO_STRING_FUNC(name, DO_FOR_EACH_THREE_WITH_CONST, _VALUE_ENUM_CASE_STRING, name, __VA_ARGS__)

#define _VALUE_ENUM_FUNC_TEMPLATE(type, name, base, declaration, size_func, string_func, ...)\
_ENUM_TEMPLATE(type, name, base, declaration, size_func, string_func, __VA_ARGS__)

#define _VALUE_ENUM_TEMPLATE(type, name, base, ...)\
_VALUE_ENUM_FUNC_TEMPLATE(type, name, base, _VALUE_ENUM_DECLARATION, _VALUE_ENUM_SIZE_FUNC, _VALUE_ENUM_TO_STRING_FUNC, __VA_ARGS__)
#define _VALUE_ENUM_STRING_TEMPLATE(type, name, base, ...)\
_VALUE_ENUM_FUNC_TEMPLATE(type, name, base, _VALUE_ENUM_STRING_DECLARATION, _VALUE_ENUM_STRING_SIZE_FUNC, _VALUE_ENUM_STRING_TO_STRING_FUNC, __VA_ARGS__)

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

#define FIELD_PAIR_CLONE(fieldName, value) cloned->fieldName = value;

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
    __VA_OPT__(DO_FOR_EACH_PAIR(FIELD_PAIR_CLONE, __VA_ARGS__))\
}

#define STANDARD_CLONE(fieldName) fieldName, fieldName
#define POINTER_DEEP_CLONE(fieldName, valueType) fieldName, new valueType(*fieldName)

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
	__VA_OPT__(DO_FOR_EACH_PAIR(FIELD_PAIR_CLONE, __VA_ARGS__))\
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