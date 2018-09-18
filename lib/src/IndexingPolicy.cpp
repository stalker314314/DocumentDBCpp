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

#include "IndexingPolicy.h"

#include <cpprest/json.h>

#include "DocumentDBConstants.h"
#include "exceptions.h"

using namespace documentdb;
using namespace std;
using namespace utility;
using namespace web::json;

IndexingPolicy::IndexingPolicy()
	: automatic_(true)
	, indexing_mode_(IndexingMode::CONSISTENT)
	, excluded_paths_()
	, included_paths_(0)
{
	included_paths_.push_back(make_shared<Index>(IndexType::HASH, 3, 3, _XPLATSTR("/")));
	included_paths_.push_back(make_shared<Index>(IndexType::RANGE, 6, 3, _XPLATSTR("/\"_ts\"/?")));
}

IndexingPolicy::IndexingPolicy(
	const bool automatic,
	const IndexingMode& indexing_mode,
	const vector<shared_ptr<Index>>& included_paths,
	const vector<string_t>& excluded_paths)
	: automatic_(automatic)
	, indexing_mode_(indexing_mode)
	, included_paths_(included_paths)
	, excluded_paths_(excluded_paths)
{
}

IndexingPolicy::~IndexingPolicy()
{}

IndexingPolicy IndexingPolicy::FromJson(
	const value& json_payload)
{
	bool automatic = json_payload.at(RESPONSE_INDEXING_POLICY_AUTOMATIC).as_bool();

	string_t indexing_mode_str = json_payload.at(RESPONSE_INDEXING_POLICY_INDEXING_MODE).as_string();
	IndexingMode indexing_mode;
	if (comparei(indexing_mode_str, _XPLATSTR("CONSISTENT")))
	{
		indexing_mode = IndexingMode::CONSISTENT;
	}
	else if (comparei(indexing_mode_str, _XPLATSTR("LAZY")))
	{
		indexing_mode = IndexingMode::LAZY;
	}
	else
	{
		throw DocumentDBRuntimeException(_XPLATSTR("Unsupported indexing policy: ") + indexing_mode_str);
	}

	vector<shared_ptr<Index>> included_paths;

	included_paths.push_back(make_shared<Index>(IndexType::HASH, -1, 3, _XPLATSTR("/*")));
	included_paths.push_back(make_shared<Index>(IndexType::RANGE, -1, 3, _XPLATSTR("/*")));

	vector<string_t> excluded_paths;
	auto excluded_paths_json = json_payload.at(RESPONSE_INDEXING_POLICY_EXCLUDED_PATHS).as_array();
	for (auto jsonExcludedPath : excluded_paths_json)
	{
		excluded_paths.push_back(jsonExcludedPath.at(RESPONSE_INDEXING_POLICY_PATH).as_string());
	}

	return IndexingPolicy(automatic, indexing_mode, included_paths, excluded_paths);
}
