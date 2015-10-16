#ifndef RPI2_H
#define RPI2_H

#include <string>

class rpi2 {

   public:
   rpi2() ;
   ~rpi2() ;

   void spi_setup( uint8_t cs = 0, uint8_t mode = 0 , uint16_t clk_divide = 256, uint8_t bit_order = 1, uint8_t cs_polarity = LOW ) ;
   void spi_init() ;
   void read_MCP3004( int channel ) ;
 
   private: 

   bool spi_inited ;
   uint8_t spi_cs ;
   uint8_t spi_mode ;
   uint16_t spi_clk_divide ;
   uint8_t spi_cs_polarity ;
   uint8_t spi_bit_order ;

} ;

#endif
