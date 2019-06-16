#ifndef __PCI_H
#define __PCI_H

#include "std/types.h"

typedef struct pci_bus_entry {
  uint8_t bus;
  uint8_t slot;
  uint8_t function;
  uint16_t vendorID;
  uint16_t deviceID;
  struct pciBus_Entry_s *next;
} pci_bus_entry_s;

extern uint32_t pci_read_config_register(uint8_t bus, uint8_t slot,
                                              uint8_t function,
                                              uint8_t registerNo);
extern void pci_enum_buses(void);
extern void pci_check_bus(uint8_t bus);
extern void pci_check_slot(uint8_t bus, uint8_t slot);

#endif
