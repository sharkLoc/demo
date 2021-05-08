#include <stdint.h>
#include <zlib.h>

char *readline(gzFile file);

void fq2fa(int argc, char *argv[]);

uint64_t quality20(char *line);

uint64_t quality30(char *line);

float lowQvrate(int q,char *read);

int trimfq(int argc, char *argv[]);

void summary(int argc, char *argv[]);

int mergeVcf(int argc, char *argv[]);
