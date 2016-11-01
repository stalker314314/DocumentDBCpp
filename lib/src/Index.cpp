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

#include "Index.h"
#include "DocumentDBConstants.h"

using namespace documentdb;
using namespace std;
using namespace web::json;

Index::Index(
		IndexType index_type,
		int numeric_precision,
		int string_precision,
		wstring path)
	: index_type_(index_type)
	, numeric_precision_(numeric_precision)
	, string_precision_(string_precision)
	, path_(path)
{
}

Index::~Index()
{}

bool Index::operator == (const Index& other) const
{
	if (
		(index_type_ == other.index_type_) &&
		(numeric_precision_ == other.numeric_precision_) &&
		(string_precision_ == other.string_precision_) &&
		(path_ == other.path_)
		)
	{
		return true;
	}
	else
	{
		return false;
	}
}

shared_ptr<Index> Index::FromJson(
	const value& json_payload)
{
	IndexType index_type = IndexType::HASH;
	if (comparei(json_payload.at(RESPONSE_INDEX_INDEX_TYPE).as_string(), L"RANGE"))
	{
		index_type = IndexType::RANGE;
	}
#ifdef _DEBUG
	else
	{
		assert(comparei(json_payload.at(RESPONSE_INDEX_INDEX_TYPE).as_string(), L"HASH"));
	}
#endif

	int numeric_precision = 3;
	if (json_payload.has_field(RESPONSE_INDEX_NUMERIC_PRECISION))
	{
		numeric_precision = json_payload.at(RESPONSE_INDEX_NUMERIC_PRECISION).as_integer();
	}

	int string_precision = 3;
	if (json_payload.has_field(RESPONSE_INDEX_STRING_PRECISION))
	{
		string_precision = json_payload.at(RESPONSE_INDEX_STRING_PRECISION).as_integer();
	}

	wstring path = json_payload.at(RESPONSE_INDEX_PATH).as_string();

	return make_shared<Index>(index_type, numeric_precision, string_precision, path);
}
