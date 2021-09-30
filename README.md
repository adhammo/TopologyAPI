# Topology API

Provide the functionality to _access, manage and store_ **device topologies**.

## How to Run Code

You need **CMake** in order to build and run the project.

```bash
# Create build directory
mkdir build

# Run CMake on project inside build
cd build
cmake ..

# Build all
cmake --build . --config Debug --target ALL_BUILD
cd Debug

# Run tests
topology_api_tests.exe

# Run interface
topology_api.exe
```

## Documentation

Code is thoroughly documented (doxygen-style comments) and a sample usage of API is included in **main.cpp**.

## Tests

### **Tests** Output:

I used _googletest_ for unit testing of API.

![Tests Output](/screenshots/tests.png)

## Interface

### Example of **Interface** Output

![Interface Output](/screenshots/interface.png)
