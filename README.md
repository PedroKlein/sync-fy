# Sync-Fy

## Introduction

TODO: Explain what the project is about and what it does.

## Requirements

1. **CMake**: This project requires CMake version 23 or higher. You can check your current version with `cmake --version`. If you need to upgrade, visit the [CMake download page](https://cmake.org/download/).

2. **Conan**: Conan is a portable package manager for C/C++ developers. If you don't have it installed, you can follow the instructions on the [Conan's installation guide](https://docs.conan.io/en/latest/installation.html).

3. **C++ Compiler**: This project requires a C++ compiler that supports C++23. We recommend using GCC 11.4.0 or higher. You can check your current version with `gcc --version`. If you need to upgrade, visit the [GCC download page](https://gcc.gnu.org/install/).

You may install cmake and conan with pip with the following command:
```bash
pip install cmake conan
```

## Development Setup

After ensuring that the requirements are met, follow these steps:

1. Run `conan install conanfile.txt` to install the necessary dependencies.

2. Run `cmake --build ./build/Release --target all` to generate the build files.

3. Run `./build/Release/server/server` to run the server app.

4. Run `./build/Release/client/client` to run the client app.
## Recommended Tools

We recommend using `gitmoji-cli` for commit messages. It's a simple CLI that helps you to use emojis on your commit messages. If you don't have it installed, you can install it with the following command:

```bash
npm i -g gitmoji-cli
```

Also install the git hook using the following command:

```bash
gitmoji -i
```

Then use the following to commit your changes:

```bash
git add .
git commit
```

Remember, the use of `gitmoji-cli` is not mandatory, but it helps to make the commit history clear and easy to understand.

Please ensure you have met all the requirements and followed the setup instructions before starting development. Happy coding!