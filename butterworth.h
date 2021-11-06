#ifndef _H_butterworth
#define _H_butterworth

#include "iir.h"

void   bw_coefs_lpf( int order, double fc, double fs,  Bq *fils);
void   bw_coefs_hpf( int order, double fc, double fs,  Bq *fils);
double bw_proc     ( int order, double in, Bq *fils);

#endif
