# libtokenizer

Tokenizer library in c++.

> [!IMPORTANT]
> Since this library is still in development, it may not work correctly or some
> functionalities might be missing

## Build

### Dependencies

- [libparser](https://github.com/giorgi.page/libparser)

### Requirements

- [CMake](https://cmake.org/)

### Compilation

```bash
git clone https://github.com/giorgi.page/libtokenizer.git
cd libparser

cmake -B .build \
    && cmake --build .build --parallel $(($(nproc) - 1))
```

#### Testing

```bash
cmake -B .build -DTOKENIZER_BUILD_TESTING=ON \
    && cmake --build .build --parallel $(($(nproc) - 1)) \
    && ctest --test-dir .build
```
