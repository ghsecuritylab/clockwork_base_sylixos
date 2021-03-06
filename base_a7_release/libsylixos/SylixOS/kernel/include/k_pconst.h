/*********************************************************************************************************
**
**                                    中国软件开源组织
**
**                                   嵌入式实时操作系统
**
**                                       SylixOS(TM)
**
**                               Copyright All Rights Reserved
**
**--------------文件信息--------------------------------------------------------------------------------
**
** 文   件   名: k_pconst.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2008 年 05 月 02 日
**
** 描        述: 这是系统 POSIX 相关宏定义。
*********************************************************************************************************/

#ifndef __K_PCONST_H
#define __K_PCONST_H

/*********************************************************************************************************
  exit() 参数定义
*********************************************************************************************************/

#define EXIT_FAILURE                    1                               /*  执行失败等等                */
#define EXIT_SUCCESS                    0                               /*  执行成功等等                */

/*********************************************************************************************************
  pthread cancel type
*********************************************************************************************************/

#define PTHREAD_CANCEL_ASYNCHRONOUS     LW_THREAD_CANCEL_ASYNCHRONOUS
#define PTHREAD_CANCEL_DEFERRED         LW_THREAD_CANCEL_DEFERRED

#define PTHREAD_CANCEL_ENABLE           LW_THREAD_CANCEL_ENABLE
#define PTHREAD_CANCEL_DISABLE          LW_THREAD_CANCEL_DISABLE

#define PTHREAD_CANCELED                LW_THREAD_CANCELED

/*********************************************************************************************************
  sylixos add the following
*********************************************************************************************************/

#define PX_ERROR                        (-1)
#define PX_EOS                          '\0'

#endif                                                                  /*  __K_PCONST_H                */
/*********************************************************************************************************
  END
*********************************************************************************************************/
