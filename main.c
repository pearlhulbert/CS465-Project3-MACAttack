#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

void prepareSchedule(u_int32_t*);

void sha1(u_int32_t, int, u_int32_t*);

void rotLeft(u_int32_t, int);
u_int32_t ch(u_int32_t, u_int32_t, u_int32_t);
u_int32_t parity(u_int32_t, u_int32_t, u_int32_t);
u_int32_t maj(u_int32_t, u_int32_t, u_int32_t);

int main(int argc, char **argv) {

    char *message = argv[1];
    u_int32_t messageAsBytes[16];
    u_int32_t initHashes[5] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0};
    //sha1()
    //array of chars, 1st character a, 2nd 0x80, 0x08
    //hash and see if it's the same as sha1sum for a
    return 0;
}

void prepareSchedule(u_int32_t *messageAsBytes) {
    bzero(messageAsBytes, sizeOf(messageAsBytes));

}

void rotLeft(u_int32_t w, int numBits) {
    return (w << numBits) | (w >> (32 - numBits));
}

u_int32_t ch(u_int32_t x, u_int32_t y, u_int32_t z) {
    return (x & y) ^ (!x & z);
}

u_int32_t parity(u_int32_t, u_int32_t, u_int32_t) {
    return x ^ y ^ z;
}

u_int32_t maj(u_int32_t, u_int32_t, u_int32_t) {
    return (x & y) ^ (x & z) ^ (y & z);
}

void sha1(u_int8_t* w, int numBlocks, u_int32_t* initHashes) {

    u_int32_t keySchedule[numBlocks];

    for (int i = 1; i < numBlocks; ++i) {
        if (i < 16) {
            memcpy(keySchedule + i, w + (4*i), 4);
        }
        else {
            rotLeft((keySchedule[i-3] ^ keySchedule[i-8] ^ keySchedule[i-14] ^ keySchedule[i-16]), 1);
        }
    }


    u_int32_t a = initHashes[0];
    u_int32_t b = initHashes[1];
    u_int32_t c = initHashes[2];
    u_int32_t d = initHashes[3];
    u_int32_t e = initHashes[4];

    u_int32_t T;
    u_int_32_t K;

    for (int i = 0; i < 80; ++i) {
        if (i <= 19) {
            K = 0x5a827999;
            T = rotLeft(a, 5)+ ch(b, c, d) + e + keySchedule[i];
        }
        else if ((i <= 20) && (i <= 39)) {
            K = 0x6ed9eba1;
            T = rotLeft(a, 5)+ parity(b, c, d) + e + keySchedule[i];
        }
        else if ((i <= 40) && (i <= 59)) {
            K = 0x8f1bbcdc;
            T = rotLeft(a, 5)+ maj(b, c, d) + e + keySchedule[i];
        }
        else {
            K = 0xca62c1d6;
            T = rotLeft(a, 5)+ parity(b, c, d) + e + keySchedule[i];
        }

        e = d;
        d = c;
        c = rotLeft(b, 30);
        b = a;
        a = T;
    }

    initHashes[0] += a;
    initHashes[1] += b;
    initHashes[2] += c;
    initHashes[3] += d;
    initHashes[4] += e;

}