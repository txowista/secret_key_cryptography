#include <stdbool.h>
#include <stdlib.h>

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#include "secretKeyTest.h"



void mikeypermutation(const unsigned char *main_key, unsigned char *key, unsigned char *subkey) {
    int key_permutation[] = {
            57, 49, 41, 33, 25, 17, 9,
            1, 58, 50, 42, 34, 26, 18,
            10, 2, 59, 51, 43, 35, 27,
            19, 11, 3, 60, 52, 44, 36,
            63, 55, 47, 39, 31, 23, 15,
            7, 62, 54, 46, 38, 30, 22,
            14, 6, 61, 53, 45, 37, 29,
            21, 13, 5, 28, 20, 12, 4};
    int sub_key_permutation[] = {
            14, 17, 11, 24, 1, 5,
            3, 28, 15, 6, 21, 10,
            23, 19, 12, 4, 26, 8,
            16, 7, 27, 20, 13, 2,
            41, 52, 31, 37, 47, 55,
            30, 40, 51, 45, 33, 48,
            44, 49, 39, 56, 34, 53,
            46, 42, 50, 36, 29, 32};
    int m, n;
    int shift_size;
    unsigned char shift_byte, first_bit, second_bit, third_bit, fourth_bit;
    for (m = 0; m < 56; m++) {
        shift_size = key_permutation[m];
        shift_byte = 0x80 >> ((shift_size - 1) % 8);
        shift_byte &= main_key[(shift_size - 1) / 8];
        shift_byte <<= ((shift_size - 1) % 8);
        key[m / 8] |= (shift_byte >> m % 8);
    }
    unsigned char C[4] = {0};
    unsigned char D[4] = {0};
    for (m = 0; m < 3; m++) {
        C[m] = key[m];
    }
    C[3] = key[3] & 0xF0;
    for (m = 0; m < 3; m++) {
        D[m] = (key[m + 3] & 0x0F) << 4;
        D[m] |= (key[m + 4] & 0xF0) >> 4;
    }
    D[3] = (key[6] & 0x0F) << 4;
    shift_size = 1;
    if (shift_size == 1) {
        shift_byte = 0x80;
    } else {
        shift_byte = 0xC0;
    }
    first_bit = shift_byte & C[0];
    second_bit = shift_byte & C[1];
    third_bit = shift_byte & C[2];
    fourth_bit = shift_byte & C[3];
    C[0] <<= shift_size;
    C[0] |= (second_bit >> (8 - shift_size));
    C[1] <<= shift_size;
    C[1] |= (third_bit >> (8 - shift_size));
    C[2] <<= shift_size;
    C[2] |= (fourth_bit >> (8 - shift_size));
    C[3] <<= shift_size;
    C[3] |= (first_bit >> (4 - shift_size));

    first_bit = shift_byte & D[0];
    second_bit = shift_byte & D[1];
    third_bit = shift_byte & D[2];
    fourth_bit = shift_byte & D[3];
    D[0] <<= shift_size;
    D[0] |= (second_bit >> (8 - shift_size));
    D[1] <<= shift_size;
    D[1] |= (third_bit >> (8 - shift_size));
    D[2] <<= shift_size;
    D[2] |= (fourth_bit >> (8 - shift_size));
    D[3] <<= shift_size;
    D[3] |= (first_bit >> (4 - shift_size));;
    for (n = 0; n < 48; n++) {
        shift_size = sub_key_permutation[n];
        if (shift_size <= 28) {
            shift_byte = 0x80 >> ((shift_size - 1) % 8);
            shift_byte &= C[(shift_size - 1) / 8];
            shift_byte <<= ((shift_size - 1) % 8);
        } else {
            shift_byte = 0x80 >> ((shift_size - 29) % 8);
            shift_byte &= D[(shift_size - 29) / 8];
            shift_byte <<= ((shift_size - 29) % 8);
        }
        subkey[n / 8] |= (shift_byte >> n % 8);
    }


}

void mimsgpermutation(const unsigned char *main_key, unsigned char *key) {
    int message_permutation[] = {
            58, 50, 42, 34, 26, 18, 10, 2,
            60, 52, 44, 36, 28, 20, 12, 4,
            62, 54, 46, 38, 30, 22, 14, 6,
            64, 56, 48, 40, 32, 24, 16, 8,
            57, 49, 41, 33, 25, 17, 9, 1,
            59, 51, 43, 35, 27, 19, 11, 3,
            61, 53, 45, 37, 29, 21, 13, 5,
            63, 55, 47, 39, 31, 23, 15, 7};
    int m, n;
    int shift_size;
    unsigned char shift_byte;
    for (m = 0; m < 64; m++) {
        shift_size = message_permutation[m];
        shift_byte = 0x80 >> ((shift_size - 1) % 8);
        shift_byte &= main_key[(shift_size - 1) / 8];
        shift_byte <<= ((shift_size - 1) % 8);
        key[m / 8] |= (shift_byte >> m % 8);
    }

}

void miround(unsigned char *result, unsigned char *desD, const unsigned char *key, const unsigned char *e) {
    int m, count;
    int message[] = {
            32, 1, 2, 3, 4, 5,
            4, 5, 6, 7, 8, 9,
            8, 9, 10, 11, 12, 13,
            12, 13, 14, 15, 16, 17,
            16, 17, 18, 19, 20, 21,
            20, 21, 22, 23, 24, 25,
            24, 25, 26, 27, 28, 29,
            28, 29, 30, 31, 32, 1};
    int array_a1[] = {
            14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7,
            0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8,
            4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0,
            15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13};

    int array_a2[] = {
            15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10,
            3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5,
            0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15,
            13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9};

    int array_a3[] = {
            10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8,
            13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1,
            13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7,
            1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12};

    int array_a4[] = {
            7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15,
            13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9,
            10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4,
            3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14};

    int array_a5[] = {
            2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9,
            14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6,
            4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14,
            11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3};

    int array_a6[] = {
            12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11,
            10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8,
            9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6,
            4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13};

    int array_a7[] = {
            4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1,
            13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6,
            1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2,
            6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12};

    int array_a8[] = {
            13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7,
            1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2,
            7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8,
            2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11};
    int intermediate_permutation[] = {
            16, 7, 20, 21,
            29, 12, 28, 17,
            1, 15, 23, 26,
            5, 18, 31, 10,
            2, 8, 24, 14,
            32, 27, 3, 9,
            19, 13, 30, 6,
            22, 11, 4, 25};

    int shift_size;
    unsigned char er[6] = {0}, ser[4] = {0};
    unsigned char shift_byte, row, column;
    for (m = 0; m < 48; m++) {
        shift_size = message[m];
        shift_byte = 0x80 >> ((shift_size - 1) % 8);
        shift_byte &= desD[(shift_size - 1) / 8];
        shift_byte <<= ((shift_size - 1) % 8);
        er[m / 8] |= (shift_byte >> m % 8);
    }
    for (count = 0; count < 6; count++) {
        er[count] ^= key[count];
    }
    row = 0;
    row |= ((er[0] & 0x80) >> 6);
    row |= ((er[0] & 0x04) >> 2);
    column = 0;
    column |= ((er[0] & 0x78) >> 3);
    ser[0] |= ((unsigned char) array_a1[row * 16 + column] << 4);
    row = 0;
    row |= (er[0] & 0x02);
    row |= ((er[1] & 0x10) >> 4);
    column = 0;
    column |= ((er[0] & 0x01) << 3);
    column |= ((er[1] & 0xE0) >> 5);
    ser[0] |= (unsigned char) array_a2[row * 16 + column];
    row = 0;
    row |= ((er[1] & 0x08) >> 2);
    row |= ((er[2] & 0x40) >> 6);
    column = 0;
    column |= ((er[1] & 0x07) << 1);
    column |= ((er[2] & 0x80) >> 7);
    ser[1] |= ((unsigned char) array_a3[row * 16 + column] << 4);
    row = 0;
    row |= ((er[2] & 0x20) >> 4);
    row |= (er[2] & 0x01);
    column = 0;
    column |= ((er[2] & 0x1E) >> 1);
    ser[1] |= (unsigned char) array_a4[row * 16 + column];
    row = 0;
    row |= ((er[3] & 0x80) >> 6);
    row |= ((er[3] & 0x04) >> 2);
    column = 0;
    column |= ((er[3] & 0x78) >> 3);
    ser[2] |= ((unsigned char) array_a5[row * 16 + column] << 4);
    row = 0;
    row |= (er[3] & 0x02);
    row |= ((er[4] & 0x10) >> 4);
    column = 0;
    column |= ((er[3] & 0x01) << 3);
    column |= ((er[4] & 0xE0) >> 5);
    ser[2] |= (unsigned char) array_a6[row * 16 + column];
    row = 0;
    row |= ((er[4] & 0x08) >> 2);
    row |= ((er[5] & 0x40) >> 6);
    column = 0;
    column |= ((er[4] & 0x07) << 1);
    column |= ((er[5] & 0x80) >> 7);
    ser[3] |= ((unsigned char) array_a7[row * 16 + column] << 4);
    row = 0;
    row |= ((er[5] & 0x20) >> 4);
    row |= (er[5] & 0x01);
    column = 0;
    column |= ((er[5] & 0x1E) >> 1);
    ser[3] |= (unsigned char) array_a8[row * 16 + column];

    for (count = 0; count < 32; count++) {
        shift_size = intermediate_permutation[count];
        shift_byte = 0x80 >> ((shift_size - 1) % 8);
        shift_byte &= ser[(shift_size - 1) / 8];
        shift_byte <<= ((shift_size - 1) % 8);
        result[count / 8] |= (shift_byte >> count % 8);
    }
    for (count = 0; count < 4; count++) {
        result[count] ^= e[count];
    }

}

void miAddroundKey(unsigned char *result, unsigned char *key, unsigned char *msg) {
    int count;
    for (count = 0; count < 4; count++) {
        result[count] = msg[count * 4] ^ key[count * 4];
    }
}

void byteSub(unsigned char state[][4]) {
    static const unsigned char aes_sbox[16][16] = {
            //0     1    2      3     4    5     6     7      8    9     A      B    C     D     E     F
            0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
            0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
            0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
            0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
            0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
            0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
            0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
            0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
            0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
            0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
            0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
            0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
            0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
            0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
            0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
            0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16};
    state[0][0] = aes_sbox[state[0][0] >> 4][state[0][0] & 0x0F];
    state[0][1] = aes_sbox[state[0][1] >> 4][state[0][1] & 0x0F];
    state[0][2] = aes_sbox[state[0][2] >> 4][state[0][2] & 0x0F];
    state[0][3] = aes_sbox[state[0][3] >> 4][state[0][3] & 0x0F];
    state[1][0] = aes_sbox[state[1][0] >> 4][state[1][0] & 0x0F];
    state[1][1] = aes_sbox[state[1][1] >> 4][state[1][1] & 0x0F];
    state[1][2] = aes_sbox[state[1][2] >> 4][state[1][2] & 0x0F];
    state[1][3] = aes_sbox[state[1][3] >> 4][state[1][3] & 0x0F];
    state[2][0] = aes_sbox[state[2][0] >> 4][state[2][0] & 0x0F];
    state[2][1] = aes_sbox[state[2][1] >> 4][state[2][1] & 0x0F];
    state[2][2] = aes_sbox[state[2][2] >> 4][state[2][2] & 0x0F];
    state[2][3] = aes_sbox[state[2][3] >> 4][state[2][3] & 0x0F];
    state[3][0] = aes_sbox[state[3][0] >> 4][state[3][0] & 0x0F];
    state[3][1] = aes_sbox[state[3][1] >> 4][state[3][1] & 0x0F];
    state[3][2] = aes_sbox[state[3][2] >> 4][state[3][2] & 0x0F];
    state[3][3] = aes_sbox[state[3][3] >> 4][state[3][3] & 0x0F];
}

int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) if ((a * x) % m == 1) return x;
}
long mod_exp (long b, long e, long m)
{
    long r = 1;
    long k = 0;
    while (e > 0)
    {
        if ((e & 1) > 0)
            r = r * b % m;
        e >>= 1;			/* mas rapido las potencias de dos son adquiridas asi, 1,10,100,1000 (binario) */
        b = (b * b) % m;
    }
    return r;
}
void cipher_message (long msg, long e, long modulo)
{
    printf("Cipher Message with RSA: \n");
    printf("msg= %ld, number_e=%ld, modulo=%ld \n",msg,e,modulo);
    printf("Result: %ld\n", mod_exp(msg,e,modulo));
}
void decipher_message (long msg, long e, long modulo)
{
    printf("DeCipher Message with RSA: \n");
    printf("msg= %ld, number_e=%ld, modulo=%ld \n",msg,e,modulo);
    int d=calculate_d_RSA(modulo,e);
    printf("Result: %ld\n", mod_exp(msg,d,modulo));
}
int calculate_d_RSA(int numx, int numy) {
    printf("Number are %d, %d\n",numx,numy);
    int factores[100];     //Arreglo para almacenar factores de "numero".
    int i_factores = 0;       //Indice para recorrer el arreglo "factores[]".
    int i = 2;             //Empezaremos a verificar todos los factores desde 2.
    while (i <= numx) {
        if ((numx % i) == 0)
        {
            factores[i_factores] = i;
            numx = numx / i;
            i_factores++;
            continue;
        }
        i++;
    }
    i = 0;
    printf("The number primes are: ");
    while (i < i_factores) {
        printf("%d, ", factores[i]);
        factores[i]-=1;
        i++;
    }
    i=0;
    printf("\nThe new number: ");
    while (i < i_factores) {
        printf("%d, ", factores[i]);
        i++;
    }
    int x=factores[0]*factores[1];
    int result=modInverse(numy,x);
    printf("D: %d\n",result);
    printf("\n");
    return result;

}

// End of file