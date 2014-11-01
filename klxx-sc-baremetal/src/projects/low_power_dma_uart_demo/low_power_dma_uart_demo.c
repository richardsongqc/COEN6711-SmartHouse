/*
 * File:		low_power_dma_uart_demo.c
 * Purpose:		Main process
 *
 */

#include "common.h"
#include "low_power_dma_uart_demo.h"
#include "smc.h"
#include "uart.h"
#ifdef CMSIS
#include "start.h"
#endif

void PORTA_IRQHandler(void);
void DMA0_IRQHandler(void);
void dma_init(void);
void print_uart_regs(void);
void print_dma_regs(void);
void print_banner(void);

extern int mcg_clk_hz;
int dma_done = 0;
uint32 dma_dsr_bcr_reg = 0;
uint32 dma_dsr_bcr_reg2 = 0;
uint32 dma_dcr_reg = 0;

/********************************************************************/
int main (void)
{
    int i;
    uint32 reg_temp = 0;
     
		#ifdef CMSIS  // If we are conforming to CMSIS, we need to call start here
    start();
		#endif  	
    // Initialize variables
    dma_done = 0; 
    
    // Initialize Power Modes
    SMC_PMPROT = SMC_PMPROT_AVLP_MASK;
    
    // Print Initialization
    print_banner();
    printf("Re-configuring main clock and UART0 ... \n");
    
    ///////////////////////////////////////////////////////////////////////////
    // Switch the system clock to the IR Clock !!!
    //
    // NOTE:  Due to a rev 1.0 errata, the UART and DMA must be clocked from
    // sources that are approximately the same frequency.  Otherwise, the 
    // DMA will finish before it is time.  
    ///////////////////////////////////////////////////////////////////////////
#ifdef NO_PLL_INIT
    mcg_clk_hz = fei_fbi(4000000, 1);
#else
    mcg_clk_hz = pee_pbe(CLK0_FREQ_HZ);
    mcg_clk_hz = pbe_fbe(CLK0_FREQ_HZ);
    mcg_clk_hz = fbe_fbi(4000000, 1);
#endif
    
    
   
    // Enable the Fast IRCLK (by default this clock will operate at 2MHz)
    MCG_C1 |= MCG_C1_IRCLKEN_MASK;
    MCG_C2 |= MCG_C2_IRCS_MASK;
    
    // Configure the chip to source UART0 from the IRCLK
    reg_temp = SIM_SOPT2;
    reg_temp &= ~SIM_SOPT2_UART0SRC_MASK;
    reg_temp |= SIM_SOPT2_UART0SRC(UART0_IRCLK_SRC);
    SIM_SOPT2 = reg_temp;
    
    // Re-initialize UART0
    uart0_init(UART0_BASE_PTR, 2000, TERMINAL_BAUD);
    
    // Setup address Match wakeup functionality and receiver sleep functions
    // address mark wake-up
    UART0_C1 |= UART0_C1_WAKE_MASK;
    // receiver interrupt enable, in standby waiting for wakeup
    UART0_C2 |= UART0_C2_RWU_MASK;
    // Configure Address Match functionality of UART0
    // First setup Address match (MA) register for UART0
    UART0_MA1 = 0x81;
    // Enable Address match functionality
    UART0_C4 |= UART0_C4_MAEN1_MASK;
    
    // Setup UART0 DMA
    // Enable UART0 DMA functionality on receipt of a character
    UART0_C5 |= UART0_C5_RDMAE_MASK;
    
    // Print confirmation of UART0 reinitialization
    printf("UART reinitialized.\n");
    
     // Print MCG value to ensure no errors occurred.
    printf("MCG Val = %d \n", mcg_clk_hz);
    
    // Keep the UART0 clock source running in Stop mode
    MCG_C1 |= MCG_C1_IREFSTEN_MASK;
    
    // Move to VLPS mode
    // Setup the wakeup source
    PORTA_PCR4 = (PORT_PCR_MUX(1)
                  | PORT_PCR_PE_MASK
                  | PORT_PCR_IRQC(INT_MODE_FALLING));
   
    // Enable PortA IRQ
    //enable_irq(30);
    //NVIC_EnableIRQ(PORTA_IRQn);
#ifndef CMSIS
    // Enable DMA0 IRQ
    enable_irq(0);
#else
    NVIC_EnableIRQ(DMA0_IRQn);
#endif
    
    // Enable Global Interrupts
    EnableInterrupts;
    
    // Enable Clock gating for the DMA and DMA MUX
    SIM_SCGC6 |= SIM_SCGC6_DMAMUX_MASK;
    SIM_SCGC7 |= SIM_SCGC7_DMA_MASK;
  
    // Initialize DMA module
    dma_init();
    
    // Print DMA registers
    print_dma_regs();
    
    // Print Initial data
    printf("\nPrinting initial data: \n");
    for (i = 0; i < 32; i++)
        printf("%c", *((uint8 *)(DESTINATION_ADDRESS+i)));
    
    // Print instructions
    printf("\n\n");
    printf("Type 32 characters (remember to send the magic character \n");
    printf("first).  Then watch the magic happen!!\n");
    
    // Disable the SWD pins to provide lowest power measurement
    // NOTE : If you wish to debug beyond this point the next two lines of code must be
    // removed/commented out.
    PORTA_PCR0 = PORT_PCR_MUX(0);
    PORTA_PCR3 = PORT_PCR_MUX(0);

    
    // Wait for UART buffer to flush before entering VLPS
    for (i = 0; i < 500; i++)
    {}
    
    // Check to see that the clock monitor is enabled.  Disable if it is
    // because we should not be in a mode that uses the external clock for the
    // purposes of this program (nor would you want to use the clock monitor
    // circuit when moving to a low power mode anyways
    if ((MCG_C6 & MCG_C6_CME0_MASK) == MCG_C6_CME0_MASK)
        MCG_C6 &= ~MCG_C6_CME0_MASK;
    
    // Enter VLPS mode
    enter_vlps();
    
    // Wait for the DMA buffer to become full. 
    while (dma_done == 0)
    {
      if((DMA_DSR_BCR0 & DMA_DSR_BCR_DONE_MASK) == DMA_DSR_BCR_DONE_MASK) 
      {
          // Set DMA semaphore and write DMA Done bit when buffer is full.  
        DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;
        dma_done = 1;
      }
    }
    
    // Print DMA registers and acknowledgement that the buffer is full.  
    printf("\nDMA Buffer Full!!!\n");
    print_dma_regs();
    
    // Print results
    for (i = 0; i < 32; i++)
      printf("%c", *((uint8 *)(DESTINATION_ADDRESS+i)));
    
     // End main routine
    return 1;
}
/********************************************************************/

/********************************************************************/
/** DMA Initialization **********************************************/
void dma_init(void)
{ 
    int j;   
    // Clear Destionation memory  
    for( j=0; j < BUFF_SIZE; j=j+4)
	 *((uint32 *)(DESTINATION_ADDRESS+j)) = 0x2E;
      
      // Config DMA Mux for UART0 operation
      // Disable DMA Mux channel first
      DMAMUX0_CHCFG0 = 0x00;
           
      // Clear pending errors and/or the done bit 
      if (((DMA_DSR_BCR0 & DMA_DSR_BCR_DONE_MASK) == DMA_DSR_BCR_DONE_MASK)
           | ((DMA_DSR_BCR0 & DMA_DSR_BCR_BES_MASK) == DMA_DSR_BCR_BES_MASK)
           | ((DMA_DSR_BCR0 & DMA_DSR_BCR_BED_MASK) == DMA_DSR_BCR_BED_MASK)
           | ((DMA_DSR_BCR0 & DMA_DSR_BCR_CE_MASK) == DMA_DSR_BCR_CE_MASK))
        DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;
      
      // Set Source Address (this is the UART0_D register
      DMA_SAR0 = SOURCE_ADDRESS;
      
      // Set BCR to know how many bytes to transfer
      DMA_DSR_BCR0 = DMA_DSR_BCR_BCR(32);
      
      // Clear Source size and Destination size fields.  
      DMA_DCR0 &= ~(DMA_DCR_SSIZE_MASK 
                    | DMA_DCR_DSIZE_MASK
                    );
      
      // Set DMA as follows:
      //     Source size is byte size
      //     Destination size is byte size
      //     D_REQ cleared automatically by hardware
      //     Destination address will be incremented after each transfer
      //     Cycle Steal mode
      //     External Requests are enabled
      //     Asynchronous DMA requests are enabled.
      DMA_DCR0 |= (DMA_DCR_SSIZE(1)
                   | DMA_DCR_DSIZE(1)
                   | DMA_DCR_D_REQ_MASK
                   | DMA_DCR_DINC_MASK
                   | DMA_DCR_CS_MASK
                   | DMA_DCR_ERQ_MASK
#ifdef FREEDOM
                   | DMA_DCR_EINT_MASK
#endif
                   | DMA_DCR_EADREQ_MASK
                   );
      
      // Set destination address
      DMA_DAR0 = DESTINATION_ADDRESS;
      
      // Enables the DMA channel and select the DMA Channel Source  
      DMAMUX0_CHCFG0 = 0x02; //DMAMUX_CHCFG_ENBL_MASK|DMAMUX_CHCFG_SOURCE(0x31); //0xb1; 
      DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK;
}

/******************************************************************************/
/** Port A interrupt handler *************************************************/
void PORTA_IRQHandler(void)
{
  //printf("Port A ISR - 0x%X\n", PORTA_ISFR);
  PORTA_ISFR = 0xffffffff;
}


/******************************************************************************/
/** DMA interrupt handler ****************************************************/
void DMA0_IRQHandler(void)
{
  
  dma_dsr_bcr_reg = DMA_DSR_BCR0;
  dma_dcr_reg = DMA_DCR0;
  
  // Clear pending errors or the done bit 
  if (((DMA_DSR_BCR0 & DMA_DSR_BCR_DONE_MASK) == DMA_DSR_BCR_DONE_MASK)
       | ((DMA_DSR_BCR0 & DMA_DSR_BCR_BES_MASK) == DMA_DSR_BCR_BES_MASK)
       | ((DMA_DSR_BCR0 & DMA_DSR_BCR_BED_MASK) == DMA_DSR_BCR_BED_MASK)
       | ((DMA_DSR_BCR0 & DMA_DSR_BCR_CE_MASK) == DMA_DSR_BCR_CE_MASK))
  {
    DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;
    dma_done = 1;
  }
  
  dma_dsr_bcr_reg2 = DMA_DSR_BCR0;
  
}

/******************************************************************************/
/** Print UART Control Registers *********************************************/
void print_uart_regs(void)
{
  printf("\nUART BDH = 0x%X \n", UART0_BDH);
  printf("UART BDL = 0x%X \n", UART0_BDL);
  printf("UART C1 = 0x%X\n\n", UART0_C1);
  printf("UART C2 = 0x%X\n\n", UART0_C2);
  printf("UART S1 = 0x%X\n\n", UART0_S1);
  printf("UART S2 = 0x%X\n\n", UART0_S2);
  printf("UART C3 = 0x%X\n\n", UART0_C3);
  printf("UART MA1 = 0x%X\n\n", UART0_MA1);
  printf("UART MA2 = 0x%X\n\n", UART0_MA2);
  printf("UART C4 = 0x%X\n\n", UART0_C4);
  printf("UART C5 = 0x%X\n\n", UART0_C5);
    
}

/** Print DMA Control Registers *********************************************/
void print_dma_regs(void)
{
    printf("\n");
    printf("DMA Registers \n");
    printf("-------------------------------------------- \n");
    // Print DMA Registers
    printf("DMA SAR = 0x%X \n", (unsigned int)DMA_SAR0);
    printf("DMA DAR = 0x%X \n", (unsigned int)DMA_DAR0);
    printf("DMA DSR/BCR = 0x%X \n", (unsigned int)DMA_DSR_BCR0);
    printf("DMA DCR = 0x%X \n\n", (unsigned int)DMA_DCR0);
}

/*******************************************************************************/
/** Print Opening Banner ******************************************************/
void print_banner(void)
{
    printf("\n\nRunning the low_power_dma_uart_demo project.\n\n");
    printf("This demo project configures UART0 and the DMA to operate in a \n");
    printf("low power mode.  In this low power mode the UART will transfer \n");
    printf("characters, typed to the terminal by the user, to a location \n");
    printf("in memory while the core is gated off.  The user must send the \n");
    printf("magic wake-up packet (0x81) before the UART will perform the \n");
    printf("asynchronous wake-ups. \n\n");
    printf("The KL25 chip will remain in VLPS once the buffer is full (as well as between \n");
    printf("transfers) until the user triggers a falling edge interrupt on \n");
    printf("PTA4 (SW1 on the tower board). \n\n");
}

/***************************************************************************/
/** UART initialization function ******************************************/
/**
* Parameters:
*
* uint8 clksrc_num - This is the number that corresponds to the desired
*                  UART0 clock source.  See the KL25 reference manual,
*                  SIM_SOPT2[UART0SRC] bit description, for details
*
* uint32 uart0_freq - This is the frequency of the UART0 clock source
*               in Hz.
*
* uint8 en_wake - This parameter determines whether or not to enable
*               the UART wake functionality.  For more information on
*               this functionality consult the L2K reference manual. 
*
* uint8 en_dma - This parameter enables the DMA functionality for the 
*              UART RX function.  
*
*
*/
uint8 setup_uart0(uint8 clksrc_num, uint32 uart0_freq, uint8 en_wake, uint8 en_dma)
{
  uint8 i, success;
  uint32 calculated_baud = 0;
  uint32 baud_diff = 0;
  uint32 temp = 0;
  uint32 osr_val = 0;
  uint32 sbr_val;
  uint32 baud_rate;
  uint32 reg_temp = 0;
  
  // Initialize success variable
  success = 0;
  
    // Disable UART0 before changing registers
    UART0_C2 &= ~(UART0_C2_TE_MASK | UART0_C2_RE_MASK);
  
    // Verify that a valid clock value has been passed to the function 
    if ((uart0_freq > 100000000) || (uart0_freq < 32768))
    {
        uart0_freq = 0;
        clksrc_num = 0;
    }
    
    // Verify that a valid value has been passed to clksrc_num and update
    // uart0_clk_hz accordingly.  Write 0 to clksrc_num if an invalid 
    // value has been passed.  
    if (clksrc_num > 3)
        clksrc_num = 0;
    
    reg_temp = SIM_SOPT2;
    reg_temp &= ~SIM_SOPT2_UART0SRC_MASK;
    reg_temp |= SIM_SOPT2_UART0SRC(clksrc_num);
    SIM_SOPT2 = reg_temp;
    
    if (en_wake > 0)
    {
        // address mark wake-up
        UART0_C1 |= UART0_C1_WAKE_MASK;
        // receiver interrupt enable, in standby waiting for wakeup
        UART0_C2 |= UART0_C2_RWU_MASK;
        // Configure Address Match functionality of UART0
        // First setup Address match (MA) register for UART0
        UART0_MA1 = 0x81;
        // Enable Address match functionality
        UART0_C4 |= UART0_C4_MAEN1_MASK;
    }
    
    if (en_dma > 0)
        // Enable UART0 DMA functionality on receipt of a character
        UART0_C5 |= UART0_C5_RDMAE_MASK;
    
    // Initialize baud rate
    baud_rate = TERMINAL_BAUD;
    
    
    // Calculate the first baud rate using the lowest OSR value possible.  
    i = 4;
    sbr_val = (uint32)(uart0_freq/(baud_rate * i));
    calculated_baud = (uart0_freq / (i * sbr_val));
        
    if (calculated_baud > baud_rate)
        baud_diff = calculated_baud - baud_rate;
    else
        baud_diff = baud_rate - calculated_baud;
    
    osr_val = i;
        
    // Select the best OSR value
    for (i = 5; i <= 32; i++)
    {
        sbr_val = (uint32)(uart0_freq/(baud_rate * i));
        calculated_baud = (uart0_freq / (i * sbr_val));
        
        if (calculated_baud > baud_rate)
            temp = calculated_baud - baud_rate;
        else
            temp = baud_rate - calculated_baud;
        
        if (temp <= baud_diff)
        {
            baud_diff = temp;
            osr_val = i; 
        }
    }
    
    if (baud_diff < ((baud_rate / 100) * 3))
    {
        // If the OSR is between 4x and 8x then both
        // edge sampling MUST be turned on.  
        if ((osr_val >3) && (osr_val < 9))
            UART0_C5|= UART0_C5_BOTHEDGE_MASK;
        
        // Setup OSR value for IRCLK
        // OSR should be 8 (bit val = 7) to get 115200 baud rate.  
        reg_temp = UART0_C4;
        reg_temp &= ~UART0_C4_OSR_MASK;
        reg_temp |= UART0_C4_OSR(osr_val-1);
    
        // Write reg_temp to C4 register
        UART0_C4 = reg_temp;
        
        reg_temp = UART0_C4;
        reg_temp = (reg_temp & UART0_C4_OSR_MASK) + 1;
        sbr_val = (uint32)((uart0_freq)/(baud_rate * (reg_temp)));
        
         /* Save off the current value of the uartx_BDH except for the SBR field */
        reg_temp = UART0_BDH & ~(UART0_BDH_SBR(0x1F));
   
        UART0_BDH = reg_temp |  UART0_BDH_SBR(((sbr_val & 0x1F00) >> 8));
        UART0_BDL = (uint8)(sbr_val & UART0_BDL_SBR_MASK);
        
        /* Enable receiver and transmitter */
        UART0_C2 |= (UART0_C2_TE_MASK
                    | UART0_C2_RE_MASK );
        
        success = 1;
    }   
    return success;
} /// End setup_uart0 function 
