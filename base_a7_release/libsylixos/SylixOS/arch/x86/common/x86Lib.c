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
** 文   件   名: x86Lib.c
**
** 创   建   人: Jiao.JinXing (焦进星)
**
** 文件创建日期: 2016 年 07 月 04 日
**
** 描        述: x86 体系构架内部库.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "SylixOS.h"
/*********************************************************************************************************
** 函数名称: archPageCopy
** 功能描述: 拷贝一个页面
** 输　入  : pvTo      目标
**           pvFrom    源
** 输　出  : NONE
** 全局变量:
** 调用模块:
*********************************************************************************************************/
#if LW_CFG_CPU_WORD_LENGHT == 32

VOID  archPageCopy (PVOID  pvTo, PVOID  pvFrom)
{
    INT  iD0, iD1, iD2;

    __asm__ __volatile__ ("cld\n\t"
                          "rep ; movsl"
                          : "=&c" (iD0), "=&D" (iD1), "=&S" (iD2)
                          : "0" (1024), "1" ((LONG)pvTo), "2" ((LONG)pvFrom)
                          : "memory");
}

#else

/*********************************************************************************************************
  x86-64 在 x64LibAsm.S 文件中实现 archPageCopy 函数
*********************************************************************************************************/

#endif                                                                  /*  LW_CFG_CPU_WORD_LENGHT == 32*/
/*********************************************************************************************************
  END
*********************************************************************************************************/
