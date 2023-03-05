#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t findIndex( const long int a[], int size, long int value ) {
    int index = 0;
    while ( index < size && a[index] != value ) {
      ++index;
    }
    return ( index == size ? -1 : index );
}

int main(int argc, char *argv[]) {
  if (argc == 1) {
    printf("usage: %s program.bf\n", argv[0]);
    exit(1);
  }
  char *s = "-n";
  FILE *f;
  int i;
  char fm[5] = "%c";
  for(i = 0; i < argc; ++i) {
    if (!strcmp(argv[i], s)) strcpy(fm, "%d\n");
  }
  if ((f = fopen(argv[argc-1], "r")) == NULL) {
    printf("FILE OPENING ERROR\n");
    exit(1);
  }
  int c;
  int bs = 0;
  int be = 0;
  int ss = 0;
  int mss = 0;
  while((c = fgetc(f)) != EOF) {
    if ((char)c == '[') {
      bs++;
      ss++;
      if (mss < ss) mss = ss;
    } else if ((char)c == ']') {
      be++;
      ss--;
    }
  }
  if (be != bs) {
    printf("NO BALANCE ERROR\n");
    exit(1);
  }
  int brcl = 2*bs;
  int stcl = mss;
  long int *brc = malloc(brcl*sizeof(long int));
  long int *stc = malloc(stcl*sizeof(long int));
  int pa = 0;
  rewind(f);
  for (i=0;i<brcl;i++) brc[i] = 0;
  for (i=0;i<stcl;i++) stc[i] = 0;
  ss = 0;
  while((c = fgetc(f)) != EOF) {
    if ((char)c == '[') {
      stc[ss] = ftell(f);
      ss++;
    } else if ((char)c == ']') {
      ss--;
      brc[(pa*2)] = stc[ss];
      brc[(pa*2)+1] = ftell(f);
      pa++;
    }
  }
  rewind(f);
  int arrlength = 30000;
  int ptr = 0;
  unsigned char lc;
  unsigned char *arr = malloc(sizeof(unsigned char) * arrlength);
  while((c = fgetc(f)) != EOF) {
    if ((char)c == '+') {
      arr[ptr]++;
    } else if ((char)c == '-') {
      arr[ptr]--;
    } else if ((char)c == '>') {
      ptr++;
      if (ptr == arrlength) ptr = 0;
    } else if ((char)c == '<') {
      if (ptr == 0) ptr = arrlength;
      ptr--;
    } else if ((char)c == '.') {
      lc = arr[ptr];
      printf(fm, (int)arr[ptr]);
    } else if ((char)c == ',') {
      arr[ptr] = (unsigned char)getchar();
    } else if ((char)c == '[' && arr[ptr] == 0) {
      fseek(f, brc[findIndex(brc, brcl, ftell(f)) + 1], SEEK_SET);
    } else if ((char)c == ']' && arr[ptr] != 0) {
      fseek(f, brc[findIndex(brc, brcl, ftell(f)) - 1], SEEK_SET);
    }
  }
  fclose(f);
  free(brc);
  free(stc);
  if (lc != '\n' && !strcmp(fm, "%c")) printf("\n");
  return 0;
}
