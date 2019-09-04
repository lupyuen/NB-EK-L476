/*
 * Copyright (c) 2016, Texas Instruments Incorporated
 * All rights reserved.
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

/* -----------------------------------------------------------------------------
 *  Includes
 * -----------------------------------------------------------------------------
 */
#include "Display.h"

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
/* -----------------------------------------------------------------------------
 *  Externs
 * -----------------------------------------------------------------------------
 */
extern const Display_Config Display_config[];

/* -----------------------------------------------------------------------------
 *  Constants and macros
 * -----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------
 *   Type definitions
 * -----------------------------------------------------------------------------
 */

/* -----------------------------------------------------------------------------
 *                           Local variables
 * -----------------------------------------------------------------------------
 */
/* Default Display parameters structure */
const Display_Params Display_defaultParams = {
    DISPLAY_CLEAR_BOTH,   /* Clear entire line before writing */
};

/* -----------------------------------------------------------------------------
 *                                          Functions
 * -----------------------------------------------------------------------------
 */

/*
 *  ======== Display_doOpen ========
 */
Display_Handle Display_doOpen(uint32_t id, Display_Params *params)
{
    Display_Handle handle;
    if (params == NULL)
    {
        params = (Display_Params *)&Display_defaultParams;
    }

    /* Call each driver's open function */
    uint32_t i;
    for (i = 0; Display_config[i].fxnTablePtr != NULL; i++)
    {
        handle = (Display_Handle) & Display_config[i];

        /* Open if id matches, or if meta-type matches */
        if (id == i || (handle->fxnTablePtr->getTypeFxn() & id))
        {
            if (NULL == handle->fxnTablePtr->openFxn(handle, params))
            {
                // Couldn't open. Continue trying to open in case a type was
                // provided and there's more than one of the type.
                continue;
            }
            else
            {
                // Return first matching
                return handle;
            }
        }
    }
    // Couldn't open.

    return NULL;
}

/*
 *  ======== Display_doParamsInit ========
 */
void Display_doParamsInit(Display_Params *params)
{
    *params = Display_defaultParams;
}


/*
 *  ======== Display_doClear ========
 */
void Display_doClear(Display_Handle handle)
{
    if (NULL == handle)
    {
        return;
    }

    handle->fxnTablePtr->clearFxn(handle);
}


/*
 *  ======== Display_doClearLines ========
 */
void Display_doClearLines(Display_Handle handle, uint8_t fromLine, uint8_t toLine)
{
    if (NULL == handle)
    {
        return;
    }

    handle->fxnTablePtr->clearLinesFxn(handle, fromLine, toLine);
}
/*
 *  ======== Display_doPut5 ========
 */
void Display_doPut5(Display_Handle handle, uint8_t cle,uint8_t line, uint8_t column,
                    char* fmt, ...)
{
    if (NULL == handle)
    {
        return;
    }
    va_list va;
    va_start(va,fmt);
    handle->fxnTablePtr->put5Fxn(handle,cle, line, column, fmt, va);
    va_end(va);
}

/*
 *  ======== Display_doControl ========
 */
void  Display_doControl(Display_Handle handle, unsigned int cmd, void *arg)
{
    if (NULL == handle)
    {
        return;
    }

    handle->fxnTablePtr->controlFxn(handle, cmd, arg);
}

/*
 *  ======== Display_doClose ========
 */
void Display_doClose(Display_Handle handle)
{
    if (NULL == handle)
    {
        return;
    }

    handle->fxnTablePtr->closeFxn(handle);
}
