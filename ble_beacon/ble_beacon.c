#include "gatt_counter.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pico/cyw43_arch.h"
#include "pico/btstack_cyw43.h"
#include "btstack.h"
#include "ble/gatt-service/battery_service_server.h"
#include "hardware/gpio.h"
/*
BLE_Beacon Server for Raspberry Pi Pico         
*/

#define APP_AD_FLAGS 0x06

// BLUETOOTH TYPE: LEGACY PDU can only transmit 27 bytes of payload data.
const uint8_t adv_data[] = {
    // Flags general discoverable
    0x02, BLUETOOTH_DATA_TYPE_FLAGS, APP_AD_FLAGS,
    // Name
    0x0b, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'B', 'L', 'E', ' ', 'B', 'E', 'A', 'C', 'O', 'N',
    // TX Power Level
    0x02, BLUETOOTH_DATA_TYPE_TX_POWER_LEVEL, 0x34,
    // Manufacurer data
    0x04, BLUETOOTH_DATA_TYPE_MANUFACTURER_SPECIFIC_DATA, 0xEE, 0xEE, 0x01,
    };


const uint8_t adv_data_len = sizeof(adv_data);


static void blinkLED(int ms_time){
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(100);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    sleep_ms(ms_time);
}


int main()
{
    if (cyw43_arch_init()) {
            printf("Wi-Fi init failed");
            return -1;
        }
    int count = 0;
    blinkLED(100);
    l2cap_init();
    blinkLED(100);
    // setup SM: Display only
    sm_init();
    blinkLED(100);

    // setup ATT server
    att_server_init(profile_data, NULL, NULL);   

    // setup advertisements
    uint16_t adv_int_min = 0x0600;
    uint16_t adv_int_max = 0x0600;
    uint8_t adv_type = 0;
    bd_addr_t null_addr;
    memset(null_addr, 0, 6);
    gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0, null_addr, 0x07, 0x00);
    gap_advertisements_set_data(adv_data_len, (uint8_t*) adv_data);
    // Set the device mac address in code
    uint8_t btMac[6] = { 0x4F, 0x22, 0xAF, 0x03, 0xE7, 0x4E }; // MAC address of device 1 (CF:22:AF:03:E7:4E)
    // uint8_t btMac[6] = { 0x83, 0xA4, 0xC6, 0x3D, 0xB8, 0xDF }; // MAC address of device 2 (C4:A4:C6:3D:B8:DF)
    // uint8_t btMac[6] = { 0x6F, 0x1A, 0x91, 0x4E, 0xEF, 0x5D }; // MAC address of device 3 (EF:1A:91:4E:EF:5D)
    // uint8_t btMac[6] = { 0x8C, 0xFC, 0x3F, 0x50, 0xD6, 0x42 }; // MAC address of device 4 (8C-FC-3F-50-D6-42)
    // uint8_t btMac[6] = { 0x49, 0xB7, 0x98, 0xDB, 0x08, 0x7B }; // MAC address of device 5 (49-B7-98-DB-08-7B)
    gap_random_address_set(btMac);
    gap_advertisements_enable(1); 
    hci_power_control(HCI_POWER_ON);

    while (1) {
        hci_power_control(HCI_POWER_ON);
        sleep_ms(50);
        hci_power_control(HCI_STATE_OFF);
        sleep_ms(950);
        count++;
        // blinks on and off every 5 seconds!
        if (count >= 5) {
            count = 0;
            blinkLED(0);
        }
        
    }
	
    
    return 0;
}
