# Maintained firmware

Run `FreeEEG32.cmd` or `bash FreeEEG32.sh` in the main folder.
See [Windows options](../Flash/Windows/README.md) or
[Linux options](../Flash/Linux/README.md) for direct commands. The automated build produces both the
one-time SWD factory image and subsequent USB UF2 updates.

| Folder | Purpose |
| --- | --- |
| [Beta](Beta/README.md) | Editable EEG application, commands, persistent settings and host helpers |
| [UF2](UF2/README.md) | Resident USB loader, image packager, update recovery and flash layout |
| `tools/` | Shared platform launchers, automatic tool setup and OpenOCD configuration |
| [Base](Base/README.md) | Pinned original source archive verified by SHA256 before build |
| `build/` | Ignored generated sources, binaries, manifests, test executables and release ZIPs |
| [Legacy](Legacy/README.md) | Historical AC6/CubeIDE projects, Blue Pill probe files and Docker tooling |

Edit `Beta/Inc` and `Beta/Src` for application changes, or `UF2/Inc` and `UF2/Src`
for loader changes. `Beta/prepare.py` applies checked changes to the original
archive and overlays maintained sources. Generated files are rebuilt each time.
The standalone Beta build remains available for SWD-only installations; the
normal launchers use the UF2 build to preserve the USB update path.
