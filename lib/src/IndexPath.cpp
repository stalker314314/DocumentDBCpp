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

#include <assert.h>
#include <string>

#include <cpprest/json.h>

#include "IndexPath.h"
#include "DocumentDBConstants.h"

using namespace documentdb;
using namespace std;
using namespace utility;
using namespace web::json;

IndexPath::IndexPath(
	string_t path,
	const vector<shared_ptr<Index>>& indexes)
	: path_(path)
	, indexes_(indexes)
{
}

IndexPath::~IndexPath()
{}


shared_ptr<IndexPath> IndexPath::FromJson(
	const value& json_payload)
{
	string_t path = json_payload.at(RESPONSE_INDEX_PATH).as_string();

	vector<shared_ptr<Index>> indexes;
	auto indexes_json = json_payload.at(RESPONSE_INDEXING_POLICY_INCLUDED_PATHS_INDEXES).as_array();
	for (auto jsonIndexes : indexes_json)
	{
		indexes.push_back(Index::FromJson(jsonIndexes));
	}

	return make_shared<IndexPath>(path, indexes);
}
