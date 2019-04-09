/* We use inbyte() to read from the I/O ports (e.g., to get data from
 * the keyboard). */
unsigned char inbyte (unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

/* We use outbyte() to write to the I/O ports (e.g., the screen). */
void outbyte (unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

