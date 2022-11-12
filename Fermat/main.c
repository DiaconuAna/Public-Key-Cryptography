/**
 * Let t0 = [√n].
 * For t = t0 + 1, . . . , t0 + B do
 * If t2 − n is a square s2, then s2 = t2 − n,
 *      n = (t − s)(t + s), and STOP.
 */
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

void fermat(int n, int B){
    int t0 = sqrt(n);
    printf("[√n] = %d\n", t0);

    for(int t = t0 + 1; t <= t0 + B; t++){
        float s2 = t*t - n;
        printf("t*t-n = %f\n", s2);
        if(sqrt(s2) == (int)(sqrt(s2))){
            printf("%f is a perfect square\n", s2);
            printf("s = %d, t = %d\n", (int)sqrt(s2), t);
            printf("%d can be factorized as: %d, %d",n, t-(int)(sqrt(s2)), t+(int)(sqrt(s2)));
            break;
        }
    }
}

int main() {
    fermat(7399, 21);
    return 0;
}
