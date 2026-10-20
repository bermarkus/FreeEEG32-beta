# Pinned firmware source

`STM32H743ZI_beta_80MHz_makefile.tar.gz` is the original upstream source archive
used by the maintained build. It is an input, not an alternative installation path.

SHA256: `8ea26644227925d6897f4353a4022a1b1bd587ad19ab3097487a73331da01573`.

[Beta/prepare.py](../Beta/prepare.py) checks this hash, extracts into ignored
`Firmware/build/source/`, applies checked transformations and overlays maintained
sources. Keep the archive intact; edit [Beta](../Beta/README.md) instead.
