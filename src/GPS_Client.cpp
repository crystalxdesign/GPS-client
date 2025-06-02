#include "GPS_Client.h"

#define UART_ID uart0

// Define UART pins
const uint UART_TX_PIN = 0; // or your Pico's UART pins
const uint UART_RX_PIN = 1;
const int UART_BAUD = 9600; // or your GPS module's baud rate

using namespace std;
string HDOP_IDEAL = "Ideal";
string HDOP_EXCELLENT = "Excellent";
string HDOP_GOOD = "Good";
string HDOP_MODERATE = "Moderate";
string HDOP_FAIR = "Fair";
string HDOP_POOR = "Poor";

// Define TinyGPSPlus
TinyGPSPlus gps;

string hdopAssessment(){
    double hdop = gps.hdop.value();
    if(hdop < 1.0){
        return HDOP_IDEAL;
    } else if(hdop < 2.0){
        return HDOP_EXCELLENT;
    } else if(hdop < 5.0){
        return HDOP_GOOD;
    } else if(hdop < 10.0){
        return HDOP_MODERATE;
    } else if(hdop < 20.0){
        return HDOP_FAIR;
    } else {
        return HDOP_POOR;
    }
}

// RX interrupt handler
void on_uart_rx() {

    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        gps.encode(ch);
    }
    // Print GPS data
    if (gps.location.isValid()) {
        printf("Latitude: %f, Longitude: %f, Accuracy: %s\n", gps.location.lat(), gps.location.lng(), hdopAssessment());
    }
}

int main() {
    // Initialize UART
    stdio_usb_init();
    uart_init(UART_ID, UART_BAUD);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    //gpio_set_pulls(UART_RX_PIN, gpio_pull_up, gpio_pull_down);
    uart_set_format(UART_ID, 8, 1, UART_PARITY_NONE);
    uart_set_hw_flow(UART_ID, false, false);
    uart_set_fifo_enabled(UART_ID, false);
    int UART_IRQ = UART_ID == uart0 ? UART0_IRQ : UART1_IRQ;
    // And set up and enable the interrupt handlers
    irq_set_exclusive_handler(UART_IRQ, on_uart_rx);
    irq_set_enabled(UART_IRQ, true);

    // Now enable the UART to send interrupts - RX only
    uart_set_irq_enables(UART_ID, true, false);

    while (1){
        tight_loop_contents();
    }
    return 0;
}
