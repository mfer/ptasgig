#include "stdlib.h"
#include "stdio.h"

short
graph__are_connected(int *G, int Si, int Sj){
  return false;
}

short
is_IVS(int *G, int *S, int k) {
  short hasAdj = false;
  int i, j;

  for (i = 1; i <= k && !hasAdj; i++){
    for (j = 1; j <= k && !hasAdj; j++){
      //printf ("%d, %d\n", S[i], S[j]);
      hasAdj = graph__are_connected(G, S[i], S[j]);
    }
  }

  return !hasAdj;
}

int 
main() {
  int *S, *S_max;
  int i, k, N, l, l_max;
  int *G;

  N = 39;
  S = (int *) calloc((N+1), (N+1) * sizeof(int));
  S_max = (int *) calloc(N, N * sizeof(int));
  S[0] = -1;
  k = 0;
  l_max = 0;

  while (true) 
{    // very interesting form to generate all subsets
    if (S[k] < (N-1)) {
      S[k+1] = S[k] + 1;
      k += 1;
    } else {
      S[k-1] += 1;
      k -= 1;
    }
    if (k == 0) break;
/*
    printf("%d: {", k);
    for(i = 1; i <= k; i++){
      printf ("%d, ", S[i]);
    } 
    printf("}\n");
*/
    // brute force to check if the subset is Independent Vertex Set
    if (k > l_max && is_IVS(G,S,k)) {
      for(i = 1; i <= k; i++){
        S_max[i-1] = S[i];
      }
      l_max = k;
    }
  }

}