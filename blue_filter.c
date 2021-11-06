#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "iir.h"
#include "butterworth.h"

int main(int argc, char** argv){
  if(argc != 3){
    printf("Usage : ./blue_filter input.raw blue.raw\n");
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
  double fc1 = 300.0;
  double dbgain1 = -7.8;
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
  /*
  double fc2 = 1500.0;
  double dbgain2 = -3.0;
  double q_value2 = 1.0 / sqrt(2.0);
  double a_value2 = pow(10.0, dbgain2 / 40.0);
  double omega2 = 2.0 * M_PI * fc2 / fs;
  double cos_omega2 = cos(omega2);
  double sin_omega2 = sin(omega2);
  double alpha2 = sin_omega2 / (2.0 * q_value2);
  double b2[3] = {
    (1.0 - cos_omega2) / 2.0,
    1.0 - cos_omega2,
    (1.0 - cos_omega2) / 2.0
  };
  double a2[3] = {
    1.0 + alpha2,
    -2.0 * cos_omega2,
    1.0 - alpha2
  };
  */
  int order = 6;
  int nbq = order / 2;
  Bq *fils = (Bq*)malloc(sizeof(Bq) * nbq);
  double fc = 1100.0;
  bw_coefs_lpf(order, fc, fs, fils);
  for(int i = 0; i < nbq; i++){
    bq_clearbuf(&fils[i]);
  }
  double x, x1, x2;
  double y, y1, y2;
  //double z, z1, z2;
  x1 = 0.0;
  x2 = 0.0;
  y1 = 0.0;
  y2 = 0.0;
  //z1 = 0.0;
  //z2 = 0.0;
  for(int i = 0; i < len; i++){
    double value;
    fread(&value, sizeof(double), 1, f1);
    x = value;
    y = (b1[0] / a1[0]) * x + (b1[1] / a1[0]) * x1 + (b1[2] / a1[0]) * x2 - (a1[1] / a1[0]) * y1 - (a1[2] / a1[0]) * y2;
    //z = (b2[0] / a2[0]) * y + (b2[1] / a2[0]) * y1 + (b2[2] / a2[0]) * y2 - (a2[1] / a2[0]) * z1 - (a2[2] / a2[0]) * z2;
    x2 = x1;
    x1 = x;
    y2 = y1;
    y1 = y;
    //z2 = z1;
    //z1 = z;
    double z = bw_proc(order, y, fils);
    fwrite(&z, sizeof(double), 1, f2);
  }
  return 0;
}
