# Sync-Fy

## Introduction

Sync-Fy is a project focused on implementing a service akin to Dropbox, enabling seamless sharing and automatic synchronization of files across various devices belonging to the same user. The application is designed to run in Unix environments, specifically on Linux. The implementation leverages the Unix Transmission Control Protocol (TCP) sockets API, inotify API and C++.

## Requirements

1. **CMake**: This project requires CMake version 23 or higher. You can check your current version with `cmake --version`. If you need to upgrade, visit the [CMake download page](https://cmake.org/download/).

2. **Conan**: Conan is a portable package manager for C/C++ developers. If you don't have it installed, you can follow the instructions on the [Conan's installation guide](https://docs.conan.io/en/latest/installation.html).

3. **C++ Compiler**: This project requires a C++ compiler that supports C++23. We recommend using GCC 11.4.0 or higher. You can check your current version with `gcc --version`. If you need to upgrade, visit the [GCC download page](https://gcc.gnu.org/install/).


    You may install cmake and conan with pip with the following command:
    ```bash
    pip install cmake conan
    ```

4. (optional) **Doxygen**: This project uses Doxygen for documentation, if you would like to consult it via a web browser [install Doxygen](https://www.doxygen.nl/manual/install.html#install_bin_unix) in your machine. 

    You may generate this project Doxygen documentation has a `html` page by running the following command in the root of the project:
    ```bash
    doxygen Doxyfile
    ```

    To visualize the generated documentation, open the generated `index.html` file on a browser or do it via the terminal with:
    ```bash
    firefox docs/html/index.html 
    ```

    You may use your preferred browser for this instead of `firefox`.

## Development Setup

After ensuring that the requirements are met, follow these steps:

1. Run `conan install conanfile.txt` to install the necessary dependencies.

2. Run `cmake --build ./build/Release --target all` to generate the build files.

3. Run `./build/Release/server/server` to run the server app.

4. Run `./build/Release/client/client` to run the client app.

## Tests

To test the application you can use the provided docker-compose:

1. Run `docker-compose up` to start the containers (1 server and 3 clients).
2. Attach a shel to the desired client with `docker exec -it {container-id} bash `
3. Run the client with `../scripts/run-client.sh {username} server` on the attached container.
4. The tests/files folder is binded to the container, so you can add files there to test the commands.

## Recommended Tools

### Gitmoji

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

### VS Code

If using vs code, run the following command to install the recommended extensions:

```bash
code --install-extension ms-vscode.cpptools-extension-pack ms-vscode.cmake-tools seatonjiang.gitmoji-vscode
```

Or simply press `Ctrl+Shift+P` and type `Show Recommended Extensions`
