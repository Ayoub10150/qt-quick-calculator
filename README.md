# 🧮 Qt/QML Calculator

A calculator application built with **C++ and Qt/QML**, following a modular architecture that separates user interaction, application logic, and mathematical expression evaluation.

The project aims to provide a clean, responsive, and reliable calculator experience while maintaining a clear separation of responsibilities between its main components.

## ✨ Features

- **Basic arithmetic operations:** Addition (+), subtraction (-), multiplication (×), and division (÷).
- **Decimal numbers:** Support for decimal point input.
- **Percentage calculations:** Percentage transformation.
- **Clear functionality:** Reset the current calculation using `C`.
- **Backspace:** Delete the last entered character using `⌫`.
- **Expression evaluation:** Calculate results using `=`.
- **Input validation:** Safely handle malformed expressions and invalid input.
- **Error handling:** Display `Error` for invalid, non-finite, or excessively large numerical results.

## 🏗️ Architecture

The application follows a three-layer architecture designed to keep the interface, application logic, and calculation engine independent.

### 1. AppController

**Responsibility: Application interaction and event dispatching.**

- Receives button interactions from the QML interface.
- Identifies the requested operation.
- Dispatches operations to `ContentManager`.
- Exposes the calculator display property to the QML interface.

### 2. ContentManager

**Responsibility: Calculator state and input management.**

- Maintains the current display and input state.
- Processes digits, operators, and decimal points.
- Handles clear and backspace operations.
- Coordinates expression evaluation.
- Updates the displayed result or error state.

### 3. CalculatorEngine

**Responsibility: Mathematical expression evaluation.**

- Evaluates mathematical expressions.
- Handles arithmetic operations.
- Validates expressions and numerical results.
- Prevents invalid, non-finite, and excessively large results.

### Architecture Overview

```text
          QML Interface
          (main.qml)
               |
               | Button Events
               v
        +----------------+
        | AppController  |
        +----------------+
               |
               | Operation Dispatch
               v
        +----------------+
        | ContentManager |
        +----------------+
               |
               | Expression Evaluation
               v
        +------------------+
        | CalculatorEngine |
        +------------------+
               |
               | Result / Error
               v
        ContentManager
               |
               | Display Update
               v
          QML Interface
```

## 📁 Project Structure

```text
MyCalculator/
│
├── AppController.h
├── AppController.cpp
│
├── ContentManager.h
├── ContentManager.cpp
│
├── CalculatorEngine.h
├── CalculatorEngine.cpp
│
├── main.cpp
└── main.qml
```

*Note: The structure above represents the main application files. Additional build configuration and resource files may also be present.*

## 🛠️ Technologies

- **C++** — Core application logic and calculation engine.
- **Qt** — Application framework.
- **QML** — User interface development.
- **Qt Quick** — Declarative UI components and interaction.

## ⚙️ Supported Operations

| Symbol | Operation |
|:---:|---|
| `+` | Addition |
| `-` | Subtraction |
| `×` | Multiplication |
| `÷` | Division |
| `%` | Percentage |
| `.` | Decimal point |
| `C` | Clear |
| `⌫` | Backspace |
| `=` | Evaluate expression |

## 🛡️ Error Handling

The calculator incorporates input validation and numerical safety measures to ensure reliable behavior.

It handles:

- Malformed mathematical expressions.
- Invalid numerical input.
- Division and operations producing non-finite results.
- Excessively large numerical values.
- Invalid expression evaluation.

When an invalid calculation is detected, the display shows `Error` instead of an invalid numerical result.

## 🚀 Getting Started

1. Clone the repository:

   ```bash
   git clone https://github.com/Ayoub10150/qt-quick-calculator/
   ```

2. Open the project using **Qt Creator**.

3. Configure the project with a compatible Qt kit and compiler.

4. Build the project.

5. Run the application.

*Make sure the required Qt/QML modules are installed and that your development environment is properly configured.*

## 🎯 Project Objectives

This project focuses not only on implementing calculator functionality but also on applying fundamental software engineering principles:

- Separation of concerns.
- Modular application architecture.
- Clear responsibilities between components.
- Maintainable and reusable code.
- Safe input processing and error handling.
- Integration between C++ backend logic and QML frontend components.

---

**Developed with C++ and Qt/QML.**
