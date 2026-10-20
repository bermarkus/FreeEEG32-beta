Legacy notes for the archived AC6 firmware. For the maintained firmware and USB
UF2 updates, use [Flash](../../../Flash/README.md) and [Firmware/UF2](../../UF2/README.md).

Use these files in AC6 or with OpenOCD to update the hardware.

Recent Updates
- Windows compatibility
- 80Mhz downclock for power saving / heat


Notes:
Output is functionally only 256Hz for some reason and printing duplicates at 512Hz.

Search "dec" in Src/main.c to tweak decimation rate settings which will change it, we are working on finding the best fix.
