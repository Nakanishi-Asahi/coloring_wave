//
// biquad.h
//
// IIRフィルタ処理（バイクワッド）
//　・係数設定
//　・信号処理
//
#ifndef _H_BIQUAD
#define _H_BIQUAD

typedef struct StBq{
    //buf  4
    double x1,x2;// buf in    x[n-1] x[n-2]
    double y1,y2;// buf out   y[n-1] y[n-2]
    //coef 5
    double c0,c1,c2,c3,c4;// coef  [c0,c1,c2,c3,c4] <- [b0,b1,b2,a1,a2]
} Bq;

// 初期化・信号処理・デバグ、など
void   bq_init(Bq *bq);
double bq_proc(double in, Bq *bq);
void   bq_show(Bq *bq);
void   bq_clearcoef(Bq *bq);
void   bq_clearbuf(Bq *bq);

// 係数設定
void   coef_lpf   (double fc, double q, double fs, Bq *bq);
void   coef_hpf   (double fc, double q, double fs, Bq *bq);
void   coef_lshelf(double fc, double dBgain, double fs, Bq *bq);
void   coef_hshelf(double fc, double dBgain, double fs, Bq *bq);
void   coef_apf   (double fc, double q, double fs, Bq *bq);
void   coef_peak  (double fc, double dBgain, double q, double fs, Bq *bq);
void   coef_notch (double fc, double q, double fs, Bq *bq);
#endif
