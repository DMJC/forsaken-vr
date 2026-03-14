# Forsaken

This repo holds the community port of Forsaken!

Check out the [Wiki](https://github.com/ForsakenX/forsaken/wiki) for more information.

## Virtual Reality (OpenVR)

Forsaken now includes optional OpenVR support on SDL2 builds.

### Build with OpenVR

```bash
make SDL=2 OPENVR=1
```

This requires `libopenvr` development headers and pkg-config metadata to be installed.

### Run in VR mode

```bash
./projectx -VR
```

Use `-NoVR` to explicitly disable VR initialization.
