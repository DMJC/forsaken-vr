# Forsaken

This repo holds the community port of Forsaken!

Check out the [Wiki](https://github.com/ForsakenX/forsaken/wiki) for more information.

## Building (CMake)

Forsaken now uses CMake as the build system.

### Configure + build

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
```

### Useful CMake options

- `-DOPENVR=ON` to enable OpenVR support (`libopenvr` + pkg-config metadata required)
- `-DSDL_VERSION=1|2` to select SDL 1.x (`sdl`) or SDL2 (`sdl2`) pkg-config target
- `-DGL_VERSION=1|2|3` to select the `GL` compile-time backend macro
- `-DPROFILE=ON|OFF` to enable/disable `-pg`
- `-DLUA_BOT=ON|OFF`
- `-DRENDER_DISABLED=ON|OFF`
- `-DINPUT_DISABLED=ON|OFF`
- `-DDXMOUSE=ON|OFF`

Example with OpenVR:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DOPENVR=ON
cmake --build build -j
```

## Virtual Reality (OpenVR)

Run with:

```bash
./build/projectx -VR
```

Use `-NoVR` to explicitly disable VR initialization.
