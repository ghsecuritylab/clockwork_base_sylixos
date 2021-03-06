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
** 文   件   名: posix_cfg.h
**
** 创   建   人: Han.Hui (韩辉)
**
** 文件创建日期: 2009 年 12 月 30 日
**
** 描        述: posix 兼容库配置。
*********************************************************************************************************/

#ifndef __POSIX_CFG_H
#define __POSIX_CFG_H

/*********************************************************************************************************
*                                        posix 兼容库
*
* 依存关系: 1: SylixOS 线程的所有服务
            2: 各种信号量服务
            3: rtc支持 
            4: 类 POSIX 线程高级操作功能管理
*********************************************************************************************************/

#define LW_CFG_POSIX_EN                     1                           /*  是否使能 posix 兼容库       */
#define LW_CFG_POSIXEX_EN                   1                           /*  是否是能 posix 扩展库       */
#define LW_CFG_POSIX_INTER_EN               1                           /*  是否使能 sem mqueue 信号中断*/

#if LW_CFG_CPU_WORD_LENGHT == 32
#define LW_CFG_PTHREAD_MIN_STK_SIZE         (4 * LW_CFG_KB_SIZE)
#define LW_CFG_PTHREAD_DEFAULT_STK_SIZE     (8 * LW_CFG_KB_SIZE)        /*  posix 线程默认堆栈大小      */
#else
#define LW_CFG_PTHREAD_MIN_STK_SIZE         ( 8 * LW_CFG_KB_SIZE)
#define LW_CFG_PTHREAD_DEFAULT_STK_SIZE     (16 * LW_CFG_KB_SIZE)       /*  posix 线程默认堆栈大小      */
#endif

/*********************************************************************************************************
*                                        posix mqueue
*
* 依存关系: 1: SylixOS 线程的所有服务
            2: 各种信号量服务
*********************************************************************************************************/

#define LW_CFG_POSIX_MQ_MULTI_EN            1                           /*  是否有应用多个线程发送, 多个*/
                                                                        /*  线程接收同一消息队列, 如果无*/
                                                                        /*  可设为 0 提高效率, 这种应用 */
                                                                        /*  很少见, 为了兼容性, 这里默认*/
                                                                        /*  设置为 1                    */
/*********************************************************************************************************
*                                        posix 异步 I/O 兼容库
*
* 依存关系: 1: SylixOS 线程的所有服务
            2: 各种信号量服务
            3: 信号支持
            4: 类 POSIX 线程高级操作功能管理
*********************************************************************************************************/

#define LW_CFG_POSIX_AIO_EN                 1                           /*  是否使能 AIO                */
#if LW_CFG_CPU_WORD_LENGHT == 32
#define LW_CFG_POSIX_AIO_STK_SIZE           (8 * LW_CFG_KB_SIZE)        /*  aio 代理线程默认堆栈大小    */
#else
#define LW_CFG_POSIX_AIO_STK_SIZE           (16 * LW_CFG_KB_SIZE)       /*  aio 代理线程默认堆栈大小    */
#endif
#define LW_CFG_POSIX_AIO_MAX_THREAD         5                           /*  posix aio 最多的代理线程数量*/

/*********************************************************************************************************
*                                        posix syslog
*
* 依存关系: 1: SylixOS 线程的所有服务
            2: 各种信号量服务
            3: 网络支持
*********************************************************************************************************/

#define LW_CFG_POSIX_SYSLOG_EN              1                           /*  是否使能 syslog             */

#endif                                                                  /*  __POSIX_CFG_H               */
/*********************************************************************************************************
  END
*********************************************************************************************************/
