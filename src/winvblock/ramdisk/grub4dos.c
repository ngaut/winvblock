/**
 * Copyright (C) 2009-2011, Shao Miller <shao.miller@yrdsb.edu.on.ca>.
 *
 * This file is part of WinVBlock, originally derived from WinAoE.
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

/**
 * @file
 *
 * GRUB4DOS RAM disk specifics.
 */

#include <stdio.h>
#include <ntddk.h>

#include "portable.h"
#include "winvblock.h"
#include "wv_string.h"
#include "driver.h"
#include "bus.h"
#include "device.h"
#include "disk.h"
#include "ramdisk.h"
#include "debug.h"
#include "x86.h"
#include "probe.h"
#include "grub4dos.h"

/* From bus.c */
extern WVL_S_BUS_T WvBus;

/** Create a GRUB4DOS RAM disk and add it to the WinVBlock bus. */
VOID WvRamdiskCreateG4dDisk(
    WV_SP_GRUB4DOS_DRIVE_MAPPING slot,
    WVL_E_DISK_MEDIA_TYPE media_type,
    UINT32 sector_size
  ) {
    WV_SP_RAMDISK_T ramdisk;

    ramdisk = WvRamdiskCreatePdo(media_type);
    if (!ramdisk) {
        DBG("Could not create GRUB4DOS disk!\n");
        return;
      }
    DBG("RAM Drive is type: %d\n", media_type);

    ramdisk->DiskBuf = (UINT32) (slot->SectorStart * 512);
    ramdisk->disk->LBADiskSize = ramdisk->DiskSize =
      (UINT32) slot->SectorCount;
    ramdisk->disk->Heads = slot->MaxHead + 1;
    ramdisk->disk->Sectors = slot->DestMaxSector;
    ramdisk->disk->Cylinders = (
        ramdisk->disk->LBADiskSize /
        (ramdisk->disk->Heads * ramdisk->disk->Sectors)
      );
    ramdisk->disk->Media = media_type;
    ramdisk->disk->SectorSize = sector_size;
    ramdisk->Dev->Boot = TRUE;
     /* Add the ramdisk to the bus. */
    ramdisk->disk->ParentBus = WvBus.Fdo;
    if (!WvBusAddDev(ramdisk->Dev))
      WvDevFree(ramdisk->Dev);

    return;
  }
