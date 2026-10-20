# Programming FreeEEG32 beta and making updates easier

## First flash and recovery: direct ST-Link over SWD

Use an ST-Link SWD probe to program the board's **STM32H743ZI** processor.

STLINK-V3MINIE is a practical supported probe. Use its STDC14-to-flying-lead or
appropriate adapter for this board, rather than assuming the board has a standard
Arm debug connector. Existing suitable ST-Link probes can also be used.

| Probe signal | FreeEEG32 signal | STM32 pin |
| --- | --- | --- |
| SWDIO | `MCU_105_PA13_SYS_JTMS-SWDIO` | PA13, package pin 105 |
| SWCLK | `MCU_109_PA14_SYS_JTCK-SWCLK` | PA14, package pin 109 |
| NRST | `MCU_25_NRST` | NRST, package pin 25 |
| GND | Target ground | Board ground |
| VTref / T_VCC | Measured target I/O supply | Normally 3.3 V |

The SWD connections are **PA13/SWDIO**, **PA14/SWCLK** and **NRST**, with a
separate connection to the **GND pad above BOOT**. The **5 V** power pad is just
above that GND pad. See the [SWD reference image](images/basic_layout_swd.png).

The checked-in schematic exposes SWD/reset on **J82**, with power/boot signals on
**J83**, and also has other headers. The J82 fourth signal is labeled PB2/BOOT1,
**not ground**. Use the signal names and verify continuity for the actual PCB
revision. Do not plug a standard SWD cable onto J82 by position alone.

Power the FreeEEG32 using its normal supply arrangement. VTref is a voltage sense
connection, not a guarantee that the probe powers the board. Keep the leads short,
use a shared ground, and connect NRST so under-reset recovery works. SWD connects
to the target side of the board's isolation boundary; program/test with electrodes
disconnected and restore the intended isolation arrangement for use.

1. Connect the probe directly to the target signals above. Keep normal flash boot
   selected; SWD does not require changing BOOT0.
2. Run the automated **Build-and-Flash** launcher in
   [Flash/Windows](../Flash/Windows/README.md) or
   [Flash/Linux](../Flash/Linux/README.md). It installs missing tools, builds the
   UF2 factory image, programs it through ST-Link, verifies and resets.
3. For an existing compiled factory HEX, use **Flash-Precompiled** instead; it
   installs only the tools needed to flash. Install the ST-Link USB driver if
   Windows does not recognize the probe.
4. Connect the board's USB data port and look for the FreeEEG32 serial device.

For a bare board, the factory image installs the loader and application together.
For subsequent USB updates use double-reset and the application UF2 below.
For recovery from broken firmware, use
under-reset connection with NRST attached; reduce the SWD speed if needed. Inspect
power, target voltage and continuity before changing option bytes or erasing a chip.
The helper does not change option bytes or request a whole-chip mass erase.

## Why factory USB DFU is not a drop-in update path

For the pictured 10-pin dongle, use the
[numbered pinout and probe photo](../README.md#pictured-10-pin-st-link-v2-dongle).
Its pin 4 is SWDIO, pin 2 SWCLK, and pins 5/6 GND. The `RST` label may mean
SWIM-only reset; do not assume STM32 NRST support. `-Connect normal` (Windows) or
`--connect normal` (Linux/Python) selects software-reset attachment for a normally
booting board without probe NRST. Under-reset recovery needs a verified NRST
signal. Keep the pictured probe's 3.3 V / 5 V output pins disconnected when the
board is powered normally; a genuine probe's VTref input is a different function.

The current application and schematic use **USB OTG HS's embedded full-speed PHY
on PB14 (D−) / PB15 (D+)**. For STM32H74xxx/75xxx, ST's system-memory USB DFU
bootloader uses **PA11 (D−) / PA12 (D+)**. On this board PA11/PA12 are labeled as
USART1 CTS/RTS nets. Selecting bootloader mode does not move the USB data signals.

Consequently, neither BOOT0 nor a software jump to ROM makes the current USB port
a firmware updater. The serial `REBOOT` command deliberately performs a normal
reset; it does not claim to enter usable USB DFU.

Factory DFU is an option for a board revision, or a carefully checked rework with a
separate connection to PA11/PA12, the required power/clock/VBUS arrangement, access
to BOOT0/reset, and appropriate boot option settings. Disconnect the existing
CTS/RTS consumers as required. Validate against the MCU revision's bootloader
requirements before using it as a manufacturing path. It uses ST's ROM and
STM32CubeProgrammer; it does not present a drag-and-drop disk.

## Custom UF2 through the existing USB connector

The custom loader is now implemented in `Firmware/UF2`. It uses PB14/PB15,
reserves bank-1 sector 0, relocates the application to `0x08020000`, and stages
whole verified images in bank 2 before installation. It handles reordered and
duplicate UF2 blocks, board/layout checks, CRC, commit records and interrupted
installation retries. The drive is named FREEEEG32 and includes INFO_UF2.TXT and
STATUS.TXT.

Install `freeeeg32-factory.hex` once using SWD. Later use `!BOOTLOADER` or two
resets within the 600 ms loader window, then copy `freeeeg32-application.uf2` onto
the USB data drive. The BOOT button selects ROM boot, not this UF2 loader.

[Windows/Linux automatic installers, build/flash commands, image selection and
recovery instructions](../Firmware/UF2/README.md).

Builds and host simulations pass, including simulated torn writes and ECC errors.
Physical USB enumeration, actual power interruption and ADC behavior still need
board testing. The software never erases/writes loader sector 0; no option-byte
write protection is changed automatically.

## Further improvements worth doing

1. **Manufacturing fixture:** pogo pads for SWDIO/SWCLK/NRST/GND/VTref, consistent
   orientation, automatic board identification and a flash/verify/ADC/USB self-test.
   Use the same fixture for failed-update recovery. Add reset/boot buttons and clear
   connector labels on the next PCB revision.
2. **Release packaging:** publish tested HEX/BIN plus SHA256, a manifest with board
   revision and sample defaults, and the Windows shortcut. The build now packages UF2 and
   the corresponding factory loader bundle together; publish only after board tests.
3. **Settings persistence is implemented:** `!SAVE` / `!LOAD` / `!FACTORY`, versioned
   CRC records in bank 2 sector 6, and no writes until explicitly requested. A full
   journal requires `!FACTORY` to recycle it. `!HELP SETTINGS` explains the commands.
4. **Host support:** have clients query INFO/STATUS, set their processing rate, and
   report drops. A second CDC interface could separate configuration text from
   continuous EEG, but it needs composite USB descriptors and compatibility testing.
5. **ADC validation:** measured capture-rate reporting is implemented in `!DIAG`;
   remaining work includes converter error/status
   readout, channel masks, test signals, and synchronized changes across all four
   ADCs. Verify the new eight-word transfer fix and FIFO handling on a logic analyzer
   before adding rates beyond the current 512/1024/2048 choices.

## Primary references

- [ST AN2606: system-memory bootloader](https://www.st.com/resource/en/application_note/an2606-stm32-microcontroller-system-memory-boot-mode-stmicroelectronics.pdf)
  — STM32H74xxx/75xxx interface pins and boot requirements.
- [STLINK-V3MINIE manual](https://www.st.com/resource/en/user_manual/um2910-stlinkv3minie-evaluation-board-stmicroelectronics.pdf)
  — SWD, reset, voltage reference and connector details.
- [Microsoft USB serial driver](https://learn.microsoft.com/en-us/windows-hardware/drivers/usbcon/usb-driver-installation-based-on-compatible-ids)
  — CDC class/subclass matching for the built-in Windows driver.
- [Analog Devices AD7779 datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/AD7779.pdf)
  — HR modulator clock, SRC limits, PGA gain and settling behavior.
