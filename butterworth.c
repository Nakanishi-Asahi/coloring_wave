#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "butterworth.h"

//--------------------
//proc  サンプル処理で
double bw_proc(int order, double in, Bq *fils){
  int n   = order;		//フィルタ全体の次数
  int nbq = n/2;		//biquadの段数
  double val = in;
  for(int i=0;i<nbq;i++){
    val = bq_proc(val,&fils[i]);
  }
  return val;
}

//--------------------
//各段のQを求めて係数をつくる  LPF
void bw_coefs_lpf( int order, double fc, double fs,  Bq *fils){
  int n   = order;		//フィルタ全体の次数
  int nbq = n/2;		//biquadの段数
  
  //NULLチェック
  if(fils==NULL){ puts("bw_coefs_lpf: Error. fils seems to be empty. bye.");  exit(EXIT_FAILURE);  }

  //IIRの係数を生成
  for(int k=1;k<=(n/2);k++){
    double coef = -2 * cos( M_PI * (2*k+n-1)/(2*n) );//多項式のs^2の係数
    double q    = 1/coef;// LPFのQ値
    int i = k-1;
    coef_lpf(fc,q,fs, &fils[i]);// LPF
  }
}

//--------------------
//各段のQを求めて係数をつくる  HPF
void bw_coefs_hpf( int order, double fc, double fs,  Bq *fils){
  int n   = order;		//フィルタ全体の次数
  int nbq = n/2;		//biquadの段数
  
  //NULLチェック
  if(fils==NULL){  puts("bw_coefs_lpf: Error. fils seems to be empty. bye.");  exit(EXIT_FAILURE); }


  //IIRの係数を生成
  for(int k=1;k<=(n/2);k++){
    double coef = -2 * cos( M_PI * (2*k+n-1)/(2*n) );//多項式のs^2の係数
    double q    = 1/coef;// LPFのQ値
    int i = k-1;
    coef_hpf(fc,q,fs, &fils[i]);// HPF
  }
}
