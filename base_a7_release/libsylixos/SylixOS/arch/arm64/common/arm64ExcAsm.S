;/*********************************************************************************************************
;**
;**                                    �й�������Դ��֯
;**
;**                                   Ƕ��ʽʵʱ����ϵͳ
;**
;**                                       SylixOS(TM)
;**
;**                               Copyright  All Rights Reserved
;**
;**--------------�ļ���Ϣ--------------------------------------------------------------------------------
;**
;** ��   ��   ��: arm64ExcAsm.S
;**
;** ��   ��   ��: Wang.Xuan (���Q)
;**
;** �ļ���������: 2018 �� 07 �� 05 ��
;**
;** ��        ��: ARM64 ��ϵ�����쳣����.
;*********************************************************************************************************/

#ifndef ASSEMBLY
#define ASSEMBLY 1
#endif

#include <arch/assembler.h>
#include "arch/arm64/arch_regs.h"
#include "arm64ContextAsm.h"

    FILE_BEGIN()

;/*********************************************************************************************************
;  ARM ��ϵ��������쳣��������
;*********************************************************************************************************/

    IMPORT_LABEL(API_ThreadTcbInter)
    IMPORT_LABEL(API_InterStackBaseGet)
    IMPORT_LABEL(API_InterEnter)
    IMPORT_LABEL(API_InterExit)

    IMPORT_LABEL(bspIntHandle)
    IMPORT_LABEL(archSyncExcHandle)
    IMPORT_LABEL(archInvalidExcHandle)

    EXPORT_LABEL(archEL1IrqEntry)
    EXPORT_LABEL(archEL1SyncExcEntry)
    EXPORT_LABEL(archEL1SyncInvalidEntry)
    EXPORT_LABEL(archEL1IrqInvalidEntry)
    EXPORT_LABEL(archEL1FiqInvalidEntry)
    EXPORT_LABEL(archEL1ErrInvalidEntry)
    EXPORT_LABEL(vector)

;/*********************************************************************************************************
;  �ж����
;*********************************************************************************************************/

FUNC_DEF(archEL1IrqEntry)
    ;/*
    ; * ʹ���쳣��ʱջ, �����쳣��ʱջ������ʱ�����ı�����, �� volatile �Ĵ������浽��ʱ�����ı�����
    ; * SP ָ���쳣��ʱջ
    ; */
    EXC_SAVE_VOLATILE

    BL      API_InterEnter

    ;/*
    ; * ������ǵ�һ�ν����ж�, ��ת
    ; */
    CMP     X0 , #1
    BNE     1f

    ;/*
    ; * ��õ�ǰ���� TCB �� ARCH_REG_CTX ��ַ
    ; */
    BL      API_ThreadTcbInter                                          ;/*  get current tcb             */

    ;/*
    ; * ���� volatile �Ĵ�������ǰ���� TCB �� ARCH_REG_CTX ��
    ; */
    EXC_COPY_VOLATILE

    ;/*
    ; * ���� non volatile �Ĵ�������ǰ���� TCB �� ARCH_REG_CTX ��
    ; */
    EXC_SAVE_NON_VOLATILE

    ;/*
    ; * ��һ�ν����ж�: ��õ�ǰ CPU �ж϶�ջջ��, ������ SP
    ; */
    BL      API_InterStackBaseGet
    MOV     SP , X0

2:
    ;/*
    ; * bspIntHandle()
    ; */
    BL      bspIntHandle

    ;/*
    ; * API_InterExit()
    ; * ���û�з����ж�Ƕ��, �� API_InterExit ����� archIntCtxLoad ����
    ; */
    BL      API_InterExit

    ;/*
    ; * ��������, ˵���������ж�Ƕ��
    ; */
    MOV     X18 , SP
    RESTORE_BIG_REG_CTX                                                 ;/*  �ָ����мĴ���              */

1:
    ;/*
    ; * ���ǵ�һ�ν����ж�
    ; */
    LDR     X0  , [SP, #XSP_OFFSET]                                     ;/*  ��ȡ�쳣ǰ SP               */
    SUB     X0  , X0 , ARCH_REG_CTX_SIZE                                ;/*  ���쳣��ջ���������ı�����  */

    ;/*
    ; * ���� volatile �Ĵ������쳣��ջ��������ı�����
    ; */
    EXC_COPY_VOLATILE

    ;/*
    ; * ���� non volatile �Ĵ������쳣��ջ��
    ; */
    EXC_SAVE_NON_VOLATILE

    MOV     SP , X0                                                     ;/*  ʹ���쳣��ջ                */
    B       2b
    FUNC_END()
    
;/*********************************************************************************************************
;  ͬ���쳣���
;*********************************************************************************************************/

FUNC_DEF(archEL1SyncExcEntry)
    ;/*
    ; * ʹ���쳣��ʱջ, �����쳣��ʱջ������ʱ�����ı�����, �� volatile �Ĵ������浽��ʱ�����ı�����
    ; * SP ָ���쳣��ʱջ
    ; */
    EXC_SAVE_VOLATILE

    BL      API_InterEnter
            
    ;/*
    ; * ������ǵ�һ�ν����ж�, ��ת
    ; */
    CMP     X0 , #1
    BNE     1f
    
    ;/*
    ; * ��õ�ǰ���� TCB �� ARCH_REG_CTX ��ַ
    ; */
    BL      API_ThreadTcbInter                                          ;/*  get current tcb             */

    ;/*
    ; * ���� volatile �Ĵ�������ǰ���� TCB �� ARCH_REG_CTX ��
    ; */
    EXC_COPY_VOLATILE

    ;/*
    ; * ���� non volatile �Ĵ�������ǰ���� TCB �� ARCH_REG_CTX ��
    ; */
    EXC_SAVE_NON_VOLATILE

    MOV     X18, X0
    ;/*
    ; * ��һ�ν����ж�: ��õ�ǰ CPU �ж϶�ջջ��, ������ SP
    ; */
    BL      API_InterStackBaseGet
    MOV     SP , X0

    MOV     X0 , X18
2:
    ;/*
    ; * archSyncExcHandle()
    ; */
    MRS     X1 , ESR_EL1    
    BL      archSyncExcHandle

    ;/*
    ; * API_InterExit()
    ; * ���û�з����ж�Ƕ��, �� API_InterExit ����� archIntCtxLoad ����
    ; */
    BL      API_InterExit

    ;/*
    ; * ��������, ˵���������ж�Ƕ��
    ; */
    MOV     X18 , SP
    RESTORE_BIG_REG_CTX                                                 ;/*  �ָ����мĴ���              */

1:
    ;/*
    ; * ���ǵ�һ�ν����ж�
    ; */
    LDR     X0  , [SP, #XSP_OFFSET]                                     ;/*  ��ȡ�쳣ǰ SP               */
    SUB     X0  , X0 , ARCH_REG_CTX_SIZE                                ;/*  ���쳣��ջ���������ı�����  */

    ;/*
    ; * ���� volatile �Ĵ������쳣��ջ��������ı�����
    ; */
    EXC_COPY_VOLATILE

    ;/*
    ; * ���� non volatile �Ĵ������쳣��ջ��
    ; */
    EXC_SAVE_NON_VOLATILE

    MOV     SP , X0                                                     ;/*  ʹ���쳣��ջ                */
    B       2b
    FUNC_END()

;/*********************************************************************************************************
;  Invalid �쳣���
;*********************************************************************************************************/

MACRO_DEF(ARCH_INVALID_EXC_ENTRY type)
    ;/*
    ; * ʹ���쳣��ʱջ, �����쳣��ʱջ������ʱ�����ı�����, �� volatile �Ĵ������浽��ʱ�����ı�����
    ; * SP ָ���쳣��ʱջ
    ; */
    EXC_SAVE_VOLATILE

    BL      API_InterEnter

    ;/*
    ; * ������ǵ�һ�ν����ж�, ��ת
    ; */
    CMP     X0 , #1
    BNE     1f

    ;/*
    ; * ��õ�ǰ���� TCB �� ARCH_REG_CTX ��ַ
    ; */
    BL      API_ThreadTcbInter                                          ;/*  get current tcb             */

    ;/*
    ; * ���� volatile �Ĵ�������ǰ���� TCB �� ARCH_REG_CTX ��
    ; */
    EXC_COPY_VOLATILE

    ;/*
    ; * ���� non volatile �Ĵ�������ǰ���� TCB �� ARCH_REG_CTX ��
    ; */
    EXC_SAVE_NON_VOLATILE

    MOV     X18, X0

    ;/*
    ; * ��һ�ν����ж�: ��õ�ǰ CPU �ж϶�ջջ��, ������ SP
    ; */
    BL      API_InterStackBaseGet
    MOV     SP , X0

    MOV     X0 , X18
2:
    ;/*
    ; * archInvalidExcHandle()
    ; */
    MRS     X1 , ESR_EL1
    MOV     X2 , \type
    BL      archInvalidExcHandle

    ;/*
    ; * ��������, ˵���������ж�Ƕ��
    ; */
    MOV     X18 , SP
    RESTORE_BIG_REG_CTX                                                 ;/*  �ָ����мĴ���              */

1:
    ;/*
    ; * ���ǵ�һ�ν����ж�
    ; */
    LDR     X0  , [SP, #XSP_OFFSET]                                     ;/*  ��ȡ�쳣ǰ SP               */
    SUB     X0  , X0 , ARCH_REG_CTX_SIZE                                ;/*  ���쳣��ջ���������ı�����  */

    ;/*
    ; * ���� volatile �Ĵ������쳣��ջ��������ı�����
    ; */
    EXC_COPY_VOLATILE

    ;/*
    ; * ���� non volatile �Ĵ������쳣��ջ��
    ; */
    EXC_SAVE_NON_VOLATILE

    MOV     SP , X0                                                     ;/*  ʹ���쳣��ջ                */
    B       2b
    MACRO_END()

FUNC_DEF(archEL1SyncInvalidEntry)
    ARCH_INVALID_EXC_ENTRY      EL1_SYN_INVALID
    FUNC_END()

FUNC_DEF(archEL1IrqInvalidEntry)
    ARCH_INVALID_EXC_ENTRY      EL1_IRQ_INVALID
    FUNC_END()

FUNC_DEF(archEL1FiqInvalidEntry)
    ARCH_INVALID_EXC_ENTRY      EL1_FIQ_INVALID
    FUNC_END()

FUNC_DEF(archEL1ErrInvalidEntry)
    ARCH_INVALID_EXC_ENTRY      EL1_ERR_INVALID
    FUNC_END()

FUNC_DEF(archEL2AArch64IrqEntry)
    ARCH_INVALID_EXC_ENTRY      EL2_IRQ_AARCH64_INVALID
    FUNC_END()

FUNC_DEF(archEL2AArch64FiqInvalidEntry)
    ARCH_INVALID_EXC_ENTRY      EL2_FIQ_AARCH64_INVALID
    FUNC_END()

FUNC_DEF(archEL2AArch64ErrInvalidEntry)
    ARCH_INVALID_EXC_ENTRY      EL2_ERR_AARCH64_INVALID
    FUNC_END()

FUNC_DEF(archEL2AArch32SyncExcEntry)
    ARCH_INVALID_EXC_ENTRY      EL2_SYN_AARCH32_INVALID
    FUNC_END()

FUNC_DEF(archEL2AArch32IrqEntry)
    ARCH_INVALID_EXC_ENTRY      EL2_IRQ_AARCH32_INVALID
    FUNC_END()

FUNC_DEF(archEL2AArch32FiqInvalidEntry)
    ARCH_INVALID_EXC_ENTRY      EL2_FIQ_AARCH32_INVALID
    FUNC_END()

FUNC_DEF(archEL2AArch32ErrInvalidEntry)
    ARCH_INVALID_EXC_ENTRY      EL2_ERR_AARCH32_INVALID
    FUNC_END()

;/*********************************************************************************************************
;  EL2 Sync �쳣�������
;*********************************************************************************************************/

FUNC_DEF(archEL2AArch64SyncExcEntry)
    MOV     X2 , X0                                                     ;/*  ��������� X2               */
    MRS     X3 , ELR_EL2                                                ;/*  ��¼�쳣���ص�ַ            */

    MRS     X0 , ESR_EL2
    LSR     X1 , X0, #26                                                ;/*  ��ȡ ESR �е� EC            */

    CMP     X1 , #0x16                                                  ;/*  �ж� HVC ��������           */
    B.NE    switchEL1                                                   ;/*  Ŀǰֻ���� HVC ����         */

    AND     X0 , X0, #0xff
    CMP     X0 , #HVC_CMD_FPU                                           ;/*  �ж��Ƿ��Ǵ��� FPU �Ĵ���   */
    B.NE    switchEL1                                                   ;/*  ������ǣ���ת�� EL1        */

    BL      arm64VfpV4El2FpuHandle                                      ;/*  ���� FPU ����               */

LINE_LABEL(switchEL1)                                                   ;/*  ���� EL1                    */
    ;/*
    ; * �� EL2 ��ת�� EL1 AARCH64
    ; */
    MSR     ELR_EL2  , X3                                               ;/*  �ûط��ص�ַ                */
    ERET
    FUNC_END()

;/*********************************************************************************************************
;  ����������
;*********************************************************************************************************/

MACRO_DEF(VENTRY label)
    .balign  128
    B      \label
MACRO_END()

    .balign  2048
FUNC_DEF(vector)
    VENTRY(archEL1SyncInvalidEntry)                                     ;/*  Sync  EL1t                  */
    VENTRY(archEL1IrqInvalidEntry)                                      ;/*  IRQ   EL1t                  */
    VENTRY(archEL1FiqInvalidEntry)                                      ;/*  FIQ   EL1t                  */
    VENTRY(archEL1ErrInvalidEntry)                                      ;/*  Error EL1t                  */

    VENTRY(archEL1SyncExcEntry)                                         ;/*  Sync  EL1h                  */
    VENTRY(archEL1IrqEntry)                                             ;/*  IRQ   EL1h                  */
    VENTRY(archEL1FiqInvalidEntry)                                      ;/*  FIQ   EL1h                  */
    VENTRY(archEL1ErrInvalidEntry)                                      ;/*  Error EL1h                  */

    VENTRY(archEL2AArch64SyncExcEntry)                                  ;/*  Sync  EL2 AArch64           */
    VENTRY(archEL2AArch64IrqEntry)                                      ;/*  IRQ   EL2 AArch64           */
    VENTRY(archEL2AArch64FiqInvalidEntry)                               ;/*  FIQ   EL2 AArch64           */
    VENTRY(archEL2AArch64ErrInvalidEntry)                               ;/*  Error EL2 AArch64           */

    VENTRY(archEL2AArch32SyncExcEntry)                                  ;/*  Sync  EL2 AArch32           */
    VENTRY(archEL2AArch32IrqEntry)                                      ;/*  IRQ   EL2 AArch32           */
    VENTRY(archEL2AArch32FiqInvalidEntry)                               ;/*  FIQ   EL2 AArch32           */
    VENTRY(archEL2AArch32ErrInvalidEntry)                               ;/*  Error EL2 AArch32           */
    FUNC_END()

    FILE_END()
;/*********************************************************************************************************
;  END
;*********************************************************************************************************/
