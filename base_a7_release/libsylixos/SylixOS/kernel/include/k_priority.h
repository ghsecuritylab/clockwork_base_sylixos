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
** 文   件   名: k_priority.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2014 年 01 月 16 日
**
** 描        述: 系统优先级定义.
*********************************************************************************************************/

#ifndef __K_PRIORITY_H
#define __K_PRIORITY_H

/*********************************************************************************************************
  任务优先级范围
*********************************************************************************************************/

#define LW_PRIO_HIGHEST         0                                       /*  SylixOS 最高优先级          */
#define LW_PRIO_LOWEST          255                                     /*  SylixOS 最低优先级          */

/*********************************************************************************************************
  优先级 (一般应用的最高优先级不能高于 LW_PRIO_CRITICAL 最低不能低过 LW_PRIO_LOW)
*********************************************************************************************************/

#define LW_PRIO_EXTREME         LW_PRIO_HIGHEST                         /*  最高优先级                  */
#define LW_PRIO_CRITICAL        50                                      /*  关键处理任务                */
#define LW_PRIO_REALTIME        100                                     /*  实时处理任务                */
#define LW_PRIO_HIGH            150                                     /*  高优先级任务                */
#define LW_PRIO_NORMAL          200                                     /*  正常优先级                  */
#define LW_PRIO_LOW             250                                     /*  低优先级                    */
#define LW_PRIO_IDLE            LW_PRIO_LOWEST                          /*  最低优先级                  */

/*********************************************************************************************************
  系统任务优先级
*********************************************************************************************************/

#define LW_PRIO_T_EXCPT         (LW_PRIO_EXTREME)                       /*  t_except 线程               */
#define LW_PRIO_T_TICK          (LW_PRIO_EXTREME + 10)                  /*  t_tick 线程                 */
#define LW_PRIO_T_ITIMER        (LW_PRIO_EXTREME + 20)                  /*  t_itimer 线程               */
#define LW_PRIO_T_LOG           (LW_PRIO_CRITICAL + 10)                 /*  t_log 线程                  */
#define LW_PRIO_T_NETPROTO      (LW_PRIO_REALTIME + 10)                 /*  网络系统线程                */
#define LW_PRIO_T_NETJOB        (LW_PRIO_REALTIME + 10)                 /*  网络接口驱动服务线程        */
#define LW_PRIO_T_BUS           (LW_PRIO_REALTIME + 10)                 /*  所有总线服务线程 USB / SD 等*/
#define LW_PRIO_T_SHELL         (LW_PRIO_HIGH)                          /*  t_shell 线程                */
#define LW_PRIO_T_SERVICE       (LW_PRIO_HIGH + 10)                     /*  服务类线程 ftp / tftp 等    */
#define LW_PRIO_T_SYSMSG        (LW_PRIO_LOW)                           /*  热插拔等操作系统基本信息服务*/
#define LW_PRIO_T_CACHE         (LW_PRIO_IDLE - 3)                      /*  磁盘 CACHE 回写             */
#define LW_PRIO_T_RECLAIM       (LW_PRIO_IDLE - 2)                      /*  资源回收                    */
#define LW_PRIO_T_POWER         (LW_PRIO_IDLE - 1)                      /*  电源管理                    */

/*********************************************************************************************************
  系统内部 mutex 默认天花板优先级
*********************************************************************************************************/

#define LW_PRIO_DEF_CEILING     LW_PRIO_REALTIME

/*********************************************************************************************************
  中断优先级范围
*********************************************************************************************************/

#define LW_INTER_PRIO_HIGHEST   0                                       /*  最高优先级中断              */
#define LW_INTER_PRIO_LOWEST    255                                     /*  最低优先级中断              */

#endif                                                                  /*  __K_PRIORITY_H              */
/*********************************************************************************************************
  END
*********************************************************************************************************/
