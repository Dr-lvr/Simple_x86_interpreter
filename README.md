# Simple x86 Interpreter

This project implements a **procedural interpreter** for a modified assembly-like language.  
It features a **21-token instruction set** designed for procedural programming.

The language supports advanced concepts such as:

- Recursion  
- Unconditional and conditional jumps  
- Subroutines (procedures) via `call` and `ret`

---

## Key Concepts & Operands

- **`x`** – Always a register name (destination operand).  
- **`y`** – Either an integer literal or the value of another register.  
- **`lbl`** – A label identifier (`label:`).

---

# Instruction Set (21 Tokens)

## 1. Arithmetic and Data Movement

These instructions handle register manipulation and mathematical operations (`div` is integer division).

| Instruction | Syntax      | Description                                         | Equivalent Operation      |
|------------|-------------|-----------------------------------------------------|---------------------------|
| `mov`      | `mov x, y`  | Copies value of `y` into register `x`.              | `x ← y`                   |
| `inc`      | `inc x`     | Increases value of `x` by 1.                        | `x ← x + 1`               |
| `dec`      | `dec x`     | Decreases value of `x` by 1.                        | `x ← x - 1`               |
| `add`      | `add x, y`  | Adds `y` to `x`.                                    | `x ← x + y`               |
| `sub`      | `sub x, y`  | Subtracts `y` from `x`.                             | `x ← x - y`               |
| `mul`      | `mul x, y`  | Multiplies `x` by `y`.                              | `x ← x × y`               |
| `div`      | `div x, y`  | Performs integer division `x / y`.                 | `x ← x // y`              |

---

## 2. Control Flow and Subroutines

These commands control the execution flow, including jumps and procedure calls.

| Instruction | Syntax      | Description |
|------------|-------------|-------------|
| **Label**  | `identifier:` | Defines a target position for `jmp` or `call`. |
| `jmp`      | `jmp lbl`   | Unconditional jump to label `lbl`. |
| `call`     | `call lbl`  | Calls a subroutine at `lbl`, pushing a return address. |
| `ret`      | `ret`       | Returns from the current subroutine. |

---

## 3. Comparison & Conditional Jumps

Conditional jumps depend on the result of the immediately preceding `cmp`.

| Instruction | Syntax       | Description            | Condition |
|-------------|--------------|------------------------|-----------|
| `cmp`       | `cmp x, y`   | Compares `x` and `y`. Sets flags. | N/A |
| `jne`       | `jne lbl`    | Jump if not equal.     | `x ≠ y` |
| `je`        | `je lbl`     | Jump if equal.         | `x = y` |
| `jge`       | `jge lbl`    | Jump if greater/equal. | `x ≥ y` |
| `jg`        | `jg lbl`     | Jump if greater.       | `x > y` |
| `jle`       | `jle lbl`    | Jump if less/equal.    | `x ≤ y` |
| `jl`        | `jl lbl`     | Jump if less.          | `x < y` |

---

## 4. Utility and Program Termination

| Instruction | Syntax                    | Description |
|------------|-----------------------------|-------------|
| `msg`      | `msg 'Text:', x, ' End'`   | Outputs text and register values to buffer. |
| `end`      | `end`                      | Ends execution and returns output. |
| `; comment`| `; ...`                    | Comments (ignored by interpreter). |

