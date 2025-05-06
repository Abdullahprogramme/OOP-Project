# **OOP Game Project**

This is a simple object-oriented programming (OOP) game project implemented in C++ using Raylib. 
The game involves navigating through rooms, fighting enemies, collecting potions, and progressing through levels.

---

## **Features**
- **Player Movement**: Navigate through rooms and interact with objects.
- **Enemy AI**: Enemies move independently and engage in combat with the player.
- **Room Progression**: Clear rooms by collecting the coins and the key.
- **Bonus**: Bonus coin appears for 10 seconds and so does a big enemy.

---

## **Setup Instructions**

### **Prerequisites**
1. Install a C++ compiler (e.g., GCC or MinGW for Windows).
2. Install [Raylib](https://www.raylib.com/) (a simple and easy-to-use library for game development).
3. Ensure you have a terminal or command prompt to run commands.

---

### **Compilation**
To compile the project, use the following command:

```bash
g++ -o maze.exe Sources/*.cpp  Main/main.cpp -I C:\raylib1\include -L C:\raylib1\lib -lraylib -lopengl32 -lgdi32 -lwinmm
```

### **Running the game**
```bash
./maze.exe
```

---


### File Structure
```
├── Headers/                # Header files for classes
│   ├── Enemy.hpp           # Enemy class
│   ├── Player.hpp          # Player class
│   ├── Room.hpp            # Room class
│   └── ...                 # Other headers
├── Sources/                # Source files for implementation
│   ├── Enemy.cpp           # Enemy implementation
│   ├── Player.cpp          # Player implementation
│   ├── Room.cpp            # Room implementation
│   └── ...                 # Other sources
├── Main/                   # Main entry point
│   └── main.cpp            # Main game loop
├── README.md               # Project documentation
└── raylib.h                # raylibs file
```

### Steps to run the game:
1. 
```bash
git clone https://github.com/your-username/your-repo-name.git
cd your-repo-name
```

2.
```bash
g++ -o maze.exe Sources/*.cpp  Main/main.cpp -I C:\raylib1\include -L C:\raylib1\lib -lraylib -lopengl32 -lgdi32 -lwinmm
```

3.
```bash
./maze.exe
```

---

### **Dependencies**
1. Raylib: A simple and easy-to-use library for game development.
2. Installation instructions: Raylib Installation Guide
3. C++17: Ensure your compiler supports C++17 or higher.
