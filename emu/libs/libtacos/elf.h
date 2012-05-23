#ifndef __ELF_H__
#define __ELF_H__

#ifdef __cplusplus
extern "C" {
#endif

int exec_elf(char* cmdline, int orphan __attribute__((unused)));

#ifdef __cplusplus
}
#endif

#endif //__ELF_H__
