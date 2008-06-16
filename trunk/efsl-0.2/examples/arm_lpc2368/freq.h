#ifndef freq_h_
#define freq_h_

#include <stdint.h>

#define I_RC_OSC_FREQ   (4000000UL)
#define MAIN_OSC_FREQ   (12000000UL)
#define RTC_OSC_FREQ    (32768UL)

/*************************************************************************
 * Function Name: SYS_GetFsclk
 * Parameters: none
 * Return: uint32_t
 *
 * Description: return Sclk [Hz]
 *
 *************************************************************************/
uint32_t SYS_GetFsclk(void);

/*************************************************************************
 * Function Name: SYS_GetFpclk
 * Parameters: Int32U Periphery
 * Return: uint32_t
 *
 * Description: return Pclk [Hz]
 *
 *************************************************************************/
uint32_t SYS_GetFpclk(uint32_t Periphery);


#endif
