#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "secretKeyTest.h"

// Main
int main(int argc, char ** argv)
{

    int count=0;
    const unsigned char mimsg[8]={
            0x48,0x45,0x45,0x58,0x49,0x53,0x54,0x45

    };

    unsigned char msgexpanded[8]={0};
    mimsgpermutation(mimsg, msgexpanded);
    printf("Des Message Expandied: \n");
    for(count = 0; count < 8; count++)
    {
        printf("%02X ", msgexpanded[count]);
    }
    printf("\n");
    const unsigned char mikey[8]={
//            0x13,0x34,0x57,0x79,0x9B,0xBC,0xDF,0xF1
            0x48,0x45,0x45,0x58,0x49,0x53,0x54,0x45

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

    const unsigned char D[4]={
            0x00,0x00,0xA1,0x50

    };
    const unsigned char E[4]={
            0xFF ,0x54 ,0x1C ,0xBA

    };
    const unsigned char K[6]={
            0xA0,0x92,0xCA,0x33,0x48,0xD8

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
            0x47,0x36,0xF2,0xA7,0xA0,0x50,0x2C,0x1C,0x1D,0x69,0x53,0x11,0x52,0xDA,0x3E,0x55

    };
    unsigned char aesmsg[16]={
            0xFE,0xDA,0x7A,0x3E,0x20,0x83,0xBD,0x11,0x6F,0x0C,0x68,0xA9,0xF7,0x18,0xBA,0x89

    };
    unsigned char aesresult[4]={0};
    miAddroundKey(aesresult,aeskey,aesmsg);
    printf("Aes AddRoundKey: \n");
    for(count = 0; count < 4; count++)
    {
        printf("%02X ", aesresult[count]);
    }
    printf("\n");
    unsigned char aesbyteSub[][4]={
            0xd0,0xc2 ,0x73,0x28,
            0x7b,0xcd ,0x6a,0x21,
            0xab,0xc3 ,0x7f,0x61,
            0x72,0x23 ,0xfe,0xc1
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



    return EXIT_SUCCESS;
}
