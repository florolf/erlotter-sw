/*
 * COPYRIGHT(c) 2016 STMicroelectronics
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#include <stdint.h>

/******************************************************************************/
/*                                                                            */
/*                      Analog Comparators (COMP)                             */
/*                                                                            */
/******************************************************************************/

typedef struct
{
  volatile uint32_t CSR;         /*!< COMP control and status register, Address offset: 0x00 */
} COMP_TypeDef;

#define COMPx(x) ((COMP_TypeDef*)(0x40010018 + 4 * (x)))

#define COMP2 COMPx(2)
#define COMP4 COMPx(4)
#define COMP6 COMPx(6)

/**********************  Bit definition for COMP_CSR register  ****************/
#define COMP_CSR_COMPxEN_Pos            (0U)
#define COMP_CSR_COMPxEN_Msk            (0x1U << COMP_CSR_COMPxEN_Pos)         /*!< 0x00000001 */
#define COMP_CSR_COMPxEN                COMP_CSR_COMPxEN_Msk                   /*!< COMPx enable */
#define COMP_CSR_COMPxINSEL_Pos         (4U)
#define COMP_CSR_COMPxINSEL_Msk         (0x40007U << COMP_CSR_COMPxINSEL_Pos)  /*!< 0x00400070 */
#define COMP_CSR_COMPxINSEL             COMP_CSR_COMPxINSEL_Msk                /*!< COMPx inverting input select */
#define COMP_CSR_COMPxINSEL_0           (0x00000010U)                          /*!< COMPx inverting input select bit 0 */
#define COMP_CSR_COMPxINSEL_1           (0x00000020U)                          /*!< COMPx inverting input select bit 1 */
#define COMP_CSR_COMPxINSEL_2           (0x00000040U)                          /*!< COMPx inverting input select bit 2 */
#define COMP_CSR_COMPxINSEL_3           (0x00400000U)                          /*!< COMPx inverting input select bit 3 */
#define COMP_CSR_COMPxOUTSEL_Pos        (10U)
#define COMP_CSR_COMPxOUTSEL_Msk        (0xFU << COMP_CSR_COMPxOUTSEL_Pos)     /*!< 0x00003C00 */
#define COMP_CSR_COMPxOUTSEL            COMP_CSR_COMPxOUTSEL_Msk               /*!< COMPx output select */
#define COMP_CSR_COMPxOUTSEL_0          (0x1U << COMP_CSR_COMPxOUTSEL_Pos)     /*!< 0x00000400 */
#define COMP_CSR_COMPxOUTSEL_1          (0x2U << COMP_CSR_COMPxOUTSEL_Pos)     /*!< 0x00000800 */
#define COMP_CSR_COMPxOUTSEL_2          (0x4U << COMP_CSR_COMPxOUTSEL_Pos)     /*!< 0x00001000 */
#define COMP_CSR_COMPxOUTSEL_3          (0x8U << COMP_CSR_COMPxOUTSEL_Pos)     /*!< 0x00002000 */
#define COMP_CSR_COMPxPOL_Pos           (15U)
#define COMP_CSR_COMPxPOL_Msk           (0x1U << COMP_CSR_COMPxPOL_Pos)        /*!< 0x00008000 */
#define COMP_CSR_COMPxPOL               COMP_CSR_COMPxPOL_Msk                  /*!< COMPx output polarity */
#define COMP_CSR_COMPxBLANKING_Pos      (18U)
#define COMP_CSR_COMPxBLANKING_Msk      (0x3U << COMP_CSR_COMPxBLANKING_Pos)   /*!< 0x000C0000 */
#define COMP_CSR_COMPxBLANKING          COMP_CSR_COMPxBLANKING_Msk             /*!< COMPx blanking */
#define COMP_CSR_COMPxBLANKING_0        (0x1U << COMP_CSR_COMPxBLANKING_Pos)   /*!< 0x00040000 */
#define COMP_CSR_COMPxBLANKING_1        (0x2U << COMP_CSR_COMPxBLANKING_Pos)   /*!< 0x00080000 */
#define COMP_CSR_COMPxBLANKING_2        (0x4U << COMP_CSR_COMPxBLANKING_Pos)   /*!< 0x00100000 */
#define COMP_CSR_COMPxOUT_Pos           (30U)
#define COMP_CSR_COMPxOUT_Msk           (0x1U << COMP_CSR_COMPxOUT_Pos)        /*!< 0x40000000 */
#define COMP_CSR_COMPxOUT               COMP_CSR_COMPxOUT_Msk                  /*!< COMPx output level */
#define COMP_CSR_COMPxLOCK_Pos          (31U)
#define COMP_CSR_COMPxLOCK_Msk          (0x1U << COMP_CSR_COMPxLOCK_Pos)       /*!< 0x80000000 */
#define COMP_CSR_COMPxLOCK              COMP_CSR_COMPxLOCK_Msk                 /*!< COMPx lock */
