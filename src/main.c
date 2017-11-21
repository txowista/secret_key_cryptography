#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "secretKeyTest.h"

// Main
int main(int argc, char ** argv)
{
//    DES - Permutación inicial
    int count=0;
    const unsigned char mimsg[8]={
            0x44,0x45,0x46,0x4F,0x52,0x41,0x4C,0x41

    };

    unsigned char msgexpanded[8]={0};
    mimsgpermutation(mimsg, msgexpanded);
    printf("Des Message Expandied: \n");
    for(count = 0; count < 8; count++)
    {
        printf("%02X ", msgexpanded[count]);
    }
    printf("\n");
//    DES - Generación de subclaves
    const unsigned char mikey[8]={
//            0x13,0x34,0x57,0x79,0x9B,0xBC,0xDF,0xF1
            0x55,0x4E,0x41,0x41,0x4C,0x54,0x52,0x41

    };
    unsigned char keyexpanded[7]={0};
    unsigned char subkeyexpanded[6]={0};
    mikeypermutation(mikey, keyexpanded, subkeyexpanded);
    printf("Des SubKey: \n");
    for(count = 0; count < 6; count++)
    {
        printf("%02X ", subkeyexpanded[count]);

    }
    printf("\n");
//    DES - Detalle de una iteración
    const unsigned char D[4]={
            0x00,0x00,0x24,0x63

    };
    const unsigned char E[4]={
            0xFF ,0x5E ,0x70 ,0xE5

    };
    const unsigned char K[6]={
            0xA0,0x92,0xC2,0xD3,0xAF,0x8C

    };
    unsigned char result[4]={0};
    miround(result,D,K,E);
    printf("Des First Iteration: \n");
    for(count = 0; count < 4; count++)
    {
        printf("%02X ", result[count]);
    }
    printf("\n");

    unsigned char aeskey[16]={
            0x98,0xBB,0x7A,0xBC,0x5E,0xC5,0xD5,0x99,0x05,0x8F,0xAC,0xA4,0x74,0x5F,0x72,0x8B

    };
    unsigned char aesmsg[16]={
            0x8C,0xB0,0xDD,0x95,0xF7,0x1F,0x18,0xD7,0x94,0x2B,0x74,0xA8,0x3D,0x0B,0xA0,0xA9

    };
    unsigned char aesresult[4]={0};
    miAddroundKey(aesresult,aeskey,aesmsg);
    printf("Aes AddRoundKey: \n");
    for(count = 0; count < 4; count++)
    {
        printf("%02X ", aesresult[count]);
    }
    printf("\n");
//    AES - Función ByteSub
    unsigned char aesbyteSub[][4]={
            0x3d,0x27 ,0xa5,0xb1,
            0xc4,0x19 ,0xf7,0x49,
            0xbe,0xc3 ,0xe6,0x93,
            0x92,0x49 ,0xdd,0xc6
    };
    byteSub(aesbyteSub);
    int colum, row;
    printf("Aes Sub_Bytes:\n");
    for(row=0;row <4; row ++) {
        for (colum = 0; colum < 4; colum++) {
            printf("%02X ", aesbyteSub[row][colum]);
        }
        printf("\n");
    }
    printf("\n");
    calculate_d_RSA(141, 57);
    cipher_message(27,461,1177);
    decipher_message(74,57,141);



    return EXIT_SUCCESS;
}