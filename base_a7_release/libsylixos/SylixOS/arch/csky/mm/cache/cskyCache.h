/*********************************************************************************************************
**
**                                    �й�������Դ��֯
**
**                                   Ƕ��ʽʵʱ����ϵͳ
**
**                                       SylixOS(TM)
**
**                               Copyright  All Rights Reserved
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**
** ��   ��   ��: cskyCache.h
**
** ��   ��   ��: Wang.Xuan (���Q)
**
** �ļ���������: 2018 �� 05 �� 14 ��
**
** ��        ��: C-SKY ��ϵ���� CACHE ����.
*********************************************************************************************************/

#ifndef __ARCH_CSKYCACHE_H
#define __ARCH_CSKYCACHE_H

VOID  cskyCacheInit(LW_CACHE_OP *pcacheop,
                    CACHE_MODE   uiInstruction,
                    CACHE_MODE   uiData,
                    CPCHAR       pcMachineName);
VOID  cskyCacheReset(CPCHAR  pcMachineName);

#endif                                                                  /*  __ARCH_CSKYCACHE_H          */
/*********************************************************************************************************
  END
*********************************************************************************************************/