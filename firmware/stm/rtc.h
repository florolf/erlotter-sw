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
/*                           Real-Time Clock (RTC)                            */
/*                                                                            */
/******************************************************************************/

typedef struct
{
  volatile uint32_t TR;         /*!< RTC time register,                                        Address offset: 0x00 */
  volatile uint32_t DR;         /*!< RTC date register,                                        Address offset: 0x04 */
  volatile uint32_t CR;         /*!< RTC control register,                                     Address offset: 0x08 */
  volatile uint32_t ISR;        /*!< RTC initialization and status register,                   Address offset: 0x0C */
  volatile uint32_t PRER;       /*!< RTC prescaler register,                                   Address offset: 0x10 */
  volatile uint32_t WUTR;       /*!< RTC wakeup timer register,                                Address offset: 0x14 */
  uint32_t RESERVED0;           /*!< Reserved, 0x18                                                                 */
  volatile uint32_t ALRMAR;     /*!< RTC alarm A register,                                     Address offset: 0x1C */
  volatile uint32_t ALRMBR;     /*!< RTC alarm B register,                                     Address offset: 0x20 */
  volatile uint32_t WPR;        /*!< RTC write protection register,                            Address offset: 0x24 */
  volatile uint32_t SSR;        /*!< RTC sub second register,                                  Address offset: 0x28 */
  volatile uint32_t SHIFTR;     /*!< RTC shift control register,                               Address offset: 0x2C */
  volatile uint32_t TSTR;       /*!< RTC time stamp time register,                             Address offset: 0x30 */
  volatile uint32_t TSDR;       /*!< RTC time stamp date register,                             Address offset: 0x34 */
  volatile uint32_t TSSSR;      /*!< RTC time-stamp sub second register,                       Address offset: 0x38 */
  volatile uint32_t CALR;       /*!< RTC calibration register,                                 Address offset: 0x3C */
  volatile uint32_t TAFCR;      /*!< RTC tamper and alternate function configuration register, Address offset: 0x40 */
  volatile uint32_t ALRMASSR;   /*!< RTC alarm A sub second register,                          Address offset: 0x44 */
  volatile uint32_t ALRMBSSR;   /*!< RTC alarm B sub second register,                          Address offset: 0x48 */
  uint32_t RESERVED7;           /*!< Reserved, 0x4C                                                                 */
  volatile uint32_t BKP0R;      /*!< RTC backup register 0,                                    Address offset: 0x50 */
  volatile uint32_t BKP1R;      /*!< RTC backup register 1,                                    Address offset: 0x54 */
  volatile uint32_t BKP2R;      /*!< RTC backup register 2,                                    Address offset: 0x58 */
  volatile uint32_t BKP3R;      /*!< RTC backup register 3,                                    Address offset: 0x5C */
  volatile uint32_t BKP4R;      /*!< RTC backup register 4,                                    Address offset: 0x60 */
} RTC_TypeDef;

#define RTC ((RTC_TypeDef*)0x40002800)

/********************  Bits definition for RTC_TR register  *******************/
#define RTC_TR_PM_Pos                (22U)
#define RTC_TR_PM_Msk                (0x1U << RTC_TR_PM_Pos)                   /*!< 0x00400000 */
#define RTC_TR_PM                    RTC_TR_PM_Msk
#define RTC_TR_HT_Pos                (20U)
#define RTC_TR_HT_Msk                (0x3U << RTC_TR_HT_Pos)                   /*!< 0x00300000 */
#define RTC_TR_HT                    RTC_TR_HT_Msk
#define RTC_TR_HT_0                  (0x1U << RTC_TR_HT_Pos)                   /*!< 0x00100000 */
#define RTC_TR_HT_1                  (0x2U << RTC_TR_HT_Pos)                   /*!< 0x00200000 */
#define RTC_TR_HU_Pos                (16U)
#define RTC_TR_HU_Msk                (0xFU << RTC_TR_HU_Pos)                   /*!< 0x000F0000 */
#define RTC_TR_HU                    RTC_TR_HU_Msk
#define RTC_TR_HU_0                  (0x1U << RTC_TR_HU_Pos)                   /*!< 0x00010000 */
#define RTC_TR_HU_1                  (0x2U << RTC_TR_HU_Pos)                   /*!< 0x00020000 */
#define RTC_TR_HU_2                  (0x4U << RTC_TR_HU_Pos)                   /*!< 0x00040000 */
#define RTC_TR_HU_3                  (0x8U << RTC_TR_HU_Pos)                   /*!< 0x00080000 */
#define RTC_TR_MNT_Pos               (12U)
#define RTC_TR_MNT_Msk               (0x7U << RTC_TR_MNT_Pos)                  /*!< 0x00007000 */
#define RTC_TR_MNT                   RTC_TR_MNT_Msk
#define RTC_TR_MNT_0                 (0x1U << RTC_TR_MNT_Pos)                  /*!< 0x00001000 */
#define RTC_TR_MNT_1                 (0x2U << RTC_TR_MNT_Pos)                  /*!< 0x00002000 */
#define RTC_TR_MNT_2                 (0x4U << RTC_TR_MNT_Pos)                  /*!< 0x00004000 */
#define RTC_TR_MNU_Pos               (8U)
#define RTC_TR_MNU_Msk               (0xFU << RTC_TR_MNU_Pos)                  /*!< 0x00000F00 */
#define RTC_TR_MNU                   RTC_TR_MNU_Msk
#define RTC_TR_MNU_0                 (0x1U << RTC_TR_MNU_Pos)                  /*!< 0x00000100 */
#define RTC_TR_MNU_1                 (0x2U << RTC_TR_MNU_Pos)                  /*!< 0x00000200 */
#define RTC_TR_MNU_2                 (0x4U << RTC_TR_MNU_Pos)                  /*!< 0x00000400 */
#define RTC_TR_MNU_3                 (0x8U << RTC_TR_MNU_Pos)                  /*!< 0x00000800 */
#define RTC_TR_ST_Pos                (4U)
#define RTC_TR_ST_Msk                (0x7U << RTC_TR_ST_Pos)                   /*!< 0x00000070 */
#define RTC_TR_ST                    RTC_TR_ST_Msk
#define RTC_TR_ST_0                  (0x1U << RTC_TR_ST_Pos)                   /*!< 0x00000010 */
#define RTC_TR_ST_1                  (0x2U << RTC_TR_ST_Pos)                   /*!< 0x00000020 */
#define RTC_TR_ST_2                  (0x4U << RTC_TR_ST_Pos)                   /*!< 0x00000040 */
#define RTC_TR_SU_Pos                (0U)
#define RTC_TR_SU_Msk                (0xFU << RTC_TR_SU_Pos)                   /*!< 0x0000000F */
#define RTC_TR_SU                    RTC_TR_SU_Msk
#define RTC_TR_SU_0                  (0x1U << RTC_TR_SU_Pos)                   /*!< 0x00000001 */
#define RTC_TR_SU_1                  (0x2U << RTC_TR_SU_Pos)                   /*!< 0x00000002 */
#define RTC_TR_SU_2                  (0x4U << RTC_TR_SU_Pos)                   /*!< 0x00000004 */
#define RTC_TR_SU_3                  (0x8U << RTC_TR_SU_Pos)                   /*!< 0x00000008 */

/********************  Bits definition for RTC_DR register  *******************/
#define RTC_DR_YT_Pos                (20U)
#define RTC_DR_YT_Msk                (0xFU << RTC_DR_YT_Pos)                   /*!< 0x00F00000 */
#define RTC_DR_YT                    RTC_DR_YT_Msk
#define RTC_DR_YT_0                  (0x1U << RTC_DR_YT_Pos)                   /*!< 0x00100000 */
#define RTC_DR_YT_1                  (0x2U << RTC_DR_YT_Pos)                   /*!< 0x00200000 */
#define RTC_DR_YT_2                  (0x4U << RTC_DR_YT_Pos)                   /*!< 0x00400000 */
#define RTC_DR_YT_3                  (0x8U << RTC_DR_YT_Pos)                   /*!< 0x00800000 */
#define RTC_DR_YU_Pos                (16U)
#define RTC_DR_YU_Msk                (0xFU << RTC_DR_YU_Pos)                   /*!< 0x000F0000 */
#define RTC_DR_YU                    RTC_DR_YU_Msk
#define RTC_DR_YU_0                  (0x1U << RTC_DR_YU_Pos)                   /*!< 0x00010000 */
#define RTC_DR_YU_1                  (0x2U << RTC_DR_YU_Pos)                   /*!< 0x00020000 */
#define RTC_DR_YU_2                  (0x4U << RTC_DR_YU_Pos)                   /*!< 0x00040000 */
#define RTC_DR_YU_3                  (0x8U << RTC_DR_YU_Pos)                   /*!< 0x00080000 */
#define RTC_DR_WDU_Pos               (13U)
#define RTC_DR_WDU_Msk               (0x7U << RTC_DR_WDU_Pos)                  /*!< 0x0000E000 */
#define RTC_DR_WDU                   RTC_DR_WDU_Msk
#define RTC_DR_WDU_0                 (0x1U << RTC_DR_WDU_Pos)                  /*!< 0x00002000 */
#define RTC_DR_WDU_1                 (0x2U << RTC_DR_WDU_Pos)                  /*!< 0x00004000 */
#define RTC_DR_WDU_2                 (0x4U << RTC_DR_WDU_Pos)                  /*!< 0x00008000 */
#define RTC_DR_MT_Pos                (12U)
#define RTC_DR_MT_Msk                (0x1U << RTC_DR_MT_Pos)                   /*!< 0x00001000 */
#define RTC_DR_MT                    RTC_DR_MT_Msk
#define RTC_DR_MU_Pos                (8U)
#define RTC_DR_MU_Msk                (0xFU << RTC_DR_MU_Pos)                   /*!< 0x00000F00 */
#define RTC_DR_MU                    RTC_DR_MU_Msk
#define RTC_DR_MU_0                  (0x1U << RTC_DR_MU_Pos)                   /*!< 0x00000100 */
#define RTC_DR_MU_1                  (0x2U << RTC_DR_MU_Pos)                   /*!< 0x00000200 */
#define RTC_DR_MU_2                  (0x4U << RTC_DR_MU_Pos)                   /*!< 0x00000400 */
#define RTC_DR_MU_3                  (0x8U << RTC_DR_MU_Pos)                   /*!< 0x00000800 */
#define RTC_DR_DT_Pos                (4U)
#define RTC_DR_DT_Msk                (0x3U << RTC_DR_DT_Pos)                   /*!< 0x00000030 */
#define RTC_DR_DT                    RTC_DR_DT_Msk
#define RTC_DR_DT_0                  (0x1U << RTC_DR_DT_Pos)                   /*!< 0x00000010 */
#define RTC_DR_DT_1                  (0x2U << RTC_DR_DT_Pos)                   /*!< 0x00000020 */
#define RTC_DR_DU_Pos                (0U)
#define RTC_DR_DU_Msk                (0xFU << RTC_DR_DU_Pos)                   /*!< 0x0000000F */
#define RTC_DR_DU                    RTC_DR_DU_Msk
#define RTC_DR_DU_0                  (0x1U << RTC_DR_DU_Pos)                   /*!< 0x00000001 */
#define RTC_DR_DU_1                  (0x2U << RTC_DR_DU_Pos)                   /*!< 0x00000002 */
#define RTC_DR_DU_2                  (0x4U << RTC_DR_DU_Pos)                   /*!< 0x00000004 */
#define RTC_DR_DU_3                  (0x8U << RTC_DR_DU_Pos)                   /*!< 0x00000008 */

/********************  Bits definition for RTC_CR register  *******************/
#define RTC_CR_COE_Pos               (23U)
#define RTC_CR_COE_Msk               (0x1U << RTC_CR_COE_Pos)                  /*!< 0x00800000 */
#define RTC_CR_COE                   RTC_CR_COE_Msk
#define RTC_CR_OSEL_Pos              (21U)
#define RTC_CR_OSEL_Msk              (0x3U << RTC_CR_OSEL_Pos)                 /*!< 0x00600000 */
#define RTC_CR_OSEL                  RTC_CR_OSEL_Msk
#define RTC_CR_OSEL_0                (0x1U << RTC_CR_OSEL_Pos)                 /*!< 0x00200000 */
#define RTC_CR_OSEL_1                (0x2U << RTC_CR_OSEL_Pos)                 /*!< 0x00400000 */
#define RTC_CR_POL_Pos               (20U)
#define RTC_CR_POL_Msk               (0x1U << RTC_CR_POL_Pos)                  /*!< 0x00100000 */
#define RTC_CR_POL                   RTC_CR_POL_Msk
#define RTC_CR_COSEL_Pos             (19U)
#define RTC_CR_COSEL_Msk             (0x1U << RTC_CR_COSEL_Pos)                /*!< 0x00080000 */
#define RTC_CR_COSEL                 RTC_CR_COSEL_Msk
#define RTC_CR_BKP_Pos               (18U)
#define RTC_CR_BKP_Msk               (0x1U << RTC_CR_BKP_Pos)                  /*!< 0x00040000 */
#define RTC_CR_BKP                   RTC_CR_BKP_Msk
#define RTC_CR_SUB1H_Pos             (17U)
#define RTC_CR_SUB1H_Msk             (0x1U << RTC_CR_SUB1H_Pos)                /*!< 0x00020000 */
#define RTC_CR_SUB1H                 RTC_CR_SUB1H_Msk
#define RTC_CR_ADD1H_Pos             (16U)
#define RTC_CR_ADD1H_Msk             (0x1U << RTC_CR_ADD1H_Pos)                /*!< 0x00010000 */
#define RTC_CR_ADD1H                 RTC_CR_ADD1H_Msk
#define RTC_CR_TSIE_Pos              (15U)
#define RTC_CR_TSIE_Msk              (0x1U << RTC_CR_TSIE_Pos)                 /*!< 0x00008000 */
#define RTC_CR_TSIE                  RTC_CR_TSIE_Msk
#define RTC_CR_WUTIE_Pos             (14U)
#define RTC_CR_WUTIE_Msk             (0x1U << RTC_CR_WUTIE_Pos)                /*!< 0x00004000 */
#define RTC_CR_WUTIE                 RTC_CR_WUTIE_Msk
#define RTC_CR_ALRBIE_Pos            (13U)
#define RTC_CR_ALRBIE_Msk            (0x1U << RTC_CR_ALRBIE_Pos)               /*!< 0x00002000 */
#define RTC_CR_ALRBIE                RTC_CR_ALRBIE_Msk
#define RTC_CR_ALRAIE_Pos            (12U)
#define RTC_CR_ALRAIE_Msk            (0x1U << RTC_CR_ALRAIE_Pos)               /*!< 0x00001000 */
#define RTC_CR_ALRAIE                RTC_CR_ALRAIE_Msk
#define RTC_CR_TSE_Pos               (11U)
#define RTC_CR_TSE_Msk               (0x1U << RTC_CR_TSE_Pos)                  /*!< 0x00000800 */
#define RTC_CR_TSE                   RTC_CR_TSE_Msk
#define RTC_CR_WUTE_Pos              (10U)
#define RTC_CR_WUTE_Msk              (0x1U << RTC_CR_WUTE_Pos)                 /*!< 0x00000400 */
#define RTC_CR_WUTE                  RTC_CR_WUTE_Msk
#define RTC_CR_ALRBE_Pos             (9U)
#define RTC_CR_ALRBE_Msk             (0x1U << RTC_CR_ALRBE_Pos)                /*!< 0x00000200 */
#define RTC_CR_ALRBE                 RTC_CR_ALRBE_Msk
#define RTC_CR_ALRAE_Pos             (8U)
#define RTC_CR_ALRAE_Msk             (0x1U << RTC_CR_ALRAE_Pos)                /*!< 0x00000100 */
#define RTC_CR_ALRAE                 RTC_CR_ALRAE_Msk
#define RTC_CR_FMT_Pos               (6U)
#define RTC_CR_FMT_Msk               (0x1U << RTC_CR_FMT_Pos)                  /*!< 0x00000040 */
#define RTC_CR_FMT                   RTC_CR_FMT_Msk
#define RTC_CR_BYPSHAD_Pos           (5U)
#define RTC_CR_BYPSHAD_Msk           (0x1U << RTC_CR_BYPSHAD_Pos)              /*!< 0x00000020 */
#define RTC_CR_BYPSHAD               RTC_CR_BYPSHAD_Msk
#define RTC_CR_REFCKON_Pos           (4U)
#define RTC_CR_REFCKON_Msk           (0x1U << RTC_CR_REFCKON_Pos)              /*!< 0x00000010 */
#define RTC_CR_REFCKON               RTC_CR_REFCKON_Msk
#define RTC_CR_TSEDGE_Pos            (3U)
#define RTC_CR_TSEDGE_Msk            (0x1U << RTC_CR_TSEDGE_Pos)               /*!< 0x00000008 */
#define RTC_CR_TSEDGE                RTC_CR_TSEDGE_Msk
#define RTC_CR_WUCKSEL_Pos           (0U)
#define RTC_CR_WUCKSEL_Msk           (0x7U << RTC_CR_WUCKSEL_Pos)              /*!< 0x00000007 */
#define RTC_CR_WUCKSEL               RTC_CR_WUCKSEL_Msk
#define RTC_CR_WUCKSEL_0             (0x1U << RTC_CR_WUCKSEL_Pos)              /*!< 0x00000001 */
#define RTC_CR_WUCKSEL_1             (0x2U << RTC_CR_WUCKSEL_Pos)              /*!< 0x00000002 */
#define RTC_CR_WUCKSEL_2             (0x4U << RTC_CR_WUCKSEL_Pos)              /*!< 0x00000004 */

/* Legacy defines */
#define RTC_CR_BCK_Pos               RTC_CR_BKP_Pos
#define RTC_CR_BCK_Msk               RTC_CR_BKP_Msk
#define RTC_CR_BCK                   RTC_CR_BKP

/********************  Bits definition for RTC_ISR register  ******************/
#define RTC_ISR_RECALPF_Pos          (16U)
#define RTC_ISR_RECALPF_Msk          (0x1U << RTC_ISR_RECALPF_Pos)             /*!< 0x00010000 */
#define RTC_ISR_RECALPF              RTC_ISR_RECALPF_Msk
#define RTC_ISR_TAMP2F_Pos           (14U)
#define RTC_ISR_TAMP2F_Msk           (0x1U << RTC_ISR_TAMP2F_Pos)              /*!< 0x00004000 */
#define RTC_ISR_TAMP2F               RTC_ISR_TAMP2F_Msk
#define RTC_ISR_TAMP1F_Pos           (13U)
#define RTC_ISR_TAMP1F_Msk           (0x1U << RTC_ISR_TAMP1F_Pos)              /*!< 0x00002000 */
#define RTC_ISR_TAMP1F               RTC_ISR_TAMP1F_Msk
#define RTC_ISR_TSOVF_Pos            (12U)
#define RTC_ISR_TSOVF_Msk            (0x1U << RTC_ISR_TSOVF_Pos)               /*!< 0x00001000 */
#define RTC_ISR_TSOVF                RTC_ISR_TSOVF_Msk
#define RTC_ISR_TSF_Pos              (11U)
#define RTC_ISR_TSF_Msk              (0x1U << RTC_ISR_TSF_Pos)                 /*!< 0x00000800 */
#define RTC_ISR_TSF                  RTC_ISR_TSF_Msk
#define RTC_ISR_WUTF_Pos             (10U)
#define RTC_ISR_WUTF_Msk             (0x1U << RTC_ISR_WUTF_Pos)                /*!< 0x00000400 */
#define RTC_ISR_WUTF                 RTC_ISR_WUTF_Msk
#define RTC_ISR_ALRBF_Pos            (9U)
#define RTC_ISR_ALRBF_Msk            (0x1U << RTC_ISR_ALRBF_Pos)               /*!< 0x00000200 */
#define RTC_ISR_ALRBF                RTC_ISR_ALRBF_Msk
#define RTC_ISR_ALRAF_Pos            (8U)
#define RTC_ISR_ALRAF_Msk            (0x1U << RTC_ISR_ALRAF_Pos)               /*!< 0x00000100 */
#define RTC_ISR_ALRAF                RTC_ISR_ALRAF_Msk
#define RTC_ISR_INIT_Pos             (7U)
#define RTC_ISR_INIT_Msk             (0x1U << RTC_ISR_INIT_Pos)                /*!< 0x00000080 */
#define RTC_ISR_INIT                 RTC_ISR_INIT_Msk
#define RTC_ISR_INITF_Pos            (6U)
#define RTC_ISR_INITF_Msk            (0x1U << RTC_ISR_INITF_Pos)               /*!< 0x00000040 */
#define RTC_ISR_INITF                RTC_ISR_INITF_Msk
#define RTC_ISR_RSF_Pos              (5U)
#define RTC_ISR_RSF_Msk              (0x1U << RTC_ISR_RSF_Pos)                 /*!< 0x00000020 */
#define RTC_ISR_RSF                  RTC_ISR_RSF_Msk
#define RTC_ISR_INITS_Pos            (4U)
#define RTC_ISR_INITS_Msk            (0x1U << RTC_ISR_INITS_Pos)               /*!< 0x00000010 */
#define RTC_ISR_INITS                RTC_ISR_INITS_Msk
#define RTC_ISR_SHPF_Pos             (3U)
#define RTC_ISR_SHPF_Msk             (0x1U << RTC_ISR_SHPF_Pos)                /*!< 0x00000008 */
#define RTC_ISR_SHPF                 RTC_ISR_SHPF_Msk
#define RTC_ISR_WUTWF_Pos            (2U)
#define RTC_ISR_WUTWF_Msk            (0x1U << RTC_ISR_WUTWF_Pos)               /*!< 0x00000004 */
#define RTC_ISR_WUTWF                RTC_ISR_WUTWF_Msk
#define RTC_ISR_ALRBWF_Pos           (1U)
#define RTC_ISR_ALRBWF_Msk           (0x1U << RTC_ISR_ALRBWF_Pos)              /*!< 0x00000002 */
#define RTC_ISR_ALRBWF               RTC_ISR_ALRBWF_Msk
#define RTC_ISR_ALRAWF_Pos           (0U)
#define RTC_ISR_ALRAWF_Msk           (0x1U << RTC_ISR_ALRAWF_Pos)              /*!< 0x00000001 */
#define RTC_ISR_ALRAWF               RTC_ISR_ALRAWF_Msk

/********************  Bits definition for RTC_PRER register  *****************/
#define RTC_PRER_PREDIV_A_Pos        (16U)
#define RTC_PRER_PREDIV_A_Msk        (0x7FU << RTC_PRER_PREDIV_A_Pos)          /*!< 0x007F0000 */
#define RTC_PRER_PREDIV_A            RTC_PRER_PREDIV_A_Msk
#define RTC_PRER_PREDIV_S_Pos        (0U)
#define RTC_PRER_PREDIV_S_Msk        (0x7FFFU << RTC_PRER_PREDIV_S_Pos)        /*!< 0x00007FFF */
#define RTC_PRER_PREDIV_S            RTC_PRER_PREDIV_S_Msk

/********************  Bits definition for RTC_WUTR register  *****************/
#define RTC_WUTR_WUT_Pos             (0U)
#define RTC_WUTR_WUT_Msk             (0xFFFFU << RTC_WUTR_WUT_Pos)             /*!< 0x0000FFFF */
#define RTC_WUTR_WUT                 RTC_WUTR_WUT_Msk

/********************  Bits definition for RTC_ALRMAR register  ***************/
#define RTC_ALRMAR_MSK4_Pos          (31U)
#define RTC_ALRMAR_MSK4_Msk          (0x1U << RTC_ALRMAR_MSK4_Pos)             /*!< 0x80000000 */
#define RTC_ALRMAR_MSK4              RTC_ALRMAR_MSK4_Msk
#define RTC_ALRMAR_WDSEL_Pos         (30U)
#define RTC_ALRMAR_WDSEL_Msk         (0x1U << RTC_ALRMAR_WDSEL_Pos)            /*!< 0x40000000 */
#define RTC_ALRMAR_WDSEL             RTC_ALRMAR_WDSEL_Msk
#define RTC_ALRMAR_DT_Pos            (28U)
#define RTC_ALRMAR_DT_Msk            (0x3U << RTC_ALRMAR_DT_Pos)               /*!< 0x30000000 */
#define RTC_ALRMAR_DT                RTC_ALRMAR_DT_Msk
#define RTC_ALRMAR_DT_0              (0x1U << RTC_ALRMAR_DT_Pos)               /*!< 0x10000000 */
#define RTC_ALRMAR_DT_1              (0x2U << RTC_ALRMAR_DT_Pos)               /*!< 0x20000000 */
#define RTC_ALRMAR_DU_Pos            (24U)
#define RTC_ALRMAR_DU_Msk            (0xFU << RTC_ALRMAR_DU_Pos)               /*!< 0x0F000000 */
#define RTC_ALRMAR_DU                RTC_ALRMAR_DU_Msk
#define RTC_ALRMAR_DU_0              (0x1U << RTC_ALRMAR_DU_Pos)               /*!< 0x01000000 */
#define RTC_ALRMAR_DU_1              (0x2U << RTC_ALRMAR_DU_Pos)               /*!< 0x02000000 */
#define RTC_ALRMAR_DU_2              (0x4U << RTC_ALRMAR_DU_Pos)               /*!< 0x04000000 */
#define RTC_ALRMAR_DU_3              (0x8U << RTC_ALRMAR_DU_Pos)               /*!< 0x08000000 */
#define RTC_ALRMAR_MSK3_Pos          (23U)
#define RTC_ALRMAR_MSK3_Msk          (0x1U << RTC_ALRMAR_MSK3_Pos)             /*!< 0x00800000 */
#define RTC_ALRMAR_MSK3              RTC_ALRMAR_MSK3_Msk
#define RTC_ALRMAR_PM_Pos            (22U)
#define RTC_ALRMAR_PM_Msk            (0x1U << RTC_ALRMAR_PM_Pos)               /*!< 0x00400000 */
#define RTC_ALRMAR_PM                RTC_ALRMAR_PM_Msk
#define RTC_ALRMAR_HT_Pos            (20U)
#define RTC_ALRMAR_HT_Msk            (0x3U << RTC_ALRMAR_HT_Pos)               /*!< 0x00300000 */
#define RTC_ALRMAR_HT                RTC_ALRMAR_HT_Msk
#define RTC_ALRMAR_HT_0              (0x1U << RTC_ALRMAR_HT_Pos)               /*!< 0x00100000 */
#define RTC_ALRMAR_HT_1              (0x2U << RTC_ALRMAR_HT_Pos)               /*!< 0x00200000 */
#define RTC_ALRMAR_HU_Pos            (16U)
#define RTC_ALRMAR_HU_Msk            (0xFU << RTC_ALRMAR_HU_Pos)               /*!< 0x000F0000 */
#define RTC_ALRMAR_HU                RTC_ALRMAR_HU_Msk
#define RTC_ALRMAR_HU_0              (0x1U << RTC_ALRMAR_HU_Pos)               /*!< 0x00010000 */
#define RTC_ALRMAR_HU_1              (0x2U << RTC_ALRMAR_HU_Pos)               /*!< 0x00020000 */
#define RTC_ALRMAR_HU_2              (0x4U << RTC_ALRMAR_HU_Pos)               /*!< 0x00040000 */
#define RTC_ALRMAR_HU_3              (0x8U << RTC_ALRMAR_HU_Pos)               /*!< 0x00080000 */
#define RTC_ALRMAR_MSK2_Pos          (15U)
#define RTC_ALRMAR_MSK2_Msk          (0x1U << RTC_ALRMAR_MSK2_Pos)             /*!< 0x00008000 */
#define RTC_ALRMAR_MSK2              RTC_ALRMAR_MSK2_Msk
#define RTC_ALRMAR_MNT_Pos           (12U)
#define RTC_ALRMAR_MNT_Msk           (0x7U << RTC_ALRMAR_MNT_Pos)              /*!< 0x00007000 */
#define RTC_ALRMAR_MNT               RTC_ALRMAR_MNT_Msk
#define RTC_ALRMAR_MNT_0             (0x1U << RTC_ALRMAR_MNT_Pos)              /*!< 0x00001000 */
#define RTC_ALRMAR_MNT_1             (0x2U << RTC_ALRMAR_MNT_Pos)              /*!< 0x00002000 */
#define RTC_ALRMAR_MNT_2             (0x4U << RTC_ALRMAR_MNT_Pos)              /*!< 0x00004000 */
#define RTC_ALRMAR_MNU_Pos           (8U)
#define RTC_ALRMAR_MNU_Msk           (0xFU << RTC_ALRMAR_MNU_Pos)              /*!< 0x00000F00 */
#define RTC_ALRMAR_MNU               RTC_ALRMAR_MNU_Msk
#define RTC_ALRMAR_MNU_0             (0x1U << RTC_ALRMAR_MNU_Pos)              /*!< 0x00000100 */
#define RTC_ALRMAR_MNU_1             (0x2U << RTC_ALRMAR_MNU_Pos)              /*!< 0x00000200 */
#define RTC_ALRMAR_MNU_2             (0x4U << RTC_ALRMAR_MNU_Pos)              /*!< 0x00000400 */
#define RTC_ALRMAR_MNU_3             (0x8U << RTC_ALRMAR_MNU_Pos)              /*!< 0x00000800 */
#define RTC_ALRMAR_MSK1_Pos          (7U)
#define RTC_ALRMAR_MSK1_Msk          (0x1U << RTC_ALRMAR_MSK1_Pos)             /*!< 0x00000080 */
#define RTC_ALRMAR_MSK1              RTC_ALRMAR_MSK1_Msk
#define RTC_ALRMAR_ST_Pos            (4U)
#define RTC_ALRMAR_ST_Msk            (0x7U << RTC_ALRMAR_ST_Pos)               /*!< 0x00000070 */
#define RTC_ALRMAR_ST                RTC_ALRMAR_ST_Msk
#define RTC_ALRMAR_ST_0              (0x1U << RTC_ALRMAR_ST_Pos)               /*!< 0x00000010 */
#define RTC_ALRMAR_ST_1              (0x2U << RTC_ALRMAR_ST_Pos)               /*!< 0x00000020 */
#define RTC_ALRMAR_ST_2              (0x4U << RTC_ALRMAR_ST_Pos)               /*!< 0x00000040 */
#define RTC_ALRMAR_SU_Pos            (0U)
#define RTC_ALRMAR_SU_Msk            (0xFU << RTC_ALRMAR_SU_Pos)               /*!< 0x0000000F */
#define RTC_ALRMAR_SU                RTC_ALRMAR_SU_Msk
#define RTC_ALRMAR_SU_0              (0x1U << RTC_ALRMAR_SU_Pos)               /*!< 0x00000001 */
#define RTC_ALRMAR_SU_1              (0x2U << RTC_ALRMAR_SU_Pos)               /*!< 0x00000002 */
#define RTC_ALRMAR_SU_2              (0x4U << RTC_ALRMAR_SU_Pos)               /*!< 0x00000004 */
#define RTC_ALRMAR_SU_3              (0x8U << RTC_ALRMAR_SU_Pos)               /*!< 0x00000008 */

/********************  Bits definition for RTC_ALRMBR register  ***************/
#define RTC_ALRMBR_MSK4_Pos          (31U)
#define RTC_ALRMBR_MSK4_Msk          (0x1U << RTC_ALRMBR_MSK4_Pos)             /*!< 0x80000000 */
#define RTC_ALRMBR_MSK4              RTC_ALRMBR_MSK4_Msk
#define RTC_ALRMBR_WDSEL_Pos         (30U)
#define RTC_ALRMBR_WDSEL_Msk         (0x1U << RTC_ALRMBR_WDSEL_Pos)            /*!< 0x40000000 */
#define RTC_ALRMBR_WDSEL             RTC_ALRMBR_WDSEL_Msk
#define RTC_ALRMBR_DT_Pos            (28U)
#define RTC_ALRMBR_DT_Msk            (0x3U << RTC_ALRMBR_DT_Pos)               /*!< 0x30000000 */
#define RTC_ALRMBR_DT                RTC_ALRMBR_DT_Msk
#define RTC_ALRMBR_DT_0              (0x1U << RTC_ALRMBR_DT_Pos)               /*!< 0x10000000 */
#define RTC_ALRMBR_DT_1              (0x2U << RTC_ALRMBR_DT_Pos)               /*!< 0x20000000 */
#define RTC_ALRMBR_DU_Pos            (24U)
#define RTC_ALRMBR_DU_Msk            (0xFU << RTC_ALRMBR_DU_Pos)               /*!< 0x0F000000 */
#define RTC_ALRMBR_DU                RTC_ALRMBR_DU_Msk
#define RTC_ALRMBR_DU_0              (0x1U << RTC_ALRMBR_DU_Pos)               /*!< 0x01000000 */
#define RTC_ALRMBR_DU_1              (0x2U << RTC_ALRMBR_DU_Pos)               /*!< 0x02000000 */
#define RTC_ALRMBR_DU_2              (0x4U << RTC_ALRMBR_DU_Pos)               /*!< 0x04000000 */
#define RTC_ALRMBR_DU_3              (0x8U << RTC_ALRMBR_DU_Pos)               /*!< 0x08000000 */
#define RTC_ALRMBR_MSK3_Pos          (23U)
#define RTC_ALRMBR_MSK3_Msk          (0x1U << RTC_ALRMBR_MSK3_Pos)             /*!< 0x00800000 */
#define RTC_ALRMBR_MSK3              RTC_ALRMBR_MSK3_Msk
#define RTC_ALRMBR_PM_Pos            (22U)
#define RTC_ALRMBR_PM_Msk            (0x1U << RTC_ALRMBR_PM_Pos)               /*!< 0x00400000 */
#define RTC_ALRMBR_PM                RTC_ALRMBR_PM_Msk
#define RTC_ALRMBR_HT_Pos            (20U)
#define RTC_ALRMBR_HT_Msk            (0x3U << RTC_ALRMBR_HT_Pos)               /*!< 0x00300000 */
#define RTC_ALRMBR_HT                RTC_ALRMBR_HT_Msk
#define RTC_ALRMBR_HT_0              (0x1U << RTC_ALRMBR_HT_Pos)               /*!< 0x00100000 */
#define RTC_ALRMBR_HT_1              (0x2U << RTC_ALRMBR_HT_Pos)               /*!< 0x00200000 */
#define RTC_ALRMBR_HU_Pos            (16U)
#define RTC_ALRMBR_HU_Msk            (0xFU << RTC_ALRMBR_HU_Pos)               /*!< 0x000F0000 */
#define RTC_ALRMBR_HU                RTC_ALRMBR_HU_Msk
#define RTC_ALRMBR_HU_0              (0x1U << RTC_ALRMBR_HU_Pos)               /*!< 0x00010000 */
#define RTC_ALRMBR_HU_1              (0x2U << RTC_ALRMBR_HU_Pos)               /*!< 0x00020000 */
#define RTC_ALRMBR_HU_2              (0x4U << RTC_ALRMBR_HU_Pos)               /*!< 0x00040000 */
#define RTC_ALRMBR_HU_3              (0x8U << RTC_ALRMBR_HU_Pos)               /*!< 0x00080000 */
#define RTC_ALRMBR_MSK2_Pos          (15U)
#define RTC_ALRMBR_MSK2_Msk          (0x1U << RTC_ALRMBR_MSK2_Pos)             /*!< 0x00008000 */
#define RTC_ALRMBR_MSK2              RTC_ALRMBR_MSK2_Msk
#define RTC_ALRMBR_MNT_Pos           (12U)
#define RTC_ALRMBR_MNT_Msk           (0x7U << RTC_ALRMBR_MNT_Pos)              /*!< 0x00007000 */
#define RTC_ALRMBR_MNT               RTC_ALRMBR_MNT_Msk
#define RTC_ALRMBR_MNT_0             (0x1U << RTC_ALRMBR_MNT_Pos)              /*!< 0x00001000 */
#define RTC_ALRMBR_MNT_1             (0x2U << RTC_ALRMBR_MNT_Pos)              /*!< 0x00002000 */
#define RTC_ALRMBR_MNT_2             (0x4U << RTC_ALRMBR_MNT_Pos)              /*!< 0x00004000 */
#define RTC_ALRMBR_MNU_Pos           (8U)
#define RTC_ALRMBR_MNU_Msk           (0xFU << RTC_ALRMBR_MNU_Pos)              /*!< 0x00000F00 */
#define RTC_ALRMBR_MNU               RTC_ALRMBR_MNU_Msk
#define RTC_ALRMBR_MNU_0             (0x1U << RTC_ALRMBR_MNU_Pos)              /*!< 0x00000100 */
#define RTC_ALRMBR_MNU_1             (0x2U << RTC_ALRMBR_MNU_Pos)              /*!< 0x00000200 */
#define RTC_ALRMBR_MNU_2             (0x4U << RTC_ALRMBR_MNU_Pos)              /*!< 0x00000400 */
#define RTC_ALRMBR_MNU_3             (0x8U << RTC_ALRMBR_MNU_Pos)              /*!< 0x00000800 */
#define RTC_ALRMBR_MSK1_Pos          (7U)
#define RTC_ALRMBR_MSK1_Msk          (0x1U << RTC_ALRMBR_MSK1_Pos)             /*!< 0x00000080 */
#define RTC_ALRMBR_MSK1              RTC_ALRMBR_MSK1_Msk
#define RTC_ALRMBR_ST_Pos            (4U)
#define RTC_ALRMBR_ST_Msk            (0x7U << RTC_ALRMBR_ST_Pos)               /*!< 0x00000070 */
#define RTC_ALRMBR_ST                RTC_ALRMBR_ST_Msk
#define RTC_ALRMBR_ST_0              (0x1U << RTC_ALRMBR_ST_Pos)               /*!< 0x00000010 */
#define RTC_ALRMBR_ST_1              (0x2U << RTC_ALRMBR_ST_Pos)               /*!< 0x00000020 */
#define RTC_ALRMBR_ST_2              (0x4U << RTC_ALRMBR_ST_Pos)               /*!< 0x00000040 */
#define RTC_ALRMBR_SU_Pos            (0U)
#define RTC_ALRMBR_SU_Msk            (0xFU << RTC_ALRMBR_SU_Pos)               /*!< 0x0000000F */
#define RTC_ALRMBR_SU                RTC_ALRMBR_SU_Msk
#define RTC_ALRMBR_SU_0              (0x1U << RTC_ALRMBR_SU_Pos)               /*!< 0x00000001 */
#define RTC_ALRMBR_SU_1              (0x2U << RTC_ALRMBR_SU_Pos)               /*!< 0x00000002 */
#define RTC_ALRMBR_SU_2              (0x4U << RTC_ALRMBR_SU_Pos)               /*!< 0x00000004 */
#define RTC_ALRMBR_SU_3              (0x8U << RTC_ALRMBR_SU_Pos)               /*!< 0x00000008 */

/********************  Bits definition for RTC_WPR register  ******************/
#define RTC_WPR_KEY_Pos              (0U)
#define RTC_WPR_KEY_Msk              (0xFFU << RTC_WPR_KEY_Pos)                /*!< 0x000000FF */
#define RTC_WPR_KEY                  RTC_WPR_KEY_Msk

/********************  Bits definition for RTC_SSR register  ******************/
#define RTC_SSR_SS_Pos               (0U)
#define RTC_SSR_SS_Msk               (0xFFFFU << RTC_SSR_SS_Pos)               /*!< 0x0000FFFF */
#define RTC_SSR_SS                   RTC_SSR_SS_Msk

/********************  Bits definition for RTC_SHIFTR register  ***************/
#define RTC_SHIFTR_SUBFS_Pos         (0U)
#define RTC_SHIFTR_SUBFS_Msk         (0x7FFFU << RTC_SHIFTR_SUBFS_Pos)         /*!< 0x00007FFF */
#define RTC_SHIFTR_SUBFS             RTC_SHIFTR_SUBFS_Msk
#define RTC_SHIFTR_ADD1S_Pos         (31U)
#define RTC_SHIFTR_ADD1S_Msk         (0x1U << RTC_SHIFTR_ADD1S_Pos)            /*!< 0x80000000 */
#define RTC_SHIFTR_ADD1S             RTC_SHIFTR_ADD1S_Msk

/********************  Bits definition for RTC_TSTR register  *****************/
#define RTC_TSTR_PM_Pos              (22U)
#define RTC_TSTR_PM_Msk              (0x1U << RTC_TSTR_PM_Pos)                 /*!< 0x00400000 */
#define RTC_TSTR_PM                  RTC_TSTR_PM_Msk
#define RTC_TSTR_HT_Pos              (20U)
#define RTC_TSTR_HT_Msk              (0x3U << RTC_TSTR_HT_Pos)                 /*!< 0x00300000 */
#define RTC_TSTR_HT                  RTC_TSTR_HT_Msk
#define RTC_TSTR_HT_0                (0x1U << RTC_TSTR_HT_Pos)                 /*!< 0x00100000 */
#define RTC_TSTR_HT_1                (0x2U << RTC_TSTR_HT_Pos)                 /*!< 0x00200000 */
#define RTC_TSTR_HU_Pos              (16U)
#define RTC_TSTR_HU_Msk              (0xFU << RTC_TSTR_HU_Pos)                 /*!< 0x000F0000 */
#define RTC_TSTR_HU                  RTC_TSTR_HU_Msk
#define RTC_TSTR_HU_0                (0x1U << RTC_TSTR_HU_Pos)                 /*!< 0x00010000 */
#define RTC_TSTR_HU_1                (0x2U << RTC_TSTR_HU_Pos)                 /*!< 0x00020000 */
#define RTC_TSTR_HU_2                (0x4U << RTC_TSTR_HU_Pos)                 /*!< 0x00040000 */
#define RTC_TSTR_HU_3                (0x8U << RTC_TSTR_HU_Pos)                 /*!< 0x00080000 */
#define RTC_TSTR_MNT_Pos             (12U)
#define RTC_TSTR_MNT_Msk             (0x7U << RTC_TSTR_MNT_Pos)                /*!< 0x00007000 */
#define RTC_TSTR_MNT                 RTC_TSTR_MNT_Msk
#define RTC_TSTR_MNT_0               (0x1U << RTC_TSTR_MNT_Pos)                /*!< 0x00001000 */
#define RTC_TSTR_MNT_1               (0x2U << RTC_TSTR_MNT_Pos)                /*!< 0x00002000 */
#define RTC_TSTR_MNT_2               (0x4U << RTC_TSTR_MNT_Pos)                /*!< 0x00004000 */
#define RTC_TSTR_MNU_Pos             (8U)
#define RTC_TSTR_MNU_Msk             (0xFU << RTC_TSTR_MNU_Pos)                /*!< 0x00000F00 */
#define RTC_TSTR_MNU                 RTC_TSTR_MNU_Msk
#define RTC_TSTR_MNU_0               (0x1U << RTC_TSTR_MNU_Pos)                /*!< 0x00000100 */
#define RTC_TSTR_MNU_1               (0x2U << RTC_TSTR_MNU_Pos)                /*!< 0x00000200 */
#define RTC_TSTR_MNU_2               (0x4U << RTC_TSTR_MNU_Pos)                /*!< 0x00000400 */
#define RTC_TSTR_MNU_3               (0x8U << RTC_TSTR_MNU_Pos)                /*!< 0x00000800 */
#define RTC_TSTR_ST_Pos              (4U)
#define RTC_TSTR_ST_Msk              (0x7U << RTC_TSTR_ST_Pos)                 /*!< 0x00000070 */
#define RTC_TSTR_ST                  RTC_TSTR_ST_Msk
#define RTC_TSTR_ST_0                (0x1U << RTC_TSTR_ST_Pos)                 /*!< 0x00000010 */
#define RTC_TSTR_ST_1                (0x2U << RTC_TSTR_ST_Pos)                 /*!< 0x00000020 */
#define RTC_TSTR_ST_2                (0x4U << RTC_TSTR_ST_Pos)                 /*!< 0x00000040 */
#define RTC_TSTR_SU_Pos              (0U)
#define RTC_TSTR_SU_Msk              (0xFU << RTC_TSTR_SU_Pos)                 /*!< 0x0000000F */
#define RTC_TSTR_SU                  RTC_TSTR_SU_Msk
#define RTC_TSTR_SU_0                (0x1U << RTC_TSTR_SU_Pos)                 /*!< 0x00000001 */
#define RTC_TSTR_SU_1                (0x2U << RTC_TSTR_SU_Pos)                 /*!< 0x00000002 */
#define RTC_TSTR_SU_2                (0x4U << RTC_TSTR_SU_Pos)                 /*!< 0x00000004 */
#define RTC_TSTR_SU_3                (0x8U << RTC_TSTR_SU_Pos)                 /*!< 0x00000008 */

/********************  Bits definition for RTC_TSDR register  *****************/
#define RTC_TSDR_WDU_Pos             (13U)
#define RTC_TSDR_WDU_Msk             (0x7U << RTC_TSDR_WDU_Pos)                /*!< 0x0000E000 */
#define RTC_TSDR_WDU                 RTC_TSDR_WDU_Msk
#define RTC_TSDR_WDU_0               (0x1U << RTC_TSDR_WDU_Pos)                /*!< 0x00002000 */
#define RTC_TSDR_WDU_1               (0x2U << RTC_TSDR_WDU_Pos)                /*!< 0x00004000 */
#define RTC_TSDR_WDU_2               (0x4U << RTC_TSDR_WDU_Pos)                /*!< 0x00008000 */
#define RTC_TSDR_MT_Pos              (12U)
#define RTC_TSDR_MT_Msk              (0x1U << RTC_TSDR_MT_Pos)                 /*!< 0x00001000 */
#define RTC_TSDR_MT                  RTC_TSDR_MT_Msk
#define RTC_TSDR_MU_Pos              (8U)
#define RTC_TSDR_MU_Msk              (0xFU << RTC_TSDR_MU_Pos)                 /*!< 0x00000F00 */
#define RTC_TSDR_MU                  RTC_TSDR_MU_Msk
#define RTC_TSDR_MU_0                (0x1U << RTC_TSDR_MU_Pos)                 /*!< 0x00000100 */
#define RTC_TSDR_MU_1                (0x2U << RTC_TSDR_MU_Pos)                 /*!< 0x00000200 */
#define RTC_TSDR_MU_2                (0x4U << RTC_TSDR_MU_Pos)                 /*!< 0x00000400 */
#define RTC_TSDR_MU_3                (0x8U << RTC_TSDR_MU_Pos)                 /*!< 0x00000800 */
#define RTC_TSDR_DT_Pos              (4U)
#define RTC_TSDR_DT_Msk              (0x3U << RTC_TSDR_DT_Pos)                 /*!< 0x00000030 */
#define RTC_TSDR_DT                  RTC_TSDR_DT_Msk
#define RTC_TSDR_DT_0                (0x1U << RTC_TSDR_DT_Pos)                 /*!< 0x00000010 */
#define RTC_TSDR_DT_1                (0x2U << RTC_TSDR_DT_Pos)                 /*!< 0x00000020 */
#define RTC_TSDR_DU_Pos              (0U)
#define RTC_TSDR_DU_Msk              (0xFU << RTC_TSDR_DU_Pos)                 /*!< 0x0000000F */
#define RTC_TSDR_DU                  RTC_TSDR_DU_Msk
#define RTC_TSDR_DU_0                (0x1U << RTC_TSDR_DU_Pos)                 /*!< 0x00000001 */
#define RTC_TSDR_DU_1                (0x2U << RTC_TSDR_DU_Pos)                 /*!< 0x00000002 */
#define RTC_TSDR_DU_2                (0x4U << RTC_TSDR_DU_Pos)                 /*!< 0x00000004 */
#define RTC_TSDR_DU_3                (0x8U << RTC_TSDR_DU_Pos)                 /*!< 0x00000008 */

/********************  Bits definition for RTC_TSSSR register  ****************/
#define RTC_TSSSR_SS_Pos             (0U)
#define RTC_TSSSR_SS_Msk             (0xFFFFU << RTC_TSSSR_SS_Pos)             /*!< 0x0000FFFF */
#define RTC_TSSSR_SS                 RTC_TSSSR_SS_Msk

/********************  Bits definition for RTC_CAL register  *****************/
#define RTC_CALR_CALP_Pos            (15U)
#define RTC_CALR_CALP_Msk            (0x1U << RTC_CALR_CALP_Pos)               /*!< 0x00008000 */
#define RTC_CALR_CALP                RTC_CALR_CALP_Msk
#define RTC_CALR_CALW8_Pos           (14U)
#define RTC_CALR_CALW8_Msk           (0x1U << RTC_CALR_CALW8_Pos)              /*!< 0x00004000 */
#define RTC_CALR_CALW8               RTC_CALR_CALW8_Msk
#define RTC_CALR_CALW16_Pos          (13U)
#define RTC_CALR_CALW16_Msk          (0x1U << RTC_CALR_CALW16_Pos)             /*!< 0x00002000 */
#define RTC_CALR_CALW16              RTC_CALR_CALW16_Msk
#define RTC_CALR_CALM_Pos            (0U)
#define RTC_CALR_CALM_Msk            (0x1FFU << RTC_CALR_CALM_Pos)             /*!< 0x000001FF */
#define RTC_CALR_CALM                RTC_CALR_CALM_Msk
#define RTC_CALR_CALM_0              (0x001U << RTC_CALR_CALM_Pos)             /*!< 0x00000001 */
#define RTC_CALR_CALM_1              (0x002U << RTC_CALR_CALM_Pos)             /*!< 0x00000002 */
#define RTC_CALR_CALM_2              (0x004U << RTC_CALR_CALM_Pos)             /*!< 0x00000004 */
#define RTC_CALR_CALM_3              (0x008U << RTC_CALR_CALM_Pos)             /*!< 0x00000008 */
#define RTC_CALR_CALM_4              (0x010U << RTC_CALR_CALM_Pos)             /*!< 0x00000010 */
#define RTC_CALR_CALM_5              (0x020U << RTC_CALR_CALM_Pos)             /*!< 0x00000020 */
#define RTC_CALR_CALM_6              (0x040U << RTC_CALR_CALM_Pos)             /*!< 0x00000040 */
#define RTC_CALR_CALM_7              (0x080U << RTC_CALR_CALM_Pos)             /*!< 0x00000080 */
#define RTC_CALR_CALM_8              (0x100U << RTC_CALR_CALM_Pos)             /*!< 0x00000100 */

/********************  Bits definition for RTC_TAFCR register  ****************/
#define RTC_TAFCR_PC15MODE_Pos       (23U)
#define RTC_TAFCR_PC15MODE_Msk       (0x1U << RTC_TAFCR_PC15MODE_Pos)          /*!< 0x00800000 */
#define RTC_TAFCR_PC15MODE           RTC_TAFCR_PC15MODE_Msk
#define RTC_TAFCR_PC15VALUE_Pos      (22U)
#define RTC_TAFCR_PC15VALUE_Msk      (0x1U << RTC_TAFCR_PC15VALUE_Pos)         /*!< 0x00400000 */
#define RTC_TAFCR_PC15VALUE          RTC_TAFCR_PC15VALUE_Msk
#define RTC_TAFCR_PC14MODE_Pos       (21U)
#define RTC_TAFCR_PC14MODE_Msk       (0x1U << RTC_TAFCR_PC14MODE_Pos)          /*!< 0x00200000 */
#define RTC_TAFCR_PC14MODE           RTC_TAFCR_PC14MODE_Msk
#define RTC_TAFCR_PC14VALUE_Pos      (20U)
#define RTC_TAFCR_PC14VALUE_Msk      (0x1U << RTC_TAFCR_PC14VALUE_Pos)         /*!< 0x00100000 */
#define RTC_TAFCR_PC14VALUE          RTC_TAFCR_PC14VALUE_Msk
#define RTC_TAFCR_PC13MODE_Pos       (19U)
#define RTC_TAFCR_PC13MODE_Msk       (0x1U << RTC_TAFCR_PC13MODE_Pos)          /*!< 0x00080000 */
#define RTC_TAFCR_PC13MODE           RTC_TAFCR_PC13MODE_Msk
#define RTC_TAFCR_PC13VALUE_Pos      (18U)
#define RTC_TAFCR_PC13VALUE_Msk      (0x1U << RTC_TAFCR_PC13VALUE_Pos)         /*!< 0x00040000 */
#define RTC_TAFCR_PC13VALUE          RTC_TAFCR_PC13VALUE_Msk
#define RTC_TAFCR_TAMPPUDIS_Pos      (15U)
#define RTC_TAFCR_TAMPPUDIS_Msk      (0x1U << RTC_TAFCR_TAMPPUDIS_Pos)         /*!< 0x00008000 */
#define RTC_TAFCR_TAMPPUDIS          RTC_TAFCR_TAMPPUDIS_Msk
#define RTC_TAFCR_TAMPPRCH_Pos       (13U)
#define RTC_TAFCR_TAMPPRCH_Msk       (0x3U << RTC_TAFCR_TAMPPRCH_Pos)          /*!< 0x00006000 */
#define RTC_TAFCR_TAMPPRCH           RTC_TAFCR_TAMPPRCH_Msk
#define RTC_TAFCR_TAMPPRCH_0         (0x1U << RTC_TAFCR_TAMPPRCH_Pos)          /*!< 0x00002000 */
#define RTC_TAFCR_TAMPPRCH_1         (0x2U << RTC_TAFCR_TAMPPRCH_Pos)          /*!< 0x00004000 */
#define RTC_TAFCR_TAMPFLT_Pos        (11U)
#define RTC_TAFCR_TAMPFLT_Msk        (0x3U << RTC_TAFCR_TAMPFLT_Pos)           /*!< 0x00001800 */
#define RTC_TAFCR_TAMPFLT            RTC_TAFCR_TAMPFLT_Msk
#define RTC_TAFCR_TAMPFLT_0          (0x1U << RTC_TAFCR_TAMPFLT_Pos)           /*!< 0x00000800 */
#define RTC_TAFCR_TAMPFLT_1          (0x2U << RTC_TAFCR_TAMPFLT_Pos)           /*!< 0x00001000 */
#define RTC_TAFCR_TAMPFREQ_Pos       (8U)
#define RTC_TAFCR_TAMPFREQ_Msk       (0x7U << RTC_TAFCR_TAMPFREQ_Pos)          /*!< 0x00000700 */
#define RTC_TAFCR_TAMPFREQ           RTC_TAFCR_TAMPFREQ_Msk
#define RTC_TAFCR_TAMPFREQ_0         (0x1U << RTC_TAFCR_TAMPFREQ_Pos)          /*!< 0x00000100 */
#define RTC_TAFCR_TAMPFREQ_1         (0x2U << RTC_TAFCR_TAMPFREQ_Pos)          /*!< 0x00000200 */
#define RTC_TAFCR_TAMPFREQ_2         (0x4U << RTC_TAFCR_TAMPFREQ_Pos)          /*!< 0x00000400 */
#define RTC_TAFCR_TAMPTS_Pos         (7U)
#define RTC_TAFCR_TAMPTS_Msk         (0x1U << RTC_TAFCR_TAMPTS_Pos)            /*!< 0x00000080 */
#define RTC_TAFCR_TAMPTS             RTC_TAFCR_TAMPTS_Msk
#define RTC_TAFCR_TAMP2TRG_Pos       (4U)
#define RTC_TAFCR_TAMP2TRG_Msk       (0x1U << RTC_TAFCR_TAMP2TRG_Pos)          /*!< 0x00000010 */
#define RTC_TAFCR_TAMP2TRG           RTC_TAFCR_TAMP2TRG_Msk
#define RTC_TAFCR_TAMP2E_Pos         (3U)
#define RTC_TAFCR_TAMP2E_Msk         (0x1U << RTC_TAFCR_TAMP2E_Pos)            /*!< 0x00000008 */
#define RTC_TAFCR_TAMP2E             RTC_TAFCR_TAMP2E_Msk
#define RTC_TAFCR_TAMPIE_Pos         (2U)
#define RTC_TAFCR_TAMPIE_Msk         (0x1U << RTC_TAFCR_TAMPIE_Pos)            /*!< 0x00000004 */
#define RTC_TAFCR_TAMPIE             RTC_TAFCR_TAMPIE_Msk
#define RTC_TAFCR_TAMP1TRG_Pos       (1U)
#define RTC_TAFCR_TAMP1TRG_Msk       (0x1U << RTC_TAFCR_TAMP1TRG_Pos)          /*!< 0x00000002 */
#define RTC_TAFCR_TAMP1TRG           RTC_TAFCR_TAMP1TRG_Msk
#define RTC_TAFCR_TAMP1E_Pos         (0U)
#define RTC_TAFCR_TAMP1E_Msk         (0x1U << RTC_TAFCR_TAMP1E_Pos)            /*!< 0x00000001 */
#define RTC_TAFCR_TAMP1E             RTC_TAFCR_TAMP1E_Msk

/* Reference defines */
#define RTC_TAFCR_ALARMOUTTYPE               RTC_TAFCR_PC13VALUE

/********************  Bits definition for RTC_ALRMASSR register  *************/
#define RTC_ALRMASSR_MASKSS_Pos      (24U)
#define RTC_ALRMASSR_MASKSS_Msk      (0xFU << RTC_ALRMASSR_MASKSS_Pos)         /*!< 0x0F000000 */
#define RTC_ALRMASSR_MASKSS          RTC_ALRMASSR_MASKSS_Msk
#define RTC_ALRMASSR_MASKSS_0        (0x1U << RTC_ALRMASSR_MASKSS_Pos)         /*!< 0x01000000 */
#define RTC_ALRMASSR_MASKSS_1        (0x2U << RTC_ALRMASSR_MASKSS_Pos)         /*!< 0x02000000 */
#define RTC_ALRMASSR_MASKSS_2        (0x4U << RTC_ALRMASSR_MASKSS_Pos)         /*!< 0x04000000 */
#define RTC_ALRMASSR_MASKSS_3        (0x8U << RTC_ALRMASSR_MASKSS_Pos)         /*!< 0x08000000 */
#define RTC_ALRMASSR_SS_Pos          (0U)
#define RTC_ALRMASSR_SS_Msk          (0x7FFFU << RTC_ALRMASSR_SS_Pos)          /*!< 0x00007FFF */
#define RTC_ALRMASSR_SS              RTC_ALRMASSR_SS_Msk

/********************  Bits definition for RTC_ALRMBSSR register  *************/
#define RTC_ALRMBSSR_MASKSS_Pos      (24U)
#define RTC_ALRMBSSR_MASKSS_Msk      (0xFU << RTC_ALRMBSSR_MASKSS_Pos)         /*!< 0x0F000000 */
#define RTC_ALRMBSSR_MASKSS          RTC_ALRMBSSR_MASKSS_Msk
#define RTC_ALRMBSSR_MASKSS_0        (0x1U << RTC_ALRMBSSR_MASKSS_Pos)         /*!< 0x01000000 */
#define RTC_ALRMBSSR_MASKSS_1        (0x2U << RTC_ALRMBSSR_MASKSS_Pos)         /*!< 0x02000000 */
#define RTC_ALRMBSSR_MASKSS_2        (0x4U << RTC_ALRMBSSR_MASKSS_Pos)         /*!< 0x04000000 */
#define RTC_ALRMBSSR_MASKSS_3        (0x8U << RTC_ALRMBSSR_MASKSS_Pos)         /*!< 0x08000000 */
#define RTC_ALRMBSSR_SS_Pos          (0U)
#define RTC_ALRMBSSR_SS_Msk          (0x7FFFU << RTC_ALRMBSSR_SS_Pos)          /*!< 0x00007FFF */
#define RTC_ALRMBSSR_SS              RTC_ALRMBSSR_SS_Msk

/********************  Bits definition for RTC_BKP0R register  ****************/
#define RTC_BKP0R_Pos                (0U)
#define RTC_BKP0R_Msk                (0xFFFFFFFFU << RTC_BKP0R_Pos)            /*!< 0xFFFFFFFF */
#define RTC_BKP0R                    RTC_BKP0R_Msk

/********************  Bits definition for RTC_BKP1R register  ****************/
#define RTC_BKP1R_Pos                (0U)
#define RTC_BKP1R_Msk                (0xFFFFFFFFU << RTC_BKP1R_Pos)            /*!< 0xFFFFFFFF */
#define RTC_BKP1R                    RTC_BKP1R_Msk

/********************  Bits definition for RTC_BKP2R register  ****************/
#define RTC_BKP2R_Pos                (0U)
#define RTC_BKP2R_Msk                (0xFFFFFFFFU << RTC_BKP2R_Pos)            /*!< 0xFFFFFFFF */
#define RTC_BKP2R                    RTC_BKP2R_Msk

/********************  Bits definition for RTC_BKP3R register  ****************/
#define RTC_BKP3R_Pos                (0U)
#define RTC_BKP3R_Msk                (0xFFFFFFFFU << RTC_BKP3R_Pos)            /*!< 0xFFFFFFFF */
#define RTC_BKP3R                    RTC_BKP3R_Msk

/********************  Bits definition for RTC_BKP4R register  ****************/
#define RTC_BKP4R_Pos                (0U)
#define RTC_BKP4R_Msk                (0xFFFFFFFFU << RTC_BKP4R_Pos)            /*!< 0xFFFFFFFF */
#define RTC_BKP4R                    RTC_BKP4R_Msk
