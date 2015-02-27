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

#include "TriggerIterator.h"

#include <cpprest/http_client.h>

#include "Collection.h"
#include "ConnectionHelper.h"
#include "DocumentDBConstants.h"
#include "exceptions.h"

using namespace documentdb;
using namespace std;
using namespace web::http;
using namespace web::json;
using namespace web::http::client;

TriggerIterator::TriggerIterator(
	const shared_ptr<const Collection>& collection,
	const wstring& original_query,
	const int page_size,
	const wstring& original_request_uri,
	const wstring& continuation_id,
	const value& buffer)
	: collection_(collection)
	, original_query_(original_query)
	, page_size_(page_size)
	, original_request_uri_(original_request_uri)
	, continuation_id_(continuation_id)
	, buffer_(buffer)
	, current_(0)
{}

TriggerIterator::~TriggerIterator()
{}

bool TriggerIterator::HasMore()
{
	if (current_ < buffer_.as_array().size())
	{
		return true;
	}


	if (continuation_id_.empty())
	{
		return false;
	}

	// Case where we have continuation, but we are not really sure do we have anything more, need to fetch it.
	//
	http_request request = CreateQueryRequest(
		original_query_,
		page_size_,
		RESOURCE_PATH_TRIGGERS,
		collection_->resource_id(),
		collection_->document_db_configuration()->master_key(),
		continuation_id_);
	request.set_request_uri(original_request_uri_);

	http_response response = collection_->document_db_configuration()->http_client().request(request).get();
	value json_response = response.extract_json().get();

	if (response.status_code() == status_codes::OK)
	{
		wstring new_continuation_id = response.headers()[HEADER_MS_CONTINUATION];
		int count = stoi(response.headers()[HEADER_MS_MAX_ITEM_COUNT]);

		buffer_ = json_response.at(RESPONSE_QUERY_DOCUMENTS);
		continuation_id_ = new_continuation_id;
		current_ = 0;
		return count > 0;
	}

	ThrowExceptionFromResponse(response.status_code(), json_response);
}

shared_ptr<Trigger> TriggerIterator::Next()
{
	if (current_ < buffer_.as_array().size())
	{
		value trigger_json = buffer_.as_array().at(current_++);
		return collection_->TriggerFromJson(&trigger_json);
	}

	// Did you called hasMore()?
	//
	assert(false);
	throw DocumentDBRuntimeException(wstring(L"Calling Next without checking HasMore before that."));
}
