#include "common.h"
#include "low_power_dma_uart_demo.h"

/*
void dma_init(void)
{ 
      // Config DMA Mux for UART0 Asynchronous operation
      // Disable DMA channel first
      DMAMUX0_CHCFG0 = 0x00;
           
      // Clear pending errors or the done bit 
      if (((DMA_DSR_BCR0 & DMA_DSR_BCR_DONE_MASK) == DMA_DSR_BCR_DONE_MASK)
           | ((DMA_DSR_BCR0 & DMA_DSR_BCR_BES_MASK) == DMA_DSR_BCR_BES_MASK)
           | ((DMA_DSR_BCR0 & DMA_DSR_BCR_BED_MASK) == DMA_DSR_BCR_BED_MASK)
           | ((DMA_DSR_BCR0 & DMA_DSR_BCR_CE_MASK) == DMA_DSR_BCR_CE_MASK))
        DMA_DSR_BCR0 |= DMA_DSR_BCR_DONE_MASK;
      
      // Set Source Address
      DMA_SAR0 = SOURCE_ADDRESS;
      // Set Source/Destination data transfer size(8bit)
      DMA_DCR0 &= ~(DMA_DCR_SSIZE_MASK | DMA_DCR_CS_MASK);//0x0202;
      // Source Increment (increment by one)
      DMA_DCR0 |= (DMA_DCR_SINC_MASK 
                   | DMA_DCR_SMOD(2) 
                   | DMA_DCR_SSIZE(1)
                   | DMA_DCR_ERQ_MASK
                   | DMA_DCR_EADREQ_MASK);
      //DMA_REQC0 = DMA_REQC_ARR_DMAC(15);  //Use req_src-15 for DMA0, req on pin PTE
      DMA_DSR_BCR0 = DMA_DSR_BCR_BCR(1);
      
      // Set Destination Address
      DMA_DAR0 = DESTINATION_ADDRESS;
      
      // Set Destination transfer parameters
      DMA_DCR0 &= ~(DMA_DCR_DSIZE_MASK | DMA_DCR_DINC_MASK);
      DMA_DCR0 |= (DMA_DCR_DSIZE(1));
      
      // Enable UART0 operation in the DMA MUX
      DMAMUX0_CHCFG0 = 0x2; 
      // Enable the DMA MUX
      DMAMUX0_CHCFG0 |= DMAMUX_CHCFG_ENBL_MASK;
}
*/