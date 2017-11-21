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

#ifndef _DOCUMENTDB_INDEX_H_
#define _DOCUMENTDB_INDEX_H_

#include <memory>
#include <string>

#include <cpprest/json.h>

#include "IndexType.h"

namespace documentdb
{
	bool comparei(
		utility::string_t string1,
		utility::string_t string2);

	class Index
	{
	public:
		Index(
			IndexType kind,
			const utility::string_t& dataType,
			int string_precision);

		virtual ~Index();

		bool operator==(const Index& other) const;

		bool operator!=(const Index& other) const
		{
			return !(*this == other);
		}

		static std::shared_ptr<Index> FromJson(
			const web::json::value& json_payload);

		IndexType kind() const
		{
			return kind_;
		}

		utility::string_t data_type() const
		{
			return data_type_;
		}

		int string_precision() const
		{
			return string_precision_;
		}

	private:
		IndexType kind_;
		utility::string_t data_type_;
		int string_precision_;
	};
}

#endif // !_DOCUMENTDB_INDEX_H_
