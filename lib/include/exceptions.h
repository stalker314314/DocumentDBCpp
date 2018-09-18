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

#ifndef _DOCUMENTDB_EXCEPTIONS_H_
#define _DOCUMENTDB_EXCEPTIONS_H_

#include <cpprest/http_client.h>

#define MAX_QUERY_RETRY_CNT	(20)

namespace documentdb
{
	class DocumentDBRuntimeException
	{
	public:
		DocumentDBRuntimeException(
			const utility::string_t& message)
			: message_(message)
		{
		}

		utility::string_t message() const
		{
			return message_;
		}

	private:
		utility::string_t message_;
	};

	class DocumentDBResponseException : public DocumentDBRuntimeException
	{
	public:
		DocumentDBResponseException(
			const web::http::status_code& status_code,
			const utility::string_t& code,
			const utility::string_t& message)
			: DocumentDBRuntimeException(message)
			, status_code_(status_code)
			, code_(code)
		{
		}

	private:
		web::http::status_code status_code_;
		utility::string_t code_;
	};

	class ResourceAlreadyExistsException : public DocumentDBResponseException
	{
	public:
		ResourceAlreadyExistsException(
			const web::http::status_code& status_code,
			const utility::string_t& code,
			const utility::string_t& message)
			: DocumentDBResponseException(status_code, code, message)
		{
		}
	};

	class ResourceNotFoundException : public DocumentDBResponseException
	{
	public:
		ResourceNotFoundException(
			const web::http::status_code& status_code,
			const utility::string_t& code,
			const utility::string_t& message)
			: DocumentDBResponseException(status_code, code, message)
		{
		}
	};

	class DocumentTooLargeException : public DocumentDBResponseException
	{
	public:
		DocumentTooLargeException(
			const web::http::status_code& status_code,
			const utility::string_t& code,
			const utility::string_t& message)
			: DocumentDBResponseException(status_code, code, message)
		{
		}
	};
}
#endif // !_DOCUMENTDB_EXCEPTIONS_H_
