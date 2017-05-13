// ==============================================================
// File generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2016.4
// Copyright (C) 1986-2017 Xilinx, Inc. All Rights Reserved.
// 
// ==============================================================

/***************************** Include Files *********************************/
#include "xsha256.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XSha256_CfgInitialize(XSha256 *InstancePtr, XSha256_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Axilites_BaseAddress = ConfigPtr->Axilites_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XSha256_Start(XSha256 *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XSha256_ReadReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_AP_CTRL) & 0x80;
    XSha256_WriteReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_AP_CTRL, Data | 0x01);
}

u32 XSha256_IsDone(XSha256 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XSha256_ReadReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XSha256_IsIdle(XSha256 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XSha256_ReadReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XSha256_IsReady(XSha256 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XSha256_ReadReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XSha256_EnableAutoRestart(XSha256 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XSha256_WriteReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_AP_CTRL, 0x80);
}

void XSha256_DisableAutoRestart(XSha256 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XSha256_WriteReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_AP_CTRL, 0);
}

void XSha256_Set_base_offset(XSha256 *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XSha256_WriteReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_BASE_OFFSET_DATA, Data);
}

u32 XSha256_Get_base_offset(XSha256 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XSha256_ReadReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_BASE_OFFSET_DATA);
    return Data;
}

void XSha256_Set_bytes(XSha256 *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XSha256_WriteReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_BYTES_DATA, Data);
}

u32 XSha256_Get_bytes(XSha256 *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XSha256_ReadReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_BYTES_DATA);
    return Data;
}

u32 XSha256_Get_data_BaseAddress(XSha256 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axilites_BaseAddress + XSHA256_AXILITES_ADDR_DATA_BASE);
}

u32 XSha256_Get_data_HighAddress(XSha256 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axilites_BaseAddress + XSHA256_AXILITES_ADDR_DATA_HIGH);
}

u32 XSha256_Get_data_TotalBytes(XSha256 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XSHA256_AXILITES_ADDR_DATA_HIGH - XSHA256_AXILITES_ADDR_DATA_BASE + 1);
}

u32 XSha256_Get_data_BitWidth(XSha256 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XSHA256_AXILITES_WIDTH_DATA;
}

u32 XSha256_Get_data_Depth(XSha256 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XSHA256_AXILITES_DEPTH_DATA;
}

u32 XSha256_Write_data_Words(XSha256 *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XSHA256_AXILITES_ADDR_DATA_HIGH - XSHA256_AXILITES_ADDR_DATA_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Axilites_BaseAddress + XSHA256_AXILITES_ADDR_DATA_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XSha256_Read_data_Words(XSha256 *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XSHA256_AXILITES_ADDR_DATA_HIGH - XSHA256_AXILITES_ADDR_DATA_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Axilites_BaseAddress + XSHA256_AXILITES_ADDR_DATA_BASE + (offset + i)*4);
    }
    return length;
}

u32 XSha256_Write_data_Bytes(XSha256 *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XSHA256_AXILITES_ADDR_DATA_HIGH - XSHA256_AXILITES_ADDR_DATA_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Axilites_BaseAddress + XSHA256_AXILITES_ADDR_DATA_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XSha256_Read_data_Bytes(XSha256 *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XSHA256_AXILITES_ADDR_DATA_HIGH - XSHA256_AXILITES_ADDR_DATA_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Axilites_BaseAddress + XSHA256_AXILITES_ADDR_DATA_BASE + offset + i);
    }
    return length;
}

u32 XSha256_Get_digest_BaseAddress(XSha256 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axilites_BaseAddress + XSHA256_AXILITES_ADDR_DIGEST_BASE);
}

u32 XSha256_Get_digest_HighAddress(XSha256 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axilites_BaseAddress + XSHA256_AXILITES_ADDR_DIGEST_HIGH);
}

u32 XSha256_Get_digest_TotalBytes(XSha256 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XSHA256_AXILITES_ADDR_DIGEST_HIGH - XSHA256_AXILITES_ADDR_DIGEST_BASE + 1);
}

u32 XSha256_Get_digest_BitWidth(XSha256 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XSHA256_AXILITES_WIDTH_DIGEST;
}

u32 XSha256_Get_digest_Depth(XSha256 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XSHA256_AXILITES_DEPTH_DIGEST;
}

u32 XSha256_Write_digest_Words(XSha256 *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XSHA256_AXILITES_ADDR_DIGEST_HIGH - XSHA256_AXILITES_ADDR_DIGEST_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Axilites_BaseAddress + XSHA256_AXILITES_ADDR_DIGEST_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XSha256_Read_digest_Words(XSha256 *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XSHA256_AXILITES_ADDR_DIGEST_HIGH - XSHA256_AXILITES_ADDR_DIGEST_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Axilites_BaseAddress + XSHA256_AXILITES_ADDR_DIGEST_BASE + (offset + i)*4);
    }
    return length;
}

u32 XSha256_Write_digest_Bytes(XSha256 *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XSHA256_AXILITES_ADDR_DIGEST_HIGH - XSHA256_AXILITES_ADDR_DIGEST_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Axilites_BaseAddress + XSHA256_AXILITES_ADDR_DIGEST_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XSha256_Read_digest_Bytes(XSha256 *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XSHA256_AXILITES_ADDR_DIGEST_HIGH - XSHA256_AXILITES_ADDR_DIGEST_BASE + 1))
        return 0;

    printk(KERN_INFO "to [0x%X] from [0x%X]\n",data,InstancePtr->Axilites_BaseAddress+XSHA256_AXILITES_ADDR_DIGEST_BASE+offset);
    printk(KERN_INFO "leggo..\n");
    //memcpy_fromio((void*)data, (void *)(InstancePtr->Axilites_BaseAddress + XSHA256_AXILITES_ADDR_DIGEST_BASE + offset), 8);
    printk(KERN_INFO "done..\n");
    //for (i = 0; i < length; i++) {
    //   printk(KERN_INFO "reading data at 0x%X\n",data+i); 
    //   memcpy_toio(data, void *source, unsigned int count);
    //    *(data + i) = *(char *)(InstancePtr->Axilites_BaseAddress + XSHA256_AXILITES_ADDR_DIGEST_BASE + offset + i);
    //}
    return length;
}

void XSha256_InterruptGlobalEnable(XSha256 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XSha256_WriteReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_GIE, 1);
}

void XSha256_InterruptGlobalDisable(XSha256 *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XSha256_WriteReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_GIE, 0);
}

void XSha256_InterruptEnable(XSha256 *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XSha256_ReadReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_IER);
    XSha256_WriteReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_IER, Register | Mask);
}

void XSha256_InterruptDisable(XSha256 *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XSha256_ReadReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_IER);
    XSha256_WriteReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_IER, Register & (~Mask));
}

void XSha256_InterruptClear(XSha256 *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XSha256_WriteReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_ISR, Mask);
}

u32 XSha256_InterruptGetEnabled(XSha256 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XSha256_ReadReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_IER);
}

u32 XSha256_InterruptGetStatus(XSha256 *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XSha256_ReadReg(InstancePtr->Axilites_BaseAddress, XSHA256_AXILITES_ADDR_ISR);
}

