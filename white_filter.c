#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "iir.h"
#include "butterworth.h"

int main(int argc, char** argv){
  if(argc != 3){
    printf("Usage : ./white_filter input.raw white.raw\n");
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
  int len = fs * 10;
  //hpf
  int order1 = 4;
  int nbq1 = order1 / 2;
  Bq *fils1 = (Bq*)malloc(sizeof(Bq) * nbq1);
  double fc1 = 3000.0;
  bw_coefs_hpf(order1, fc1, fs, fils1);
  for(int i = 0; i < nbq1; i++){
    bq_clearbuf(&fils1[i]);
  }
  //lpf
  int order2 = 2;
  int nbq2 = order2 / 2;
  Bq *fils2 = (Bq*)malloc(sizeof(Bq) * nbq2);
  double fc2 = 9500.0;
  bw_coefs_lpf(order2, fc2, fs, fils2);
  for(int i = 0; i < nbq1; i++){
    bq_clearbuf(&fils1[i]);
  }

  for(int i = 0; i < len; i++){
    double value;
    fread(&value, sizeof(double), 1, f1);
    double x = value;
    double y = bw_proc(order1, x, fils1);
    double z = bw_proc(order2, y, fils2);
    fwrite(&z, sizeof(double), 1, f2);
  }
  return 0;
}
