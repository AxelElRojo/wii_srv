# WIP - wii_srv
Wii HTTP server written in C++ with DevkitPro.
# WIP Warning
This is a work in progress. It crashes regularly, if you can reproduce any bugs and/or find their causes, please open an issue.
# Usage
## Compiling
Make sure you've installed devkitPPC, and make, then type:
```console
make
```
This will generate a .dol file. Rename it to `boot.dol`.
## Adding to the Wii
1. Insert the Wii's SD card into your computer.
2. Create a subfolder named "wii_srv" under `apps/` in your SD card.
3. Copy/move the boot.dol and meta.xml files to the folder just created.
4. Unmount the SD and insert it to the Wii.
5. Open the Homebrew Channel.
### You need a modded Wii.
