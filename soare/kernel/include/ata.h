#pragma once
#include "defs.h"

#define ATA_CMD_READ_PIO	0x20
#define ATA_CMD_WRITE_PIO	0x30
#define ATA_CMD_IDENTIFY	0xec
#define ATA_CMD_CACHE_FLUSH 0xE7

#define ATA_MASTER	0x00
#define ATA_SLAVE   0x01

#define ATA_STS_BSY	 0x80 // Busy.
#define ATA_STS_DRDY 0x40 // Device Ready.
#define ATA_STS_DRQ  0x08 // Data Request.

#define ATA_CTR_SRST 0x04 // Software Reset.

#define ATA_DEV_MBS 0xa0 // Must be set.
#define ATA_DEV_LBA 0x40 // Linear based addressing.
#define ATA_DEV_DEV 0x10 // Select device: 0 = master, 1 = slave.

#define ATA_CHANNEL_CNT 1

#define ATA_PRIMARY_BUS   0x1F0
#define ATA_SECONDARY_BUS 0x170

#define ATA_REG_DATA       0x00
#define ATA_REG_ERROR      0x01
#define ATA_REG_FEATURES   0x01
#define ATA_REG_SECCOUNT0  0x02
#define ATA_REG_LBA0       0x03
#define ATA_REG_LBA1       0x04
#define ATA_REG_LBA2       0x05
#define ATA_REG_HDDEVSEL   0x06
#define ATA_REG_COMMAND    0x07
#define ATA_REG_STATUS     0x07
#define ATA_REG_SECCOUNT1  0x08
#define ATA_REG_LBA3       0x09
#define ATA_REG_LBA4       0x0A
#define ATA_REG_LBA5       0x0B
#define ATA_REG_CONTROL    0x0C
#define ATA_REG_ALTSTATUS  0x0C
#define ATA_REG_DEVADDRESS 0x0D

#define SECTOR_SIZE 512

struct ata_disk
{
	char name[8];			 // e.g. "hd0"
	struct channel *channel;
	int type;			     // master / slave
};
typedef struct ata_disk ata_disk_t;

struct channel
{
	char name[8];            // e.g. "ide0"
	uint16_t reg_base;         // Base I/O port.
	uint8_t irq;               // Interrupt in use.
	ata_disk_t devices[2];   // Devices on this channel
};
typedef struct channel channel_t;

void ata_init(void);
void ata_read(uint16_t bus, uint32_t lba, uint8_t *buf);
void ata_write(uint16_t bus, uint32_t lba, uint8_t *buf);
