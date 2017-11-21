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

#ifndef _DOCUMENTDB_INDEX_PATH_H_
#define _DOCUMENTDB_INDEX_PATH_H_

#include <memory>
#include <vector>
#include <string>

#include <cpprest/json.h>

#include "Index.h"

namespace documentdb
{
	class IndexPath
	{
	public:
		IndexPath(
			utility::string_t path,
			const std::vector<std::shared_ptr<Index>>& indexes
			);

		virtual ~IndexPath();

		static std::shared_ptr<IndexPath> FromJson(
			const web::json::value& json_payload);

		utility::string_t path() const
		{
			return path_;
		}

	private:
		utility::string_t path_;
		std::vector<std::shared_ptr<Index>> indexes_;
	};
}

#endif // !_DOCUMENTDB_INDEX_PATH_H_
