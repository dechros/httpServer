# HTTP Server

This project is an HTTP server developed for Windows operating systems and built using the GNU C++ compiler (g++.exe tdm64-1 10.3.0) with the following configurations:

- Compiler Path: `C:\TDM-GCC-64\bin\g++.exe`
- C++ Standard: GNU C++20

## Naming Conventions

### camelCase

Start with a lowercase letter and capitalize the first letter of each subsequent word.

- **Variables**: `myVariable`, `totalAmount`
- **Function Names**: `calculateTotal()`, `getSum()`
- **Member Functions and Variables**: `getLength()`, `setFirstName()`
- **Parameters**: `int numItems`, `const std::string& name`
- **Local Variables**: `int count`, `std::string message`
- **Source Files**: `myClass.cpp`, `utilsFunctions.cpp`
- **Header Files**: `myClass.hpp`, `utilsFunctions.hpp`
- **Namespaces**: `namespace utils { /*... */ }`, `namespace mynamespace { /*...*/ }`

### PascalCase

Start with an uppercase letter and capitalize the first letter of each subsequent word.

- **Class Names**: `MyClass`, `BankAccount`
- **Struct Names**: `MyStruct`, `CustomerInfo`
- **Enum Types**: `enum Color { RED, GREEN, BLUE };`

### UPPERCASE_WITH_UNDERSCORES

Use all uppercase letters with underscores separating words.

- **Constants**: `MAX_SIZE`, `DEFAULT_TIMEOUT`
- **Macro Constants**: `#define PI 3.14159`, `#define BUFFER_SIZE 1024`

## Additional Coding Guidelines

- **Structs with Typedef**: It's recommended to use struct with a typedef instead of using it like `struct StructName varName;` everytime.
- **constexpr vs #define**: Use `constexpr` if possible instead of `#define` macro for defining constants. `constexpr` provides better type safety and scoping, and allows for compile-time evaluation.
- **Use Smart Pointers**: Prefer `std::unique_ptr` or `std::shared_ptr` over raw pointers for managing dynamic memory.
- **RAII**: Follow the RAII (Resource Acquisition Is Initialization) principle to ensure resource cleanup upon object destruction.
- **Avoid Using `using namespace`**: Avoid using `using namespace` directive, especially in header files, to prevent namespace pollution.
- **Const-Correctness**: Use const appropriately to indicate immutability and improve code readability.
- **Follow Single Responsibility Principle (SRP)**: Each class or function should have a single responsibility or reason to change.
- **Use Modern C++ Features**: Utilize features introduced in newer C++ standards (e.g., C++11, C++14, C++17) to write cleaner and more expressive code.
