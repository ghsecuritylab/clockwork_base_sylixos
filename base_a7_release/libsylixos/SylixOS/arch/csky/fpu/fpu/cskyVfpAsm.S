;/*********************************************************************************************************
;**
;**                                    �й�������Դ��֯
;**
;**                                   Ƕ��ʽʵʱ����ϵͳ
;**
;**                                       SylixOS(TM)
;**
;**                               Copyright  All Rights Reserved
;**
;**--------------�ļ���Ϣ--------------------------------------------------------------------------------
;**
;** ��   ��   ��: cskyVfpAsm.S
;**
;** ��   ��   ��: Wang.Xuan (���Q)
;**
;** �ļ���������: 2018 �� 05 �� 14 ��
;**
;** ��        ��: C-SKY ��ϵ�ܹ� VFP ֧��.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>

#if LW_CFG_CPU_FPU_EN > 0
#include "arch/csky/arch_float.h"
#include "arch/csky/inc/cskyregs.h"

    FILE_BEGIN()

    EXPORT_LABEL(cskyVfpInit)
    EXPORT_LABEL(cskyVfpSave)
    EXPORT_LABEL(cskyVfpRestore)
    EXPORT_LABEL(cskyVfpGetFESR)
    EXPORT_LABEL(cskyVfpSetFESR)

;/*********************************************************************************************************
;  FCR ����
;*********************************************************************************************************/

#define FCR_FM              (0 << 27)                                   /*  ���ǹ����ˢ���з�����     */
#define FCR_RM              (0 << 24)                                   /*  ���뵽��ӽ�ֵ               */

#if LW_CFG_CPU_FPU_IDE > 0
#define FCR_IDE             (1 << 5)
#else
#define FCR_IDE             0
#endif

#if LW_CFG_CPU_FPU_IXE > 0
#define FCR_IXE             (1 << 4)
#else
#define FCR_IXE             0
#endif

#if LW_CFG_CPU_FPU_UFE > 0
#define FCR_UFE             (1 << 3)
#else
#define FCR_UFE             0
#endif

#if LW_CFG_CPU_FPU_OFE > 0
#define FCR_OFE             (1 << 2)
#else
#define FCR_OFE             0
#endif

#if LW_CFG_CPU_FPU_DZE > 0
#define FCR_DZE             (1 << 1)
#else
#define FCR_DZE             0
#endif

#if LW_CFG_CPU_FPU_IOE > 0
#define FCR_IOE             (1 << 0)
#else
#define FCR_IOE             0
#endif

#define FCR_CONFIG          (FCR_FM | FCR_RM |FCR_IDE | FCR_IXE | FCR_UFE | FCR_OFE | FCR_DZE | FCR_IOE)

;/*********************************************************************************************************
;  �궨��
;*********************************************************************************************************/

#if defined(__SYLIXOS_CSKY_ARCH_CK803__)

MACRO_DEF(FPU_REG_RESTORE fr, tmp, base, offset)
    LD.W        \tmp , (\base , \offset)
    FMTVRL      \fr  , \tmp
    MACRO_END()

MACRO_DEF(FPU_REG_SAVE fr, tmp, base, offset)
    FMFVRL      \tmp  , \fr
    ST.W        \tmp , (\base , \offset)
    MACRO_END()

#else

MACRO_DEF(FPU_REG_RESTORE fr, tmp, base, offsetl, offseth)
    LD.W        \tmp , (\base , \offsetl)
    FMTVRL      \fr  , \tmp
    LD.W        \tmp , (\base , \offseth)
    FMTVRH      \fr  , \tmp
    MACRO_END()

MACRO_DEF(FPU_REG_SAVE fr, tmp, base, offsetl, offseth)
    FMFVRL      \tmp  , \fr
    ST.W        \tmp , (\base , \offsetl)
    FMFVRH      \tmp  , \fr
    ST.W        \tmp , (\base , \offseth)
    MACRO_END()

#endif

;/*********************************************************************************************************
;  ��ʼ�� FPU
;*********************************************************************************************************/

FUNC_DEF(cskyVfpInit)
    MOVI        A0 , FCR_CONFIG                                         ;/*  ���� FPU �Ĵ���             */
    MTCR        A0 , CR<1, 2>

    MOVI        A0 , 0

    FMTVRL      FR0 , A0                                                ;/*  ���� FPU �Ĵ���             */
    FMTVRL      FR1 , A0
    FMTVRL      FR2 , A0
    FMTVRL      FR3 , A0
    FMTVRL      FR4 , A0
    FMTVRL      FR5 , A0
    FMTVRL      FR6 , A0
    FMTVRL      FR7 , A0
    FMTVRL      FR8 , A0
    FMTVRL      FR9 , A0
    FMTVRL      FR10, A0
    FMTVRL      FR11, A0
    FMTVRL      FR12, A0
    FMTVRL      FR13, A0
    FMTVRL      FR14, A0
    FMTVRL      FR15, A0

#if !defined(__SYLIXOS_CSKY_ARCH_CK803__)

    FMTVRH      FR0 , A0
    FMTVRH      FR1 , A0
    FMTVRH      FR2 , A0
    FMTVRH      FR3 , A0
    FMTVRH      FR4 , A0
    FMTVRH      FR5 , A0
    FMTVRH      FR6 , A0
    FMTVRH      FR7 , A0
    FMTVRH      FR8 , A0
    FMTVRH      FR9 , A0
    FMTVRH      FR10, A0
    FMTVRH      FR11, A0
    FMTVRH      FR12, A0
    FMTVRH      FR13, A0
    FMTVRH      FR14, A0
    FMTVRH      FR15, A0

#endif

    RTS
    FUNC_END(cskyVfpInit)

;/*********************************************************************************************************
;  ���� FPU �Ĵ���
;*********************************************************************************************************/

FUNC_DEF(cskyVfpSave)

#if defined(__SYLIXOS_CSKY_ARCH_CK803__)

    FPU_REG_SAVE    FR0 ,  A1,  A0,  FPU_OFFSET_REG(0)
    FPU_REG_SAVE    FR1 ,  A1,  A0,  FPU_OFFSET_REG(1)
    FPU_REG_SAVE    FR2 ,  A1,  A0,  FPU_OFFSET_REG(2)
    FPU_REG_SAVE    FR3 ,  A1,  A0,  FPU_OFFSET_REG(3)
    FPU_REG_SAVE    FR4 ,  A1,  A0,  FPU_OFFSET_REG(4)
    FPU_REG_SAVE    FR5 ,  A1,  A0,  FPU_OFFSET_REG(5)
    FPU_REG_SAVE    FR6 ,  A1,  A0,  FPU_OFFSET_REG(6)
    FPU_REG_SAVE    FR7 ,  A1,  A0,  FPU_OFFSET_REG(7)
    FPU_REG_SAVE    FR8 ,  A1,  A0,  FPU_OFFSET_REG(8)
    FPU_REG_SAVE    FR9 ,  A1,  A0,  FPU_OFFSET_REG(9)
    FPU_REG_SAVE    FR10,  A1,  A0,  FPU_OFFSET_REG(10)
    FPU_REG_SAVE    FR11,  A1,  A0,  FPU_OFFSET_REG(11)
    FPU_REG_SAVE    FR12,  A1,  A0,  FPU_OFFSET_REG(12)
    FPU_REG_SAVE    FR13,  A1,  A0,  FPU_OFFSET_REG(13)
    FPU_REG_SAVE    FR14,  A1,  A0,  FPU_OFFSET_REG(14)
    FPU_REG_SAVE    FR15,  A1,  A0,  FPU_OFFSET_REG(15)

#else

    FPU_REG_SAVE    FR0 ,  A1,  A0,  FPU_OFFSET_REG_LO(0),  FPU_OFFSET_REG_HI(0)
    FPU_REG_SAVE    FR1 ,  A1,  A0,  FPU_OFFSET_REG_LO(1),  FPU_OFFSET_REG_HI(1)
    FPU_REG_SAVE    FR2 ,  A1,  A0,  FPU_OFFSET_REG_LO(2),  FPU_OFFSET_REG_HI(2)
    FPU_REG_SAVE    FR3 ,  A1,  A0,  FPU_OFFSET_REG_LO(3),  FPU_OFFSET_REG_HI(3)
    FPU_REG_SAVE    FR4 ,  A1,  A0,  FPU_OFFSET_REG_LO(4),  FPU_OFFSET_REG_HI(4)
    FPU_REG_SAVE    FR5 ,  A1,  A0,  FPU_OFFSET_REG_LO(5),  FPU_OFFSET_REG_HI(5)
    FPU_REG_SAVE    FR6 ,  A1,  A0,  FPU_OFFSET_REG_LO(6),  FPU_OFFSET_REG_HI(6)
    FPU_REG_SAVE    FR7 ,  A1,  A0,  FPU_OFFSET_REG_LO(7),  FPU_OFFSET_REG_HI(7)
    FPU_REG_SAVE    FR8 ,  A1,  A0,  FPU_OFFSET_REG_LO(8),  FPU_OFFSET_REG_HI(8)
    FPU_REG_SAVE    FR9 ,  A1,  A0,  FPU_OFFSET_REG_LO(9),  FPU_OFFSET_REG_HI(9)
    FPU_REG_SAVE    FR10,  A1,  A0,  FPU_OFFSET_REG_LO(10), FPU_OFFSET_REG_HI(10)
    FPU_REG_SAVE    FR11,  A1,  A0,  FPU_OFFSET_REG_LO(11), FPU_OFFSET_REG_HI(11)
    FPU_REG_SAVE    FR12,  A1,  A0,  FPU_OFFSET_REG_LO(12), FPU_OFFSET_REG_HI(12)
    FPU_REG_SAVE    FR13,  A1,  A0,  FPU_OFFSET_REG_LO(13), FPU_OFFSET_REG_HI(13)
    FPU_REG_SAVE    FR14,  A1,  A0,  FPU_OFFSET_REG_LO(14), FPU_OFFSET_REG_HI(14)
    FPU_REG_SAVE    FR15,  A1,  A0,  FPU_OFFSET_REG_LO(15), FPU_OFFSET_REG_HI(15)

#endif

    MFCR        A1  , CR<1, 2>
    ST.W        A1  , (A0 , FPU_OFFSET_FCR)                             ;/*  ���� FCR ״̬�Ĵ���         */

    MFCR        A1  , CR<2, 2>
    ST.W        A1  , (A0 , FPU_OFFSET_FESR)                            ;/*  ���� FECR ״̬�Ĵ���        */

    RTS
    FUNC_END(cskyVfpSave)

;/*********************************************************************************************************
;  �ָ� FPU �Ĵ���
;*********************************************************************************************************/

FUNC_DEF(cskyVfpRestore)
    LD.W        A1  , (A0 , FPU_OFFSET_FESR)                            ;/*  �ָ� FECR ״̬�Ĵ���        */
    MTCR        A1  , CR<2, 2>

#if defined(__SYLIXOS_CSKY_ARCH_CK803__)

    FPU_REG_RESTORE FR0 ,  A1,  A0,  FPU_OFFSET_REG(0)
    FPU_REG_RESTORE FR1 ,  A1,  A0,  FPU_OFFSET_REG(1)
    FPU_REG_RESTORE FR2 ,  A1,  A0,  FPU_OFFSET_REG(2)
    FPU_REG_RESTORE FR3 ,  A1,  A0,  FPU_OFFSET_REG(3)
    FPU_REG_RESTORE FR4 ,  A1,  A0,  FPU_OFFSET_REG(4)
    FPU_REG_RESTORE FR5 ,  A1,  A0,  FPU_OFFSET_REG(5)
    FPU_REG_RESTORE FR6 ,  A1,  A0,  FPU_OFFSET_REG(6)
    FPU_REG_RESTORE FR7 ,  A1,  A0,  FPU_OFFSET_REG(7)
    FPU_REG_RESTORE FR8 ,  A1,  A0,  FPU_OFFSET_REG(8)
    FPU_REG_RESTORE FR9 ,  A1,  A0,  FPU_OFFSET_REG(9)
    FPU_REG_RESTORE FR10,  A1,  A0,  FPU_OFFSET_REG(10)
    FPU_REG_RESTORE FR11,  A1,  A0,  FPU_OFFSET_REG(11)
    FPU_REG_RESTORE FR12,  A1,  A0,  FPU_OFFSET_REG(12)
    FPU_REG_RESTORE FR13,  A1,  A0,  FPU_OFFSET_REG(13)
    FPU_REG_RESTORE FR14,  A1,  A0,  FPU_OFFSET_REG(14)
    FPU_REG_RESTORE FR15,  A1,  A0,  FPU_OFFSET_REG(15)

#else

    FPU_REG_RESTORE FR0 ,  A1,  A0,  FPU_OFFSET_REG_LO(0),  FPU_OFFSET_REG_HI(0)
    FPU_REG_RESTORE FR1 ,  A1,  A0,  FPU_OFFSET_REG_LO(1),  FPU_OFFSET_REG_HI(1)
    FPU_REG_RESTORE FR2 ,  A1,  A0,  FPU_OFFSET_REG_LO(2),  FPU_OFFSET_REG_HI(2)
    FPU_REG_RESTORE FR3 ,  A1,  A0,  FPU_OFFSET_REG_LO(3),  FPU_OFFSET_REG_HI(3)
    FPU_REG_RESTORE FR4 ,  A1,  A0,  FPU_OFFSET_REG_LO(4),  FPU_OFFSET_REG_HI(4)
    FPU_REG_RESTORE FR5 ,  A1,  A0,  FPU_OFFSET_REG_LO(5),  FPU_OFFSET_REG_HI(5)
    FPU_REG_RESTORE FR6 ,  A1,  A0,  FPU_OFFSET_REG_LO(6),  FPU_OFFSET_REG_HI(6)
    FPU_REG_RESTORE FR7 ,  A1,  A0,  FPU_OFFSET_REG_LO(7),  FPU_OFFSET_REG_HI(7)
    FPU_REG_RESTORE FR8 ,  A1,  A0,  FPU_OFFSET_REG_LO(8),  FPU_OFFSET_REG_HI(8)
    FPU_REG_RESTORE FR9 ,  A1,  A0,  FPU_OFFSET_REG_LO(9),  FPU_OFFSET_REG_HI(9)
    FPU_REG_RESTORE FR10,  A1,  A0,  FPU_OFFSET_REG_LO(10), FPU_OFFSET_REG_HI(10)
    FPU_REG_RESTORE FR11,  A1,  A0,  FPU_OFFSET_REG_LO(11), FPU_OFFSET_REG_HI(11)
    FPU_REG_RESTORE FR12,  A1,  A0,  FPU_OFFSET_REG_LO(12), FPU_OFFSET_REG_HI(12)
    FPU_REG_RESTORE FR13,  A1,  A0,  FPU_OFFSET_REG_LO(13), FPU_OFFSET_REG_HI(13)
    FPU_REG_RESTORE FR14,  A1,  A0,  FPU_OFFSET_REG_LO(14), FPU_OFFSET_REG_HI(14)
    FPU_REG_RESTORE FR15,  A1,  A0,  FPU_OFFSET_REG_LO(15), FPU_OFFSET_REG_HI(15)

#endif

    LD.W        A1  , (A0 , FPU_OFFSET_FCR)                             ;/*  �ָ� FCR ״̬�Ĵ���         */
    MTCR        A1  , CR<1, 2>

    RTS
    FUNC_END(cskyVfpRestore)

;/*********************************************************************************************************
;  ��� FESR �Ĵ���
;*********************************************************************************************************/

FUNC_DEF(cskyVfpGetFESR)
    MFCR        A0 , CR<2, 2>
    RTS
    FUNC_END(cskyVfpGetFESR)

;/*********************************************************************************************************
;  ���� FESR �Ĵ���
;*********************************************************************************************************/

FUNC_DEF(cskyVfpSetFESR)
    MTCR        A0 , CR<2, 2>
    RTS
    FUNC_END(cskyVfpSetFESR)

    FILE_END()

#endif
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/