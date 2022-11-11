#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#define MAXROW      2 // maximum numbers of rows of a matrix (2 since m = 2)
#define MAXCOL      2 // maximum numbers of columns of a matrix (2 since m = 2)
#define MAXNUMLETTERS 50 // maximum number of letters of a word

int gcd(int a, int b){
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

/**
 * Function to read a matrix from user input
 * @param a: the matrix
 * @param n: number of rows
 * @param m: number of columns
 */
void readMatrix(int a[MAXROW][MAXCOL], int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("a[%d][%d] =", i + 1, j + 1);
            scanf("%d", &a[i][j]);
        }
    }
}

/**
 * Function to print a matrix row by row
 * @param a: the matrix
 * @param n: number of rows
 * @param m: number of columns
 */
void printMatrix(int a[MAXROW][MAXCOL], int n, int m) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            printf("%d ", a[i][j]);
        }
        printf("\n");
    }
}

/**
 * Function to multiply two matrices a and b: c = b * a
 * @param a : 2x2 matrix
 * @param b : 1x2 matrix
 * @param c : output matrix - 1x2
 * @param n : stands for n in Z_n
 */
void matrixMultiplication(int a[MAXROW][MAXCOL], int b[1][2], int c[1][2], int n) {
    int i, j, k;
    for (i = 0; i < 1; i++) {
        for (j = 0; j < 2; j++) {
            c[i][j] = 0;
            for (k = 0; k < 2; k++) {
                c[i][j] += b[i][k] * a[k][j];
            }
            c[i][j] %= n;
        }
    }
}

/**
 * Function used to turn the text into its numeric equivalent
 * Example : four => 6 15 21 18
 * @param v: plaintext/ciphertext - in param
 * @param res : array of numbers, each corresponding to a string character - out param
 */
void lettersToDigits(char v[MAXNUMLETTERS], int res[MAXNUMLETTERS]) {
    for (int i = 0; i < strlen(v); i++) {
        if (v[i] == ' ') { // whitespace is a special character which has the numerical form 0
            res[i] = 0;
        } else {
            // otherwise the number equivalent to a letter is computed as:
            // ascii code of uppercase letter - 64
            // ex. : A - 1, B - 2, ..., Z - 26
            res[i] = toupper(v[i]) - 64;
        }
    }
}

/**
 * Function used to turn an array of numbers into text
 * @param v: plaintext/ ciphertext - out param
 * @param res: array of number, each corresponding to a string character - in param
 */
void digitsToLetters(char v[MAXNUMLETTERS], const int res[MAXNUMLETTERS]) {

    int len = strlen(v);
    for (int i = 0; i < len; i++) {
        if (res[i] == 0) { // 0 stands for whitespace ' '
            v[i] = ' ';
        } else {
            v[i] = res[i] + 64; // otherwise the characters have ascii code froom 65 upwards
        }
    }
}

/**
 * Function used to compute the modular inverse of a number x
 * a*x = 1 (mod n); a = ?
 * @param x
 * @param n
 * @return
 */
int modularInverse(int x, int n) {
    for (int i = 0; i < n; i++) {
        if ((i * x) % n == 1)
            return i;
    }
    return -1; //if the modular inverse does not exist
}

/**
 * Function used to compute the inverse of a matrix a (mod n)
 * @param a
 * @param res : a^(-1) (mod n)
 * @param n
 */
void matrixInverse(int a[MAXROW][MAXCOL], int res[MAXROW][MAXCOL], int n) {
    // the determinant is the modular inverse of the initial determinant of matrix a (mod n)
    int determinant = modularInverse((a[0][0] * a[1][1] - a[0][1] * a[1][0]) % n, n);
    // the adjacent matrix used in computing the inverse
    int adjA[2][2];

    adjA[0][0] = a[1][1] % n;
    adjA[1][1] = a[0][0] % n;
    adjA[0][1] = (n - a[0][1]) % n;
    adjA[1][0] = (n - a[1][0]) % n;

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++)
            res[i][j] = ((adjA[i][j]) * determinant) % n; // every element mod n
    }

}

/**
 * Hill cipher with m = 2
 * Can be used for both encryption and decryption based on the nature of matrix a
 * a is the initial matrix => encryption
 * a is the inverse of a matrix => decryption
 * @param n
 * @param text
 * @param a
 */
void hill(int n, char text[MAXNUMLETTERS], int a[MAXROW][MAXCOL]) {
    int b[1][2], c[1][2];
    int res[MAXNUMLETTERS]; // numerical form of the initial text
    int res_enc[MAXNUMLETTERS]; // encrypted numerical form

    // turn text into its numeric form
    lettersToDigits(text, res);

    // print the numerical form of the initial text
    printf("\n Numerical >> ");
    for (int i = 0; i < strlen(text); i++) {
        printf("%d ", res[i]);
    }
    printf("\n");

    int len = strlen(text);

    // group letters in groups of 2 and multiply the corresponding matrices according to the cipher
    for (int i = 0; i < len; i += 2) {
        b[0][0] = res[i];
        b[0][1] = res[i + 1];

        matrixMultiplication(a, b, c, n);

        // save the resulting matrix elements as elements of the encrypted numerical form array
        res_enc[i] = c[0][0];
        res_enc[i + 1] = c[0][1];
    }

    // encryption : turn the encrypted numerical form into the ciphertext
    // decryption : turn the numerical form into the plaintext
    digitsToLetters(text, res_enc);

    printf("\nNumerical encryption: ");
    for (int i = 0; i < len; i++) {
        printf("%d ", res_enc[i]);
    }

    // Print the result
    printf("\n >>> %s <<< \n", text);
}

int main() {
    int a[2][2]; // todo: check if a is invertible and gcd(DetA, n) = 1
    int invA[2][2];
    int n;

    char plaintext[MAXNUMLETTERS];
    printf("Enter a string: ");
    scanf("%[^\n]", plaintext);

    // if the plaintext has an odd length, we add a whitespace at the end of it
    // in order to be able to group characters in groups of 2 (as hill cipher has m = 2 in this case)
    if (strlen(plaintext) % 2) {
        int len = strlen(plaintext);
        plaintext[len] = ' ';
        plaintext[len + 1] = '\0';
    }
    printf("\n Plaintext: %s\n", plaintext);

    printf("n = ");
    scanf("%d", &n);

    readMatrix(a, 2, 2);

    if(a[0][0] * a[1][1] - a[0][1] * a[1][0] == 0){
        printf("A is not invertible");
        return -1;
    }

    if(gcd(n, (a[0][0] * a[1][1] - a[0][1] * a[1][0])%27) != 1){
        printf("The key does not respect the condition gcd(detK, n) = 1");
        return -2;
    }

    // encrypt plaintext
    hill(n, plaintext, a);

    // compute the inverse of the key matrix
    matrixInverse(a, invA, n);

    //decrypt ciphertext
    hill(n, plaintext, invA);

    return 0;
}
