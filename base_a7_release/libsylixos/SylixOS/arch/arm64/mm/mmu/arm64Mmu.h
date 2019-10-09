/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                       SylixOS(TM)
**
**                               Copyright  All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: arm64Mmu.h
**
** 创   建   人: Wang.Xuan (王翾)
**
** 文件创建日期: 2018 年 07 月 04 日
**
** 描        述: ARM64 体系构架 MMU 驱动.
*********************************************************************************************************/

#ifndef __ARM64MMU_H
#define __ARM64MMU_H

VOID   arm64MmuInit(LW_MMU_OP  *pmmuop, CPCHAR  pcMachineName);
ULONG  arm64MmuAbtFaultAddr(VOID);

#endif                                                                  /*  __ARM64MMU_H                */
/*********************************************************************************************************
  END
*********************************************************************************************************/
