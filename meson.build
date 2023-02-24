project('stm32_template', 'c')

# dependencies
dependencies = []

local_headers = ['include']

# List of C sources to compile. Relative to project root.
c_sources = [
# hal drivers
    'src/stm32f3hal/flash.c',
    'src/stm32f3hal/rcc.c',
    'src/stm32f3hal/gpio.c',
    'src/stm32f3hal/usart.c',
    'src/stm32f3hal/i2c.c',
    'src/stm32f3hal/dma.c',
# board drivers
    'src/drivers/leds.c',
    'src/drivers/uart.c',
# board devices
    'src/devices/bmx055.c',
    'src/devices/ahrs_ioconfig.c',
# utilities
    'src/utils/queue.c',
    'src/utils/bin2hex.c',
    'src/utils/aproto.c',
    'src/utils/mpack.c',
# algorithms
    'src/algos/quaternion.c',
    'src/algos/madgwick.c',
# core m4 drivers
    'src/cm4/systick.c',
# programs
    'src/main.c',
]

# List of ASM sources to compile.  Relative to project root.
asm_sources = [
  #    'src/draw_isr.S'
]

# mpack configuration, probably move this into options
add_project_arguments('-DMPACK_STDLIB=0', language: 'c')
add_project_arguments('-DMPACK_STDIO=0', language: 'c')
add_project_arguments('-DMPACK_READER=0', language: 'c')
add_project_arguments('-DMPACK_EXPECT=0', language: 'c')
add_project_arguments('-DMPACK_NODE=0', language: 'c')
add_project_arguments('-DMPACK_BUILDER=0', language: 'c')

# add this back in if you like funrolling
# add_project_arguments('-funroll-loops', language: 'c')

### DON'T EDIT BELOW THIS LINE ###
add_project_arguments('-D' + get_option('mcu'), language: 'c')

objcopy = find_program('objcopy')


# These are additional sources that are needed for normal system init, provided
# by STMicro in the BSP.
# XXX: CHANGE THIS TO YOUR CHIP'S FILES
# NOTE: REMEMBER TO ADD HAL SOURCES HERE IF YOU USE THEM
c_sources = c_sources + ['core/src/system_stm32f3xx.c']
asm_sources = asm_sources + ['core/src/startup_stm32f302xc.s']

# this is a hack - these must be copied in from the BSP.
_includes = local_headers + [
    'core/include/bsp/Drivers/STM32F3xx_HAL_Driver/Inc', 
    'core/include/bsp/Drivers/CMSIS/Core/Include',
    'core/include/bsp/Drivers/CMSIS/Include',
    'core/include/bsp/Drivers/CMSIS/Device/ST/STM32F3xx/Include'
]
_incl_dirs = include_directories(_includes)

asm_static_object = static_library(
    'asm_static_object',
    asm_sources,
    include_directories: _incl_dirs,
    c_args: get_option('c_args') + ['-x', 'assembler-with-cpp'],
    install: false,
)

main = executable(
    'main.elf',
    c_sources,
    include_directories: _incl_dirs,
    link_args: ['-Wl,-Map=output.map'],
    link_whole: asm_static_object,
    dependencies: dependencies,
    install: true,
    install_dir: false,
)

# copy everything - custom linker script takes care of what goes where
# and non-flashing sections.
main_bin = custom_target(
    'main.bin',
    command: [
        objcopy,
        '-O', 'binary',
        '@INPUT@', '@OUTPUT@'
    ],
    input: main,
    output: 'main.bin',
    build_by_default: true
)
