# Board design and production files

| Material | Location |
| --- | --- |
| KiCad schematic/PCB sources | `KiCAD/` |
| Gerbers | [gerber.zip](KiCAD/out/gerber.zip) |
| Assembly export | [assembly.zip](KiCAD/out/assembly.zip) |
| Bill of materials | [components FreeEEG32 beta.csv](bom/components%20FreeEEG32%20beta.csv) |

PCB fabrication notes from the original project:

- 70 mm × 70 mm, four layers, 1.6 mm thickness (can vary).
- HASL lead-free or ENIG finish; FR4 material.

Assembly: 33 unique parts (27 without pin headers), 169 SMT pads, 23 through-hole
positions (pin headers), and components on both sides.

For current firmware installation use [Windows](../Flash/Windows/README.md) or
[Linux](../Flash/Linux/README.md). [SWD wiring and photos](../README.md#swd-programming).
