# Gaussian Elimination Solver in C

A robust, terminal-based tool for solving systems of linear equations ($$N * N$$). This project implements **Gaussian Elimination with Partial Pivoting**, allowing it to handle unique solutions, inconsistent systems (no solution), and dependent systems (infinite solutions) with high stability.

## 🚀 Key Features

* **Advanced Solver Engine:** Handles up to 100x100 matrices.
* **Partial Pivoting:** Automatically swaps rows to prevent division-by-zero errors and improve numerical stability.
* **Edge Case Detection:** Correctly identifies and reports systems with **No Solutions** or **Infinite Solutions**.
* **Interactive Editor:** Modify specific cells in your matrix without re-typing the entire system.
* **Demo Mode:** Includes a library of preset matrices (3x3, 4x4, 5x5) to demonstrate different mathematical scenarios.

## 📦 Project Variants

This repository contains three distinct versions of the solver to demonstrate different programming philosophies:

| Version | File Name | Description | Best For |
| --- | --- | --- | --- |
| **Visual / Pro** | `GaussianSolverUI.c` | **The flagship experience.** Features an app-like interface with arrow-key navigation, instant feedback, and a visual grid cursor. Uses `conio.h`. | Windows Users who want a polished UX. |
| **Universal / Safe** | `GaussianSolverSafeMode.c` | **The cross-platform standard.** Stripped of OS-specific dependencies. Uses standard `stdio` menus and works on any C compiler. | Linux, macOS, or legacy terminals. |
| **Annotated** | `GaussianSolver_annotated.c` | **The educational build.** Contains heavy line-by-line comments explaining the logic, memory management, and algorithms. | Students & Code Reviewers. |

## 📂 Repository Structure

* **`GaussianSolverUI.c`**: The "Revamped" version with arrow-key menus and interactive grid editing.
* **`GaussianSolverSafeMode.c`**: The "Universal" version compatible with Linux/macOS.
* **`GaussianSolver.c`**: The original core implementation.
* **`GaussianSolver_annotated.c`**: Heavily commented version for educational purposes.
* **`ForwardEliminationTest.c` / `BackSubstitutionTest.c`**: Unit tests used during development to verify specific parts of the algorithm.

## 🛠️ How to Compile & Run

### Prerequisites

You need a C compiler (like GCC or Clang).

### 1. The Visual UI Version (Windows Only)

This version uses `conio.h` for the dashboard interface.

```bash
gcc GaussianSolverUI.c -o solver_ui
./solver_ui

```

### 2. The Universal Safe Mode (Linux/Mac/Windows)

This version works on any operating system.

```bash
gcc GaussianSolverSafeMode.c -o solver_safe
./solver_safe

```

## 🎮 Controls (UI Version)

* **Arrow Keys (↑ ↓ ← →):** Navigate menus and the matrix grid.
* **ENTER:** Select an option or edit the highlighted cell.
* **ESC:** Go back / Cancel.

## 🧮 How It Works: The Mathematics & Logic

The solver uses the **Gaussian Elimination** algorithm, a systematic method for solving systems of linear equations. The process is divided into three distinct phases:

### 1. Forward Elimination (Row Echelon Form)

The goal of this phase is to transform the augmented matrix into an **Upper Triangular Matrix**, where all elements below the main diagonal are zero.

* **The Pivot:** For each column, the program identifies the "Pivot" (the element on the main diagonal).
* **Partial Pivoting (Row Swapping):** If a pivot is , the program cannot divide by it. The `rowSwap()` function searches rows below the current one to find a non-zero element and swaps the entire rows. This is critical for numerical stability.
* **Row Operations:**
For every row below the pivot, the program calculates a `ratio`:

## $$\text{ratio} = \frac{A_{i,h}}{A_{h,h}}$$

It then performs the row operation:

## $$R_i = R_i - (\text{ratio} \times R_h)$$

It then subtracts the (ratio × Pivot Row) from the Target Row to create a zero.

### 2. Determinant & Consistency Analysis

Before solving, the program analyzes the matrix to see if a unique solution even exists.

* **Determinant Calculation:** The determinant is the product of the diagonal elements, adjusted by the number of row swaps ($k$):
## $$\det(A) = (-1)^k \prod_{i=1}^{n} A_{ii}$$

* **Singular Matrices:** If the Determinant is effectively zero (), the system is singular.
* **No Solution (Inconsistent):** If the variable side of the last row is all zeros but the constant side is non-zero (e.g., ), the system is inconsistent.
* **Infinite Solutions (Dependent):** If the entire last row, including the constant, is zero (e.g., ), the system is dependent.



### 3. Back Substitution

Once the matrix is in Row Echelon Form and confirmed to be consistent, the program solves for the variables in reverse order (from bottom to top).

1. It solves for the last variable () directly.
2. It then moves to the row above, plugs in the known value of , and solves for .
3. This continues until the first variable  is found.

The value of each variable $x_i$ is calculated using:
## $$x_i = \frac{1}{A_{ii}} \left( B_i - \sum_{j=i+1}^{n} A_{ij}x_j \right)$$
---

### 💻 Implementation Details (Software Engineering)

* **Precision:** The solver uses `float` precision with an "Epsilon" check (`1e-6`) to account for floating-point errors during zero-checks.
* **Memory Efficiency:** Uses a static 2D array approach for the `matrix[100][101]` to ensure rapid access without the overhead of complex heap allocation, making it suitable for embedded or low-level systems.
* **Input Validation:** A custom `validinput()` buffer-clearing loop prevents the "Infinite Loop" bug commonly found in C programs when a user enters a character instead of a number.

---

## 📝 Example Output

```text
==== Raw Matrix ====
  [   1.00   1.00   1.00 |   6.00 ]
  [   0.00   2.00   1.00 |   7.00 ]
  [   3.00   2.00  -1.00 |   4.00 ]

==== Partial REF ====
  [   3.00   2.00  -1.00 |   4.00 ]
  [   0.00   2.00   1.00 |   7.00 ]
  [   0.00   0.00   1.00 |   3.00 ]

x =   1.00
y =   2.00
z =   3.00

Determinant = -6

```

---
## 📝 License

This project is open-source and free to use for educational purposes.

# *Created by Renosanceprog*
