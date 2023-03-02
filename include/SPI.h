#ifndef		__SPI_H
#define		__SPI_H

#include	"config.h"








#define NRF_MISO									P03	//

#define NRF_IRQ										P01	//

#define	NRF_CEH										P06 = 1 //
#define	NRF_CEL										P06 = 0 //

#define	NRF_CSH										P04 = 1 //
#define	NRF_CSL										P04 = 0 //

#define NRF_CLKH									P02 = 1 //
#define NRF_CLKL									P02 = 0 //

#define NRF_MOSIH									P00 = 1 //
#define NRF_MOSIL									P00 = 0 //

#define NRF_LEDH									P07 = 1 //
#define NRF_LEDL									P07 = 0 //



unsigned char SPI_ReadWriteByte(unsigned char dat);
void InitSpi(void);


#endif
