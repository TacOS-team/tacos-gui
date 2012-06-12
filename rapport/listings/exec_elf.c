int exec_elf(char* cmdline, int orphan __attribute__((unused))) {
  char cmdline2[strlen(cmdline) + 3];
  if (strcpy(cmdline2, cmdline) == NULL) {
    return -1;
  }
  if (strcat(cmdline2, " &") == NULL) {
    return -1;
  }
  return system(cmdline2);
}
