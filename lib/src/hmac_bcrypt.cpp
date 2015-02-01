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

#include <windows.h>
#include <stdio.h>
#include <bcrypt.h>
#include <string>

#include "exceptions.h"

#pragma comment(lib, "bcrypt.lib")

#define NT_SUCCESS(Status)          (((NTSTATUS)(Status)) >= 0)

#define STATUS_UNSUCCESSFUL         ((NTSTATUS)0xC0000001L)

BYTE* hmac (const BYTE* key, DWORD keySize, const BYTE* message, DWORD messageSize, DWORD* cbHash)
{
	BCRYPT_ALG_HANDLE       hAlg = NULL;
	BCRYPT_HASH_HANDLE      hHash = NULL;
	NTSTATUS                status = STATUS_UNSUCCESSFUL;
	DWORD                   cbData = 0, cbHashObject = 0;
	PBYTE                   pbHashObject = NULL;
	PBYTE                   pbHash = NULL;
	bool                    error = false;

	//open an algorithm handle
	if (!NT_SUCCESS (status = BCryptOpenAlgorithmProvider (
		&hAlg,
		BCRYPT_SHA256_ALGORITHM,
		NULL,
		BCRYPT_ALG_HANDLE_HMAC_FLAG)))
	{
		error = true;
		goto Cleanup;
	}

	//calculate the size of the buffer to hold the hash object
	if (!NT_SUCCESS (status = BCryptGetProperty (
		hAlg,
		BCRYPT_OBJECT_LENGTH,
		(PBYTE)&cbHashObject,
		sizeof (DWORD),
		&cbData,
		0)))
	{
		error = true;
		goto Cleanup;
	}

	//allocate the hash object on the heap
	pbHashObject = (PBYTE)HeapAlloc (GetProcessHeap (), 0, cbHashObject);
	if (NULL == pbHashObject)
	{
		error = true;
		goto Cleanup;
	}

	//calculate the length of the hash
	if (!NT_SUCCESS (status = BCryptGetProperty (
		hAlg,
		BCRYPT_HASH_LENGTH,
		(PBYTE)cbHash,
		sizeof (DWORD),
		&cbData,
		0)))
	{
		error = true;
		goto Cleanup;
	}

	//allocate the hash buffer on the heap
	pbHash = (PBYTE)HeapAlloc (GetProcessHeap (), 0, *cbHash);
	if (NULL == pbHash)
	{
		error = true;
		goto Cleanup;
	}

	//create a hash
	if (!NT_SUCCESS (status = BCryptCreateHash (
		hAlg,
		&hHash,
		pbHashObject,
		cbHashObject,
		(PBYTE)key,
		keySize,
		0)))
	{
		error = true;
		goto Cleanup;
	}

	//hash some data
	if (!NT_SUCCESS (status = BCryptHashData (
		hHash,
		(PBYTE)message,
		messageSize,
		0)))
	{
		error = true;
		goto Cleanup;
	}

	//close the hash
	if (!NT_SUCCESS (status = BCryptFinishHash (
		hHash,
		pbHash,
		*cbHash,
		0)))
	{
		error = true;
		goto Cleanup;
	}

Cleanup:

	if (hAlg)
	{
		BCryptCloseAlgorithmProvider (hAlg, 0);
	}

	if (hHash)
	{
		BCryptDestroyHash (hHash);
	}

	if (pbHashObject)
	{
		HeapFree (GetProcessHeap (), 0, pbHashObject);
	}

	if (error)
	{
		if (pbHash != nullptr)
		{
			HeapFree (GetProcessHeap (), 0, pbHash);
		}

		throw documentdb::DocumentDBRuntimeException (L"Error during hmac hasing");
	}

	return pbHash;
}
