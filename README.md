# Extendible Hashing

## Requirements
- `g++` (GNU Compiler) or Visual Studio (MSVC)

If `g++` is not installed, the following commands will install it based on your operating system:

### For Unix-based Systems (Linux/macOS):
1. Open a terminal and run the following command:
    ```bash
    sudo apt-get install g++     # For Debian-based Linux (Ubuntu)
    sudo yum install gcc-c++     # For RedHat-based Linux (CentOS, Fedora)
    brew install gcc             # For macOS using Homebrew
    ```

### For Windows:  
You have two options either install `g++` or run with Visual Studio.
1. If using `g++`, install it using [MSYS2](https://www.msys2.org/) or the [MinGW-w64](https://www.mingw-w64.org/downloads/#mingw-w64-builds) installer:
    - Install `MSYS2` and run the following in the MSYS2 terminal:
    ```bash
    pacman -S mingw-w64-x86_64-gcc
    ```
    - Or, use the MinGW-w64 installer.

## Build Instructions

### For Windows / VSCode (g++ required)
1. Open a terminal in the project directory.
2. Run the following command to build the project:
    ```bash
        .\build.bat
    ```
3. Run the following command to execute the project:
    ```bash
        .\build\run.exe
    ```

### For Visual Studio
1. Open a new project in Visual Studio.
2. Add all the files in the `src/` directory to the project.
3. **Set the C++ language standard to C++17 or higher**:
    1. Right-click on your project in the **Solution Explorer**.
    2. Select **Properties**.
    3. In the left-hand menu, navigate to **Configuration Properties** > **C/C++** > **Language**.
    4. Under **C++ Language Standard**, select **ISO C++17 Standard (/std:c++17)** or higher from the dropdown menu.
    5. Click **Apply** and **OK** to save the changes.
4. Build the project with `CTRL+B`
5. Run the project

### For Unix-based Systems (Linux/macOS)
1. Open a terminal in the project directory.
2. Run the following command to build the project:
    ```bash
        make
    ```
3. Run the following command to execute the project:
    ```bash
        ./build/run
    ```
4. Run the following command to clean the project:
    ```bash
        make clean
    ```
