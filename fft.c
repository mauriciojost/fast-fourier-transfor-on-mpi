#include "fft.h"
#include "complejo.h"#include <stdio.h>



void mariposa(struct complex a, struct complex b, struct complex* A, struct complex* B, int wn)
{
  struct complex w;

  // A = (a+b)
  A->re = a.re+b.re;
  A->im = a.im+b.im;

  // B = (a-b)*W
  // B = (a-b)... 1� parte...
  B->re = a.re-b.re;
  B->im = a.im-b.im;
  
  // B = (a-b)*W  2� parte...
  float arg = 3.14159265*2*wn/512;
	w.re = cos((double)arg);
	w.im = sin((double)arg);
  multcmplxp(*B,w,B);  
}
void ffttras(struct complex *x,struct complex *y, int n)
{
  int i, semi_n=n>>1;
  
  for(i=0;i<semi_n;i++){ 
    mariposa     (x[i],x[i+semi_n] ,&x[i],&x[i+semi_n],i);
  }
  
  fft(x,   semi_n,2);
  fft(x+semi_n, semi_n,2);
  
  for(i=0;i<n;i++){ 
    igcmplxp(x[i],  &y[reversal_bit(i,9)]);
  }
  
}


void fft(struct complex *x, int len, int profundidad)
{
  int i, N=len>>1;
  
  if (len>1)
  { 
    for(i=0;i<N;i++)
      mariposa(x[i],x[i+N],&x[i],&x[i+N],i*profundidad);
    
    fft(x,   N,profundidad*2);
    fft(x+N, N,profundidad*2);
  }
}


unsigned int reversal_bit(unsigned int a, unsigned int n){
  unsigned int i=0;
  unsigned int base=0;
  unsigned int mask = 1<<(n-1);
  unsigned int shift=1;

  //printf("base=%u, mask=%u, shift=%u, i=%u.\n",base,mask,shift,i);
  for (i=0;i<n;i++){
    base |= (mask & a)?shift:0;
    mask = mask >> 1;
    shift = shift << 1;
    //printf("base=%u, mask=%u, shift=%u, i=%u.\n",base,mask,shift,i);
  }
  return base;
}
