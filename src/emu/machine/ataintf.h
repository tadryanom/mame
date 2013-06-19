/***************************************************************************

    ataintf.h

    ATA Interface implementation.

    Copyright Nicola Salmoria and the MAME Team.
    Visit http://mamedev.org for licensing and usage restrictions.

***************************************************************************/

#pragma once

#ifndef __ATAINTF_H__
#define __ATAINTF_H__

#include "atadev.h"

/***************************************************************************
    TYPE DEFINITIONS
***************************************************************************/

// ======================> ata_slot_device

class ata_slot_device : public device_t,
						public device_slot_interface
{
public:
	// construction/destruction
	ata_slot_device(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock);

	ata_device_interface *dev() { return m_dev; }
protected:
	// device-level overrides
	virtual void device_start();
	virtual void device_config_complete();
private:
	ata_device_interface *m_dev;
};

// device type definition
extern const device_type ATA_SLOT;

/***************************************************************************
    TYPE DEFINITIONS
***************************************************************************/

#define MCFG_ATA_INTERFACE_IRQ_HANDLER(_devcb) \
	devcb = &ata_interface_device::set_irq_handler(*device, DEVCB2_##_devcb);

#define MCFG_ATA_INTERFACE_DMARQ_HANDLER(_devcb) \
	devcb = &ata_interface_device::set_dmarq_handler(*device, DEVCB2_##_devcb);

SLOT_INTERFACE_EXTERN(ata_devices);

/***************************************************************************
    DEVICE CONFIGURATION MACROS
***************************************************************************/

#define MCFG_ATA_INTERFACE_ADD(_tag, _slotintf, _master, _slave, _fixed) \
	MCFG_DEVICE_ADD(_tag, ATA_INTERFACE, 0) \
	MCFG_ATA_SLOT_ADD(_tag ":0", _slotintf, _master, _fixed) \
	MCFG_ATA_SLOT_ADD(_tag ":1", _slotintf, _slave, _fixed) \
	MCFG_DEVICE_MODIFY(_tag)

#define MCFG_ATA_SLOT_ADD(_tag, _slot_intf, _def_slot, _fixed) \
	MCFG_DEVICE_ADD(_tag, ATA_SLOT, 0) \
	MCFG_DEVICE_SLOT_INTERFACE(_slot_intf, _def_slot, _fixed)

/***************************************************************************
    TYPE DEFINITIONS
***************************************************************************/

/* ----- device interface ----- */

class ata_interface_device : public device_t
{
public:
	ata_interface_device(const machine_config &mconfig, const char *tag, device_t *owner, UINT32 clock);
	ata_interface_device(const machine_config &mconfig, device_type type, const char *name, const char *tag, device_t *owner, UINT32 clock);

	// static configuration helpers
	template<class _Object> static devcb2_base &set_irq_handler(device_t &device, _Object object) { return downcast<ata_interface_device &>(device).m_irq_handler.set_callback(object); }
	template<class _Object> static devcb2_base &set_dmarq_handler(device_t &device, _Object object) { return downcast<ata_interface_device &>(device).m_dmarq_handler.set_callback(object); }

	UINT8 *identify_device_buffer(int drive);
	void set_master_password(int drive, const UINT8 *password);
	void set_user_password(int drive, const UINT8 *password);

	UINT16 read_dma();
	virtual DECLARE_READ16_MEMBER(read_cs0);
	virtual DECLARE_READ16_MEMBER(read_cs1);

	void write_dma(UINT16 data);
	virtual DECLARE_WRITE16_MEMBER(write_cs0);
	virtual DECLARE_WRITE16_MEMBER(write_cs1);
	DECLARE_WRITE_LINE_MEMBER(write_dmack);

protected:
	// device-level overrides
	virtual void device_start();
	virtual void device_reset();

	virtual void set_irq(int state);
	virtual void set_dmarq(int state);

private:
	DECLARE_WRITE_LINE_MEMBER(irq0_write_line);
	DECLARE_WRITE_LINE_MEMBER(dmarq0_write_line);

	DECLARE_WRITE_LINE_MEMBER(irq1_write_line);
	DECLARE_WRITE_LINE_MEMBER(dmarq1_write_line);

	ata_slot_device *m_slot[2];
	int m_irq[2];
	int m_dmarq[2];

	devcb2_write_line m_irq_handler;
	devcb2_write_line m_dmarq_handler;
};

extern const device_type ATA_INTERFACE;

#endif  /* __ATAINTF_H__ */
