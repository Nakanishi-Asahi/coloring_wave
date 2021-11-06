#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(int argc, char** argv){
  if(argc != 3){
    printf("Usage : ./generate_drawdata input.raw drawdata.txt\n");
    exit(-1);
  }
  FILE *f1, *f2;
  if((f1 = fopen(argv[1], "rb")) == NULL){
    printf("Cannot open %s\n", argv[1]);
    exit(-2);
  }
  if((f2 = fopen(argv[2], "w")) == NULL){
    printf("Cannot open %s\n", argv[2]);
    exit(-3);
  }
  double fs = 44100.0;
  int len = fs * 10;
  int num = 512;
  double* nums = (double*)malloc(sizeof(double) * num);
  int count = 0;
  int index = 0;
  for(int i = 0; i < len; i++){
    double value;
    fread(&value, sizeof(double), 1, f1);
    nums[count] = value;
    count++;
    if(num <= count){
      double max_value = 0.0;
      for(int j = 0; j < num; j++){
        if(max_value < fabs(nums[j])){
          max_value = fabs(nums[j]);
        }
      }
      fprintf(f2, "%d %f\n", index, max_value);
      index++;
      count = 0;
    }
  }
  fclose(f1);
  fclose(f2);
  return 0;
}
