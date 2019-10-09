/*********************************************************************************************************
**
**                                    �й�������Դ��֯
**
**                                   Ƕ��ʽʵʱ����ϵͳ
**
**                                SylixOS(TM)  LW : long wing
**
**                               Copyright All Rights Reserved
**
**--------------�ļ���Ϣ--------------------------------------------------------------------------------
**
** ��   ��   ��: pciNullDrv.h
**
** ��   ��   ��: Gong.YuJian (�����)
**
** �ļ���������: 2016 �� 06 �� 11 ��
**
** ��        ��: PCI NULL (ʾ��) �豸����.
*********************************************************************************************************/

#ifndef __PCINULLDRV_H
#define __PCINULLDRV_H

/*********************************************************************************************************
  ��������
*********************************************************************************************************/
#define PCI_NULL_DRV_NAME       "pci_nulldrv"                           /* PCI �豸������������         */
#define PCI_NULL_DRV_VER_NUM    0x01000102                              /* �����汾��ֵ V1.0.1-2        */

/*********************************************************************************************************
  ��������
*********************************************************************************************************/
INT  pciNullDrvRemove(VOID);
INT  pciNullDrvInit(VOID);

#endif                                                                  /*  __PCINULLDRV_H              */
/*********************************************************************************************************
  END
*********************************************************************************************************/