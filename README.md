# mstd - Maipa's Standard Library

## About

**mstd** (Maipa's Standard Library) is an extension of the C++ standard library, providing additional utility functions, new data types, and helpful templates for type comparisons. It aims to enhance the standard functionality with useful features while maintaining compatibility with modern C++.

## Features

- **Extended String Manipulation**: Additional functions for modifying and handling strings.
- **Terminal Utilities**: Functions for clearing the terminal and retrieving its dimensions.
- **New Data Structures**:
  - `ordered_map`: A map that preserves the insertion order, similar to `vector`.
  - `ordered_set`: A set that maintains insertion order, like `vector`.
- **Safe Arithmetic Operations**: Functions for performing arithmetic with overflow detection:
  - `add_overflow`
  - `sub_overflow`
  - `mul_overflow`
  - `div_overflow`
- **Type Comparisons for Functions**: Templates that allow comparing function types, treating lambdas and function pointers as equivalent when they share the same signature.

## Requirements

- **C++ Standard**: The library is designed for **C++20** and later.
- **Supported Platforms**: The library is intended to work on **Windows, macOS, and Linux**.

## Installation

No additional dependencies are required. Simply include the **mstd** headers in your project.

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

bool testFunc(string s) { return !s.empty(); }

int main() {
    auto lambda = [](string s) -> bool { return !s.empty(); };
    static_assert(is_same_function_v<decltype(lambda), bool(*)(string)>);
    cout << "Lambda and function pointer are of the same type!" << endl;
    return 0;
}
```

## License

This project is licensed under the **BSD 3-Clause License with Attribution Requirement**. See the [`LICENSE`](./LICENSE) file for more details.