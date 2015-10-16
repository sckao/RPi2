#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <getopt.h>
#include <bcm2835.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "rpi2.h"

using namespace std ;


void help()
{
    printf("Usage: ./rpi2.exe \n");
    printf(" --read,            -r   Read SPI \n");
    printf(" --write,           -w   Write \n");
    printf(" --help,            -h   Showing this help menu\n");
}

int main(int argc, char* argv[])
{

    int opt;
    int option_index = 0;
    static struct option long_options[] = {
        //{"readflash",     required_argument, 0, 'r'},
        {"help",                no_argument, 0, 'h'},
        {"write",               no_argument, 0, 'w'},
        {"read",                no_argument, 0, 'r'}
    };                                                                

    if ( argc == 1 ) {
        help();
        return 0;
    }

    if (!bcm2835_init())
        return 1;

   rpi2* pi = new rpi2() ;
   pi->spi_init() ;
   int adc_channel = 0 ;
   string prompt ;

   //while ((opt = getopt_long(argc, argv, "wh:r:", long_options, &option_index)) != -1)  {
   while ((opt = getopt_long(argc, argv, "whr", long_options, &option_index)) != -1)  {
        switch (opt) {

            case 'w':
                prompt = readline(" Give your input : " ) ; 
                printf(" your input -> %s \n", prompt.c_str() ) ;
                adc_channel = int(atoi( prompt.c_str() ));
                printf(" your input -> %d \n", adc_channel ) ;
                break;
            case 'r':
                prompt = readline(" Read MCP3004 Channle : " ) ; 
                //adc_channel = int(atoi(optarg));
                //printf(" Read MCP3004 Channle (%d) : ", adc_channel ) ;
                adc_channel = int(atoi( prompt.c_str() ));
                pi->read_MCP3004( adc_channel ) ;
                break;
            case 'h':
		help() ;
		break ;
	    default:
		help() ;
		return 0;

         }
     }
}

rpi2::rpi2() {

   spi_cs = 0 ;
   spi_mode = 0 ;
   spi_clk_divide = 2048 ;
   spi_cs_polarity = LOW ;
   spi_bit_order = BCM2835_SPI_BIT_ORDER_MSBFIRST ;
   spi_inited = false ;

}

rpi2::~rpi2() {

   bcm2835_spi_end() ;

}

void rpi2::spi_setup( uint8_t cs, uint8_t mode, uint16_t clk_divide, uint8_t bit_order, uint8_t cs_polarity ) {

     // CS : 0, 1, 2
     spi_cs = cs ; 
     // 0: CPOL = 0, CPHA = 0 ,  1: CPOL = 0, CPHA = 1 ,  
     // 2: CPOL = 1, CPHA = 0 ,  3: CPOL = 1, CPHA = 1
     spi_mode = mode ;  
     // freq = 250M / clk_divide , clk_divide = 2 ~ 65536 
     spi_clk_divide = clk_divide ;
     //  BCM2835_SPI_BIT_ORDER_MSBFIRST (1) ,  BCM2835_SPI_BIT_ORDER_LSBFIRST (0)
     spi_bit_order = bit_order ;
     // LOW (0) or HIGHT (1) 
     spi_cs_polarity = cs_polarity ;
}

void rpi2::spi_init( ) {

    bcm2835_spi_begin();
    bcm2835_spi_setBitOrder( spi_bit_order );      // The default
    bcm2835_spi_setDataMode( spi_mode );                   // The default
    bcm2835_spi_setClockDivider( spi_clk_divide ); // The default
    bcm2835_spi_chipSelect( spi_cs );                      // The default
    bcm2835_spi_setChipSelectPolarity( spi_cs, spi_cs_polarity );      // the default
    
    spi_inited = true ;

}

// Mode = 0, clk_divide >= 64
void rpi2::read_MCP3004( int channel ) {

     if ( spi_inited != true ) {
        spi_setup( 0,0,256,  BCM2835_SPI_BIT_ORDER_MSBFIRST, LOW  ) ;
        spi_init() ;
     }

    // channel: 0: 
    int ch =  (8 + channel) << 4 ; 
    char ibuff[3] = { 0x1, ch , 0x0 } ;
    char obuff[3] = { 0,  0, 0 } ;
    bcm2835_spi_transfernb( ibuff, obuff, 3 );
    int readout = int( obuff[1] & 3) ;
    readout = readout << 8;
    readout += int( obuff[2] ) ; 
    double vin = readout*2.048/ 1024. ;
    //printf("channel(%d) [%d][%d][%d] = %d -> %.3f \n", channel, int(obuff[0]), int(obuff[1]), int(obuff[2]), readout, vin ) ;
    printf("   channel(%d)  = %.3f V \n", channel, vin ) ;
     

}	
