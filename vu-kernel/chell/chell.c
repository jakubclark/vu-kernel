#include "chell/commands/ata_cmd.h"
#include "chell/commands/colors_cmd.h"
#include "chell/commands/memtest_cmd.h"
#include "chell/commands/multiboot_cmd.h"
#include "chell/commands/vfs_cmd.h"
#include "drivers/pci/pci.h"
#include "io/keyboard.h"
#include "io/scrn.h"
#include "memory/memutil.h"
#include "memory/paging.h"
#include "memory/physmem.h"
#include "routines.h"
#include "std/colors.h"
#include "std/string.h"
#include "std/types.h"

uint8_t IN_SHELL = 0;

extern uint8_t keyboard_char;
extern uint32_t min_x;

uint8_t *chell_logo =
    (uint8_t *)"---------------------------------------------------------------"
               "-----------------"
               "|  _____ _          _ _ \t\t\t\t\t\t\t\t\t\t\t\t\t  |\n"
               "| /  __ \\ |        | | |\t\t\t\t\t\t\t\t\t\t\t\t\t  |\n"
               "| | /  \\/ |__   ___| | |\t\t\t\t\t\t\t\t\t\t\t\t\t  |\n"
               "| | |   | '_ \\ / _ \\ | |\t\t\t\t\t\t\t\t\t\t\t\t\t  |\n"
               "| | \\__/\\ | | |  __/ | |\t\t\t\t\t\t\t\t\t\t\t\t\t  |\n"
               "|  \\____/_| |_|\\___|_|_|\t\t\t\t\t\t\t\t\t\t\t\t\t  |\n"
               "---------------------------------------------------------------"
               "----------------\n";

/* Prints "user@vu" */
void print_prompt() {
  puts_col((uint8_t *)"user", LIGHTBLUE, DEFAULTBACKGROUND);
  puts_col((uint8_t *)"@", GREEN, DEFAULTBACKGROUND);
  puts_col((uint8_t *) vfs_cmd_pwd(), RED, DEFAULTBACKGROUND);
  puts_col((uint8_t *) ">", RED, DEFAULTBACKGROUND);
}

/* Prints the Chell logo */
void print_logo() { puts_col(chell_logo, RED, DEFAULTBACKGROUND); }

/* Prints the `help` command */
void print_help() {
  println("Available commands:");
  puts_col((uint8_t *)"\thelp \t|\t clear     \t|\t colors\n"
                      "\tlogo \t|\t multiboot \t|\t mem\n"
                      "\tata  \t|\t pci",
           BROWN, DEFAULTBACKGROUND);
}

/* Prints a welcome screen, to chell! */
void print_welcome() {
  // print_logo();

  for (uint8_t i = 0; i < 80; i++)
    putcharCol('-', DEFAULTBACKGROUND << 4 | DEFAULTFOREGROUND);

  puts_col((uint8_t *)"Welcome to ", DEFAULTFOREGROUND, DEFAULTBACKGROUND);
  puts_col((uint8_t *)"Chell", RED, DEFAULTBACKGROUND);
  puts_col((uint8_t *)", a barebones shell!\n", DEFAULTFOREGROUND,
           DEFAULTBACKGROUND);
  puts_col((uint8_t *)"help", BROWN, DEFAULTBACKGROUND);
  puts_col((uint8_t *)": list available commands\n", DEFAULTFOREGROUND,
           DEFAULTBACKGROUND);

  for (uint8_t i = 0; i < 80; i++) {
    putchar('-');
  }
  print_prompt();
}

void chell_main() {
  uint8_t char_;

  uint8_t cmd_buffer[256];
  uint8_t cmd_buffer_index = 0;
  cmd_buffer[cmd_buffer_index] = '\0';

  IN_SHELL = 1;

  /* The len of "user@vu>"*/
  min_x = strlen("user@") + strlen((char*)  vfs_cmd_pwd);
  print_welcome();

  while (1) {
  loop:
    char_ = get_char();
    disable();
    putchar(char_);

    if (char_ == '\n') {
      cmd_buffer[cmd_buffer_index] = '\0';

      if (cmd_buffer_index == 0)
        goto prompt;

      if (strcmp(cmd_buffer, (uint8_t *)"help") == 0) {
        print_help();
        goto cmd_end;
      }

      if (strcmp(cmd_buffer, (uint8_t *)"multiboot") == 0) {
        print_multiboot_info();
        goto cmd_end;
      }

      if (strcmp(cmd_buffer, (uint8_t *)"clear") == 0) {
        clear();
        goto prompt;
      }

      if (strcmp(cmd_buffer, (uint8_t *)"logo") == 0) {
        print_logo();
        goto cmd_end;
      }

      if (strcmp(cmd_buffer, (uint8_t *)"pci") == 0) {
        pci_enum_buses();
        goto cmd_end;
      }

      if (strcmp(cmd_buffer, (uint8_t *)"ata") == 0) {
        ata_cmd_main();
        goto cmd_end;
      }

      if (strcmp(cmd_buffer, (uint8_t *)"vfs") == 0) {
        vfs_cmd_main();
        goto cmd_end;
      }

      if (strcmp(cmd_buffer, (uint8_t *)"ls") == 0) {
        vfs_cmd_ls();
        goto cmd_end;
      }

      if (strcmp(cmd_buffer, (uint8_t *)"mem") == 0) {
        mem_cmd_main();
        goto cmd_end;
      }

      if(strcmp(cmd_buffer, (uint8_t *)"pwd") == 0){
        char* res =  vfs_cmd_pwd();
        printf("%s", res);
        goto cmd_end;
      }

      if ((cmd_buffer[0] == 'c') && (cmd_buffer[1] == 'd')){
        int res = vfs_cmd_cd((char*) cmd_buffer+3);

        if(res == 1){
          printf("%s exists\n", cmd_buffer+3);
        }else{
          printf("%s does not exist\n", cmd_buffer+3);
        }

        goto cmd_end;
      }

      if (strcmp(cmd_buffer, (uint8_t *)"colors") == 0) {
        print_colored_text();
        goto cmd_end;
      }

      printf("UNKOWN COMMAND: `%s`", cmd_buffer);
      goto cmd_end;
    }

    if (char_ == '\b') {
      cmd_buffer[cmd_buffer_index] = '\0';
      if (cmd_buffer_index != 0) {
        cmd_buffer_index--;
      }
      goto end;
    }

    if (cmd_buffer_index < 255) {
      cmd_buffer[cmd_buffer_index] = char_;
      cmd_buffer_index++;
    } else {
      printf("\nCommand buffer overflow, please re-enter your command");
      goto cmd_end;
    }

    goto end;

  cmd_end:
    println("");
    cmd_buffer_index = 0;
    memset(cmd_buffer, 0, 256);
    goto prompt;

  prompt:
    print_prompt();
    cmd_buffer_index = 0;
    goto end;

  end:
    keyboard_char = NULL;
    enable();
    goto loop;
  }
  IN_SHELL = 0;
}
