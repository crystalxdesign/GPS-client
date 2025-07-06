#include "GPS_Client.h"

#define UART_ID uart0

void GPIO_Button_Callback(uint gpio, uint32_t events);
void waiting4Satellite(void);
void introGraphic(void);
void mode0(void);
void mode1(void);
uint8_t modeSelection = 0;
bool gpsActive = false;
const uint8_t MENU_COUNT = 2;

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
    if (gps.location.isValid() && gps.location.isUpdated()) {
        gpsActive = true;
        std::cout << "Latitude: " << gps.location.lat() << ", Longitude: " << gps.location.lng() << ", Accuracy: " << hdopAssessment() << "\n";
        switch(modeSelection){
            case 0: mode0(); break;
            case 1: mode1(); break;
        }
    }
}

std::string timePad(uint8_t time){
    std::string retval;
    if (time < 10) {
        retval = "0" + std::to_string(time);
    } else {
        retval = std::to_string(time);
    }
    return retval;
}

void mode1(){
    uint8_t hours = gps.time.hour();
    uint8_t minutes = gps.time.minute();
    uint8_t seconds = gps.time.second();
    std::string time = "Time: " + timePad(gps.time.hour()) + ":" + timePad(gps.time.minute()) + ":" + timePad(gps.time.second());
    std::string speed = std::to_string(gps.speed.mph());
    std::string altitude = std::to_string(gps.altitude.feet());
    clearDisplay();
    setCursor(0, 0);
    printString((char *)time.c_str());
    setCursor(0, 10);
    printString((char *)"Speed (mph):");
    setCursor(10, 20);
    printString((char *)speed.c_str());
    setCursor(0, 30);
    printString((char *)"Alt (feet): ");
    setCursor(10, 40);
    printString((char *)altitude.c_str());
    display();
}

void mode0(){
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

void GPIO_Button_Callback(uint gpio, uint32_t events)
{
    if (gpio == BUTTON_PIN){
        modeSelection++;
        if(modeSelection>=MENU_COUNT){
            modeSelection = 0;
        }
    }
}

bool timer_callback(repeating_timer_t *mst) {
    if(gpsActive == false) waiting4Satellite();
    gpsActive = false;
    return true;
}

void introGraphic(){
	testdrawroundrect();
    sleep_ms(2000);
    clearDisplay();
    setCursor(10, 0);
    setTextSize(1);
    printString((char *)"Crystalx");
    setCursor(10, 10);
    setTextSize(1);
    printString((char *)"Design's");
    setCursor(10, 20);
    setTextSize(1);
    printString((char *)"GPS");
    setCursor(10, 30);
    setTextSize(1);
    printString((char *)"Client");
	setCursor(10, 40);
	printString((char *)FIRMWARE_VERSION);
    display();
	sleep_ms(3000);
}

void waiting4Satellite(){
    clearDisplay();
    drawBitmap(0, 0, epd_bitmap_sat1, 0x0054, 0x0030, BLACK);
    setCursor(10, 40);
    setTextSize(1);
    printString((char*)"Waiting ...");
    display();
}

static repeating_timer_t mst;

int main() {
    // Initialize UART
    stdio_usb_init();
    uart_init(UART_ID, UART_BAUD);
    gpio_set_function(UART_TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(UART_RX_PIN, GPIO_FUNC_UART);
    // Setup menu select button
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    // Enable the internal pullup resistor to Button
    //gpio_pull_up(BUTTON_PIN);
    // Enable the interrupt for Button pin
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_RISE, true, &GPIO_Button_Callback);
    // Add an interval time to test for satallite activity
    add_repeating_timer_ms(10000, timer_callback, NULL, &mst);
    // call the LCD initialization
    Nokia5110_Init();
    clearDisplay();
    setContrast(0x1f);
    setRotation(2);
    introGraphic();
    //draw satellite image
    waiting4Satellite();
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
