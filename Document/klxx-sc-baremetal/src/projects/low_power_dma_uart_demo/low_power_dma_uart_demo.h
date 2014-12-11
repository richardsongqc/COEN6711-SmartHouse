


// DMA Definitions
#define  DESTINATION_ADDRESS 0x20000000
#define  SOURCE_ADDRESS 0x4006A007
#define  BUFF_SIZE 32

// Port Definitions
#define INT_MODE_LOGIC_ZERO   0x08
#define INT_MODE_LOGIC_ONE    0x0C
#define INT_MODE_FALLING      0x0A
#define INT_MODE_RISING       0x09
#define INT_MODE_EITHER       0x0B

// UART Definitions
// Clock Source
#define UART0_IRCLK_SRC       0x03
#define UART0_OSCER_SRC       0x02
#define UART0_PLLFLL_SRC      0x01
#define UART0_CLK_DIS         0x00
// DMA
#define UART0_DMA_EN          0x01
#define UART0_DMA_DIS         0x00
#define UART0_WAKE_EN         0x01
#define UART0_WAKE_DIS        0x00

// Global Variables
extern uint32 uart0_clk_hz;
extern int periph_clk_khz;

