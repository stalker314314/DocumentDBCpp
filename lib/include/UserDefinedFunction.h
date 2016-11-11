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

#ifndef _DOCUMENTDB_USER_DEFINED_FUNCTION_H_
#define _DOCUMENTDB_USER_DEFINED_FUNCTION_H_

#include <string>
#include <memory>

#include <cpprest/http_client.h>

#include "DocumentDBEntity.h"
#include "DocumentDBConfiguration.h"
#include "exceptions.h"

namespace documentdb {
	class UserDefinedFunction : public DocumentDBEntity
	{
	public:
		UserDefinedFunction(
			const std::shared_ptr<const DocumentDBConfiguration>& document_db_configuration,
			const utility::string_t& id,
			const utility::string_t& resource_id,
			unsigned long ts,
			const utility::string_t& self,
			const utility::string_t& etag,
			const utility::string_t& body);

		virtual ~UserDefinedFunction();

		utility::string_t body() const
		{
			return body_;
		}

	private:
		utility::string_t body_;
	};
}

#endif // !_DOCUMENTDB_USER_DEFINED_FUNCTION_H_
