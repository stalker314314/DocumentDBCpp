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

#ifndef _DOCUMENTDB_DOCUMENT_DB_CONFIGURATION_H_
#define _DOCUMENTDB_DOCUMENT_DB_CONFIGURATION_H_

#include <string>
#include <vector>

#include <cpprest/http_client.h>

class DocumentDBConfiguration
{
public:
	DocumentDBConfiguration (
		std::wstring url_connection,
		std::wstring master_key);

	virtual ~DocumentDBConfiguration ();

	std::vector<unsigned char> master_key () const
	{
		return master_key_;
	}

	web::http::client::http_client http_client () const
	{
		return http_client_;
	}

private:
	std::wstring url_connection_;
	std::vector<unsigned char> master_key_;
	web::http::client::http_client http_client_;
};

#endif // !_DOCUMENTDB_DOCUMENT_DB_CONFIGURATION_H_
