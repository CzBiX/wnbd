/*
 * Copyright (c) 2019 SUSE LLC
 *
 * Licensed under LGPL-2.1 (see LICENSE)
 */

#ifndef UTIL_H
#define UTIL_H 1

#include "common.h"
#include "userspace.h"
#include "rbd_protocol.h"

static const int MultiplyDeBruijnBitPosition2[32] =
{
  0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
  31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
};

VOID
WnbdDeviceCleanerThread(_In_ PVOID Context);

VOID
WnbdDeleteScsiInformation(_In_ PVOID ScsiInformation);

PWNBD_SCSI_DEVICE
WnbdFindDevice(_In_ PWNBD_LU_EXTENSION LuExtension,
               _In_ PWNBD_EXTENSION DeviceExtension,
               _In_ PSCSI_REQUEST_BLOCK Srb);

typedef struct _SRB_QUEUE_ELEMENT {
    LIST_ENTRY Link;
    PSCSI_REQUEST_BLOCK Srb;
    UINT64 StartingLbn;
    ULONG ReadLength;
    BOOLEAN FUA;
    PVOID DeviceExtension;
    UINT64 Tag;
    BOOLEAN Aborted;
} SRB_QUEUE_ELEMENT, * PSRB_QUEUE_ELEMENT;

VOID
WnbdDeviceRequestThread(_In_ PVOID Context);
#pragma alloc_text (PAGE, WnbdDeviceRequestThread)
VOID
WnbdDeviceReplyThread(_In_ PVOID Context);
#pragma alloc_text (PAGE, WnbdDeviceReplyThread)
BOOLEAN
IsReadSrb(_In_ PSCSI_REQUEST_BLOCK Srb);
VOID
WnbdProcessDeviceThreadReplies(_In_ PSCSI_DEVICE_INFORMATION DeviceInformation);
VOID CloseConnection(_In_ PSCSI_DEVICE_INFORMATION DeviceInformation);
VOID DisconnectConnection(_In_ PSCSI_DEVICE_INFORMATION DeviceInformation);
int ScsiOpToNbdReqType(_In_ int ScsiOp);
BOOLEAN ValidateScsiRequest(
    _In_ PSCSI_DEVICE_INFORMATION DeviceInformation,
    _In_ PSRB_QUEUE_ELEMENT Element);


#define LIST_FORALL_SAFE(_headPtr, _itemPtr, _nextPtr)                \
    for (_itemPtr = (_headPtr)->Flink, _nextPtr = (_itemPtr)->Flink;  \
         _itemPtr != _headPtr;                                        \
         _itemPtr = _nextPtr, _nextPtr = (_itemPtr)->Flink)

#endif
