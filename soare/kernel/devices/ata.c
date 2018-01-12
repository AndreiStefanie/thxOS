#include "ata.h"
#include "screen.h"
#include "isr.h"
#include "timer.h"

static channel_t *get_channel_for_bus(uint16_t bus);

static channel_t channels[ATA_CHANNEL_CNT];

static void send_pio(const channel_t *channel, uint16_t reg, uint8_t command)
{
	__outbyte(channel->reg_base + reg, command);
}

static void ata_callback(const interrupt_context_t *context)
{
	UNUSED(context);
}

static void ata_wait_for_idle(const channel_t *channel)
{
	while (__inbyte(channel->reg_base + ATA_REG_STATUS) & ATA_STS_BSY)
	{
	};
}

void ata_io_wait(const channel_t *channel)
{
	__inbyte(channel->reg_base + ATA_REG_ALTSTATUS);
	__inbyte(channel->reg_base + ATA_REG_ALTSTATUS);
	__inbyte(channel->reg_base + ATA_REG_ALTSTATUS);
	__inbyte(channel->reg_base + ATA_REG_ALTSTATUS);
}

void ata_init()
{
	channels[0].reg_base = ATA_PRIMARY_BUS;
	channels[0].irq = IDE_1;

	if (ATA_CHANNEL_CNT > 1)
	{
		channels[1].reg_base = ATA_SECONDARY_BUS;
		channels[1].irq = IDE_2;
	}

	for (size_t ch = 0; ch < ATA_CHANNEL_CNT; ch++)
	{
		channel_t *channel = &channels[ch];

		//snprintf(channel->name, sizeof(channel->name), "ide%u", (unsigned)ch);

		for (size_t dev = 0; dev < 2; dev++)
		{
			ata_disk_t *disk = &channel->devices[dev];

			//snprintf(disk->name, sizeof(disk->name), "hd%c", (char)('a' + ch * 2 + dev));
			disk->channel = channel;
			disk->type = dev == 0 ? ATA_MASTER : ATA_SLAVE;
		}

		register_interrupt_handler(channel->irq, &ata_callback);

		send_pio(channel, ATA_REG_HDDEVSEL, 0xa0);

		send_pio(channel, ATA_REG_SECCOUNT0, 0);
		send_pio(channel, ATA_REG_LBA0, 0);
		send_pio(channel, ATA_REG_LBA1, 0);
		send_pio(channel, ATA_REG_LBA2, 0);
		send_pio(channel, ATA_REG_COMMAND, ATA_CMD_IDENTIFY);

		ata_io_wait(channel);

		/*PrintString("Bus status: ");
		PrintInt(__inbyte(channel->reg_base + ATA_REG_COMMAND));
		PrintChar('\n');*/

		ata_wait_for_idle(channel);

		// TODO - identify
	}
}

void ata_read(uint16_t bus, uint32_t lba, uint8_t *buf)
{
	channel_t *channel = get_channel_for_bus(bus);
	send_pio(channel, ATA_REG_CONTROL, 0x02);

	ata_wait_for_idle(channel);

	send_pio(channel, ATA_REG_HDDEVSEL,
		(uint8_t)(0xe0 | (lba & 0x0f000000) >> 24));

	send_pio(channel, ATA_REG_FEATURES, 0x00);
	send_pio(channel, ATA_REG_SECCOUNT0, 1);
	send_pio(channel, ATA_REG_LBA0, (lba & 0x000000ff) >> 0);
	send_pio(channel, ATA_REG_LBA1, (lba & 0x0000ff00) >> 8);
	send_pio(channel, ATA_REG_LBA2, (uint8_t)((lba & 0x00ff0000) >> 16));
	send_pio(channel, ATA_REG_COMMAND, ATA_CMD_READ_PIO);

	__inbytestring(channel->reg_base, buf, SECTOR_SIZE);
	ata_wait_for_idle(channel);
}

void ata_write(uint16_t bus, uint32_t lba, uint8_t *buf)
{
	channel_t *channel = get_channel_for_bus(bus);
	send_pio(channel, ATA_REG_CONTROL, 0x02);

	ata_wait_for_idle(channel);

	send_pio(channel, ATA_REG_HDDEVSEL, 0xe0 | (lba & 0x0f000000) >> 24);

	send_pio(channel, ATA_REG_FEATURES, 0x00);
	send_pio(channel, ATA_REG_SECCOUNT0, 0x01);
	send_pio(channel, ATA_REG_LBA0, (lba & 0x000000ff) >> 0);
	send_pio(channel, ATA_REG_LBA1, (lba & 0x0000ff00) >> 8);
	send_pio(channel, ATA_REG_LBA2, (uint8_t)((lba & 0x00ff0000) >> 16));
	send_pio(channel, ATA_REG_COMMAND, ATA_CMD_WRITE_PIO);

	__outbytestring(channel->reg_base, buf, SECTOR_SIZE);
	send_pio(channel, ATA_REG_COMMAND, ATA_CMD_CACHE_FLUSH);
	ata_wait_for_idle(channel);
}

static channel_t *get_channel_for_bus(uint16_t bus)
{
	for (size_t i = 0; i < ATA_CHANNEL_CNT; i++)
	{
		if (bus == channels[i].reg_base)
		{
			return &channels[i];
		}
	}

	return NULL;
}
