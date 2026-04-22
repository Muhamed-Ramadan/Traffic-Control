# 🚦 Traffic Light System with On-Demand Crosswalk

A bare-metal embedded traffic light controller for the **ATmega32 microcontroller**, featuring interrupt-driven pedestrian crossing and a clean 3-layer software architecture. Built as the capstone project of the **Embedded Software Professional Nanodegree** — offered by the **Egyptian Ministry of Communications and Information Technology (MCIT)** in collaboration with **Udacity**.

[![GitHub](https://img.shields.io/badge/GitHub-Muhamed--Ramadan-black?logo=github)](https://github.com/Muhamed-Ramadan)
[![LinkedIn](https://img.shields.io/badge/LinkedIn-muhamed--ramadan-blue?logo=linkedin)](https://www.linkedin.com/in/muhamed-ramadan)

---

## 📌 Table of Contents

- [Project Overview](#project-overview)
- [System Demo](#system-demo)
- [Hardware Requirements](#hardware-requirements)
- [Software Architecture](#software-architecture)
- [System Behavior](#system-behavior)
- [State Machine Flowchart](#state-machine-flowchart)
- [Project Structure](#project-structure)
- [Drivers & APIs](#drivers--apis)
- [Design Decisions](#design-decisions)
- [Getting Started](#getting-started)
- [Testing](#testing)
- [Known Limitations & Future Work](#known-limitations--future-work)

---

## Project Overview

This project implements a **real-time traffic light controller** for vehicles and pedestrians. In normal mode the vehicle signal cycles automatically through Green → Yellow (blink) → Red → Yellow (blink). A pedestrian crosswalk button connected to the external interrupt INT0 can interrupt the current cycle at any point and trigger a safe crossing sequence.

The system is written in **bare-metal C** with no operating system and no HAL library — all hardware access goes through a custom driver stack built from scratch using memory-mapped register macros, following **SOLID principles** and a **layered architecture**.

---

## System Demo

### Proteus Simulation Circuit

![Proteus Simulation](simulation_screenshot.png)

The circuit includes two 3-LED traffic lights (vehicle and pedestrian) and a push button connected to INT0.

### Simulation GIF

![Demo](demo.gif)

---

## Hardware Requirements

| Component | Details |
|---|---|
| Microcontroller | ATmega32 @ 8 MHz |
| Vehicle LEDs | Green (PA0), Yellow (PA1), Red (PA2) — Port A |
| Pedestrian LEDs | Green (PB0), Yellow (PB1), Red (PB2) — Port B |
| Push Button | INT0 (PD2), rising-edge triggered, 230Ω pull-down |
| IDE | Microchip Studio (Atmel Studio) |
| Simulator | Proteus Design Suite |

---

## Software Architecture

The project follows a strict 3-layer architecture — each layer depends only on the one below it:

```
┌──────────────────────────────────────────┐
│           Application Layer              │
│   APPS.c / APPS.h                        │
│   System logic, state machine, ISR       │
├──────────────────────────────────────────┤
│              MCAL Layer                  │
│   EXT_INT.c  — External Interrupt (INT0) │
│   Timers.c   — Timer0 driver             │
│   ISR_Interrupt.h — ISR vector macros    │
├──────────────────────────────────────────┤
│            Utility Layer                 │
│   bitMath.h    — Bit manipulation macros │
│   dataTypes.h  — Portable type aliases   │
│   registers.h  — Register address macros │
└──────────────────────────────────────────┘
```

---

## System Behavior

### Normal Mode

The vehicle signal cycles continuously. Each state lasts 5 seconds:

```
GREEN  ──►  YELLOW (blink×4)  ──►  RED  ──►  YELLOW (blink×4)  ──►  GREEN ...
```

The pedestrian signal is **inactive during normal mode** — it only activates when the crosswalk button is pressed. This matches real-world pedestrian signal behavior.

### Pedestrian Mode

Pressing the button triggers INT0 (rising edge only — long press has no effect). The response depends on which normal-mode state was active at the moment of the press:

| Scenario | Condition at press | Response |
|---|---|---|
| 1 | Car Green is ON | Yellow blink (5s) → Car Red + Ped Green (5s) → transition back |
| 2 | Car Yellow is blinking | Yellow blink (5s) → Car Red + Ped Green (5s) → transition back |
| 3 | Car Red is already ON | Car Red + Ped Green immediately (5s) → transition back |
| 4 | Long press | No action (INT0 triggers only on rising edge) |
| 5 | Double press | First press handled normally; second press ignored |

**End of every crossing sequence:**

```
Both Yellow blink (5s)  ──►  Car Green + Ped Red (5s)  ──►  Normal Mode resumes
```

---

## State Machine Flowchart

![System Flowchart](docs/system-flowchart.svg)

---

## Project Structure

```
traffic-light-system/
├── src/
│   ├── main.c
│   ├── APPs/
│   │   ├── APPS.c                  # System logic, state machine, ISR
│   │   └── APPS.h                  # Pin defines, state enum, prototypes
│   ├── MCAL/
│   │   ├── External_Interrupt/
│   │   │   ├── EXT_INT.c           # INT0/1/2 driver
│   │   │   └── EXT_INT.h
│   │   ├── ISR_Interrupts/
│   │   │   └── ISR_Interrupt.h     # ISR macro, sei/cli, vector names
│   │   └── Timer/
│   │       ├── Timers.c            # Timer0: init, delay, force-stop
│   │       └── Timers.h
│   └── Utility/
│       ├── bitMath.h               # SET/CLR/TGL/GET bit macros
│       ├── dataTypes.h             # u8, u16, u32, s8 ...
│       └── registers.h             # PORTA, TCCR0, GICR ... macros
├── Test Modules/
│   ├── Testing.atsln               # Microchip Studio solution for driver tests
│   ├── src/
│   │   ├── main.c                  # Entry point — uncomment the target test to run
│   │   ├── Testing.cproj
│   │   ├── External_Interrupt/
│   │   │   ├── APP_E.c             # Test: INT0 init + toggle on interrupt
│   │   │   ├── EXT_INT.c / .h      # Driver under test
│   │   │   └── main.c
│   │   ├── ISR_Interrupt/
│   │   │   ├── APP_I.c             # Test: ISR macro and vector binding
│   │   │   └── ISR_Interrupt.h
│   │   ├── Timers/
│   │   │   ├── APP_T.c             # Test: Timer0 delay accuracy (toggle every 1s)
│   │   │   └── Timers.c / .h
│   │   └── bitMath/
│   │       ├── APP_M.c             # Test: SET/CLR/TGL macros on real pins
│   │       └── bitMath.h
│   └── Simulation/
│       ├── Sim.pdsprj              # Proteus simulation for driver tests
│       └── Testing.hex             # Pre-built firmware for the test simulation
├── Simulation/
│   ├── Sim.pdsprj                  # Main system simulation
│   └── Traffic_Light_System.hex   # Pre-built firmware — load directly into Proteus
├── Design/
│   ├── System Design.docx          # Full system design document
│   └── System Design.pdf
├── docs/
│   ├── ATmega32_Datasheet.pdf
│   └── system-flowchart.svg
├── Videos/
│   ├── static design.mkv                        # System design walkthrough
│   ├── Application_layer App Driver.mkv          # App layer code walkthrough
│   ├── MCAL_layer 01 ISR_Interrupts_Driver.mkv   # ISR driver walkthrough
│   ├── MCAL_layer 02 External_Interrupt_Driver.mkv
│   ├── MCAL_layer 03 Timers_Driver.mkv
│   ├── Utility layer 01 dataTypes.mkv
│   ├── Utility layer 02 bitMath.mkv
│   ├── Utility layer 03 registers.mkv
│   ├── User Story 1 .mkv                         # Crossing while car Green is ON
│   ├── User Story 2.mkv                          # Crossing while Yellow is blinking
│   ├── User Story 3.mkv                          # Crossing while car Red is ON
│   ├── User Story 4.mkv                          # Long press — no action
│   ├── User Story 5.mkv                          # Double press — only first handled
│   └── Testing Videos/
│       ├── Test1_External_Interrupt.mkv          # INT0 driver test demo
│       ├── Test2_ISR_Interrupt.mkv               # ISR macro test demo
│       ├── Test3_bitMath.mkv                     # bitMath macros test demo
│       └── Test4_Timer.mkv                       # Timer0 delay accuracy test demo
├── simulation_screenshot.png
├── traffic_light.atsln
├── .gitignore
└── README.md
```

> **Note:** All walkthrough and test videos are narrated in Arabic.

---

## Drivers & APIs

### Utility Layer

#### `bitMath.h`
```c
SETBit(REG, BIT_NO)    // Set single bit
CLRBit(REG, BIT_NO)    // Clear single bit
TGLBit(REG, BIT_NO)    // Toggle single bit
GETBit(REG, BIT_NO)    // Read single bit
SETBits(REG, bMsk)     // Set bits by mask
CLRBits(REG, bMsk)     // Clear bits by mask
SETALLBits(REG)        // Set all bits
CLRALLBits(REG)        // Clear all bits
```

#### `dataTypes.h`
```c
typedef unsigned char  u8;
typedef unsigned int   u16;
typedef unsigned long  u32;
typedef signed char    s8;
```

#### `registers.h`
Memory-mapped access to ATmega32 registers via `SELECTOR(ADDRESS)` — covers Port A/B/C/D, External Interrupt registers (MCUCR, GICR), and Timer0 registers (TCCR0, TCNT0, TIMSK).

---

### MCAL Layer

#### `Timers.c`
```c
void Timer0_Init(timer_modes_EN mode, Prescaler_EN prescaler);
void Timer0_Start(void);
void Timer0_Stop(void);
void ResetTimer(void);
void timer_delay_us(u32 delay);   // Busy-wait delay in microseconds
void Force_Stop_Timer0(void);     // Forces any in-progress delay to exit immediately
```

#### `EXT_INT.c`
```c
void INT_init(INT_NUM int_num, SENSE_CONTROL sense_control);
// int_num:       INT_0 | INT_1 | INT_2
// sense_control: low_level | any_level | rising_edge | falling_edge
```

#### `ISR_Interrupt.h`
```c
sei()          // Enable global interrupts
cli()          // Disable global interrupts
ISR(INT_VECT)  // Declare ISR — vectors: INT0_vect, TIMER0_OVF_vect
```

---

### Application Layer

#### `APPS.c`
```c
void APP_Start(void);       // Initialize GPIO, Timer0, INT0
void APP_Run(void);         // Main loop: Normal Mode + mode switching
void Pedestrian_Mode(void); // Full crossing sequence (state-aware)
ISR(INT0_vect);             // Button handler: sets flag, exits current delay
```

**Global state variables:**

| Variable | Type | Role |
|---|---|---|
| `state` | `STATE_type` | Current normal-mode state (Yellow_BEFORE_GREEN, GREEN, Yellow_AFTER_GREEN, RED) |
| `pedestrian_mode_flag` | `u8` | 0 = normal mode, 1 = pedestrian mode active |
| `ON_Period` | `u32` | Active delay period; zeroed by ISR to skip remaining wait |

---

## Design Decisions

### ISR placed in the application layer

`ISR(INT0_vect)` lives in `APPS.c` rather than the MCAL layer. This is intentional — the ISR contains application-level logic: it manages the `pedestrian_mode_flag` state and zeroes `ON_Period` to control system flow. Placing it in MCAL would require a callback mechanism that adds unnecessary complexity for this scope.

### Interrupt-safe state exit

When the button is pressed mid-cycle, the system must exit the current 5-second delay immediately. Two mechanisms work together inside `ISR(INT0_vect)`:

```c
ISR(INT0_vect) {
    if (pedestrian_mode_flag == 0) {
        pedestrian_mode_flag = 1;
        ON_Period = 0;          // next timer_delay_us(0) call returns instantly
        Force_Stop_Timer0();    // exits any in-progress delay on its next iteration
    }
}
```

`ON_Period = 0` handles a press that arrives *between* two delay calls. `Force_Stop_Timer0()` handles a press that arrives *during* a delay call — it sets the internal overflow counter to `~0` so the busy-wait loop exits on its very next check without waiting for the remaining time.

### Re-entrancy and double-press prevention

`pedestrian_mode_flag` acts as a one-shot guard. Once set to `1` by the ISR, the `if (pedestrian_mode_flag == 0)` check at the top of the ISR blocks any subsequent button press — whether a double press or a press during the crossing sequence — from re-triggering the pedestrian logic. The flag is only reset to `0` at the very end of `Pedestrian_Mode()`, after the full crossing sequence completes.

### State-aware crossing

`Pedestrian_Mode()` reads the `state` variable to decide whether the Yellow warning blink is necessary before giving the pedestrian a green signal:

- State is `RED` → vehicles are already stopped → skip the Yellow blink, proceed directly to Ped Green.
- Any other state → show Yellow blink on both signals first, then proceed to Ped Green.

This ensures the system never skips a safety step regardless of when the button is pressed.

---

## Getting Started

### Prerequisites
- [Microchip Studio](https://www.microchip.com/en-us/tools-resources/develop/microchip-studio)
- [Proteus Design Suite](https://www.labcenter.com/)

### Run the simulation
1. Clone the repository
2. Open `Simulation/Sim.pdsprj` in Proteus
3. The pre-built `Traffic_Light_System.hex` is already loaded — press Run
4. Press the button in the simulation to trigger pedestrian mode

### Build from source
1. Open `traffic_light.atsln` in Microchip Studio
2. Build → produces `src/Debug/traffic_light.hex`
3. Load the `.hex` into the ATmega32 component in Proteus and run

---

## Testing

Each MCAL driver was tested independently before system integration. The approach: modify `Test Modules/src/main.c` to call the target driver's test module, rebuild, and run the dedicated test simulation in `Test Modules/Simulation/Sim.pdsprj`.

| Driver | Test module | What it validates |
|---|---|---|
| `bitMath` | `APP_M.c` | SET/CLR/TGL macros observed on real pins — LED toggles at 1s intervals |
| `Timer0` | `APP_T.c` | `timer_delay_us(1000000)` produces accurate 1-second toggle on PA0 |
| `External Interrupt` | `APP_E.c` | INT0 rising-edge fires ISR and toggles LED; INT1 has no effect (isolation check) |
| `ISR macro` | `APP_I.c` | ISR macro expands correctly and vector binding compiles without conflict |

> The test simulation circuit contains two LEDs and two push buttons (INT0 and INT1). INT0 toggles the red LED on each press; INT1 has no effect — this validates both correct interrupt initialization and driver isolation.

### User story results

| Story | Scenario | Expected | Result |
|---|---|---|---|
| US-1 | Short press while car Green ON | Yellow blink → Car Red + Ped Green | ✅ Pass |
| US-2 | Short press while car Yellow blinking | Yellow blink → Car Red + Ped Green | ✅ Pass |
| US-3 | Short press while car Red ON | Ped Green immediately | ✅ Pass |
| US-4 | Long press | No action | ✅ Pass |
| US-5 | Double press | First press handled, second ignored | ✅ Pass |

Demo recordings for all user stories and driver tests are in `Videos/`. All videos are narrated in Arabic.

---

## Known Limitations & Future Work

**No HAL for LEDs and button**
The application layer controls LEDs and reads the button directly using `bitMath.h` macros, without a dedicated GPIO Hardware Abstraction Layer. This couples pin assignments to the application logic. A proper HAL (e.g., `hal/led.c`, `hal/button.c`) would improve portability and testability, and is the natural next step in the architecture.

**Potential improvements**
- Add GPIO HAL for LED and button abstraction
- Add software debouncing at the HAL level
- Replace busy-wait delays with interrupt-driven timing for better CPU efficiency
- Extend to support multiple crosswalk directions

---

## Author

**Mohamed Ramadan**

[![GitHub](https://img.shields.io/badge/GitHub-Muhamed--Ramadan-black?logo=github)](https://github.com/Muhamed-Ramadan)
[![LinkedIn](https://img.shields.io/badge/LinkedIn-muhamed--ramadan-blue?logo=linkedin)](https://www.linkedin.com/in/muhamed-ramadan)