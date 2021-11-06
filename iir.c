//
// IIRフィルタ処理（バイクワッド）
//　・係数設定
//　・信号処理
//
// memo: 以下のコードとは関係なし
//   バターワース特性の4次の伝達関数
//   1/(s^2 + 0.7654s + 1)(s^2 + 1.8478s + 1)
//
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include "iir.h"

void bq_clearbuf(Bq *bq){
    //buf
    bq->x1 = bq->x2 = 0.0;
    bq->y1 = bq->y2 = 0.0;
}
void bq_clearcoef(Bq *bq){
    //coef
    bq->c0 = bq->c1 = bq->c2 = bq->c3 = bq->c4 = 0.0;
}
void bq_init(Bq *bq){
    bq_clearcoef(bq);
    bq_clearbuf(bq);
}

// 1サンプル入力、1サンプル出力
double bq_proc(double in, Bq *bq){

    //proc
    double y0 = 0.0;
    y0 += bq->c0 * in;
    y0 += bq->c1 * bq->x1;
    y0 += bq->c2 * bq->x2;
    y0 -= bq->c3 * bq->y1;
    y0 -= bq->c4 * bq->y2;

    //update bq buf for next
    bq->x2 = bq->x1;    bq->x1 = in;
    bq->y2 = bq->y1;    bq->y1 = y0;
    
    return y0;
}

//係数などを画面に表示する
void   bq_show(Bq *bq){
    printf("c0: %+.6le\n",bq->c0);
    printf("c1: %+.6le\n",bq->c1);
    printf("c2: %+.6le\n",bq->c2);
    printf("c3: %+.6le\n",bq->c3);
    printf("c4: %+.6le\n",bq->c4);
}

//----係数生成------------------------------------
//LPF
void coef_lpf(double fc, double q, double fs, Bq *bq){
    
    double w0    = 2.0*M_PI*fc/fs;
    //double q     = 1.0/sqrt(2.0);
    double alpha = 0.5*sin(w0)/q;

    double b0 =  (1 - cos(w0))/2;
    double b1 =   1 - cos(w0);
    double b2 =  (1 - cos(w0))/2;
    double a0 =   1 + alpha;
    double a1 =  -2*cos(w0);
    double a2 =   1 - alpha;

    bq->c0 = b0/a0;    bq->c1 = b1/a0;    bq->c2 = b2/a0;    bq->c3 = a1/a0;    bq->c4 = a2/a0;
}

//HPF
void coef_hpf(double fc, double q, double fs, Bq *bq){
    
    double w0   = 2.0*M_PI*fc/fs;
    //double q    = 1.0/sqrt(2.0);
    double alpha = 0.5*sin(w0)/q;

    double b0 =  (1 + cos(w0))/2;
    double b1 = -(1 + cos(w0));
    double b2 =  (1 + cos(w0))/2;
    double a0 =   1 + alpha;
    double a1 =  -2*cos(w0);
    double a2 =   1 - alpha;

    bq->c0 = b0/a0;    bq->c1 = b1/a0;    bq->c2 = b2/a0;    bq->c3 = a1/a0;    bq->c4 = a2/a0;
}

//LShelf
void coef_lshelf(double fc, double dBgain, double fs, Bq *bq){
    double A     = pow(10,dBgain/40);
    double w0    = 2.0*M_PI*fc/fs;
    double S     = 1.0;
    double alpha = sin(w0)/2 * sqrt( (A + 1/A)*(1/S - 1) + 2 );
    
    double b0 =    A*( (A+1) - (A-1)*cos(w0) + 2*sqrt(A)*alpha );
    double b1 =  2*A*( (A-1) - (A+1)*cos(w0)                   );
    double b2 =    A*( (A+1) - (A-1)*cos(w0) - 2*sqrt(A)*alpha );
    double a0 =        (A+1) + (A-1)*cos(w0) + 2*sqrt(A)*alpha;
    double a1 =   -2*( (A-1) + (A+1)*cos(w0)                   );
    double a2 =        (A+1) + (A-1)*cos(w0) - 2*sqrt(A)*alpha;
                
    bq->c0 = b0/a0;    bq->c1 = b1/a0;    bq->c2 = b2/a0;    bq->c3 = a1/a0;    bq->c4 = a2/a0;
}

//HShelf
void coef_hshelf(double fc, double dBgain, double fs, Bq *bq){
    double A     = pow(10,dBgain/40);
    double w0    = 2.0*M_PI*fc/fs;
    double S     = 1.0;
    double alpha = sin(w0)/2 * sqrt( (A + 1/A)*(1/S - 1) + 2 );

    double b0 =    A*( (A+1) + (A-1)*cos(w0) + 2*sqrt(A)*alpha );
    double b1 = -2*A*( (A-1) + (A+1)*cos(w0)                   );
    double b2 =    A*( (A+1) + (A-1)*cos(w0) - 2*sqrt(A)*alpha );
    double a0 =        (A+1) - (A-1)*cos(w0) + 2*sqrt(A)*alpha;
    double a1 =    2*( (A-1) - (A+1)*cos(w0)                   );
    double a2 =        (A+1) - (A-1)*cos(w0) - 2*sqrt(A)*alpha;

    bq->c0 = b0/a0;    bq->c1 = b1/a0;    bq->c2 = b2/a0;    bq->c3 = a1/a0;    bq->c4 = a2/a0;
}

//peak
void coef_peak(double fc, double dBgain, double q, double fs, Bq *bq){
    
    if(q<0.001) q = 0.001;
  
    double A     = pow(10,dBgain/40);
    double w0    = 2.0*M_PI*fc/fs;
    double alpha = 0.5 * sin(w0)/q; // sin(w0)/(2*q)
    
    double b0 =   1 + alpha * A;
    double b1 =  -2 * cos(w0);
    double b2 =   1 - alpha * A;
    double a0 =   1 + alpha / A;
    double a1 =  -2 * cos(w0);
    double a2 =   1 - alpha / A;
    
    bq->c0 = b0/a0;    bq->c1 = b1/a0;    bq->c2 = b2/a0;    bq->c3 = a1/a0;    bq->c4 = a2/a0;
}

void coef_apf(double fc, double q, double fs, Bq *bq){
    double w0    = 2.0*M_PI*fc/fs;
    double alpha = 0.5 * sin(w0)/q; // sin(w0)/(2*q)
    double b0 =   1 - alpha;
    double b1 =  -2*cos(w0);
    double b2 =   1 + alpha;
    double a0 =   1 + alpha;
    double a1 =  -2*cos(w0);
    double a2 =   1 - alpha;
    bq->c0 = b0/a0;    bq->c1 = b1/a0;    bq->c2 = b2/a0;    bq->c3 = a1/a0;    bq->c4 = a2/a0;
}

void coef_notch(double fc, double q, double fs, Bq *bq){
    double w0    = 2.0*M_PI*fc/fs;
    double alpha = 0.5 * sin(w0)/q; // sin(w0)/(2*q)
    double b0 =   1;
    double b1 =  -2*cos(w0);
    double b2 =   1;
    double a0 =   1 + alpha;
    double a1 =  -2*cos(w0);
    double a2 =   1 - alpha;
    bq->c0 = b0/a0;    bq->c1 = b1/a0;    bq->c2 = b2/a0;    bq->c3 = a1/a0;    bq->c4 = a2/a0;
}
