#include "std/types.h"
#include "io/basicio.h"
#include "pci/pci.h"
#include "io/scrn.h"

const char *classNames[] = {
    "Unknown",
    "Mass Storage Controller",
    "Network Controller",
    "Display Controller",
    "Multimedia Controller",
    "Memory Controller",
    "Bridge Device",
    "Simple Communications Controller",
    "Base System Peripherals",
    "Input Devices",
    "Docking Stations",
    "Processors",
    "Serial Bus Controllers",
    "Wireless Controllers",
    "Intelligent I/O Controllers",
    "Satellite Communication Controllers",
    "Encryption/Decryption Controllers",
    "Data Acquisition and Signal Processing Controllers",
    "Reserved"};

void pci_enum_buses(void) {
  for (uint16_t bus = 0; bus <= 255; bus++)
    pci_check_bus(bus);
}

void pci_check_bus(uint8_t bus) {
  for (uint8_t slot = 0; slot < 32; slot++)
    pci_check_slot(bus, slot);
}

void pci_check_slot(uint8_t bus, uint8_t slot) {
  uint8_t function = 0;

  for (function = 0; function < 32; function++) {
    uint32_t deviceAndVendor =
        pci_read_config_register(bus, slot, function, 0x00);

    if ((deviceAndVendor & 0xFFFF) == 0xFFFF)
      break;

    uint32_t classRegister =
        pci_read_config_register(bus, slot, function, 0x08);
    uint8_t class = (uint8_t)(classRegister >> 24);

    printf("PCI device at %d:%d:%d has vendor 0x%x and device 0x%x, of class %s\n",
           bus, slot, function, deviceAndVendor & 0xFFFF, deviceAndVendor >> 16,
           classNames[class]);
  }
}

uint32_t pci_read_config_register(uint8_t bus, uint8_t slot, uint8_t function,
                                  uint8_t registerNo) {
  uint32_t address = 0x80000000 | (uint32_t)((bus & 0xFF) << 16) |
                     (uint32_t)((slot & 0x1F) << 11) |
                     (uint32_t)((function & 0x5) << 8) |
                     (uint32_t)((registerNo & 0xFC));
  outword(0xCF8, address);
  // return inword(0xCFC);
  return inbyte(0xCFC);
}
