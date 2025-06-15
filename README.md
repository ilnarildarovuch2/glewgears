# GLEWGears

![Demo](https://github.com/ilnarildarovuch2/glewgears/raw/master/screen/screenshot.png)

## Description

Classic 3D gear simulation using OpenGL (glxgears), implemented with GLFW for window management and GLEW for OpenGL extension loading. The program renders three interlocking gears with different sizes and colors that rotate in sync.

## Requirements

- C compiler (GCC recommended)
- OpenGL 2.1 compatible graphics hardware
- GLFW library (*nix only)
- GLEW library (*nix only)

## Build Instructions

### Linux/macOS
```bash
gcc main.c -lGLEW -lglfw -lGL -lm -o gears
```

### Windows (32-bit)
```bash
gcc -m32 main.c -o gears.exe -Iinclude -Llib -lglfw3 -lglew32 -lopengl32 -lgdi32
copy lib\glew32.dll . && copy lib\glfw3.dll .
```

## Usage

Run the compiled program:
```bash
./gears
```

### Controls
- **Arrow Keys**: Rotate the camera view
- **ESC**: Exit the program

## License

This code is provided as-is for educational purposes. You're free to use and modify it according to your needs.

## Credits

Based on classic OpenGL gear demos, adapted to GLFW/GLEW for window and context management.