#!/usr/bin/env bash
sudo STM32_Programmer_CLI -c port=usb1 -w ./build/main.bin 0x08000000
#WHERE=$(dirname $0)
#source $WHERE/openocd_server.sh
#start_oocd
#(echo "init;reset halt; stm32l4x mass_erase 0;flash write_image build/main.bin 0x8000000 bin;reset run"; sleep 1; echo "exit")| telnet localhost 4444

