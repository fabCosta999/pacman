# 🎮 Interrupt-Driven Pacman on ARM Cortex-M3 (LPC1768)

Embedded implementation of the Pacman game running on the NXP LPC1768 (ARM Cortex-M3) microcontroller, developed for the LandTiger development board.

The project is fully interrupt-driven and designed following low-level embedded programming principles, including custom startup code, vector table configuration, peripheral management, and low-power execution.

---

## 🧠 Hardware Platform

- Microcontroller: NXP LPC1768 (ARM Cortex-M3)
- Board: LandTiger LPC1768
- Display: GLCD
- Input: Joystick + external button
- Peripherals used:
  - RIT (Repetitive Interrupt Timer)
  - Timer
  - ADC
  - CAN
  - External Interrupts

---

## 🏗 Architecture Overview

The system is fully event-driven.

The `main()` function initializes peripherals and then places the CPU in sleep mode:

```c
while (1) {
    __ASM("wfi");
}
```

Execution continues exclusively through interrupts.

### Key Architectural Features

- Custom startup file (`startup_LPC17xx.s`)
- CMSIS-based system configuration (`system_LPC17xx.c`)
- Vector table defined in assembly
- PLL-based clock configuration
- Interrupt-driven game loop (RIT-based tick)
- Low-power design using `WFI`
- Modular peripheral abstraction

---

## 🎮 Game Logic

The game engine is structured in independent modules:

- Game logic
- Ghost AI
- GLCD rendering
- Input handling (Joystick + Button)
- Timer management

### Ghost AI

Ghost movement is implemented using a Breadth-First Search (BFS) pathfinding algorithm over the maze grid.

Key aspects:

- Static queue allocation
- Predecessor tracking matrix
- No dynamic memory allocation
- Fully compatible with embedded memory constraints

This ensures deterministic behavior and low overhead.

---

## 📂 Project Structure

```
pacman-lpc1768/
│
├── sample.c
├── adc/
├── button/
├── CAN/
├── CMSIS_core/
├── Game/
├── GLCD/
├── joystick/
├── led/
├── music/
├── RIT/
├── timer/
├── startup_LPC17xx.s
├── system_LPC17xx.c
└── README.md
```

Build artifacts and IDE-specific files are intentionally excluded.

---

## ⚙ Technical Highlights

- Bare-metal programming (no RTOS)
- Interrupt prioritization via NVIC
- Peripheral register-level configuration
- Low-power execution model
- Deterministic real-time game loop
- Static memory management
- Embedded-safe data structures

---

## 🧪 Development Environment

- Keil µVision
- ARM CMSIS
- C + Assembly

---

## 📌 Educational Context

Developed as part of an Embedded Systems course.

The objective was to design a fully functional real-time game on bare-metal ARM, emphasizing:

- Hardware-level understanding
- Interrupt management
- Memory constraints
- Real-time constraints
- Modular embedded design

---

## 📜 Notes

The project uses CMSIS startup and system files for LPC17xx provided by ARM. Only minimal configuration adjustments were applied where necessary.

This repository focuses on the application logic and embedded system integration.
