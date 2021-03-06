/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: ttinyShellFsCmd.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2008 年 07 月 27 日
**
** 描        述: 系统内部命令定义 (文件系统相关).
*********************************************************************************************************/

#ifndef __TTINYSHELLFSCMD_H
#define __TTINYSHELLFSCMD_H

/*********************************************************************************************************
  裁剪控制
*********************************************************************************************************/
#if LW_CFG_SHELL_EN > 0

VOID  __tshellFsCmdInit(VOID);

#endif                                                                  /*  LW_CFG_SHELL_EN > 0         */
#endif                                                                  /*  __TTINYSHELLFSCMD_H         */
/*********************************************************************************************************
  END
*********************************************************************************************************/
