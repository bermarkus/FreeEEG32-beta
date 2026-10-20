# Historical firmware and tools

These files preserve previous workflows. For current builds, SWD programming and
UF2 updates use **[Flash/Windows](../../Flash/Windows/README.md)** or
**[Flash/Linux](../../Flash/Linux/README.md)**. None of the files here is needed by
the maintained build; its pinned input is in [Base](../Base/README.md).

| Material | Historical use |
| --- | --- |
| [AC6](AC6/README.md) | System Workbench/AC6 firmware projects and compatibility snapshots |
| `CMSIS-DAP_hex/` | Blue Pill firmware for using an STM32F103 as a debug probe |
| `gcc-arm-none-eabi-ubuntu/`, `OpenOCD/` and their `.tar.xz` archives | Old Docker toolchain/build infrastructure |
| `STM32CubeIDE-workspace_*.zip` | Earlier IDE workspace snapshots |
| `STM32H743ZI_alpha1.5_makefile.*` | Earlier alpha source snapshots |
| `STM32H743ZI_alpha1.5_makefile-root-copy.tar.xz` | Distinct alpha archive previously in the repository root |
| `usbd_cdc_if.c` | Historical loose USB CDC source; not used by the maintained build |

The duplicate root copies of the GCC and OpenOCD archives were removed only after
SHA256 comparison with the copies retained here. Both differing alpha `.tar.xz`
archives are preserved. No historical project source was deleted.

[Historical AC6/Blue Pill guide](../../SetupGuides/FreeEEG32_Flashing_via_AC6_w_STM32F103_BluePill.md).
