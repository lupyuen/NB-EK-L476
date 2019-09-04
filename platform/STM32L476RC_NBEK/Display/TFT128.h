/*
 * Copyright (c) 2015, Ghostyu  Co.Ltd.,
 *          All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef LCD_TFT_1_44_128X128_H
#define LCD_TFT_1_44_128X128_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
 
#include "gpio.h"
#include "spi.h"
/*! @internal Number of pixels in LCD display */
#define LCD_PIXELS          16384         /* 128 * 128 = 16384 */
/*! Number of bytes needed in LCD buffer */
#define LCD_BYTES           1024
/*! Number of pixel columns */
#define LCD_COLS            128
/*! First pixel on LCD x-axis */
#define LCD_X_MIN           0
/*! Last pixel on LCD x-axis */
#define LCD_X_MAX           (LCD_COLS-1)
/*! Number of pixel rows */
#define LCD_ROWS            128
/*! First pixel on LCD y-axis */
#define LCD_Y_MIN           0
/*! Last pixel on LCD y-axis */
#define LCD_Y_MAX           (LCD_ROWS-1)
/*! @internal Number of pages */
#define LCD_PAGES           8
/*! Number of pixel rows per LCD page */
#define LCD_PAGE_ROWS       16
/*! Space used for each character */
#define LCD_CHAR_WIDTH      8
/*! Actual font character width */
#define LCD_FONT_WIDTH      8
#define MAX_CHAR_NUMM       (LCD_COLS >> 3)

/*!
*  @brief      A handle that is returned from a LCD_open() call.
*/
typedef struct LCD_Config   *LCD_Handle;

/*!
 *  @brief      LCD alignment enum
 *
 *  This enumeration defines the text alignment.
 *  It is used by LCD_bufferPrintxxxAligned functions.
 *
 */
typedef enum LCD_Align
{
    LCD_ALIGN_LEFT,     /*!< Text is aligned to the left */
    LCD_ALIGN_CENTER,   /*!< Text is aligned in the center */
    LCD_ALIGN_RIGHT     /*!< Text is aligned to the right */
} LCD_Align;

/*!
 *  @brief      LCD page enum
 *
 *  This enumeration defines the LCD pages.
 *  It is used by LCD_bufferxxx functions
 *
 */
typedef enum LCD_Page
{
    LCD_PAGE0 = 0,
    LCD_PAGE1,
    LCD_PAGE2,
    LCD_PAGE3,
    LCD_PAGE4,
    LCD_PAGE5,
    LCD_PAGE6,
    LCD_PAGE7,
    LCD_PAGE_COUNT
} LCD_Page;

/*!
 *  @brief      LCD x-axis enum
 *
 *  This enumeration defines x axis limit
 *  It is used for x argument in LCD_bufferxxx functions
 *
 */
typedef enum LCD_X_Limit
{
    LCD_X_FIRST = 0,
    LCD_X_LAST = (LCD_COLS-1)
} LCD_X_Limit;

/*!
 *  @brief      LCD y-axis enum
 *
 *  This enumeration defines y axis limit
 *  It is used for y argument in LCD_bufferxxx functions
 *
 */
typedef enum LCD_Y_Limit
{
    LCD_Y_FIRST = 0,
    LCD_Y_LAST = (LCD_ROWS-1)
} LCD_Y_Limit;

/*
 * The LCD Color enum
 */
// 高低字节已经对调。以RED为例：真实数据为0xf800
typedef enum Color
{
  RED    = 0x00f8,
  GREEN  = 0xe007,
  BULE   = 0x1f00,
  WHITE  = 0xffff,
  BLACK  = 0X0000,
  YELLOW = 0XE0FF,
  GRAY0  = 0X7DEF,
  GRAY1  = 0x1084,
  GRAY2  = 0x0842
}Color;
/*
 * 定义背景色与前景色
 */
typedef struct LCD_Color
{
  Color blackColor;
  Color frontColor;
}LCD_Color;
/*!
 *  @brief
 *  LCD Parameters are used to with the LCD_open() call. Default values for
 *  these parameters are set using LCD_Params_init().
 *
 *  @sa     SPI_Params_init
 */
typedef struct LCD_Params
{
     unsigned int       lcdWriteTimeout;    /*!< Timeout for write semaphore */

     /* SPI parameters */
     unsigned int       spiBitRate;         /*!< SPI bit rate in Hz */
     unsigned int       CPOL;               /*!< Clock Polarity */
     unsigned int       CPHA;               /*!< Clock Phare */
} LCD_Params;

/*
 * 定义LCD 睡眠指令
 */
typedef struct LCD_Sleep_CMD
{
  char cmdId;
  char delayMs;
}LCD_Sleep_CMD;

typedef struct LCD_Param_CMD
{
  char cmdId;
  char count;
  char param[20];
}LCD_Param_CMD;

/*!
 *  @brief Format of LCD commands used by the LCD controller.
 *
 */
typedef struct LCD_Command
{
    LCD_Sleep_CMD  sleepExit;
    LCD_Param_CMD      frameCtrl1;
    LCD_Param_CMD      frameCtrl2;
    LCD_Param_CMD      frameCtrl3;
    LCD_Param_CMD      frameCtrl4;
    LCD_Param_CMD      powerSequence0;
    LCD_Param_CMD      powerSequence1;
    LCD_Param_CMD      powerSequence2;
    LCD_Param_CMD      powerSequence3;
    LCD_Param_CMD      powerSequence4;
    LCD_Param_CMD      powerSequence5;
    
    LCD_Param_CMD      vcom;
    LCD_Param_CMD      memDataAccessCtrl;
    LCD_Param_CMD      gammaSequence1;
    LCD_Param_CMD      gammaSequence2;
    LCD_Param_CMD      columnAddrSet;
    LCD_Param_CMD      rowAddrSet;
    LCD_Param_CMD      testSet;           //4k,65k,256k
    LCD_Param_CMD      powerSave; 
    LCD_Param_CMD      rgbMode; 
    LCD_Param_CMD      displayOn; 
} LCD_Command;

/*!
 *  @brief  LCD_Buffer used to store data to be printed on the LCD display.
 *
 *  A sample structure is shown below:
 *  @code
 *  LCD_Buffer lcdBuffers[] = {
 *      {lcdBuffer0, LCD_BYTES, NULL},
 *      {lcdBuffer1, LCD_BYTES, NULL},
 *  };
 *  @endcode
 */
typedef struct LCD_Buffer
{
    unsigned short      *pcBuffer;  /*!< Ptr to a buffer with data to be transmitted */
    unsigned int        bufSize;    /*!< Size of the buffer */
    //Semaphore_Struct    bufMutex;   /*!< Semaphore associated with the buffer */
} LCD_Buffer;

/*!
 *  @brief      LCD Object
 *
 *  The application must not access any member variables of this structure!
 */
typedef struct LCD_Object
{
    /* LCD control variables */
  uint8_t              isOpen;            /* Has the obj been opened? */
  SPI_HandleTypeDef   *spiHandle;         /* SPI handle used by the LCD */
  unsigned int         lcdWriteTimeout;   /* Timeout for write semaphore */
  uint32_t             lcdMutex;          /* Semaphore associated with the lcd */
  LCD_Buffer           *lcdBuffers;       /* Pointer to the array
                                               with lcd buffers */
  uint8_t              nBuffers;          /* number of buffers availible */
  LCD_Color*           pColorInfo;        /* the color infomation */
} LCD_Object;

/*!
 *  @brief      LCD Hardware attributes
 *
 *  A sample structure is shown below:
 *  @code
 *  const LCD_HWAttrs lcdHWAttrs = {
 *      .LCD_initCmd = &LCD_initCmd,
 *      .lcdResetPin = Board_LCD_RST,
 *      .lcdModePin = Board_LCD_MODE,
 *      .lcdCsnPin = Board_LCD_CSN,
 *      .spiIndex = Board_SPI0
 *  };
 *  @endcode
 */
typedef struct LCD_HWAttrs
{
    /*! LCD initialization command */
    LCD_Command const *LCD_initCmd;
    /* LCD mode pin */
    const PIN_HWAttrs           lcdModePin;
    /* LCD CSn pin */
    const PIN_HWAttrs           lcdCsnPin;
    /* Logical peripheral number indexed
       into the SPI_config table */
    SPI_Config     *spiIndex;

} LCD_HWAttrs;

/*!
 *  @brief
 *  The LCD_Config structure contains a set of pointers used to characterize
 *  the LCD driver implementation.
 */
typedef struct LCD_Config
{
  /*! Pointer to a driver specific data object */
  LCD_Object            *object;
  /*! Pointer to a driver specific hardware attributes structure */
  LCD_HWAttrs   const   *hwAttrs;
} LCD_Config;

/*! @internal
 *
 *  @brief   LCD initialization command sequence used to initialize
 *           The LCD display. The format of the command in defined in
 *           the LCD_Command structure
 *
 *  @sa      LCD_Command
 */
static const LCD_Command LCD_initCmd =
{
  {0x11,120},              /*Exit sleep mode,and delay 120ms     */
  {0xB1,3,0x01,0x2C,0x2D}, /*Frame rate control 1 */
  {0xB2,3,0x01,0x2C,0x2D}, /*Frame rate control 2                     */
  {0xB3,6,0x01,0x2C,0x2D,0x01,0x2C,0x2D}, /*Frame rate control 3      */
  {0xB4,1,0x07},           /*Colume inversion               */
  {0xC0,3,0xA2,0x02,0x84}, /*power control setting   */
  {0xC1,1,0xC5},           /*power control setting   */
  {0xC2,2,0x0A,0x00},      /*power control setting in normal mode (full color)*/
  {0xC3,2,0x8A,0x2A},      /*power control setting in idle mode (8 color) */
  {0xC4,2,0x8A,0xEE},      /*power control setting in partial mode(full color)*/
  {0xC5,1,0x0E},           /*vcom*/
  {0x36,1,0xC8},           /* Memery data access control */
  {0xE0,16,0x0f,0x1a,0x0f,0x18,0x2f,0x28,0x20,0x22,0x1f,0x1b,0x23,0x37,0x00,0x07,0x02,0x10}, /*gamma control setting 1 */
  {0xE1,16,0x0f,0x1B,0x0f,0x17,0x33,0x2C,0x29,0x2E,0x30,0x30,0x39,0x3F,0x00,0x07,0x03,0x10}, /*gamma control setting 2  */
  {0x2a,4,0x00,0x00,0x00,0x7f}, /*column address set  */
  {0x2b,4,0x00,0x00,0x00,0x9f}, /*row addres set*/
  {0xf0,1,0x01},
  {0xf6,1,0x00},
  {0x3a,1,0x05},                /* rgb = 65k (565)*/
  {0x29,0}          /* display on*/
};

/*!
 *  @brief  Function to close the LCD instance specified by the
 *          LCD handle.
 *
 *  @pre    LCD_open() has to be called first. Function assumes that the handle is not NULL.
 *          Calling context: Task.
 *
 *  @param  handle A LCD handle returned from LCD_open()
 *
 *  @sa     LCD_open()
 */
extern void LCDTFT_close(LCD_Handle handle);

/*!
 *  @brief  This function initializes the LCD driver module.
 *
 *  @pre    The LCD_config structure must exist and be persistent before
 *          this function can be called. This function must be called before any
 *          other LCD driver APIs.
 *          Calling context: Hwi, Swi, Task, Main.
 */
extern void LCDTFT_init(void);

/*!
 *  @brief   Function to set up the DOGM128W-6 LCD display
 *
 *  @pre     LCD controller has been initialized using LCD_init().
 *           Calling context: Task.
 *
 *  @param   buffers     Pointer to a buffer block.
 *
 *  @param   nBuffers    Number of buffers in the buffer block.
 *
 *  @param   params      Pointer to a parameter block, if NULL it will use
 *                       default values
 *
 *  @return  A pointer to a LCD_Handle on success or a NULL it was already
 *           opened
 *
 *  @sa      LCD_close()
 *  @sa      LCD_init()
 */
extern LCD_Handle LCDTFT_open(LCD_Buffer *buffers, uint8_t nBuffers, LCD_Params *params);

/*!
 *  @brief  Function to initialize the LCD_Params struct to its defaults
 *
 *  Defaults values are:
 *  @code
 *  lcdWriteTimeout = BIOS_WAIT_FOREVER;
 *  spiBitRate      = 1000000;
 *  spiFrameFormat  = SPI_POL0_PHA0;
 *  @endcode
 *
 *  @pre    Calling context: Task.
 *
 *  @param  params  Parameter structure to initialize
 */
extern void LCDTFT_Params_init(LCD_Params *params);

/*!
 *  @brief  Function that writes a string and value to a
 *          buffer and sends it to the LCD display. The written page is being
 *          cleared before it is written to.
 *
 *          LCD_write will block task execution until all
 *          the data in buffer has been written to the LCD.
 *
 *  @pre    Function assumes that the handle and buffer is not NULL.
 *          Calling context: Task
 *
 *  @param  handle     A LCD_Handle
 *
 *  @param  bufIndex   The buffer to use, specified by its index in the LCD_Buffer structure.
 *
 *  @param  str        A pointer to the string to print.
 *
 *  @param  uiValue    Value to print
 *
 *  @param  ucFormat   Base of the value to print ?2,8,16 etc.
 *                     ucFormat must be between 2 and 36, where 10 means decimal base,
 *                     16 hexadecimal, 8 octal, and 2 binary.
 *                     If ucFormat is zero, only the string will get printed.
 *
 *  @param  ucLine     The page to write. Must be a value from 0-7.
 *
 */
extern void LCDTFT_writeLine(LCD_Handle handle, char *str, unsigned int uiValue, unsigned char ucFormat, unsigned char ucLine);


/*!
 *  @brief  Function that empties the specified LCD buffer
 *
 *          LCD_bufferClear will block task execution until all
 *          the buffer modification has finished.
 *
 *  @pre    Function assumes that the handle and buffer is not NULL.
 *          Calling context: Task.
 *
 *  @param  handle      A LCD_Handle
 *
 *  @param  bufIndex    The buffer to use, specified by its index in the LCD_Buffer structure.
 *
 */
extern void LCDTFT_bufferClear(LCD_Handle handle);
/*!
 *  @brief  display start cover 
 *
 *          LCD_bufferStartCover will block task execution until all
 *          the buffer modification has finished.
 *
 *  @pre    Function assumes that the handle and buffer is not NULL.
 *          Calling context: Task.
 *
 *  @param  handle      A LCD_Handle
 *
 *  @param  bufIndex    The buffer to use, specified by its index in the LCD_Buffer structure.
 *
 */
extern void LCDTFT_bufferStartCover(LCD_Handle handle);

/*!
 *  @brief  This function clears the page specified by @e iPage in the given buffer
 *
 *          LCD_bufferClearPage will block task execution until all
 *          the buffer modification has finished.
 *
 *  @pre    Function assumes that the handle and buffer is not NULL.
 *          Calling context: Task.
 *
 *  @param  handle      A LCD_Handle
 *
 *  @param  bufIndex    The buffer to use, specified by its index in the LCD_Buffer structure.
 *
 *  @param  iPage       The page to clear. Must be one of the
 *                      following enumerated values:
 *                      @li @b LCD_PAGE0
 *                      @li @b LCD_PAGE1
 *                      @li @b LCD_PAGE2
 *                      @li @b LCD_PAGE3
 *                      @li @b LCD_PAGE4
 *                      @li @b LCD_PAGE5
 *                      @li @b LCD_PAGE6
 *                      @li @b LCD_PAGE7
 *
 *  @return   None
 *
 */
extern void LCDTFT_bufferClearPage(LCD_Handle handle, LCD_Page iPage);

/*!
 *  @brief  This function clears the pixels in a given piece of a page.
 *          Resolution is given in coulmns [0--127] and pages [0--7]. The
 *          function assumes @e ucXFrom <= @e ucXTo and
 *          @e iPageFrom <= @e iPageTo.
 *
 *          LCD_bufferClearPart will block task execution until all
 *          the buffer modification has finished.
 *
 *  @pre    Function assumes that the handle and buffer is not NULL.
 *          Calling context: Task.
 *
 *  @param  handle     A LCD_Handle
 *
 *  @param  bufIndex   The buffer to use, specified by its index in the LCD_Buffer structure.
 *
 *  @param  ucXFrom    The lowest x-position (column) to be cleared [0--127].
 *
 *  @param  ucXTo      The highest x-position to be cleared [ucXFrom--127].
 *
 *  @param  iPageFrom  The first page cleared. Must be one of the
 *                     following enumerated values:
 *                     @li @b LCD_PAGE0
 *                     @li @b LCD_PAGE1
 *                     @li @b LCD_PAGE2
 *                     @li @b LCD_PAGE3
 *                     @li @b LCD_PAGE4
 *                     @li @b LCD_PAGE5
 *                     @li @b LCD_PAGE6
 *                     @li @b LCD_PAGE7
 *
 * @param    iPageTo   The last page cleared [iPageFrom--LCD_PAGE7].
 *
 * @return   None
 *
 */
extern void LCDTFT_bufferClearPart(LCD_Handle handle,unsigned char ucXFrom, unsigned char ucXTo,
                   LCD_Page iPageFrom, LCD_Page iPageTo);

/*!
 *  @brief  This function inverts the pixels (bits) in a given region of the
 *          specified buffer.
 *
 *          LCD_bufferInvert will block task execution until all
 *          the buffer modification has finished.
 *
 *  @pre    Function assumes that the handle and buffer is not NULL.
 *          Calling context: Task.
 *
 *  @param  handle     A LCD_Handle
 *
 *  @param  bufIndex   The buffer to use, specified by its index in the LCD_Buffer structure.
 *
 *  @param  ucXFrom    The first x-position (column) to invert [0--127].
 *
 *  @param  ucYFrom    is the first y-position (row) to invert [0--63].
 *
 *  @param  ucXTo      The last x-position (column) to invert [0--127].
 *
 *  @param  ucYTo      The last y-position (row) to invert [0--63].
 *
 *  @return None
 *
 */
extern void LCDTFT_bufferInvert(LCD_Handle handle, unsigned char ucXFrom, unsigned char ucYFrom,
                    unsigned char ucXTo, unsigned char ucYTo);

/*!
 *  @brief  This function inverts a range of columns in the display buffer on a
 *          specified page (for example, @b LCD_PAGE0). This function assumes
 *          @e ucXFrom <= @e ucXTo.
 *
 *          LCD_bufferInvertPage will block task execution until all
 *          the buffer modification has finished.
 *
 *  @pre    Function assumes that the handle and buffer is not NULL.
 *          Calling context: Task.
 *
 *  @param  handle     A LCD_Handle
 *
 *  @param  bufIndex   The buffer to use, specified by its index in the LCD_Buffer structure.
 *
 *  @param  ucXFrom    The first x-position (column) to invert [0--127].
 *
 *  @param  ucXTo      The last x-position (column) to invert [ucXFrom--127].
 *
 *  @param  iPage      The page on which to invert. Must be one of the
 *                     following enumerated values:
 *                     @li @b LCD_PAGE0
 *                     @li @b LCD_PAGE1
 *                     @li @b LCD_PAGE2
 *                     @li @b LCD_PAGE3
 *                     @li @b LCD_PAGE4
 *                     @li @b LCD_PAGE5
 *                     @li @b LCD_PAGE6
 *                     @li @b LCD_PAGE7
 *
 *  @return   None
 */
extern void LCDTFT_bufferInvertPage(LCD_Handle handle, unsigned char ucXFrom, unsigned char ucXTo,
                    LCD_Page iPage);

/*!
 *  @brief  Function that writes a string to the specified buffer
 *
 *          LCD_bufferPrintString will block task execution until all
 *          the buffer modification has finished.
 *
 *  @pre    Function assumes that the handle and buffer is not NULL.
 *          Calling context: Task.
 *
 *  @param  handle      A LCD_Handle
 *
 *  @param  bufIndex    The buffer to use, specified by its index in the LCD_Buffer structure.
 *
 *  @param  pcStr       A pointer to the string to print.
 *
 *  @param  ucX         The x-position (column) to begin printing [0--127].
 *
 *  @param  iPage       The page on which to print. Must be one of the
 *                      following enumerated values:
 *                      @li @b LCD_PAGE0
 *                      @li @b LCD_PAGE1
 *                      @li @b LCD_PAGE2
 *                      @li @b LCD_PAGE3
 *                      @li @b LCD_PAGE4
 *                      @li @b LCD_PAGE5
 *                      @li @b LCD_PAGE6
 *                      @li @b LCD_PAGE7
 *
 *
 */
extern void LCDTFT_bufferPrintString(LCD_Handle handle, const char *pcStr, unsigned char ucX,
                                  LCD_Page iPage);


/*!
 *  @brief  This function draws a line in the specified buffer from
 *          (@e ucXFrom,@e ucYFrom) to (@e ucXTo,@e ucYTo). The function
 *          uses Bresenham's line algorithm.
 *
 *          This function will block task execution until all
 *          the buffer modification has finished.
 *
 *  @pre    Function assumes that the handle and buffer is not NULL.
 *          Calling context: Task.
 *
 *  @param  handle     A LCD_Handle
 *
 *  @param  bufIndex   The buffer to use, specified by its index in the LCD_Buffer structure.
 *
 *  @param  ucXFrom    The start column [0--127].
 *
 *  @param  ucXTo      The end column [0--127].
 *
 *  @param  ucYFrom    The start row [0--63].
 *
 *  @param  ucYTo      The end row [0--63].
 *
 *  @return None
 *
 */
extern void LCDTFT_bufferSetLine(LCD_Handle handle, unsigned char ucXFrom, unsigned char ucYFrom,
                 unsigned char ucXTo, unsigned char ucYTo);

/*!
 *  @brief  This function clears a line in the specified buffer from
 *          (@e ucXFrom,@e ucYFrom) to (@e ucXTo,@e ucYTo). The function
 *          uses Bresenham's line algorithm.
 *
 *          This function will block task execution until all
 *          the buffer modification has finished.
 *
 *  @pre    Function assumes that the handle and buffer is not NULL.
 *          Calling context: Task.
 *
 *  @param  handle     A LCD_Handle
 *
 *  @param  bufIndex   The buffer to use, specified by its index in the LCD_Buffer structure.
 *
 *  @param  ucXFrom    The start column [0--127].
 *
 *  @param  ucXTo      The end column [0--127].
 *
 *  @param  ucYFrom    The start row [0--63].
 *
 *  @param  ucYTo      The end row [0--63].
 *
 *  @return None
 *
 */
extern void LCDTFT_bufferClearLine(LCD_Handle handle, unsigned char ucXFrom, unsigned char ucYFrom,
                   unsigned char ucXTo, unsigned char ucYTo);

/*!
 *  @brief   This function draws a horizontal line from (@e ucXFrom,@e ucY) to
 *           (@e ucXTo,@e ucY) into the specified buffer.
 *
 *           This function will block task execution until all
 *           the buffer modification has finished.
 *
 *  @pre    Function assumes that the handle and buffer is not NULL.
 *          Calling context: Task.
 *
 *  @param   handle     A LCD_Handle
 *
 *  @param   bufIndex   The buffer to use, specified by its index in the LCD_Buffer structure.
 *
 *  @param   ucXFrom    The start column [0--127].
 *
 *  @param   ucXTo      The end column [0--127].
 *
 *  @param   ucY        The row [0--63].
 *
 *  @return  None
 *
 */
extern void LCDTFT_bufferSetHLine(LCD_Handle handle, unsigned char ucXFrom, unsigned char ucXTo,
                  unsigned char ucY);

/*!
 *  @brief  This function clears a horizontal line from (@e ucXFrom,@e ucY) to
 *          (@e ucXTo,@e ucY) from the specified buffer.
 *
 *          This function will block task execution until all
 *          the buffer modification has finished.
 *
 *  @pre    Function assumes that the handle and buffer is not NULL.
 *          Calling context: Task.
 *
 *  @param  handle      A LCD_Handle
 *
 *  @param  bufIndex    The buffer to use, specified by its index in the LCD_Buffer structure.
 *
 *  @param  ucXFrom     The start column [0--127].
 *
 *  @param  ucXTo       The end column [0--127].
 *
 *  @param  ucY         The row [0--63].
 *
 *  @return None
 *
 */
extern void LCDTFT_bufferClearHLine(LCD_Handle handle, unsigned char ucXFrom,
                    unsigned char ucXTo, unsigned char ucY);

/*!
 *  @brief  This function draws a vertical line from (ucX,ucYFrom) to
 *          (ucX,ucYTo) into the specified buffer.
 *
 *          This function will block task execution until all
 *          the buffer modification has finished.
 *
 *  @pre    Function assumes that the handle and buffer is not NULL.
 *          Calling context: Task.
 *
 *  @param  handle     A LCD_Handle
 *
 *  @param  bufIndex   The buffer to use, specified by its index in the LCD_Buffer structure.
 *
 *  @param  ucX        The x-position (column) of the line [0--127].
 *
 *  @param  ucYFrom    The start row [0--63].
 *
 *  @param  ucYTo      The end row [0--63].
 *
 *  @return None
 *
 */
extern void LCDTFT_bufferSetVLine(LCD_Handle handle, unsigned char ucX, unsigned char ucYFrom,
                  unsigned char ucYTo);

/*!
 *  @brief  This function clears a vertical line from (@e ucX,@e ucYFrom) to
 *          (@e ucX,@e ucYTo) from the buffer specified.
 *
 *          This function will block task execution until all
 *          the buffer modification has finished.
 *
 *  @pre    Function assumes that the handle and buffer is not NULL.
 *          Calling context: Task.
 *
 *  @param  handle     A LCD_Handle
 *
 *  @param  bufIndex   The buffer to use, specified by its index in the LCD_Buffer structure.
 *
 *  @param  ucX        The x-position (column) of the line [0--127].
 *
 *  @param  ucYFrom    The start row [0--63].
 *
 *  @param  ucYTo      The end row [0--63].
 *
 *  @return  None
 *
 */
extern void LCDTFT_bufferClearVLine(LCD_Handle handle, unsigned char ucX, unsigned char ucYFrom,
                    unsigned char ucYTo);

/*!
 *  @brief  This function draws a horizontal arrow from (@e ucXFrom,@e ucY) to
 *          (@e ucXTo,@e ucY) to buffer specified. The function
 *          assumes @e ucY to be in the range [2--61] in order for arrowhead to
 *          fit on the LCD.
 *
 *          This function will block task execution until all
 *          the buffer modification has finished.
 *
 *  @pre    Function assumes that the handle and buffer is not NULL.
 *          Calling context: Task.
 *
 *  @param  handle     A LCD_Handle
 *
 *  @param  bufIndex   The buffer to use, specified by its index in the LCD_Buffer structure.
 *
 *  @param  ucXFrom    The start column [0--127].
 *
 *  @param  ucXTo      The end column [0--127].
 *
 *  @param  ucY        The the y-position (row) of the arrow [2--61].
 *
 *  @return None
 *
 */
extern void LCDTFT_bufferHArrow(LCD_Handle handle, unsigned char ucXFrom, unsigned char ucXTo, unsigned char ucY);

/*!
 *  @brief  This function draws a vertical arrow from (@e ucX,@e ucYFrom) to
 *          (@e ucX,@e ucYTo) to the buffer specified.
 *          The function assumes that @e ucX is in the range [2--125] for the
 *          arrowhead to fit on the LCD.
 *
 *          This function will block task execution until all
 *          the buffer modification has finished.
 *
 *  @pre    Function assumes that the handle and buffer is not NULL.
 *          Calling context: Task.
 *
 *  @param  handle     A LCD_Handle
 *
 *  @param  bufIndex   The buffer to use, specified by its index in the LCD_Buffer structure.
 *
 *  @param  ucX        The the x-position (column) of the arrow [2--125].
 *
 *  @param  ucYFrom    The start row [0--63].
 *
 *  @param  ucYTo      The end row [0--63].
 *
 *
 *  @return   None
 */
extern void LCDTFT_bufferVArrow(LCD_Handle handle, unsigned char ucX, unsigned char ucYFrom, unsigned char ucYTo);

/*!
 *  @brief  This function sets a pixel on (@e ucX,@e ucY).
 *
 *          This function will block task execution until all
 *          the buffer modification has finished.
 *
 *  @pre    Function assumes that the handle and buffer is not NULL.
 *          Calling context: Task.
 *
 *  @param  handle     A LCD_Handle
 *
 *  @param  bufIndex   The buffer to use, specified by its index in the LCD_Buffer structure.
 *
 *  @param  ucX        The pixel x-position (column) [0--127].
 *
 *  @param  ucY        The pixel y-position (row) [0--63].
 *
 *  @return None
 *
 */
extern void LCDTFT_bufferSetPx(LCD_Handle handle, unsigned char ucX, unsigned char ucY);

/*!
 *  @brief  This function clears the pixel at (@e ucX,@e ucY).
 *
 *          This function will block task execution until all
 *          the buffer modification has finished.
 *
 *  @pre    Function assumes that the handle and buffer is not NULL.
 *          Calling context: Task.
 *
 *  @param  handle      A LCD_Handle
 *
 *  @param  bufIndex    The buffer to use, specified by its index in the LCD_Buffer structure.
 *
 *  @param  ucX         The pixel x-position (column) [0--127].
 *
 *  @param  ucY         The pixel y-position (row) [0--63].
 *
 *  @return None
 */
extern void LCDTFT_bufferClearPx(LCD_Handle handle, unsigned char ucX, unsigned char ucY);


#ifdef __cplusplus
}
#endif

#endif /* LCD_TFT_1_44_128X128_H */
