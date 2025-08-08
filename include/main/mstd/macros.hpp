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

#define DO_FOR_EACH(func, ...)\
	__VA_OPT__(_RESCAN(DO_FOR_EACH_HELPER(func, __VA_ARGS__)))
#define DO_FOR_EACH_HELPER(func, a1, ...)\
	func(a1)\
	__VA_OPT__(DO_FOR_EACH_AGAIN _PARENS (func, __VA_ARGS__))
#define DO_FOR_EACH_AGAIN() DO_FOR_EACH_HELPER

#define LIST_DO_FOR_EACH(func, ...)\
	__VA_OPT__(_RESCAN(LIST_DO_FOR_EACH_HELPER(func, __VA_ARGS__)))
#define LIST_DO_FOR_EACH_HELPER(func, a1, ...)\
	func(a1)\
	__VA_OPT__(_NEXT_ELEM LIST_DO_FOR_EACH_AGAIN _PARENS (func, __VA_ARGS__))
#define LIST_DO_FOR_EACH_AGAIN() LIST_DO_FOR_EACH_HELPER

#pragma endregion SIGNLE_FOR_EACH

#pragma region PAIR_FOR_EACH

#define DO_FOR_EACH_PAIR(func, a1, ...)\
	__VA_OPT__(_RESCAN(DO_FOR_EACH_PAIR_HELPER(func, a1, __VA_ARGS__)))
#define DO_FOR_EACH_PAIR_HELPER(func, a1, a2, ...)\
	func(a1, a2)\
	__VA_OPT__(DO_FOR_EACH_PAIR_AGAIN _PARENS (func, __VA_ARGS__))
#define DO_FOR_EACH_PAIR_AGAIN() DO_FOR_EACH_PAIR_HELPER

#define LIST_DO_FOR_EACH_PAIR(func, a1, ...)\
	__VA_OPT__(_RESCAN(LIST_DO_FOR_EACH_PAIR_HELPER(func, a1, __VA_ARGS__)))
#define LIST_DO_FOR_EACH_PAIR_HELPER(func, a1, a2, ...)\
	func(a1, a2)\
	__VA_OPT__(_NEXT_ELEM LIST_DO_FOR_EACH_PAIR_AGAIN _PARENS (func, __VA_ARGS__))
#define LIST_DO_FOR_EACH_PAIR_AGAIN() LIST_DO_FOR_EACH_PAIR_HELPER

#pragma endregion

#pragma region THREE_FOR_EACH

#define DO_FOR_EACH_THREE(func, a1, a2, ...)\
	__VA_OPT__(_RESCAN(DO_FOR_EACH_THREE_HELPER(func, a1, a2, __VA_ARGS__)))
#define DO_FOR_EACH_THREE_HELPER(func, a1, a2, a3, ...)\
	func(a1, a2, a3)\
	__VA_OPT__(DO_FOR_EACH_THREE_AGAIN _PARENS (func, __VA_ARGS__))
#define DO_FOR_EACH_THREE_AGAIN() DO_FOR_EACH_THREE_HELPER

#define LIST_DO_FOR_EACH_THREE(func, a1, a2, ...)\
	__VA_OPT__(_RESCAN(LIST_DO_FOR_EACH_THREE_HELPER(func, a1, a2, __VA_ARGS__)))
#define LIST_DO_FOR_EACH_THREE_HELPER(func, a1, a2, a3, ...)\
	func(a1, a2, a3)\
	__VA_OPT__(_NEXT_ELEM LIST_DO_FOR_EACH_THREE_AGAIN _PARENS (func, __VA_ARGS__))
#define LIST_DO_FOR_EACH_THREE_AGAIN() LIST_DO_FOR_EACH_THREE_HELPER

#pragma endregion

#endif

#pragma endregion

// define USE_ENUMS_MACROS to use macros in enums region
#pragma region ENUMS

#ifdef USE_ENUMS_MACROS

#define _ENUM_ELEMENT_COUNT(...) + 1
#define _ENUM_CASE(name, ...) case name: return #name;

#define _ENUM_DECLARATION(type, name, base, list_for_each, elem_func, ...)\
type name base { list_for_each(elem_func, __VA_ARGS__) };

#define _ENUM_SIZE_FUNC(name, for_each, ...)\
template<class T> static size_t size();\
template<> static constexpr size_t size<name>() {\
	return for_each(_ENUM_ELEMENT_COUNT, __VA_ARGS__);\
}

#define _ENUM_TO_STRING_FUNC(name, for_each, case_func, ...)\
static constexpr const std::string to_string(const name& v) {\
	using enum name;\
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

#define _STANDARD_ENUM_ELEMENT(name) name
#define _STANDARD_ENUM_CASE_STRING(name, string) case name: return string;

#define _STANDARD_ENUM_DECLARATION(type, name, base, ...)\
_ENUM_DECLARATION(type, name, base, LIST_DO_FOR_EACH, _STANDARD_ENUM_ELEMENT, __VA_ARGS__)

#define _STANDARD_ENUM_SIZE_FUNC(name, ...)\
_ENUM_SIZE_FUNC(name, DO_FOR_EACH, __VA_ARGS__)

#define _STANDARD_ENUM_TO_STRING_FUNC(name, ...)\
_ENUM_TO_STRING_FUNC(name, DO_FOR_EACH, _ENUM_CASE, __VA_ARGS__)

#define _STANDARD_ENUM_TO_STRING_VALUES_FUNC(name, ...)\
_ENUM_TO_STRING_FUNC(name, DO_FOR_EACH_PAIR, _STANDARD_ENUM_CASE_STRING, __VA_ARGS__)

#define _STANDARD_ENUM_TEMPLATE(type, name, base, string_func, ...)\
_ENUM_TEMPLATE(type, name, base, _STANDARD_ENUM_DECLARATION, _STANDARD_ENUM_SIZE_FUNC, string_func, __VA_ARGS__)

#define _STANDARD_ENUM(name, base, ...) _STANDARD_ENUM_TEMPLATE(enum, name, base, _STANDARD_ENUM_TO_STRING_FUNC, __VA_ARGS__)
#define _STANDARD_ENUM_CLASS(name, base, ...) _STANDARD_ENUM_TEMPLATE(enum class, name, base, _STANDARD_ENUM_TO_STRING_FUNC, __VA_ARGS__)

#define _STANDARD_ENUM_STRING(name, base, ...) _STANDARD_ENUM_TEMPLATE(enum, name, base, _STANDARD_ENUM_TO_STRING_VALUES_FUNC, __VA_ARGS__)
#define _STANDARD_ENUM_CLASS_STRING(name, base, ...) _STANDARD_ENUM_TEMPLATE(enum class, name, base, _STANDARD_ENUM_TO_STRING_VALUES_FUNC, __VA_ARGS__)

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

#define _VALUE_ENUM_ELEMENT(name, value) name = value
#define _VALUE_ENUM_CASE_STRING(name, value, string) case name: return string;

#define _VALUE_ENUM_DECLARATION(type, name, base, ...)\
_ENUM_DECLARATION(type, name, base, LIST_DO_FOR_EACH_PAIR, _VALUE_ENUM_ELEMENT, __VA_ARGS__)

#define _VALUE_ENUM_SIZE_FUNC(name, ...)\
_ENUM_SIZE_FUNC(name, DO_FOR_EACH_PAIR, __VA_ARGS__)

#define _VALUE_ENUM_TO_STRING_FUNC(name, ...)\
_ENUM_TO_STRING_FUNC(name, DO_FOR_EACH_PAIR, _ENUM_CASE, __VA_ARGS__)

#define _VALUE_ENUM_TO_STRING_VALUES_FUNC(name, ...)\
_ENUM_TO_STRING_FUNC(name, DO_FOR_EACH_THREE, _VALUE_ENUM_CASE_STRING, __VA_ARGS__)

#define _VALUE_ENUM_TEMPLATE(type, name, base, string_func, ...)\
_ENUM_TEMPLATE(type, name, base, _VALUE_ENUM_DECLARATION, _VALUE_ENUM_SIZE_FUNC, string_func, __VA_ARGS__)

#define _VALUE_ENUM(name, base, ...) _VALUE_ENUM_TEMPLATE(enum, name, base, _VALUE_ENUM_TO_STRING_FUNC, __VA_ARGS__)
#define _VALUE_ENUM_CLASS(name, base, ...) _VALUE_ENUM_TEMPLATE(enum class, name, base, _VALUE_ENUM_TO_STRING_FUNC, __VA_ARGS__)

#define _VALUE_ENUM_STRING(name, base, ...) _VALUE_ENUM_TEMPLATE(enum, name, base, _VALUE_ENUM_TO_STRING_VALUES_FUNC, __VA_ARGS__)
#define _VALUE_ENUM_CLASS_STRING(name, base, ...) _VALUE_ENUM_TEMPLATE(enum class, name, base, _VALUE_ENUM_TO_STRING_VALUES_FUNC, __VA_ARGS__)

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

#define _CloneFuncDeclarationTemplate(class_name, base_class_operator)\
virtual class_name* Clone() const base_class_operator;\
void CloneTo(class_name* cloned) const;

#define CloneFuncDeclaration(class_name) _CLONE_FUNC_DECLARATION_TEMPLATE(class_name, )
#define CloneFuncDeclarationOverride(class_name) _CLONE_FUNC_DECLARATION_TEMPLATE(class_name, override)

#define StandardClone(fieldName) fieldName, fieldName
#define PointerDeepClone(fieldName, valueType) fieldName, new valueType(*fieldName)
#define CloneFieldPair(fieldName, value) cloned->fieldName = value;

#define _CloneFuncInClassDefinitionTemplate(class_name, base_class_func, base_class_operator, ...)\
virtual class_name* Clone() const base_class_operator\
{\
    class_name* cloned = new class_name();\
    CloneTo(cloned);\
    return cloned;\
}\
void CloneTo(class_name* cloned) const\
{\
    base_class_func\
    DO_FOR_EACH_PAIR(CloneFieldPair, __VA_ARGS__)\
}

#define CloneFuncInClassDefinition(class_name, ...)\
_CloneFuncInClassDefinitionTemplate(class_name, , , __VA_ARGS__)
#define CloneFuncInClassDefinitionOverride(class_name, base_class_name, ...)\
_CloneFuncInClassDefinitionTemplate(class_name, base_class_name::CloneTo(cloned);, override, __VA_ARGS__)

#define _CloneFuncDefinitionTemplate(class_name, base_class_func, ...)\
class_name* class_name::Clone() const\
{\
	class_name* cloned = new class_name();\
	CloneTo(cloned);\
	return cloned;\
}\
void class_name::CloneTo(class_name* cloned) const\
{\
	base_class_func\
	DO_FOR_EACH_PAIR(CloneFieldPair, __VA_ARGS__)\
}

#define CloneFuncDefinition(class_name, ...)\
_CloneFuncDefinitionTemplate(class_name, , __VA_ARGS__)
#define CloneBaseFuncDefinition(class_name, base_class_name, ...)\
_CloneFuncDefinitionTemplate(class_name, base_class_name::CloneTo(cloned);, __VA_ARGS__)

#define CloneAdvancedBaseFunc(class_name, base_class_name, ...)\
CloneFuncInClassDefinitionOverride(class_name, base_class_name, __VA_ARGS__)
#define CloneAdvancedFunc(class_name, base_class_name, ...)\
CloneFuncInClassDefinition(class_name, base_class_name, __VA_ARGS__)
#define CloneFunc(class_name, ...) CloneAdvancedFunc(class_name, LIST_DO_FOR_EACH(StandardClone, __VA_ARGS__))
#define CloneBaseFunc(class_name, base_class_name, ...)\
CloneAdvancedBaseFunc(class_name, base_class_name, LIST_DO_FOR_EACH(StandardClone, __VA_ARGS__))

#define DeclareCloneFunc(class_name) CloneFuncDeclaration(class_name)
#define DeclareCloneBaseFunc(class_name) CloneFuncDeclarationOverride(class_name)

#define DefineCloneFunc(class_name, ...) CloneFuncDefinition(class_name, __VA_ARGS__)
#define DefineCloneBaseFunc(class_name, base_class_name, ...)\
CloneBaseFuncDefinition(class_name, base_class_name, __VA_ARGS__)

#endif

#pragma endregion

// define USE_EXTRA_MACROS to use macros in extra region
#pragma region EXTRA

#ifdef USE_EXTRA_MACROS

#define VAR_TO_STRING(var) #var

#endif

#pragma endregion