#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_CODE_SEGMENT_OFFSET 0x08

/* Defined in boot.S */
extern void keyboard_handler(void);
/* Defined in boot.S */
extern void load_idt(unsigned long *idt_ptr);

struct IDT_entry {
  unsigned short int offset_lowerbits;
  unsigned short int selector;
  unsigned char zero;
  unsigned char type_attr;
  unsigned short int offset_higherbits;
};

struct IDT_entry IDT[IDT_SIZE];

/* Initialize the Interrupt Descriptor Table */
extern void idt_init(void);
