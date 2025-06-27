# 🌡️ Autonomous Environmental Monitoring System – STM32F401RE

## 📌 Overview

This project implements an autonomous environmental monitoring system using the **STM32F401RE** microcontroller, written in **C** with CMSIS and HAL libraries. The system uses **timers**, **interrupts**, **UART**, and peripherals including the **DHT11 sensor**, **LED**, and **capacitive touch sensor**.

---

## 🧰 Hardware & Tools

- **Microcontroller**: Nucleo STM32F401RE (ARM Cortex-M4)
- **Sensors & Peripherals**:
  - DHT11 – Temperature and Humidity Sensor
  - Capacitive Touch Sensor
  - Onboard LED (PA5)
- **Tools**:
  - Keil uVision5 (IDE)
  - PuTTY / Tera Term (for UART monitoring)

---

## ⚙️ Features

### 🔐 1. System Startup – Password & AEM Input
- On power-up, the system prompts for a **password** via UART.
- If the password is correct, it asks for the **AEM** (student ID) for personalized settings.
- If the password is incorrect, an error message is printed and input is requested again.

---

### 📋 2. UART Menu Interface
On successful login, the following menu appears via UART:

```text
=== Environmental Monitoring System ===
Options:
a: Increase sampling interval by 1s (minimum: 2s)
b: Decrease sampling interval by 1s (maximum: 10s)
c: Toggle display: Temperature / Humidity / Both
d: Print last readings and system status
```
Commands are accepted any time, even during normal operation.

---

### 🌡️ 3. Normal Operation – Mode A
- Periodically reads and prints **temperature** and **humidity** values from the DHT11 sensor.
- Display format depends on the active user setting (temp / humidity / both).
- Frequency of readings is user-configurable via UART menu.

---

### 🔄 4. Touch Sensor – Mode Switching
- Each press of the touch sensor toggles the system between:
  - **Mode A (Normal)** – standard monitoring
  - **Mode B (Alert)** – system enters alert behavior if:
    - Temperature > 25°C or
    - Humidity > 60%
- In **Alert Mode**:
  - An **LED blinks every 1 second** when above thresholds.
  - LED stops if values stay within limits for **5 consecutive readings**.

---

### ⏱️ 5. Dynamic Frequency Update (Every 3rd Touch)
- Every **third touch** of the capacitive sensor:
  - Computes a new sampling interval based on the **last two digits of the AEM**:
    - `new_interval = AEM[-2] + AEM[-1]` (e.g., AEM 8753 → 5 + 3 = 8s)

---

### 📡 6. UART Command: `status`
Typing the `status` command via UART displays:
```text
Mode: A (Normal)
Temp: 22.4°C, Humidity: 58%
Touch count: 1
```

---

### 🔥 7. Panic Reset – Extreme Conditions
If:
- **Temperature > 35°C** or
- **Humidity > 80%**
- Persists for **3 consecutive readings**:

Then the MCU software resets

---

## 🗂 Files

- `main.c` – Handles UART input and output and control logic.
- `dht11.c` # DHT11 sensor library
- `touch.c` # Touch sensor library
- `dht11.h` # DHT11 sensor library header
- `touch.h` # Touch sensor library header

---

## 🧪 Sample UART Output
Shown in Demo.mp4 video

---
📌 *Developed for the "Microprocessors & Peripherals" 8th semester course, AUTh, Spring 2025.*
