CC=gcc-7
SHARED_FLAGS = -fno-builtin -O2 -nostdinc -nostdlib -ffreestanding -g -Wall -Wextra \
               -Werror -I../include -MMD -mno-red-zone -fno-pie -m32
CFLAGS = $(SHARED_FLAGS)

OBJS := memutil.o virtualmemory.o
DFILES = $(patsubst %.o,%.d,$(OBJS))

all: $(OBJS)

clean:
	find -name "*~" -delete
	rm -rf $(OBJS) $(DFILES) kernel boot.o

$(OBJS): Makefile
-include $(DFILES)
