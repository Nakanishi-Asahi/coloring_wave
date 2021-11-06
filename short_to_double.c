#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

int main(int argc, char** argv){
  if(argc != 3){
    printf("Usage : ./short_to_double input.raw output.raw\n");
    exit(-1);
  }
  FILE *f1, *f2;
  if((f1 = fopen(argv[1], "rb")) == NULL){
    printf("Cannot open %s\n", argv[1]);
    exit(-2);
  }
  if((f2 = fopen(argv[2], "wb")) == NULL){
    printf("Cannot open %s\n", argv[2]);
    exit(-3);
  }
  double fs = 44100.0;
  int len = 10 * fs;
  for(int i = 0; i < len; i++){
    short value_short;
    fread(&value_short, sizeof(short), 1, f1);
    double value_double = (double)value_short / (double)SHRT_MAX;
    fwrite(&value_double, sizeof(double), 1, f2);
  }
  fclose(f1);
  fclose(f2);
  return 0;
}
