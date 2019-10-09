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
** 文   件   名: c6xCache.h
**
** 创   建   人: Jiao.JinXing (焦进星)
**
** 文件创建日期: 2017 年 05 月 12 日
**
** 描        述: c6x 体系构架 CACHE 驱动.
*********************************************************************************************************/

#ifndef __ARCH_C6XCACHE_H
#define __ARCH_C6XCACHE_H

VOID  c6xCacheInit(LW_CACHE_OP *pcacheop,
                   CACHE_MODE   uiInstruction,
                   CACHE_MODE   uiData,
                   CPCHAR       pcMachineName);
VOID  c6xCacheReset(CPCHAR  pcMachineName);

#endif                                                                  /*  __ARCH_C6XCACHE_H           */
/*********************************************************************************************************
  END
*********************************************************************************************************/
