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
** 文   件   名: ahciPm.h
**
** 创   建   人: Gong.YuJian (弓羽箭)
**
** 文件创建日期: 2016 年 03 月 31 日
**
** 描        述: AHCI 电源管理.
*********************************************************************************************************/

#ifndef __AHCI_PM_H
#define __AHCI_PM_H

/*********************************************************************************************************
  裁剪宏
*********************************************************************************************************/
#if (LW_CFG_DEVICE_EN > 0) && (LW_CFG_AHCI_EN > 0)

LW_API INT              API_AhciPmPowerModeGet(AHCI_CTRL_HANDLE hCtrl, UINT uiDrive, UINT8 *pucMode);
LW_API INT              API_AhciApmModeGet(AHCI_CTRL_HANDLE hCtrl, UINT uiDrive, UINT8 *pucMode);
LW_API INT              API_AhciApmDisable(AHCI_CTRL_HANDLE hCtrl, UINT uiDrive);
LW_API INT              API_AhciApmEnable(AHCI_CTRL_HANDLE hCtrl, UINT uiDrive, INT iApm);

LW_API INT              API_AhciPmActive(AHCI_CTRL_HANDLE hCtrl, UINT uiDrive);

#endif                                                                  /*  (LW_CFG_DEVICE_EN > 0) &&   */
                                                                        /*  (LW_CFG_AHCI_EN > 0)        */
#endif                                                                  /*  __AHCI_PM_H                 */
/*********************************************************************************************************
  END
*********************************************************************************************************/
