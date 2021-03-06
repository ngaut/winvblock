/**
 * Copyright (C) 2009-2011, Shao Miller <shao.miller@yrdsb.edu.on.ca>.
 * Copyright 2006-2008, V.
 * For WinAoE contact information, see http://winaoe.org/
 *
 * This file is part of WinVBlock, derived from WinAoE.
 *
 * WinVBlock is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WinVBlock is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WinVBlock.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef WVL_M_BUS_H_
#  define WVL_M_BUS_H_

/**
 * @file
 *
 * Bus specifics.
 */

/* Forward declaration. */
struct WVL_BUS_T;

/**
 * A bus PnP routine.
 *
 * @v bus               The bus to receive the PnP IRP.
 * @v irp               The IRP to process.
 * @ret NTSTATUS        The status of the operation.
 */
typedef NTSTATUS STDCALL WVL_F_BUS_PNP(IN struct WVL_BUS_T *, IN PIRP);
typedef WVL_F_BUS_PNP * WVL_FP_BUS_PNP;

/* Device state. */
typedef enum WVL_BUS_STATE {
    WvlBusStateNotStarted,
    WvlBusStateStarted,
    WvlBusStateStopPending,
    WvlBusStateStopped,
    WvlBusStateRemovePending,
    WvlBusStateSurpriseRemovePending,
    WvlBusStateDeleted,
    WvlBusStates
  } WVL_E_BUS_STATE, * WVL_EP_BUS_STATE;

/* The bus type. */
typedef struct WVL_BUS_T {
    PDEVICE_OBJECT LowerDeviceObject;
    PDEVICE_OBJECT Pdo;
    PDEVICE_OBJECT Fdo;
    WVL_E_BUS_STATE OldState;
    WVL_E_BUS_STATE State;
    WVL_FP_BUS_PNP QueryDevText;
    struct {
        LIST_ENTRY Nodes;
        KSPIN_LOCK NodeLock;
        KIRQL NodeLockIrql;
        USHORT NodeCount;
      } BusPrivate_;
  } WVL_S_BUS_T, * WVL_SP_BUS_T;

/* A child PDO node on a bus.  Treat this as an opaque type. */
typedef struct WVL_BUS_NODE {
    struct {
        LIST_ENTRY Link;
        PDEVICE_OBJECT Pdo;
        WVL_SP_BUS_T Bus;
        /* The child's unit number relative to the parent bus. */
        UINT32 Num;
      } BusPrivate_;
    BOOLEAN Linked;
  } WVL_S_BUS_NODE, * WVL_SP_BUS_NODE;

/* Exports. */
extern WVL_M_LIB VOID WvlBusInit(WVL_SP_BUS_T);
extern WVL_M_LIB VOID STDCALL WvlBusClear(IN WVL_SP_BUS_T);
extern WVL_M_LIB VOID WvlBusLock(IN WVL_SP_BUS_T);
extern WVL_M_LIB VOID WvlBusUnlock(IN WVL_SP_BUS_T);
extern WVL_M_LIB BOOLEAN STDCALL WvlBusInitNode(
    OUT WVL_SP_BUS_NODE,
    IN PDEVICE_OBJECT
  );
extern WVL_M_LIB NTSTATUS STDCALL WvlBusAddNode(
    WVL_SP_BUS_T,
    WVL_SP_BUS_NODE
  );
extern WVL_M_LIB NTSTATUS STDCALL WvlBusRemoveNode(WVL_SP_BUS_NODE);
extern WVL_M_LIB UINT32 STDCALL WvlBusGetNodeNum(
    IN WVL_SP_BUS_NODE
  );
extern WVL_M_LIB WVL_SP_BUS_NODE STDCALL WvlBusGetNextNode(
    IN WVL_SP_BUS_T,
    IN WVL_SP_BUS_NODE
  );
extern WVL_M_LIB PDEVICE_OBJECT STDCALL WvlBusGetNodePdo(
    IN WVL_SP_BUS_NODE
  );
extern WVL_M_LIB UINT32 STDCALL WvlBusGetNodeCount(
    WVL_SP_BUS_T
  );
/* IRP-related. */
extern WVL_M_LIB NTSTATUS STDCALL WvlBusSysCtl(
    IN WVL_SP_BUS_T,
    IN PIRP
  );
extern WVL_M_LIB NTSTATUS STDCALL WvlBusPower(
    IN WVL_SP_BUS_T,
    IN PIRP
  );
/* IRP_MJ_PNP dispatcher in libbus/pnp.c */
extern WVL_M_LIB NTSTATUS STDCALL WvlBusPnp(
    IN WVL_SP_BUS_T,
    IN PIRP
  );

#endif  /* WVL_M_BUS_H_ */
