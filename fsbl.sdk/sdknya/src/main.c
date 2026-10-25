#include <stdio.h>
#include "xparameters.h"
#include "xgpio.h"
#include "sleep.h"
#include "xil_printf.h"

#define GPIO_DEVICE_ID XPAR_AXI_GPIO_0_DEVICE_ID

XGpio Gpio;

int main() {
    int Status;
    u32 btn_state = 0;
    u32 last_btn_state = 0xFF;
    u32 led_state = 0;
    u32 last_led_state = 0xFF;

    // Inisialisasi GPIO
    Status = XGpio_Initialize(&Gpio, GPIO_DEVICE_ID);
    if (Status != XST_SUCCESS) {
        xil_printf("GPIO Init Failed\r\n");
        return XST_FAILURE;
    }

    // Set Arah: Ch 1 (LED) = Output (0x0), Ch 2 (PB) = Input (0xF)
    XGpio_SetDataDirection(&Gpio, 1, 0x0);
    XGpio_SetDataDirection(&Gpio, 2, 0xF);

    // Matikan semua LED di awal
    XGpio_DiscreteWrite(&Gpio, 1, 0x0);
    xil_printf("\r\n--- EBAZ4205 SYSTEM BOOTED ---\r\n");
    xil_printf("UART1 EMIO, SD BOOT, AXI GPIO Ready.\r\n");

    while (1) {
        // Baca status button. Tombol EBAZ4205 biasanya Active Low (Pull-up).
        // Kita negasikan (~) dan mask 4-bit (0xF) agar logic 1 = Ditekan.
        btn_state = (~XGpio_DiscreteRead(&Gpio, 2)) & 0x0F;

        if (btn_state != last_btn_state) {
            // Evaluasi logika LED
            if (btn_state & 0x01) led_state |= 0x01;  // PB1 Ditekan -> LED1 ON
            if (btn_state & 0x02) led_state |= 0x02;  // PB2 Ditekan -> LED2 ON
            if (btn_state & 0x04) led_state |= 0x04;  // PB3 Ditekan -> LED3 ON
            if (btn_state & 0x08) led_state = 0x00;   // PB4 Ditekan -> Semua LED OFF

            // Tulis ke LED
            XGpio_DiscreteWrite(&Gpio, 1, led_state);

            // Kirim via UART hanya jika ada perubahan output atau input yang fix
            if (led_state != last_led_state || btn_state != last_btn_state) {
                xil_printf("\r\n[UPDATE]\r\n");
                xil_printf("INPUT PB (1-4) : %d %d %d %d\r\n",
                           (btn_state & 0x01) ? 1 : 0,
                           (btn_state & 0x02) ? 1 : 0,
                           (btn_state & 0x04) ? 1 : 0,
                           (btn_state & 0x08) ? 1 : 0);
                xil_printf("OUTPUT LED(1-3): %s %s %s\r\n",
                           (led_state & 0x01) ? "ON " : "OFF",
                           (led_state & 0x02) ? "ON " : "OFF",
                           (led_state & 0x04) ? "ON " : "OFF");

                last_led_state = led_state;
                last_btn_state = btn_state;
            }
        }
        usleep(50000); // Debouncing delay 50ms
    }

    return 0;
}
