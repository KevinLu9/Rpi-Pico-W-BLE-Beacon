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
    //0x02, BLUETOOTH_DATA_TYPE_FLAGS, APP_AD_FLAGS,
    0x1b, BLUETOOTH_DATA_TYPE_MANUFACTURER_SPECIFIC_DATA, 0x18, 0x01, 0xBE, 0xAC, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x11, 0x22, 0x33, 0x44, 0xb3, 0x00,
    // Name
    0x02, BLUETOOTH_DATA_TYPE_COMPLETE_LOCAL_NAME, 'B', 
    // Manufacurer data
    //0x0A, BLUETOOTH_DATA_TYPE_MANUFACTURER_SPECIFIC_DATA, 0x18, 0x01, 'd', 'e', 'v', 'i', 'c', 'e', '1'
    // Altbeacon Manufacturer data example
    //0x1b, BLUETOOTH_DATA_TYPE_MANUFACTURER_SPECIFIC_DATA, 0x01, 0x18, 0xbe, 0xac, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x11, 0x22, 0x33, 0x44, 0xB3, 0x00,
    
    };
const uint8_t adv_data_len = sizeof(adv_data);

static void blinkLED(int ms_time){
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
    sleep_ms(ms_time);
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
    sleep_ms(ms_time);

}

/* LISTING_END */

int main();
int main()
{
    if (cyw43_arch_init()) {
            printf("Wi-Fi init failed");
            return -1;
        }

    blinkLED(100);
    l2cap_init();
    blinkLED(100);
    // setup SM: Display only
    sm_init();
    blinkLED(100);

    // setup ATT server
    att_server_init(profile_data, NULL, NULL);   

    // setup advertisements
    uint16_t adv_int_min = 0x0030;
    uint16_t adv_int_max = 0x0030;
    uint8_t adv_type = 0;
    bd_addr_t null_addr;
    memset(null_addr, 0, 6);
    gap_advertisements_set_params(adv_int_min, adv_int_max, adv_type, 0, null_addr, 0x07, 0x00);
    gap_advertisements_set_data(adv_data_len, (uint8_t*) adv_data);
    gap_advertisements_enable(1); 

    while (1) {
        // cycle on and off every second!
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 1);
        sleep_ms(500);
        cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);
        hci_power_control(HCI_POWER_ON);
        sleep_ms(40);
        hci_power_control(HCI_POWER_OFF);
        sleep_ms(460);
    }
	
    
    return 0;
}
