/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2012  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.16 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : LCDConf.c
Purpose     : Display controller configuration

              Display driver: GUIDRV_CompactColor_16
              Operation mode: 16bpp, 16-bit interface or SPI
							
							- 66700: Sharp LR38825
              - 66701: Renesas R63401, Renesas R61509, OriseTech SPFD5420A
              - 66702: Solomon SSD1289, Solomon SSD2119, Solomon SSD1298
              - 66703: Toshiba JBT6K71
              - 66704: Sharp LCY-A06003
              - 66705: Samsung S6D0129, Renesas R61505
              - 66706: MagnaChip D54E4PA7551
              - 66707: Himax HX8312A
              - 66708: Ilitek ILI9320, Ilitek ILI9325, LG Electronics LGDP4531, OriseTech SPFD5408
              - 66709: Novatek NT39122, Renesas R61516, Sitronix ST7628, Sitronix ST7637, Epson S1D19122
              - 66710: Novatek NT7573
              - 66711: Epson S1D13743
              - 66712: Himax HX8347
              - 66713: Himax HX8340
              - 66766: Hitachi HD66766, Samsung S6D0110A, Ilitek ILI9161
              - 66772: Hitachi HD66772, Samsung S6D0117, Sitronix ST7712, Himax HX8301, Ilitek ILI9220
              - 66789: Hitachi HD66789

---------------------------END-OF-HEADER------------------------------
*/

#include "GUI.h"
#include "LCDConf_CompactColor_16.h"
#include "GUIDRV_CompactColor_16.h"
#include "LCDConf.h"

/*********************************************************************
*       Layer configuration
*********************************************************************/

/* Physical display size */
#define XSIZE_PHYS         240 //no change!!
#define YSIZE_PHYS         320 //no change!!


#define VXSIZE_PHYS         240
#define VYSIZE_PHYS         320

/* Color conversion */
#define COLOR_CONVERSION    GUICC_M565

/* Display driver */
#define DISPLAY_DRIVER GUIDRV_COMPACT_COLOR_16

//
// Buffers / VScreens
//
#define NUM_BUFFERS   1
#define NUM_VSCREENS  1

//
// Display orientation
//

#define DISPLAY_ORIENTATION  0
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X | GUI_MIRROR_Y)
//#define DISPLAY_ORIENTATION (GUI_SWAP_XY | GUI_MIRROR_X)
//#define DISPLAY_ORIENTATION (GUI_MIRROR_X)

//
// Touch screen
//
#define USE_TOUCH   1
//
// Touch screen calibration
#define TOUCH_X_MIN 0x00E0
#define TOUCH_X_MAX 0x0F40
#define TOUCH_Y_MIN 0x00C0
#define TOUCH_Y_MAX 0x0F60

/*********************************************************************
*       Configuration checking
*********************************************************************/

#ifndef   VXSIZE_PHYS
  #define VXSIZE_PHYS XSIZE_PHYS
#endif
#ifndef   VYSIZE_PHYS
  #define VYSIZE_PHYS YSIZE_PHYS
#endif
#ifndef   XSIZE_PHYS
  #error Physical X size of display is not defined!
#endif
#ifndef   YSIZE_PHYS
  #error Physical Y size of display is not defined!
#endif
#ifndef   COLOR_CONVERSION
  #error Color conversion not defined!
#endif
#ifndef   DISPLAY_DRIVER
  #error No display driver defined!
#endif


#ifndef   NUM_VSCREENS
  #define NUM_VSCREENS 1
#else
  #if (NUM_VSCREENS <= 0)
    #error At least one screeen needs to be defined!
  #endif
#endif
#if (NUM_VSCREENS > 1) && (NUM_BUFFERS > 1)
  #error Virtual screens and multiple buffers are not allowed!
#endif

#ifndef   LCD_SWAP_XY
  #define LCD_SWAP_XY 0
#endif
#if LCD_SWAP_XY
  #define LCD_XSIZE YSIZE_PHYS
  #define LCD_YSIZE XSIZE_PHYS
#else
  #define LCD_XSIZE XSIZE_PHYS
  #define LCD_YSIZE YSIZE_PHYS
#endif
#define LCD_VXSIZE LCD_XSIZE
#define LCD_VYSIZE LCD_YSIZE



#ifndef   DISPLAY_ORIENTATION
  #define DISPLAY_ORIENTATION  0
#endif

#if ((DISPLAY_ORIENTATION & GUI_SWAP_XY) != 0)
#define LANDSCAPE   1
#else
#define LANDSCAPE   0
#endif

#if (LANDSCAPE == 1)
#define WIDTH       YSIZE_PHYS  /* Screen Width (in pixels)         */
#define HEIGHT      XSIZE_PHYS  /* Screen Hight (in pixels)         */
#else
#define WIDTH       XSIZE_PHYS  /* Screen Width (in pixels)         */
#define HEIGHT      YSIZE_PHYS  /* Screen Hight (in pixels)         */
#endif


#if ((DISPLAY_ORIENTATION & GUI_SWAP_XY) != 0)
  #if ((DISPLAY_ORIENTATION & GUI_MIRROR_X) != 0)
    #define TOUCH_TOP    TOUCH_X_MAX
    #define TOUCH_BOTTOM TOUCH_X_MIN
  #else
    #define TOUCH_TOP    TOUCH_X_MIN
    #define TOUCH_BOTTOM TOUCH_X_MAX
  #endif
  #if ((DISPLAY_ORIENTATION & GUI_MIRROR_Y) != 0)
    #define TOUCH_LEFT   TOUCH_Y_MAX
    #define TOUCH_RIGHT  TOUCH_Y_MIN
  #else
    #define TOUCH_LEFT   TOUCH_Y_MIN
    #define TOUCH_RIGHT  TOUCH_Y_MAX
  #endif
#else
  #if ((DISPLAY_ORIENTATION & GUI_MIRROR_X) != 0)
    #define TOUCH_LEFT   TOUCH_X_MAX
    #define TOUCH_RIGHT  TOUCH_X_MIN
  #else
    #define TOUCH_LEFT   TOUCH_X_MIN
    #define TOUCH_RIGHT  TOUCH_X_MAX
  #endif
  #if ((DISPLAY_ORIENTATION & GUI_MIRROR_Y) != 0)
    #define TOUCH_TOP    TOUCH_Y_MAX
    #define TOUCH_BOTTOM TOUCH_Y_MIN
  #else
    #define TOUCH_TOP    TOUCH_Y_MIN
    #define TOUCH_BOTTOM TOUCH_Y_MAX
  #endif
#endif

/*********************************************************************
*
*           Access routines
*
*********************************************************************/

/*********************************************************************
*
*       Driver Port functions
*
**********************************************************************
*/
extern void LCD_X_Init(void);
extern void LCD_X_Write00_16(U16 c);
extern void LCD_X_Write01_16(U16 c);
extern U16  LCD_X_Read01_16 (void);
extern void LCD_X_WriteM01_16(U16 * pData, int NumWords);
extern void LCD_X_ReadM01_16 (U16 * pData, int NumWords);

static void delay (int cnt) {
  cnt <<= 15;
  while (cnt--);
}

static void wr_reg (U16 reg, U16 dat) {
  LCD_X_Write00_16(reg);
  LCD_X_Write01_16(dat);
}

static U16 rd_reg (U16 reg) {
  LCD_X_Write00_16(reg);
  return LCD_X_Read01_16();
}
/*********************************************************************
*
*       LCD_X_InitController
*
* Purpose:
*   Initializes the display controller
*/
static void _InitController(void) 
{
    /* Set up the display controller and put it into operation. If the 
    *  display controller is not initialized by any external routine 
    *  this needs to be adapted by the customer.
    */
  static unsigned short driverCode;	
	LCD_X_Init();
	
 delay(5);                             /* Delay 50 ms                        */
  driverCode = rd_reg(0x00);
  if(driverCode==0x8989)
  {
    wr_reg(0x0000,0x0001);    delay(5);  
    wr_reg(0x0003,0xA8A4);    delay(5);   
    wr_reg(0x000C,0x0000);    delay(5);   
    wr_reg(0x000D,0x080C);    delay(5);   
    wr_reg(0x000E,0x2B00);    delay(5);   
    wr_reg(0x001E,0x00B0);    delay(5);   
    wr_reg(0x0001,0x2B3F);    delay(5);  
    wr_reg(0x0002,0x0600);    delay(5);
    wr_reg(0x0010,0x0000);    delay(5);
    wr_reg(0x0011,0x6070);		delay(5);	/* Set GRAM write direction  */
    wr_reg(0x0005,0x0000);    delay(5);
    wr_reg(0x0006,0x0000);    delay(5);
    wr_reg(0x0016,0xEF1C);    delay(5);
    wr_reg(0x0017,0x0003);    delay(5);
    wr_reg(0x0007,0x0133);    delay(5);         
    wr_reg(0x000B,0x0000);    delay(5);
    wr_reg(0x000F,0x0000);    delay(5);   
    wr_reg(0x0041,0x0000);    delay(5);
    wr_reg(0x0042,0x0000);    delay(5);
    wr_reg(0x0048,0x0000);    delay(5);
    wr_reg(0x0049,0x013F);    delay(5);
    wr_reg(0x004A,0x0000);    delay(5);
    wr_reg(0x004B,0x0000);    delay(5);
    wr_reg(0x0044,0xEF00);    delay(5);
    wr_reg(0x0045,0x0000);    delay(5);
    wr_reg(0x0046,0x013F);    delay(5);
    wr_reg(0x0030,0x0707);    delay(5);
    wr_reg(0x0031,0x0204);    delay(5);
    wr_reg(0x0032,0x0204);    delay(5);
    wr_reg(0x0033,0x0502);    delay(5);
    wr_reg(0x0034,0x0507);    delay(5);
    wr_reg(0x0035,0x0204);    delay(5);
    wr_reg(0x0036,0x0204);    delay(5);
    wr_reg(0x0037,0x0502);    delay(5);
    wr_reg(0x003A,0x0302);    delay(5);
    wr_reg(0x003B,0x0302);    delay(5);
    wr_reg(0x0023,0x0000);    delay(5);
    wr_reg(0x0024,0x0000);    delay(5);
    wr_reg(0x0025,0x8000);    delay(5);
    wr_reg(0x004f,0);        
    wr_reg(0x004e,0);        
  }
  delay(5);  /* delay 50 ms */
	
	
}


/*********************************************************************
*       Public code
*********************************************************************/

/*********************************************************************
*
*       LCD_X_Config
*
* Purpose:
*   Called during the initialization process in order to set up the
*   display driver configuration.
*   
*/
void LCD_X_Config(void) 
{
//
  // Set display driver and color conversion
  //
  GUI_DEVICE_CreateAndLink(DISPLAY_DRIVER, COLOR_CONVERSION, 0, 0);
  //
  // Display driver configuration
  //
 if (LCD_GetSwapXY()) {
    LCD_SetSizeEx (0, YSIZE_PHYS, XSIZE_PHYS);
    LCD_SetVSizeEx(0, YSIZE_PHYS * NUM_VSCREENS, XSIZE_PHYS);
  } else {
    LCD_SetSizeEx (0, XSIZE_PHYS, YSIZE_PHYS);
    LCD_SetVSizeEx(0, XSIZE_PHYS, YSIZE_PHYS * NUM_VSCREENS);
  }


 #if (USE_TOUCH == 1)
    //
    // Set orientation of touch screen
    //
    GUI_TOUCH_SetOrientation(DISPLAY_ORIENTATION);
    //
    // Calibrate touch screen
    //
    GUI_TOUCH_Calibrate(GUI_COORD_X, 0, WIDTH  - 1, TOUCH_LEFT, TOUCH_RIGHT);
    GUI_TOUCH_Calibrate(GUI_COORD_Y, 0, HEIGHT - 1, TOUCH_TOP,  TOUCH_BOTTOM);
  #endif	
}

/*********************************************************************
*
*       LCD_X_DisplayDriver
*
* Purpose:
*   This function is called by the display driver for several purposes.
*   To support the according task the routine needs to be adapted to
*   the display controller. Please note that the commands marked with
*   'optional' are not cogently required and should only be adapted if 
*   the display controller supports these features.
*
* Parameter:
*   LayerIndex - Index of layer to be configured
*   Cmd        - Please refer to the details in the switch statement below
*   pData      - Pointer to a LCD_X_DATA structure
*/
int LCD_X_DisplayDriver(unsigned LayerIndex, unsigned Cmd, void * pData) {
    int r;
    
    switch (Cmd) 
    {
    /*
    * Required
    */
    case LCD_X_INITCONTROLLER: 
    {
        /*
        * Called during the initialization process in order to set up the
        * display controller and put it into operation. If the display
        * controller is not initialized by any external routine this needs
        * to be adapted by the customer...
        */
        _InitController();
        r = 0;
        break;
    }
    
    /* Implement all required tasks for the LCD_X_DisplayDriver function in 
    *  the following section. */
     
    default:
        r = -1;
    }
    return r;
}
