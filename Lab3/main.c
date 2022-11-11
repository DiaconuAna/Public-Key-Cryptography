/**
 * 3. Generalized Fermat’s algorithm. It will first consider k = 1. If not successful, then it will
 * consider k = 2, 3, . . . until getting a factor
 *
 * Algorithm
 * For k = 1, 2, . . . do
 *  Let t0 = [√kn].
 *  For t = t0 + 1, . . . , t0 + B do
 *  If t^2 − kn is a square s2, then s^2 = t^2 − k*n,
 *  n = 1/k (t − s)(t + s), and STOP.
 */

#include <stdio.h>
#include <math.h>
#include <stdbool.h>

void generalizedFermatAlgorithm(int n, int B) {
    int k = 1;
    bool isOver = false;

    while (!isOver) {
        int t0 = sqrt(k * n); // t0 = [√kn]
        for (int t = t0 + 1; t <= t0 + B; t++){ // For t = t0 + 1, . . . , t0 + B do
            float s2 = t*t-k*n; // s^2 = t^2 − k*n
            // check if s2 is a perfect square
            if(sqrt(s2) == (int)(sqrt(s2))){
                //  n = 1/k (t − s)(t + s)
                printf("Answer: n = %d * %d\n", (t-(int)(sqrt(s2))), (t + (int)(sqrt(s2)))/k);
                // STOP
                isOver = true;
                break;
            }
        }
        k++; // else increment to get a new factor
    }
}

int main() {
    int n,B;
    printf("\nn = ");
    scanf("%d", &n);
    printf("\nBound B = ");
    scanf("%d", &B);
    generalizedFermatAlgorithm(n, B);
    return 0;
}
