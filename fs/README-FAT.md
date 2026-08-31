# c-OS FAT Filesystem

c-OS now includes an initial FAT filesystem module alongside its in-memory root filesystem.

## Planned support

- FAT12
- FAT16
- FAT32
- FAT boot-sector parsing
- Directory reading
- File lookup
- File reading
- Block-device integration

The current implementation provides the FAT filesystem interface and validates/parses the common boot-sector fields. Full mounting requires a storage/block-device driver, which c-OS does not have yet.

The in-memory filesystem remains the current root filesystem at `/`. FAT is an additional filesystem intended for future disks, floppy images, USB-style media, or mounted volumes.
