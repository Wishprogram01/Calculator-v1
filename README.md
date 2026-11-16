
# KALKULATOR GUI C BY ZW
<img width="410" height="532" alt="image" src="https://github.com/user-attachments/assets/5745669c-b2ad-4f2c-8e4b-aaa7707110ff" />

A lightweight dekstop calculator application built entirely in c using the **Immediate mode gui **SDL2** and **openGL3** as the backend

## Features
- Real time display of the complete
- Basic aithmetic operations
- Developed and tested in an ubuntu wsl aka wslg environment

## Compile and Run ( ubuntu/debian )

1. Install the required dependencies :
```bash
sudo apt update
sudo apt install build-essential
libsdl2-dev libgl1-mesa-dev libglew-dev

2. Compile the application :
gcc src/main.c -o calculator \
    -lSDL2 -lGL -lGLEW
./calculator


.
├── src/
│   └── main.c
├── bin/
├── README.md
└── .gitignore

=======
# Calculator-v1
a normal calculator using C language


```bash
have fun :)
