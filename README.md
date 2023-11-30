#Needed Tools
- cmake: https://cmake.org/
- mingw32 or similar compiler: https://sourceforge.net/projects/mingw/

#Build code
- Clone repo recursively (for freertos and pico-sdk)
- Create build folder
- Vavigate to build folder
- Generate make files with: Cmake .. -G "MinGW Makefiles"
- Run make: mingw32-make.exe Workshop
- For example / exercise run: mingw32-make.exe Example / mingw32-make.exe Exercise

#Flash to RP2040
- Plug into PC (power on) while holding the small button
- RP2040 should be visible as storage device
- Copy .uf2 file of the compiled program (Workshop / Example / Exercise) to the RP2040