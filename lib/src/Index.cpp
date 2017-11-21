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
using namespace utility;
using namespace web::json;

Index::Index(
		IndexType kind,
		const utility::string_t& data_type,
		int string_precision)
	: kind_(kind)
	, data_type_(data_type)
	, string_precision_(string_precision)
{
}

Index::~Index()
{}

bool Index::operator == (const Index& other) const
{
	if (
		(kind_ == other.kind_) &&
		(data_type_ == other.data_type_) &&
		(string_precision_ == other.string_precision_)
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
	IndexType kind;
	if (comparei(json_payload.at(RESPONSE_INDEX_KIND).as_string(), _XPLATSTR("RANGE")))
	{
		kind = IndexType::RANGE;
	}
	else if (comparei(json_payload.at(RESPONSE_INDEX_KIND).as_string(), _XPLATSTR("HASH")))
	{
		kind = IndexType::HASH;
	}
	else
	{
		kind = IndexType::SPATIAL;
#ifdef _DEBUG
		assert(comparei(json_payload.at(RESPONSE_INDEX_KIND).as_string(), _XPLATSTR("SPATIAL")));
#endif
	}

	utility::string_t dataType = _XPLATSTR("");
	dataType = json_payload.at(RESPONSE_INDEX_DATA_TYPE).as_string();

	int precision = -1;
	if (json_payload.has_field(RESPONSE_INDEX_PRECISION))
	{
		precision = json_payload.at(RESPONSE_INDEX_PRECISION).as_integer();
	}

	return make_shared<Index>(kind, dataType, precision);
}
