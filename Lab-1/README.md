# 🏎️ ARM Assembly Hashing & Fibonacci Project

## 🚀 Overview
This project implements a **hashing function** in ARM assembly for an **alphanumeric string** using the Keil development environment. The hash computation follows specific rules, and the final result undergoes a **Fibonacci transformation**! 🌀

📂 Inside the project folder, there are **two implementations**:
1. **Standard Version**: Stores results in memory.
2. **Direct-Result Version (📂 direct-result/)**: Returns an `int` instead of modifying memory through a pointer.

## 🛠️ Requirements
- **Microcontroller:** STM32F401RE (Nucleo Board) 🖥️
- **Tools:** Keil uVision, PuTTY (or any UART communication tool) 🛠️
- **Language:** ARM Assembly & C 🏗️

## 🔍 Functionality
### Step-by-step Execution 🔄
1️⃣ **UART Input Handling (C Main Routine):**
   - Receives **dynamic input** through UART 📡.
   - Calls **assembly routines** for processing 🏗️.

2️⃣ **Hash Computation (Assembly Routine):**
   - **Initializes hash** with the string length 🧮.
   - Modifies hash based on character types:
     - 🔠 **Uppercase letters:** `hash += ASCII * 2`
     - 🔡 **Lowercase letters:** `hash += (ASCII - 97)²`
     - 🔢 **Digits (0-9):** Uses a **lookup table** for predefined values 📊.
     - ❌ **Other characters:** Ignored 🚫.
   - Stores the **hash value** in memory 💾.

3️⃣ **Mod 7 Reduction (Assembly Routine):**
   - If `hash > 9`, **sums its digits** 🔢.
   - Computes `hash % 7` until a **single-digit** result remains ➗.
   - Stores the reduced value in memory 📦.

4️⃣ **Fibonacci Computation (Assembly Routine):**
   - Applies the **Fibonacci function** to the reduced hash value 🔄.
   - Uses **recursion** for computation 🔂.
   - Stores the final **result in memory** 📥.

5️⃣ **Result Display (C Main Routine):**
   - Reads values from memory and **prints results** using `printf()` 🖨️.
   - Displays different results for varied **input cases** 📝.

## 🏁 Example Execution
### 📥 Input:
```
A9b3
```
### 🔢 Hash Calculation:
```
4 (length) + (65*2) + 23 + (1²) + 6 = 164
```
### ➗ Mod 7 Reduction:
```
1+6+4 = 11 → 11 mod 7 = 4
```
### 🌀 Fibonacci Computation:
```
fibonacci(4) = 3
```
### 🖨️ Output:
```
Final Result: 3
```

## 🎯 Bonus Task (+0.5 Bonus Points!) 🎉
A **separate assembly routine** computes the **bitwise XOR checksum** for the input string 🔑.
- Stores **checksum** in memory 📥.
- Prints **checksum result** 🖨️.

## 📂 Files
- **`main.c`** – Handles **UART input/output**.
- **`hash.s`** – Implements the **hashing function**.
- **`mod7.s`** – Performs **Modulo 7 reduction**.
- **`fibonacci.s`** – Computes the **Fibonacci sequence**.
- **`checksum.s`** (Bonus) – Computes **bitwise XOR checksum** 🔐.

📂 **Direct-Result Version (📂 direct-result/)**: This version **returns an `int` directly** from the function instead of modifying memory through a pointer 📌.

---
📌 *Developed for the "Microprocessors & Peripherals" 8th semester course, AUTh, Spring 2025.* 🎓

