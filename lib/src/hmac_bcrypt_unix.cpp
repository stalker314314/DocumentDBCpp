/***
* The MIT License (MIT)
*
* Copyright (c) 2015 DocumentDBCpp
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
***/

#include "hmac_bcrypt.h"

#include <cstring>
#include <openssl/x509.h>
#include <openssl/hmac.h>


unsigned char* hmac(
    const unsigned char* key,       /* pointer to authentication key */
    int                  keySize,    /* length of authentication key  */
    const unsigned char* message,      /* pointer to data stream        */
    int                  messageSize,   /* length of data stream         */
    int*                 cbHash)    /* caller digest to be filled in */
{
	unsigned char* digest = new unsigned char[SHA256_DIGEST_LENGTH];

    unsigned char k_ipad[65];   /* inner padding -
                                 * key XORd with ipad
                                 */
    unsigned char k_opad[65];   /* outer padding -
                                 * key XORd with opad
                                 */
    unsigned char tk[SHA256_DIGEST_LENGTH];
    unsigned char tk2[SHA256_DIGEST_LENGTH];
    unsigned char bufferIn[1024];
    unsigned char bufferOut[1024];
    int           i;

    /* if key is longer than 64 bytes reset it to key=sha256(key) */
    if ( keySize > 64 ) {
        SHA256( key, keySize, tk );
        key     = tk;
        keySize = SHA256_DIGEST_LENGTH;
    }

    /*
     * the HMAC_SHA256 transform looks like:
     *
     * SHA256(K XOR opad, SHA256(K XOR ipad, text))
     *
     * where K is an n byte key
     * ipad is the byte 0x36 repeated 64 times
     * opad is the byte 0x5c repeated 64 times
     * and text is the data being protected
     */

    /* start out by storing key in pads */
    std::memset( k_ipad, 0, sizeof k_ipad );
    std::memset( k_opad, 0, sizeof k_opad );
    std::memcpy( k_ipad, key, keySize );
    std::memcpy( k_opad, key, keySize );

    /* XOR key with ipad and opad values */
    for ( i = 0; i < 64; i++ ) {
        k_ipad[i] ^= 0x36;
        k_opad[i] ^= 0x5c;
    }

    /*
     * perform inner SHA256
     */
    std::memset( bufferIn, 0x00, 1024 );
    std::memcpy( bufferIn, k_ipad, 64 );
    std::memcpy( bufferIn + 64, message, messageSize );

    SHA256( bufferIn, 64 + messageSize, tk2 );

    /*
     * perform outer SHA256
     */
    std::memset( bufferOut, 0x00, 1024 );
    std::memcpy( bufferOut, k_opad, 64 );
    std::memcpy( bufferOut + 64, tk2, SHA256_DIGEST_LENGTH );

    SHA256( bufferOut, 64 + SHA256_DIGEST_LENGTH, digest );

    *cbHash = SHA256_DIGEST_LENGTH;
    return digest;
}
