#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <arpa/inet.h>

void sha1(u_int8_t*, u_int32_t*);

u_int32_t rotLeft(u_int32_t, int);
u_int32_t ch(u_int32_t, u_int32_t, u_int32_t);
u_int32_t parity(u_int32_t, u_int32_t, u_int32_t);
u_int32_t maj(u_int32_t, u_int32_t, u_int32_t);

int main() {

    u_int8_t paddedMessage[189];
    bzero(paddedMessage, 189);
    u_int8_t ogMessage[47] = "No one has completed lab 2 so give them all a 0";
    u_int8_t myMessage[61] = "P.S. Except for Pearl, go ahead and give her the full points.";
    memcpy(paddedMessage, ogMessage, 47);
    paddedMessage[47] = 0x80;
    u_int32_t ogMessageLen = htonl(504);
    memcpy(paddedMessage + 124, &ogMessageLen, 4);
    memcpy(paddedMessage + 128, myMessage, 61);
    for (int i = 0; i < 189; ++i) {
        printf("%02hhx", paddedMessage[i]);
    }
    printf("\n");
    u_int8_t hashMessage[189];
    bzero(hashMessage, 189);
    memcpy(hashMessage, myMessage, 61);
    hashMessage[61] = 0x80;
    u_int32_t messageLen = htonl(1512);
    memcpy(hashMessage + 185, &messageLen, 4);
    for (int i = 0; i < 189; ++i) {
        printf("%02hhx", hashMessage[i]);
    }
    printf("\n");
    //put in mac as iv
    u_int32_t initHashes[5] = {0xe384efad, 0xf26767a6, 0x13162142, 0xb5ef0efb, 0xb9d7659a};
    sha1(hashMessage, initHashes);
    sha1((hashMessage + 63), initHashes);
    for (int i = 0; i < 5; ++i) {
        printf("%08x", initHashes[i]);
    }
    printf("\n");
    return 0;
}


u_int32_t rotLeft(u_int32_t w, int numBits) {
    return (w << numBits) | (w >> (32 - numBits));
}

u_int32_t ch(u_int32_t x, u_int32_t y, u_int32_t z) {
    return (x & y) ^ (~x & z);
}

u_int32_t parity(u_int32_t x, u_int32_t y, u_int32_t z) {
    return x ^ y ^ z;
}

u_int32_t maj(u_int32_t x, u_int32_t y, u_int32_t z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

void sha1(u_int8_t *block, u_int32_t *initial) {

    u_int32_t keys[] = {0x5a827999,
                        0x6ed9eba1,
                        0x8f1bbcdc,
                        0xca62c1d6};

    //Generate Message Schedule
    u_int32_t schedule[80];
    for (int i = 0; i < 80; i++) {
        if (i < 16) {
            memcpy(schedule + i, block + (4 * i), 4);
            schedule[i] = htonl(schedule[i]);
        }
        else {
            schedule[i] = rotLeft((schedule[i-3] ^ schedule[i-8] ^ schedule[i-14] ^ schedule[i-16]), 1);
        }
    }
    //Set initial hashes
    u_int32_t a = initial[0];
    u_int32_t b = initial[1];
    u_int32_t c = initial[2];
    u_int32_t d = initial[3];
    u_int32_t e = initial[4];
    /*
    a = htonl(a);
    b = htonl(b);
    c = htonl(c);
    d = htonl(d);
    e = htonl(e);
    */
    for (int i = 0; i < 80; i++) {
        u_int32_t tmp;
        if (i < 20) {
            tmp = rotLeft(a, 5) + ch(b, c, d) + e + keys[0] + schedule[i];
        }
        else if (i >= 20 && i < 40) {
            tmp = rotLeft(a, 5) + parity(b, c, d) + e + keys[1] + schedule[i];
        }
        else if (i >= 40 && i < 60) {
            tmp = rotLeft(a, 5) + maj(b, c, d) + e + keys[2] + schedule[i];
        }
        else {
            tmp = rotLeft(a, 5) + parity(b, c, d) + e + keys[3] + schedule[i];
        }
        e = d;
        d = c;
        c = rotLeft(b, 30);
        b = a;
        a = tmp;
    }
    initial[0] += a;
    initial[1] += b;
    initial[2] += c;
    initial[3] += d;
    initial[4] += e;
}

