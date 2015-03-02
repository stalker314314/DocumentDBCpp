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

#ifndef _DOCUMENTDB_INDEXING_POLICY_H_
#define _DOCUMENTDB_INDEXING_POLICY_H_

#include <vector>
#include <string>

#include <cpprest/json.h>

#include "IndexingMode.h"
#include "Index.h"

namespace documentdb
{
	bool compare(std::wstring string1,
		std::wstring string2);

	class IndexingPolicy
	{
	public:
		IndexingPolicy();
		IndexingPolicy(
			const bool automatic,
			const IndexingMode& indexing_mode,
			const std::vector<std::shared_ptr<Index>>& included_paths,
			const std::vector<std::wstring>& excluded_paths);

		virtual ~IndexingPolicy();

		static IndexingPolicy FromJson(const web::json::value& json_payload);

		bool automatic() const
		{
			return automatic_;
		}

		IndexingMode indexing_mode() const
		{
			return indexing_mode_;
		}

		std::vector<std::shared_ptr<Index>> included_paths() const
		{
			return included_paths_;
		}

		std::vector<std::wstring> excluded_paths() const
		{
			return excluded_paths_;
		}

	private:
		bool automatic_;
		IndexingMode indexing_mode_;
		std::vector<std::shared_ptr<Index>> included_paths_;
		std::vector<std::wstring> excluded_paths_;
	};
}

#endif // !_DOCUMENTDB_INDEXING_POLICY_H_
