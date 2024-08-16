# Custom Linux Shell

## Project Overview

This project involves the design and implementation of a custom Linux shell. The shell was engineered using fundamental system calls such as `fork`, `exec`, and `wait`, and it features an interactive prompt, background process management, and efficient reaping of child processes. Additionally, a robust signal handling mechanism was integrated to ensure that `Ctrl+C` terminates only foreground processes, while background operations continue uninterrupted, thereby maintaining the stability of the shell.

## Table of Contents

- [Project Overview](#project-overview)
- [Features](#features)
- [System Requirements](#system-requirements)
- [Installation](#installation)
- [Usage](#usage)
- [Contribution Guidelines](#contribution-guidelines)
- [License](#license)

## Features

- **Interactive Prompt:**
  - The shell provides a user-friendly interactive prompt for executing commands.
  
- **Process Management:**
  - Supports both foreground and background process execution.
  - Efficiently reaps child processes to prevent zombie processes.

- **Signal Handling:**
  - Implements a custom signal handling mechanism where `Ctrl+C` only terminates foreground processes, ensuring that background processes continue to run smoothly.
  
## System Requirements

- **Operating System:** Linux or any Unix-like system.
- **Compiler:** GCC or any C compiler that supports POSIX standards.

## Installation

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/yourusername/custom-linux-shell.git
   cd custom-linux-shell
