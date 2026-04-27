#define USER_SETUP_INFO "User_Setup"

#define ST7789_DRIVER

// 禁用ESP32-C3上的DMA警告（ESP32-C3不支持DMA是硬件限制）
#define DISABLE_ALL_LIBRARY_WARNINGS

#define TFT_RGB_ORDER TFT_BGR  // Colour order Blue-Green-Red

#define TFT_WIDTH  240
#define TFT_HEIGHT 320

#define TFT_INVERSION_OFF

#define TFT_MOSI	3  // SPI MOSI pin
#define TFT_SCLK	2  // SPI clock pin
#define TFT_CS   	7  // Chip select control pin
#define TFT_DC		4  // Data Command control pin
#define TFT_RST		5  // Reset pin (could connect to RST pin)

#define SPI_FREQUENCY  27000000  // SPI clock frequency
#define SPI_READ_FREQUENCY  20000000  // SPI read clock frequency
#define SPI_TOUCH_FREQUENCY  2500000  // SPI touch clock frequency
