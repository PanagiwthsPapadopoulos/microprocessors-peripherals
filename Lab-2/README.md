# 🔢 UART-Controlled LED Analyzer with Interrupt Handling

A C-based embedded project for ARM Cortex-M microcontrollers (e.g., NUCLEO M4) that reads integer input via UART, analyzes digits using timer interrupts, and controls an LED accordingly. Includes user interaction, button-based interrupt behavior, and dynamic analysis control.

---

## ⚙️ Features

### 📥 UART Input
- Accepts user-entered integers via UART (e.g., `52719`).
- Ignores non-numeric characters (except an optional trailing `-`).

### ⏱ Digit Analysis (Timer Interrupt)
- Analyzes digits sequentially every **0.5 seconds**.
- **Even digits** → LED blinks (200ms ON, 200ms OFF).
- **Odd digits** → LED toggles and holds the new state.
- Each action is logged to the UART terminal.

### 🔘 Button Interrupt
- During analysis:
  - Pressing the button **locks the LED** in its current state.
  - Logs: `Interrupt: Button pressed. LED locked. Count = X`
  - Further digits are logged but do not affect the LED.
- Pressing the button again **unlocks** LED behavior.

### 🔁 Input Control
- New numbers can be entered **any time**, interrupting current analysis.
- If input ends with a `-`, analysis loops infinitely on that number.
  - Loop ends only when a new valid number is entered.

---

## 🧪 Example UART Output
```
Input: 52719
Digit 5 -> Toggle LED
Digit 2 -> Blink LED
Digit 7 -> Toggle LED
Interrupt: Button pressed. LED locked. Count = 1
Digit 1 -> Skipped LED action
Digit 9 -> Skipped LED action
Interrupt: Button pressed. LED unlocked. Count = 2
End of sequence. Waiting for new number…

Input: 81-
Digit 8 -> Blink LED
Digit 1 -> Toggle LED
Digit 8 -> Blink LED
Digit 1 -> Toggle LED
…

(New input received)
Input: 42
Digit 4 -> Blink LED
Digit 2 -> Blink LED
End of sequence. Waiting for new number…
```

---

## 🛠 Requirements

- ARM Cortex-M microcontroller (e.g., **STM32 NUCLEO M4**)
- UART-capable terminal (e.g., [Tera Term](https://tera-term.en.softonic.com/))
- LED and button connected to GPIO pins
- **Keil uVision** or compatible embedded C toolchain

---

## 🧰 Build & Flash

1. Open the project in Keil or your preferred IDE.
2. Configure the target MCU and board support.
3. Connect via UART to monitor activity.
4. Flash the code and open the terminal (baud rate: 9600 by default).

---

## 📂 File Structure
```
/src
├── main.c             # Main application logic
├── uart.c/.h          # UART communication handling
├── timer.c/.h         # Timer interrupt setup
├── led.c/.h           # LED control logic
├── button.c/.h        # Button interrupt logic
```
---
📌 Developed for the "Microprocessors & Peripherals" 8th semester course, AUTh, Spring 2025. 🎓
