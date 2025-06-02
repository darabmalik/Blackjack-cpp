# 🃏 Blackjack Game (Qt C++)

A GUI-based Blackjack game built using **Qt (C++)** and **CMake**. This game provides a simple and interactive interface for playing Blackjack, simulating a real casino card game experience.

## 🎮 Features

- Graphical User Interface (GUI) built with Qt
- Clickable buttons for user interaction
- Card images displayed from Qt resources
- Basic Blackjack rules implemented (Hit, Stand, Dealer logic)
- Cross-platform support (Windows, Linux, macOS)

## 📁 Project Structure

```bash
blackjack/
├── cards/ # Contains card image assets
├── build/ # Build directory (auto-generated, can be deleted)
├── CMakeLists.txt # CMake configuration file
├── main.cpp # Application entry point
├── mainwindow.cpp # Main game logic
├── mainwindow.h # Header for main window
├── mainwindow.ui # UI layout file (Qt Designer)
├── resource.qrc # Qt resource file (links to images, icons)
```

## 🛠 Installation & Running

### ✅ Prerequisites

- [Qt SDK](https://www.qt.io/download)
- [Qt Creator IDE](https://www.qt.io/product/development-tools) (recommended)
- CMake
- MinGW compiler (on Windows)

### 🚀 Run via Qt Creator (Recommended)

1. Open `CMakeLists.txt` in **Qt Creator**
2. Select appropriate Qt Kit (e.g., Qt 6.x with MinGW 64-bit)
3. Click **Build** (or press `Ctrl+B`)
4. Click **Run** (or press `Ctrl+R`)

## 📃 License
This project is for educational/demo purposes. You may modify and use it freely in personal projects.
