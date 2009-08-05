#include "stddef.h"

void abort(void);
void exit(int status);

void *malloc(size_t size);
void free(void *ptr);

unsigned long strtoul(const char *cp,char **endp,unsigned int base);
long strtol(const char *cp,char **endp,unsigned int base);
unsigned long long strtoull(const char *cp,char **endp,unsigned int base);
long long strtoll(const char *cp,char **endp,unsigned int base);
