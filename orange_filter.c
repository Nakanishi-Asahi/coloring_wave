#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "iir.h"
#include "butterworth.h"

int main(int argc, char** argv){
  if(argc != 3){
    printf("Usage : ./orange_filter input.raw orange.raw\n");
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
  //highshelf
  double fc1 = 1250.0;
  double dbgain1 = -8.3;
  double a_value1 = pow(10.0, dbgain1 / 40.0);
  double omega1 = 2.0 * M_PI * fc1 / fs;
  double cos_omega1 = cos(omega1);
  double sin_omega1 = sin(omega1);
  double s1 = 15.0;
  double large_s1 = s1 * (log(10) / log(2)) / 40.0 * sin_omega1 / omega1 * (a_value1 * a_value1 + 1.0) / fabs(a_value1 * a_value1 + 1.0);
  double alpha1 = sin_omega1 / 2.0 * sqrt((a_value1 + 1.0 / a_value1) * (1.0 / large_s1 - 1.0) + 2.0);
  double b1[3] = {
    a_value1 * ((a_value1 + 1.0) + (a_value1 - 1.0) * cos_omega1 + 2.0 * sqrt(a_value1) * alpha1),
    -2.0 * a_value1 * ((a_value1 - 1.0) + (a_value1 + 1.0) * cos_omega1),
    a_value1 * ((a_value1 + 1.0) + (a_value1 - 1.0) * cos_omega1 - 2.0 * sqrt(a_value1) * alpha1)
  };
  double a1[3] = {
    (a_value1 + 1.0) - (a_value1 - 1.0) * cos_omega1 + 2.0 * sqrt(a_value1) * alpha1,
    2.0 * ((a_value1 - 1.0) - (a_value1 + 1.0) * cos_omega1),
    (a_value1 + 1.0) - (a_value1 - 1.0) * cos_omega1 - 2.0 * sqrt(a_value1) * alpha1
  };
  //lpf
  int order_lpf = 4;
  int nbq_lpf = order_lpf / 2;
  Bq *fils_lpf = (Bq*)malloc(sizeof(Bq) * nbq_lpf);
  double fc_lpf = 4500.0;
  bw_coefs_lpf(order_lpf, fc_lpf, fs, fils_lpf);
  for(int i = 0; i < nbq_lpf; i++){
    bq_clearbuf(&fils_lpf[i]);
  }
  //lowshelf
  double fc2 = 280.0;
  double dbgain2 = -6.0;
  double a_value2 = pow(10.0, dbgain2 / 40.0);
  double omega2 = 2.0 * M_PI * fc2 / fs;
  double cos_omega2 = cos(omega2);
  double sin_omega2 = sin(omega2);
  double s2 = 16.0;
  double large_s2 = s2 * (log(10) / log(2)) / 40.0 * sin_omega2 / omega2 * (a_value2 * a_value2 + 1.0) / fabs(a_value2 * a_value2 + 1.0);
  double alpha2 = sin_omega2 / 2.0 * sqrt((a_value2 + 1.0 / a_value2) * (1.0 / large_s2 - 1.0) + 2.0);
  double b2[3] = {
    a_value2 * ((a_value2 + 1.0) - (a_value2 - 1.0) * cos_omega2 + 2.0 * sqrt(a_value2) * alpha2),
    2.0 * a_value2 * ((a_value2 - 1.0) - (a_value2 + 1.0) * cos_omega2),
    a_value2 * ((a_value2 + 1.0) - (a_value2 - 1.0) * cos_omega2 - 2.0 * sqrt(a_value2) * alpha2)
  };
  double a2[3] = {
    (a_value2 + 1.0) + (a_value2 - 1.0) * cos_omega2 + 2.0 * sqrt(a_value2) * alpha2,
    -2.0 * ((a_value2 - 1.0) + (a_value2 + 1.0) * cos_omega2),
    (a_value2 + 1.0) + (a_value2 - 1.0) * cos_omega2 - 2.0 * sqrt(a_value2) * alpha2
  };
  //hpf
  int order_hpf = 4;
  int nbq_hpf = order_hpf / 2;
  Bq *fils_hpf = (Bq*)malloc(sizeof(Bq) * nbq_hpf);
  double fc_hpf = 70.0;
  bw_coefs_hpf(order_hpf, fc_hpf, fs, fils_hpf);
  for(int i = 0; i < nbq_hpf; i++){
    bq_clearbuf(&fils_hpf[i]);
  }

  double v, v1, v2;
  double w, w1, w2;
  double x, x1, x2;
  double y, y1, y2;
  double z, z1, z2;
  v1 = 0.0;
  v2 = 0.0;
  w1 = 0.0;
  w2 = 0.0;
  z1 = 0.0;
  z2 = 0.0;
  for(int i = 0; i < len; i++){
    double value;
    fread(&value, sizeof(double), 1, f1);
    v = value;
    w = (b1[0] / a1[0]) * v + (b1[1] / a1[0]) * v1 + (b1[2] / a1[0]) * v2 - (a1[1] / a1[0]) * w1 - (a1[2] / a1[0]) * w2;
    v2 = v1;
    v1 = v;
    w2 = w1;
    w1 = w;
    double x = bw_proc(order_lpf, w, fils_lpf);
    y = (b2[0] / a2[0]) * x + (b2[1] / a2[0]) * x1 + (b2[2] / a2[0]) * x2 - (a2[1] / a2[0]) * y1 - (a2[2] / a2[0]) * y2;
    x2 = x1;
    x1 = x;
    y2 = y1;
    y1 = y;
    double z = bw_proc(order_hpf, y, fils_hpf);
    fwrite(&z, sizeof(double), 1, f2);
  }
  return 0;
}
