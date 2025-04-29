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

#endif

#pragma endregion

// define USE_ENUMS_MACROS to use macros in enums region
#pragma region ENUMS

#ifdef USE_ENUMS_MACROS

// STANDARD ENUMS
#define _ENUM_ELEMENT(name) name
#define _ENUM_ELEMENT_COUNT(name) + 1
#define _ENUM_CASE(name) case name: return #name;

#define _ENUM_DECLARATION(type, name, base, ...)\
type name base { LIST_DO_FOR_EACH(_ENUM_ELEMENT, __VA_ARGS__) };

#define _ENUM_SIZE_FUNC(name, ...)\
template<class T> static size_t size();\
template<> static constexpr size_t size<name>() {\
	return DO_FOR_EACH(_ENUM_ELEMENT_COUNT, __VA_ARGS__); }

#define _ENUM_TO_STRING(name, ...)\
static constexpr const std::string to_string(const name& v) {\
	using enum name;\
	switch(v) {\
	DO_FOR_EACH(_ENUM_CASE, __VA_ARGS__)\
	default:\
		return "UNKNOWN";\
	}\
}

#define _ENUM_TEMPLATE(type, name, base, ...)\
	_ENUM_DECLARATION(type, name, base, __VA_ARGS__)\
	_ENUM_SIZE_FUNC(name, __VA_ARGS__)\
	_ENUM_TO_STRING(name, __VA_ARGS__)

#define ENUM(name, ...) _ENUM_TEMPLATE(enum, name, , __VA_ARGS__)
#define ENUM_BASE(name, base, ...) _ENUM_TEMPLATE(enum, name, : base, __VA_ARGS__)
#define ENUM_CLASS(name, ...) _ENUM_TEMPLATE(enum class, name, , __VA_ARGS__)
#define ENUM_CLASS_BASE(name, base, ...) _ENUM_TEMPLATE(enum class, name, : base, __VA_ARGS__)

// ENUMS WITH VALUES
#define _ENUM_VALUE_ELEMENT(name, value) name = value
#define _ENUM_VALUE_ELEMENT_COUNT(name, value) + 1
#define _ENUM_VALUE_CASE(name, value) case name: return #name;

#define _ENUM_VALUE_DECLARATION(type, name, base, ...)\
type name base { LIST_DO_FOR_EACH_PAIR(_ENUM_VALUE_ELEMENT, __VA_ARGS__) };

#define _ENUM_VALUE_SIZE_FUNC(name, ...)\
template<class T> static size_t size();\
template<> static constexpr size_t size<name>() {\
	return DO_FOR_EACH_PAIR(_ENUM_VALUE_ELEMENT_COUNT, __VA_ARGS__); }

#define _ENUM_VALUE_TO_STRING(name, ...)\
static constexpr const std::string to_string(const name& v) {\
	using enum name;\
	switch(v) {\
	DO_FOR_EACH_PAIR(_ENUM_VALUE_CASE, __VA_ARGS__)\
	default:\
		return "UNKNOWN";\
	}\
}

#define _ENUM_VALUE_TEMPLATE(type, name, base, ...)\
	_ENUM_VALUE_DECLARATION(type, name, base, __VA_ARGS__)\
	_ENUM_VALUE_SIZE_FUNC(name, __VA_ARGS__)\
	_ENUM_VALUE_TO_STRING(name, __VA_ARGS__)

#define ENUM_VALUE(name, ...) _ENUM_VALUE_TEMPLATE(enum, name, , __VA_ARGS__)
#define ENUM_BASE_VALUE(name, base, ...) _ENUM_VALUE_TEMPLATE(enum, name, : base, __VA_ARGS__)
#define ENUM_CLASS_VALUE(name, ...) _ENUM_VALUE_TEMPLATE(enum class, name, , __VA_ARGS__)
#define ENUM_CLASS_BASE_VALUE(name, base, ...) _ENUM_VALUE_TEMPLATE(enum class, name, : base, __VA_ARGS__)

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