cmake_minimum_required(VERSION 3.13)

set(PICO_BOARD pico_w)

set(gatt_counter)
set(gatt_counter.gatt)

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(ble_beacon C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(${CMAKE_PROJECT_NAME}
    ble_beacon.c
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_LIST_DIR} # For btstack config
    )

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(${CMAKE_PROJECT_NAME} 
	#SrcLib                # this library
	#McuLib                # we need the McuLib
	pico_stdlib           # pico standard library
	hardware_adc          # internal temperature and ADC
	pico_btstack_ble      # BLE stack
	pico_btstack_cyw43    # BLE stack with CYW43
	pico_cyw43_arch_threadsafe_background
	#pico_cyw43_arch_lwip_poll
	pico_cyw43_arch_lwip_threadsafe_background
	pico_cyw43_arch_none
	#pico_cyw43_arch_lwip_sys_freertos  # full lwIP stack including blocking sockets, with NO_SYS=0
	
	
	)
pico_btstack_make_gatt_header(${CMAKE_PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_LIST_DIR}/gatt_counter.gatt")
	
# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(${CMAKE_PROJECT_NAME})