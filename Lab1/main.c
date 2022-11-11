#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

uint64_t zeroCheck(uint64_t a, uint64_t b) {
    if (a == 0) {
        return b;
    }
    if (b == 0) {
        return a;
    }
    return 0; // should not reach this line
}

uint64_t gcd_1(uint64_t a, uint64_t b) {
    if (a == 0 || b == 0) {
        return zeroCheck(a, b);
    }
    if (a == 1 || b == 1) {
        return 1;
    }
    while (a != b) {
        if (a > b) {
            a = a - b;
        } else {
            b = b - a;
        }
    }
    return a;
}

uint64_t gcd_2(uint64_t a, uint64_t b) {
    if (a == 0 || b == 0) {
        return zeroCheck(a, b);
    }
    if (a == 1 || b == 1) {
        return 1;
    }
    uint64_t r; // remainder
    while (b != 0) {
        r = b;
        b = a % b;
        a = r;
    }
    return a;
}

// a = 3 * 2 * 2 * 5 * 5 => 300
// b = 2 * 7 * 5 * 5 * 2 => 700
// lowest power of common prime factors
uint64_t gcd_3(uint64_t a, uint64_t b) {
    if (a == 0 || b == 0) {
        return zeroCheck(a, b);
    }
    if (a == 1 || b == 1) {
        return 1;
    }

    uint64_t div1;
    uint64_t gcd = 1;
    for (div1 = 2; div1 * div1 <= a; div1++) {
        uint64_t power_a = 0;
        uint64_t power_b = 0;
        uint64_t tmp = a;
        while (tmp % div1 == 0) {
            power_a++;
            tmp /= div1;
        }
        tmp = b;
        while (tmp % div1 == 0) {
            power_b++;
            tmp /= div1;
        }
        if (power_a > 0 && power_b > 0) {
            uint64_t min_pow = (power_a < power_b) ? power_a : power_b;
            for (uint64_t i = 0; i < min_pow; i++)
                gcd *= div1;
        }
    }
    return gcd;
}

uint64_t rand_uint64_slow(void) {
    uint64_t r = 0;
    for (int i = 0; i < 50; i++) {
        r = r * 2 + rand() % 2;
    }
    return r;
}

void test_gcd1() {
    clock_t start, end;

    uint64_t test_array[20];
    for (int i = 0; i < 20; i++) {
        test_array[i] = rand_uint64_slow();
    }

    start = clock();
    for (int i = 0; i < 10; i += 2) {
        gcd_1(test_array[i], test_array[i + 1]);
    }
    end = clock();
    printf("gcd1 - %f seconds\n", ((double) (end - start)) / CLOCKS_PER_SEC);

}

void test_gcd2() {
    clock_t start, end;

    uint64_t test_array[20];
    for (int i = 0; i < 20; i++) {
        test_array[i] = rand_uint64_slow();
    }

    start = clock();
    for (int i = 0; i < 10; i += 2) {
        gcd_2(test_array[i], test_array[i + 1]);
    }
    end = clock();
    printf("gcd2 - %f seconds\n", ((double) (end - start)) / CLOCKS_PER_SEC);

}

void test_gcd3() {
    clock_t start, end;

    uint64_t test_array[20];
    for (int i = 0; i < 20; i++) {
        test_array[i] = rand_uint64_slow();
    }

    start = clock();
    for (int i = 0; i < 10; i += 2) {
        gcd_3(test_array[i], test_array[i + 1]);
    }
    end = clock();
    printf("gcd3 - %f seconds\n", ((double) (end - start)) / CLOCKS_PER_SEC);

}

int main() {
    clock_t start, end;
    double cpu_time_used;
    uint64_t a, b;
    printf("Enter the two numbers:");
    printf("a = ");
    scanf("%lld", &a);
    printf("b = ");
    scanf("%lld", &b);

    start = clock();
    uint64_t gcd1 = gcd_1(a, b);
    end = clock();
    printf("GCD1(%llu, %llu) = %llu in %f seconds\n", a, b, gcd1, ((double) (end - start)) / CLOCKS_PER_SEC);

    start = clock();
    uint64_t gcd2 = gcd_2(a, b);
    end = clock();
    printf("GCD2(%llu, %llu) = %llu in %f seconds\n", a, b, gcd2, ((double) (end - start)) / CLOCKS_PER_SEC);

    start = clock();
    uint64_t gcd3 = gcd_2(a, b);
    end = clock();
    printf("GCD3(%llu, %llu) = %llu in %f seconds\n", a, b, gcd3, ((double) (end - start)) / CLOCKS_PER_SEC);


    test_gcd1();
    test_gcd2();
    test_gcd3();
    return 0;
}
