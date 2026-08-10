# C Coding Style Guidelines

This document outlines the standard coding practices and style guidelines for this project.

---

## 1. Indentation and Spacing

* **Indentation:** Use tabs or 8-space indentation consistently per file context (match existing code).
* **Line Length:** Aim to keep lines under 80–100 characters to ensure readability on standard displays and split long lines gracefully at operators or commas.
* **Parentheses & Keywords:** Place a single space between control flow keywords (`if`, `while`, `for`, `switch`) and their opening parenthesis. 
* **Function Calls:** Do not place a space between a function name and its opening parenthesis.
* **Operators:** Surround binary operators (`=`, `+`, `-`, `<`, `>`, `==`) with spaces. Unary operators (`*`, `&`, `!`, `++`, `--`) should directly touch their operand with no intervening space.

---

## 2. Brace Placement

For control statements (`if`, `else`, `for`, `while`, `do`), place the opening brace on the **same line** as the statement, and the closing brace on its own line.

```c
if (condition) {
    do_something();
} else {
    do_something_else();
}

```

For function definitions, place the opening brace on the **next line** following the function signature:

```c
int compute_total(int a, int b)
{
    return a + b;
}

```

---

## 3. Naming Conventions

* **Variables and Functions:** Use lowercase words separated by underscores (`snake_case`). Avoid single-letter variables except for standard loop indices (e.g., `i`, `j`).
* **Constants and Macros:** Use fully uppercase words separated by underscores (`SCREAMING_SNAKE_CASE`).
* **Predicates vs. Actions:**
* **Actions / Imperative Commands:** Functions that execute a state change or command should be named as verbs and **return an error-code integer** (`0` on success, negative on failure).
* **Predicates:** Functions that check a condition or ask a question (e.g., using `is_`, `has_`, `can_`) should **return a boolean** success/failure indicator.



---

## 4. Preprocessor Macros

* **Multi-Statement Macros:** Any macro containing multiple statements **must** be wrapped in a `do { ... } while (0)` block. This safeguards against syntax and control-flow breakages when used inside unbraced `if/else` conditional statements.

```c
#define SAFE_UPDATE(x, y) do {    \
    lock_resource();              \
    (x) += (y);                   \
    unlock_resource();            \
} while (0)

```

---

## 5. Type Definitions (`typedef`)

Avoid unnecessary `typedef` wrappers for primitive types. Use `typedef` sparingly and only for clear architectural reasons:

1. **Totally Opaque Objects:** To hide underlying data structures completely, forcing consumers to utilize proper accessor functions.
2. **Platform-Independent Integer Abstractions:** Standardized widths where sizing rules depend on build configurations (e.g., fixed-width or architecture-safe types).
3. **Static Analysis Safety:** When companion tooling (such as Sparse) requires strict type-checking differentiation between identical underlying primitives.
