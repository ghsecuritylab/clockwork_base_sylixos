/*********************************************************************************************************
**
**                                    �й�������Դ��֯
**
**                                   Ƕ��ʽʵʱ����ϵͳ
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**
** ��   ��   ��: x86Dbg.c
**
** ��   ��   ��: Jiao.JinXing (������)
**
** �ļ���������: 2016 �� 07 �� 04 ��
**
** ��        ��: x86 ��ϵ���ܵ������.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "SylixOS.h"
/*********************************************************************************************************
  �ü�����
*********************************************************************************************************/
#if LW_CFG_GDB_EN > 0
#include "dtrace.h"
/*********************************************************************************************************
  x86 �ϵ�ʹ�� INT3 ָ�� (x86 ƽ̨����ʹ���쳣�ϵ�����)
*********************************************************************************************************/
#define X86_BREAKPOINT_INS          0xcc                                /*  INT3                        */
/*********************************************************************************************************
  SMP
*********************************************************************************************************/
#if (LW_CFG_SMP_EN > 0) && (LW_CFG_CACHE_EN > 0) && (LW_CFG_GDB_SMP_TU_LAZY > 0)
static addr_t   ulLastBpAddr[LW_CFG_MAX_PROCESSORS];
#endif                                                                  /*  LW_CFG_SMP_EN > 0           */
/*********************************************************************************************************
** ��������: archDbgBpInsert
** ��������: ����һ���ϵ�.
** �䡡��  : ulAddr         �ϵ��ַ
**           stSize         �ϵ��С
**           pulIns         ���ص�֮ǰ��ָ��
**           bLocal         �Ƿ�����µ�ǰ CPU I-CACHE
** �䡡��  : NONE
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
VOID  archDbgBpInsert (addr_t  ulAddr, size_t  stSize, ULONG  *pulIns, BOOL  bLocal)
{
    *(UINT8 *)pulIns = *(UINT8 *)ulAddr;
    *(UINT8 *)ulAddr = X86_BREAKPOINT_INS;
    KN_SMP_MB();
    
#if LW_CFG_CACHE_EN > 0
    if (bLocal) {
        API_CacheLocalTextUpdate((PVOID)ulAddr, stSize);
    } else {
        API_CacheTextUpdate((PVOID)ulAddr, stSize);
    }
#endif                                                                  /*  LW_CFG_CACHE_EN > 0         */
}
/*********************************************************************************************************
** ��������: archDbgAbInsert
** ��������: ����һ���쳣��.
** �䡡��  : ulAddr         �ϵ��ַ
**           pulIns         ���ص�֮ǰ��ָ��
** �䡡��  : NONE
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
VOID  archDbgAbInsert (addr_t  ulAddr, ULONG  *pulIns)
{
    *(UINT8 *)pulIns = *(UINT8 *)ulAddr;
    *(UINT8 *)ulAddr = X86_BREAKPOINT_INS;
    KN_SMP_MB();
    
#if LW_CFG_CACHE_EN > 0
    API_CacheTextUpdate((PVOID)ulAddr, sizeof(ULONG));
#endif                                                                  /*  LW_CFG_CACHE_EN > 0         */
}
/*********************************************************************************************************
** ��������: archDbgBpRemove
** ��������: ɾ��һ���ϵ�.
** �䡡��  : ulAddr         �ϵ��ַ
**           stSize         �ϵ��С
**           pulIns         ���ص�֮ǰ��ָ��
**           bLocal         �Ƿ�����µ�ǰ CPU I-CACHE
** �䡡��  : NONE
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
VOID  archDbgBpRemove (addr_t  ulAddr, size_t  stSize, ULONG  ulIns, BOOL  bLocal)
{
    *(UINT8 *)ulAddr = (UINT8)ulIns;
    KN_SMP_MB();
    
#if LW_CFG_CACHE_EN > 0
    if (bLocal) {
        API_CacheLocalTextUpdate((PVOID)ulAddr, stSize);
    } else {
        API_CacheTextUpdate((PVOID)ulAddr, stSize);
    }
#endif                                                                  /*  LW_CFG_CACHE_EN > 0         */
}
/*********************************************************************************************************
** ��������: archDbgApRemove
** ��������: ɾ��һ����ֹ��.
** �䡡��  : ulAddr         ��ֹ���ַ
**           pulIns         ���ص�֮ǰ��ָ��
** �䡡��  : NONE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
VOID  archDbgApRemove (addr_t  ulAddr, ULONG  ulIns)
{
    *(UINT8 *)ulAddr = (UINT8)ulIns;
    KN_SMP_MB();

#if LW_CFG_CACHE_EN > 0
    API_CacheTextUpdate((PVOID)ulAddr, sizeof(UINT8));
#endif                                                                  /*  LW_CFG_CACHE_EN > 0         */
}
/*********************************************************************************************************
** ��������: archDbgBpPrefetch
** ��������: Ԥȡһ��ָ��.
             ��ָ��� MMU ����������ʱ, ָ��ռ�Ϊ����ֻ��, ������Ҫ����һ��ȱҳ�ж�, ��¡һ������ҳ��.
** �䡡��  : ulAddr         �ϵ��ַ
** �䡡��  : NONE
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
VOID  archDbgBpPrefetch (addr_t  ulAddr)
{
    volatile UINT8  ucByte = *(UINT8 *)ulAddr;                          /*  ��ȡ�ϵ㴦����              */
    
    *(UINT8 *)ulAddr = ucByte;                                          /*  ִ��һ��д����, ����ҳ���ж�*/
}
/*********************************************************************************************************
** ��������: archDbgTrapType
** ��������: ��ȡ trap ����.
** �䡡��  : ulAddr         �ϵ��ַ
**           pvArch         ��ϵ�ṹ��ز���
** �䡡��  : LW_TRAP_INVAL / LW_TRAP_BRKPT / LW_TRAP_ABORT
** ȫ�ֱ���: 
** ����ģ��: 
*********************************************************************************************************/
UINT  archDbgTrapType (addr_t  ulAddr, PVOID  pvArch)
{
    PLW_CLASS_TCB  ptcbCur;
#if (LW_CFG_SMP_EN > 0) && (LW_CFG_CACHE_EN > 0) && (LW_CFG_GDB_SMP_TU_LAZY > 0)
    ULONG          ulCPUId;
#endif                                                                  /*  LW_CFG_SMP_EN > 0           */

    if (API_DtraceIsValid() == LW_FALSE) {                              /*  �����ڵ��Խڵ�              */
        return  (LW_TRAP_INVAL);
    }

    if (pvArch == X86_DBG_TRAP_STEP) {
        return  (LW_TRAP_ISTEP);
    }

    if (*(UINT8 *)ulAddr == X86_BREAKPOINT_INS) {
        LW_TCB_GET_CUR_SAFE(ptcbCur);

        if (ptcbCur->TCB_ulAbortPointAddr == ulAddr) {
            return  (LW_TRAP_ABORT);
        } else {
            return  (LW_TRAP_BRKPT);
        }
    }

#if (LW_CFG_SMP_EN > 0) && (LW_CFG_CACHE_EN > 0) && (LW_CFG_GDB_SMP_TU_LAZY > 0)
    if (API_CacheGetOption() & CACHE_TEXT_UPDATE_MP) {
        ulCPUId = LW_CPU_GET_CUR_ID();
        if (ulLastBpAddr[ulCPUId] == ulAddr) {                          /*  ���Ƕϵ��ֹͣ              */
            ulLastBpAddr[ulCPUId] =  LW_GDB_ADDR_INVAL;                 /*  ͬһ��ַ����ʧЧ            */
            return  (LW_TRAP_INVAL);

        } else {
            ulLastBpAddr[ulCPUId] = ulAddr;
            API_CacheLocalTextUpdate((PVOID)ulAddr, sizeof(ulAddr));    /*  ˢ��һ�� I CACHE ��ȥ����   */
            return  (LW_TRAP_RETRY);
        }
    } else
#endif                                                                  /*  LW_CFG_SMP_EN > 0           */
                                                                        /*  LW_CFG_CACHE_EN > 0         */
                                                                        /*  LW_CFG_GDB_SMP_TU_LAZY > 0  */
    {
        return  (LW_TRAP_INVAL);
    }
}
/*********************************************************************************************************
** ��������: archDbgBpAdjust
** ��������: ������ϵ�ṹ�����ϵ��ַ.
** �䡡��  : pvDtrace       dtrace �ڵ�
**           pdtm           ��ȡ����Ϣ
** �䡡��  : NONE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
VOID  archDbgBpAdjust (PVOID  pvDtrace, PVOID  pvtm)
{
}
/*********************************************************************************************************
** ��������: archGdbSetStepMode
** ��������: ���õ�������ģʽ.
** �䡡��  : pregctx        ����Ĵ���������
**           bEnable        �Ƿ�ʹ��Ӳ������ģʽ
** �䡡��  : NONE
** ȫ�ֱ���:
** ����ģ��:
*********************************************************************************************************/
#ifdef LW_DTRACE_HW_ISTEP

VOID  archDbgSetStepMode (ARCH_REG_CTX  *pregctx, BOOL  bEnable)
{
    if (bEnable) {
        pregctx->REG_XFLAGS |= X86_EFLAGS_TF;                           /*  step mode                   */

    } else {
        pregctx->REG_XFLAGS &= ~X86_EFLAGS_TF;                          /*  normal mode                 */
    }
}

#endif                                                                  /*  LW_DTRACE_HW_ISTEP          */
#endif                                                                  /*  LW_CFG_GDB_EN > 0           */
/*********************************************************************************************************
  END
*********************************************************************************************************/