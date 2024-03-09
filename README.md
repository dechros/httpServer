# HTTP Server

This project is an HTTP server written for Windows based operating systems.

## Naming Conventions

- Use `struct` with a `typedef`.
- Use `constexpr` instead `#define` macro.

### camelCase

- **Variables**: Start with a lowercase letter and capitalize the first letter of each subsequent word. (e.g., myVariable, totalAmount).
- **Function Names**: Same as variables (e.g., `calculateTotal()`, `getSum()`).
- **Member Functions and Variables**: Same as variables (e.g., `getLength()`, `setFirstName()`).
- **Parameters**: Same as variables (e.g., `int numItems`, `const std::string& name`).
- **Local Variables**: Same as variables (e.g., `int count`, `std::string message`).
- **Source Files**: Same as variables (e.g., `myClass.cpp`, `utilsFunctions.cpp`).
- **Header Files**: Same as variables, often with .h or .hpp extension (e.g., `myClass.hpp`, `utilsFunctions.hpp`).
- **Namespaces**: Same as variables (e.g., `namespace utils { /*... */ }`, `namespace mynamespace { /*...*/ }`).

### PascalCase

- **Class Names**: Start with an uppercase letter and capitalize the first letter of each subsequent word (e.g., `MyClass`, `BankAccount`).
- **Struct Names**: Same as classes (e.g., `MyStruct`, `CustomerInfo`).
- **Enum Types**: Same as classes (e.g., `enum Color { RED, GREEN, BLUE };`).

### UPPERCASE_WITH_UNDERSCORES

- **Constants**: Use all uppercase letters with underscores separating words (e.g., `MAX_SIZE`, `DEFAULT_TIMEOUT`).
- **Macro Constants**: Same as constants (e.g., `#define PI 3.14159`, `#define BUFFER_SIZE 1024`).
