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

#ifndef _DOCUMENTDB_DOCUMENTDBENTITY_H_
#define _DOCUMENTDB_DOCUMENTDBENTITY_H_

#include <memory>
#include <string>

#include "DocumentDBConfiguration.h"

namespace documentdb
{
	class  DocumentDBEntity
	{
	public:
		DocumentDBEntity(
			const std::shared_ptr<const DocumentDBConfiguration>& document_db_configuration,
			const utility::string_t& id,
			const utility::string_t& resource_id,
			const unsigned long ts,
			const utility::string_t& self,
			const utility::string_t& etag);

		virtual ~DocumentDBEntity();

		utility::string_t id() const
		{
			return id_;
		}

		utility::string_t resource_id() const
		{
			return resource_id_;
		}

		unsigned long ts() const
		{
			return ts_;
		}

		utility::string_t self() const
		{
			return self_;
		}

		utility::string_t etag() const
		{
			return etag_;
		}

		std::shared_ptr<const DocumentDBConfiguration> document_db_configuration() const
		{
			return document_db_configuration_;
		}

	private:
		std::shared_ptr<const DocumentDBConfiguration> document_db_configuration_;

		utility::string_t id_;
		utility::string_t resource_id_;
		unsigned long ts_;
		utility::string_t self_;
		utility::string_t etag_;
	};
}

#endif // ! _DOCUMENTDB_DOCUMENTDBENTITY_H_
