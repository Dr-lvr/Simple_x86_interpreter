💻 Simple x86 Interpreter

This project implements a procedural interpreter for a modified assembly-like language. It features a concise 21-token instruction set designed for procedural programming.

The language supports advanced programming concepts, including recursion, unconditional/conditional jumps, and subroutines (procedures) managed by call and ret instructions.

💡 Key Concepts & Operands

x: Always a register name (the destination operand).

y: Can be either an integer literal or the value of another register.

lbl: A label identifier defined using the label: syntax.

🚀 Instruction Set (21 Tokens)

1. Arithmetic and Data Movement

These instructions handle register manipulation and mathematical operations. Note that div performs integer division.

Instruction

Syntax

Description

Equivalent Operation

mov

mov x, y

Copies the value of $y$ into register $x$.

$x \leftarrow y$

inc

inc x

Increases the content of register $x$ by one.

$x \leftarrow x + 1$

dec

dec x

Decreases the content of register $x$ by one.

$x \leftarrow x - 1$

add

add x, y

Adds $y$ to $x$ and stores the result in $x$.

$x \leftarrow x + y$

sub

sub x, y

Subtracts $y$ from $x$ and stores the result in $x$.

$x \leftarrow x - y$

mul

mul x, y

Multiplies $x$ by $y$ and stores the result in $x$.

$x \leftarrow x \times y$

div

div x, y

Performs integer division $x / y$ and stores the result in $x$.

$x \leftarrow x // y$

2. Control Flow and Subroutines

These commands manage the program's flow of execution, including jumps and procedure calls.

Instruction

Syntax

Description

label:

identifier:

Defines a target position for jump and call commands.

jmp

jmp lbl

Unconditionally jumps to the instruction immediately following the label $lbl$.

call

call lbl

Calls the subroutine identified by $lbl$. The return address (the next instruction) is pushed onto the stack.

ret

ret

Returns from the current subroutine. The instruction pointer returns to the address saved by the most recent call.

3. Conditional Jumps and Comparison

All conditional jumps rely on the result of the immediately preceding cmp instruction.

Instruction

Syntax

Description

Condition (based on previous cmp x, y)

cmp

cmp x, y

Compares $x$ (integer or register) and $y$ (integer or register). Sets internal comparison flags.

N/A

jne

jne lbl

Jump if Not Equal.

$x \neq y$

je

je lbl

Jump if Equal.

$x = y$

jge

jge lbl

Jump if Greater or Equal.

$x \ge y$

jg

jg lbl

Jump if Greater.

$x > y$

jle

jle lbl

Jump if Less or Equal.

$x \le y$

jl

jl lbl

Jump if Less.

$x < y$

4. Utility and Program Termination

Instruction

Syntax

Description

msg

msg 'Text:', x, ' End'

Output Generation. Appends text strings (delimited by single quotes) and/or register values to the program's output buffer.

end

end

Program Termination. Indicates that the program has completed successfully and the interpreter should return the stored output.

; comment

; Any text here

Comments. Lines starting with a semicolon (;) are ignored during execution.
