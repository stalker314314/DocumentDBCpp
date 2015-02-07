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
	class Index
	{
	public:
		Index(
			IndexType index_type,
			int numeric_precision,
			int string_precision,
			std::wstring path);

		virtual ~Index();

		bool operator==(const Index& other) const;

		bool operator!=(const Index& other) const
		{
			return !(*this == other);
		}

		static std::shared_ptr<Index> FromJson(
			const web::json::value& json_payload);

		IndexType index_type() const
		{
			return index_type_;
		}

		int numeric_precision() const
		{
			return numeric_precision_;
		}

		int string_precision() const
		{
			return string_precision_;
		}

		std::wstring path() const
		{
			return path_;
		}

	private:
		IndexType index_type_;
		int numeric_precision_;
		int string_precision_;
		std::wstring path_;
	};
}

#endif // !_DOCUMENTDB_INDEX_H_