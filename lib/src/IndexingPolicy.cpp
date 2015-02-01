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

using namespace documentdb;
using namespace std;
using namespace web::json;

static bool comparei (
	wstring string1,
	wstring string2)
{
	transform (string1.begin (), string1.end (), string1.begin (), toupper);
	transform (string2.begin (), string2.end (), string2.begin (), toupper);

	return (string1 == string2);
}

IndexingPolicy::IndexingPolicy ()
	: automatic_ (true)
	, indexing_mode_ (IndexingMode::CONSISTENT)
	, excluded_paths_ ()
	, included_paths_ (0)
{
	included_paths_.push_back (make_shared<Index> (IndexType::HASH, 3, 3, L"/"));
	included_paths_.push_back (make_shared<Index> (IndexType::RANGE, 6, 3, L"/\"_ts\"/?"));
}

IndexingPolicy::IndexingPolicy (
		const bool automatic,
		const IndexingMode& indexing_mode,
		const vector<shared_ptr<Index>>& included_paths,
		const vector<wstring>& excluded_paths)
	: automatic_ (automatic)
	, indexing_mode_ (indexing_mode)
	, included_paths_ (included_paths)
	, excluded_paths_ (excluded_paths)
{
}

IndexingPolicy::~IndexingPolicy ()
{}

IndexingPolicy IndexingPolicy::FromJson (
	const value& json_payload)
{
	bool automatic = json_payload.at (RESPONSE_INDEXING_POLICY_AUTOMATIC).as_bool ();

	wstring indexing_mode_str = json_payload.at (RESPONSE_INDEXING_POLICY_INDEXING_MODE).as_string ();
	IndexingMode indexing_mode = IndexingMode::CONSISTENT;
	if (comparei (indexing_mode_str, L"LAZY"))
	{
		indexing_mode = IndexingMode::LAZY;
	}
#ifdef _DEBUG
	else
	{
		assert (comparei (indexing_mode_str, L"CONSISTENT"));
	}
#endif
	vector<shared_ptr<Index>> included_paths;
	auto included_paths_json = json_payload.at (RESPONSE_INDEXING_POLICY_INCLUDED_PATHS).as_array ();
	for (auto jsonIncludePath : included_paths_json)
	{
		included_paths.push_back (Index::FromJson (jsonIncludePath));
	}

	vector<wstring> excluded_paths;
	auto excluded_paths_json = json_payload.at (RESPONSE_INDEXING_POLICY_EXCLUDED_PATHS).as_array ();
	for (auto jsonExcludedPath : excluded_paths_json)
	{
		excluded_paths.push_back (jsonExcludedPath.at (RESPONSE_INDEXING_POLICY_PATH).as_string ());
	}

	return IndexingPolicy (automatic, indexing_mode, included_paths, excluded_paths);
}
