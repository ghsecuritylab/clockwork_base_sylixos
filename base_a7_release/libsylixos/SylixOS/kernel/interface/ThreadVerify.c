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
** 文   件   名: ThreadVerify.c
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2008 年 04 月 01 日
**
** 描        述: 检查一个线程的 ID 是否正确.

** BUG
2010.01.22  支持 SMP.
*********************************************************************************************************/
#define  __SYLIXOS_KERNEL
#include "../SylixOS/kernel/include/k_kernel.h"
/*********************************************************************************************************
** 函数名称: API_ThreadVerify
** 功能描述: 检查一个线程的 ID 是否正确.
** 输　入  : 
**           ulId      线程句柄
** 输　出  : LW_TRUE   代表正确
**           LW_FLASH  代表错误
** 全局变量: 
** 调用模块: 
                                           API 函数
*********************************************************************************************************/
LW_API  
BOOL  API_ThreadVerify (LW_OBJECT_HANDLE  ulId)
{
    REGISTER UINT16                usIndex;
	
    usIndex = _ObjectGetIndex(ulId);
	
    if (!_ObjectClassOK(ulId, _OBJECT_THREAD)) {                        /*  检查 ID 类型有效性          */
        return  (LW_FALSE);
    }
    if (_Thread_Index_Invalid(usIndex)) {                               /*  检查线程有效性              */
        return  (LW_FALSE);
    }

    __KERNEL_ENTER();                                                   /*  进入内核                    */
    if (_Thread_Invalid(usIndex)) {
        __KERNEL_EXIT();                                                /*  退出内核                    */
        return  (LW_FALSE);
    
    } else {
        __KERNEL_EXIT();                                                /*  退出内核                    */
        return  (LW_TRUE);
    }
}
/*********************************************************************************************************
  END
*********************************************************************************************************/
