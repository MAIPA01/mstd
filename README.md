# mstd - Maipa's Standard Library

## About

**mstd** (Maipa's Standard Library) is an extension of the C++ standard library, providing additional utility functions,
new data types, and helpful templates for type comparisons. It aims to enhance the standard functionality with useful
features while maintaining compatibility with modern C++. [DOCS](https://maipa01.github.io/mstd/html "Documentation")

## Requirements

- **C++ Standard**: The library is designed for **C++17** and later.
- **Supported Platforms**: The library is intended to work on **Windows, macOS, and Linux**.

## Options

### Compilation defines options

You can enable option using `#define option_name` or use cmake `set(option_name ON CACHE BOOL)`

| Cmake option/C++ define Name     | Description                                                  | Default |
|:---------------------------------|:-------------------------------------------------------------|:-------:|
| `MSTD_ENABLE_CXX20`              | enables c++20 features                                       |   OFF   |
| `MSTD_DISABLE_ASSERT_ON_RELEASE` | makes every assert in this library (void)0 on release builds |   OFF   |
| `MSTD_ENABLE_FOR_EACH_MACROS`    | enables FOR_EACH macros                                      |   OFF   |
| `MSTD_ENABLE_ENUMS_MACROS`       | enables ENUM macros                                          |   OFF   |
| `MSTD_ENABLE_CLONE_FUNC_MACROS`  | enables CLONE_FUNC macros                                    |   OFF   |
| `MSTD_ENABLE_EXTRA_MACROS`       | enables external macros                                      |   OFF   |

### External libraries options

If you want to use external libraries not installed by project using CPM

| Cmake option Name   | Description                                                                           | Default |
|:--------------------|:--------------------------------------------------------------------------------------|:-------:|
| `MSTD_FMT_EXTERNAL` | Enable if you want to use own fmt library (tested and compatible fmt version: 12.1.0) |   OFF   |

### Project developing options

These options are used while testing or changing code in project

| Cmake option Name          | Description                                        |          Default          |
|:---------------------------|:---------------------------------------------------|:-------------------------:|
| `MSTD_BUILD_TESTS`         | Build mstd tests                                   | `${PROJECT_IS_TOP_LEVEL}` |
| `MSTD_BUILD_COVERAGE`      | Enable coverage reporting                          | `${PROJECT_IS_TOP_LEVEL}` |
| `MSTD_BUILD_DOCUMENTATION` | Build documentation                                | `${PROJECT_IS_TOP_LEVEL}` |
| `MSTD_ENABLE_CLANG_TIDY`   | Enables clang-tidy check                           | `${PROJECT_IS_TOP_LEVEL}` |
| `MSTD_INSTALL`             | Enables installation of this project               | `${PROJECT_IS_TOP_LEVEL}` |
| `MSTD_INSTALL_TEST`        | This is only to test if installation of mstd works |            OFF            |

## Features

- **Extended String Manipulation**: Additional functions for modifying and handling strings.
- **Terminal Utilities**: Functions for clearing the terminal and retrieving its dimensions.
- **New Data Structures**:
    - `ordered_map`: A map that preserves the insertion order, similar to `vector`.
    - `ordered_set`: A set that maintains insertion order, like `vector`.
    - `stable_vector`: A vector with stable indexes
- **Safe Arithmetic Operations**: Functions for performing arithmetic with overflow detection:
    - `add_overflow`
    - `sub_overflow`
    - `mul_overflow`
    - `div_overflow`
- **Type Comparisons for Functions**: Templates that allow comparing function types, treating lambdas and function
  pointers as equivalent when they share the same signature.
- **Management Utilities**:
    - `flags`: A class which represents multiple values of given flag-like enum
    - `id_manager`: Manager which helps with managing ids
    - `assert`: Macros for easy defining your own assert
- **Events**:
    - `events_handler`: Event handler

## Installation

After installing, you can use `find_package(mstd)`.

### Components

You can also include components `find_pcakage(mstd COMPONENTS comp)`. They work the same way as [Compilation defines options](#compilation-defines-options), but
they provide separate components you need to include.

| Component Name       | Option                           | Target Name                |
|:---------------------|:---------------------------------|:---------------------------|
| CXX20                | `MSTD_ENABLE_CXX20`              | mstd::CXX20                |
| NO_ASSERT_ON_RELEASE | `MSTD_DISABLE_ASSERT_ON_RELEASE` | mstd::NO_ASSERT_ON_RELEASE |
| FOR_EACH_MACROS      | `MSTD_ENABLE_FOR_EACH_MACROS`    | mstd::FOR_EACH_MACROS      |
| ENUMS_MACROS         | `MSTD_ENABLE_ENUMS_MACROS`       | mstd::ENUMS_MACROS         |
| CLONE_FUNC_MACROS    | `MSTD_ENABLE_CLONE_FUNC_MACROS`  | mstd::CLONE_FUNC_MACROS    |
| EXTRA_MACROS         | `MSTD_ENABLE_EXTRA_MACROS`       | mstd::EXTRA_MACROS         |

## Macros

Before including the `mstd.hpp` or `macros.hpp` header, you can define certain macros to enable specific sets of utility
functions and features within the library.

### Available Macros:

- **MSTD_ENABLE_FOR_EACH_MACROS**: This macro enables access to the `DO_FOR_EACH` macro, which allows you to iterate over
  multiple elements and apply a function to each element.

  Example usage:
  ```cpp
  #define MSTD_ENABLE_FOR_EACH_MACROS
  #include <mstd/mstd.hpp>

  #define TO_STR(x) #x
  // Example of DO_FOR_EACH with TO_STR macro
  DO_FOR_EACH(TO_STR, 1, 2, 3); // Outputs: "1" "2" "3"
  ```

- **MSTD_ENABLE_ENUMS_MACROS**: This macro enables access to the `ENUM` macro, which allows you to define enums or enum classes
  along with helper functions such as `to_string()` and `size<Enum>()`.

  Example usage:
  ```cpp
  #define MSTD_ENABLE_ENUMS_MACROS
  #include <mstd/mstd.hpp>

  // Defining an enum using ENUM macro
  ENUM(Color, Red, Green, Blue);

  // Accessing helper functions generated by ENUM macro
  std::cout << to_string(Color::Red) << std::endl; // Outputs: Red
  std::cout << size<Color>() << std::endl; // Outputs: 3 (the number of elements in the enum)
  ```

- **MSTD_ENABLE_CLONE_FUNC_MACROS**: This macro enables access to macros that help you define basic cloning functions like
  `Clone()` and `CloneTo()` for your custom classes. Specifically, it uses the `CloneFuncInClassDefinition` macro to
  automatically generate the `Clone()` function that returns a pointer to a cloned object.

  Example usage:
  ```cpp
  #define MSTD_ENABLE_CLONE_FUNC_MACROS
  #include <mstd/mstd.hpp>

  // Example of a class with Clone() function returning a pointer using CloneFuncInClassDefinition macro
  class MyClass {
  public:
      int value;
      MyClass(int val) : value(val) {}

      // Using CloneFuncInClassDefinition to define Clone() method
      CloneFuncInClassDefinition(MyClass, value)
  };

  MyClass original(10);
  MyClass* copy = original.Clone(); // Creates a deep copy of original and returns a pointer
  std::cout << "Cloned value: " << copy->value << std::endl; // Outputs: Cloned value: 10
  ```

To enable these macros, define them before including `mstd.hpp`:

```cpp
#define MSTD_ENABLE_FOR_EACH_MACROS
#define MSTD_ENABLE_ENUMS_MACROS
#define MSTD_ENABLE_CLONE_FUNC_MACROS
#include <mstd/mstd.hpp>
```

You can enable one or more of these macros depending on your needs.

## Usage Examples

### Ordered Map & Ordered Set

```cpp
#include <mstd/mstd.hpp>
#include <iostream>

using namespace std;
using namespace mstd;

int main() {
    ordered_map<int, string> om;
    om[1] = "first";
    om[2] = "second";
    om[3] = "third";
    
    for (const auto& [key, value] : om) {
        cout << key << ": " << value << endl;
    }
    return 0;
}
```

### Overflow Detection

```cpp
#include <mstd/mstd.hpp>
#include <iostream>

using namespace std;
using namespace mstd;

int main() {
    size_t result;
    if (add_overflow(1, 2, result)) {
        cout << "Overflow occurred!" << endl;
    } else {
        cout << "Result: " << result << endl;
    }
    return 0;
}
```

### Function Type Comparison

```cpp
#include <mstd/mstd.hpp>
#include <iostream>

using namespace std;
using namespace mstd;

int main() {
    auto lambda = [](string s) -> bool { return !s.empty(); };
    static_assert(is_same_function_v<decltype(lambda), bool(*)(string)>);
    cout << "Lambda and function pointer are of the same type!" << endl;
    return 0;
}
```

## License

This project is licensed under the **BSD 3-Clause License with Attribution Requirement**. See the [LICENSE](./LICENSE "License")
file for more details.
