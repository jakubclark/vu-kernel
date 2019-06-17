#ifndef __BUS_H
#define __BUS_H

#include "std/types.h"

extern void pci_enum_buses(void);
extern void pci_check_bus(uint8_t bus);
extern void pci_check_slot(uint8_t bus, uint8_t slot);
extern uint32_t pci_read_config_register(uint8_t bus, uint8_t slot,
                                       uint8_t function, uint8_t registerNo);

#endif
