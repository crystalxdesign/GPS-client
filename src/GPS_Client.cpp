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
    string ret;
    if(hdop < 1.0){
        ret = HDOP_IDEAL;
    } else if(hdop < 2.0){
        ret = HDOP_EXCELLENT;
    } else if(hdop < 5.0){
        ret = HDOP_GOOD;
    } else if(hdop < 10.0){
        ret =  HDOP_MODERATE;
    } else if(hdop < 20.0){
        ret = HDOP_FAIR;
    } else {
        ret = HDOP_POOR;
    }
    return ret + " (" + std::to_string((int)hdop) + ")";
}

// RX interrupt handler
void on_uart_rx() {

    while (uart_is_readable(UART_ID)) {
        uint8_t ch = uart_getc(UART_ID);
        gps.encode(ch);
    }
    // Print GPS data
    if (gps.location.isValid()) {
        std::cout << "Latitude: " << gps.location.lat() << ", Longitude: " << gps.location.lng() << ", Accuracy: " << hdopAssessment() << "\n";
        clearDisplay();
        setCursor(0, 0);
        printString((char*)"Latitude:");
        setCursor(10, 9);
        printString(std::to_string(gps.location.lat()).data());
        setCursor(0, 18);
        printString((char*)"Longitude:");
        setCursor(10, 27);
        printString(std::to_string(gps.location.lng()).data());
        setCursor(0, 37);
        printString(hdopAssessment().data());
        display();
    }
}

int main() {
    // Initialize UART
    stdio_usb_init();
    uart_init(UART_ID, UART_BAUD);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    // call the LCD initialization
    Nokia5110_Init();
    clearDisplay();
    setContrast(0x1f);
    setRotation(2);
    introGraphic();
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

    while (1)
        tight_loop_contents();
    return 0;
}