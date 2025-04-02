# ARM Assembly Hashing & Fibonacci Project

## Overview
This project implements a hashing function in ARM assembly for an alphanumeric string using the Keil development environment. The hash computation follows specific rules, and the final result undergoes a Fibonacci transformation.

## Requirements
- **Microcontroller:** Nucleo STM32F401RE
- **Tools:** Keil uVision, PuTTY (or any UART communication tool)
- **Language:** ARM Assembly & C

## Functionality
The program follows these steps:
1. **UART Input Handling (C Main Routine):**
   - Receives a string input dynamically through UART.
   - Calls assembly routines for processing.

2. **Hash Computation (Assembly Routine):**
   - Initializes hash with the string length.
   - Modifies hash based on character types:
     - **Uppercase letters:** `hash += ASCII * 2`
     - **Lowercase letters:** `hash += (ASCII - 97)²`
     - **Digits (0-9):** Uses a lookup table for predefined values.
     - **Other characters:** Ignored.
   - Stores the hash value in memory.

3. **Mod 7 Reduction (Assembly Routine):**
   - If hash > 9, sums its digits.
   - Computes `hash % 7` until a single-digit result remains.
   - Stores the reduced value in memory.

4. **Fibonacci Computation (Assembly Routine):**
   - Applies the Fibonacci function to the reduced hash value.
   - Uses recursion for computation.
   - Stores the final result in memory.

5. **Result Display (C Main Routine):**
   - Reads values from memory and prints results using `printf()`.
   - Displays different results for varied input cases.

## Example Execution
### Input:
```
A9b3
```
### Hash Calculation:
```
4 (length) + (65*2) + 23 + (1²) + 6 = 164
```
### Mod 7 Reduction:
```
1+6+4 = 11 → 11 mod 7 = 4
```
### Fibonacci Computation:
```
fibonacci(4) = 3
```
### Output:
```
Final Result: 3
```

## Bonus Task (+0.5 Bonus Points)
A separate assembly routine computes the **bitwise XOR checksum** for the input string, storing and printing the result.

## Files
- `main.c` – Handles UART input and output.
- `hash.s` – Implements the hashing function.
- `mod7.s` – Performs modulo 7 reduction.
- `fibonacci.s` – Computes Fibonacci sequence.
- `checksum.s` (Bonus) – Computes bitwise XOR checksum.
---
📌 *Developed for the "Microprocessors & Peripherals" 8th semester course, AUTh, Spring 2025.*
