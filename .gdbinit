# automatically connect to the device through the bmp
target extended-remote /dev/ttyBmpGdb

# enable power
monitor tpwr enable

# scan and attach
monitor swdp_scan
attach 1

# reload the code
file build/main.elf
load build/main.elf
compare-sections
