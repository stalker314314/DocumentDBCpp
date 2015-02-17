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

#pragma comment(lib, "rpcrt4.lib")  // UuidCreate - Minimum supported OS Win 2000

#include "Collection.h"

#include <windows.h>

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>

#include "ConnectionHelper.h"
#include "DocumentDBConstants.h"
#include "exceptions.h"

using namespace documentdb;
using namespace std;
using namespace web::http;
using namespace web::json;
using namespace web::http::client;

Collection::Collection(
		const shared_ptr<const DocumentDBConfiguration>& document_db_configuration,
		const wstring& id,
		const wstring& resource_id,
		const unsigned long ts,
		const wstring& self,
		const wstring& etag,
		const wstring& docs,
		const wstring& sprocs,
		const wstring& triggers,
		const wstring& udfs,
		const wstring& conflicts,
		const IndexingPolicy& indexing_policy)
	: DocumentDBEntity(document_db_configuration, id, resource_id, ts, self, etag)
	, docs_(docs)
	, sprocs_(sprocs)
	, triggers_(triggers)
	, udfs_(udfs)
	, conflicts_(conflicts)
	, indexing_policy_(indexing_policy)
{}

Collection::~Collection()
{}

shared_ptr<Document> Collection::DocumentFromJson(
	const value& json_collection) const
{
	wstring id = json_collection.at(DOCUMENT_ID).as_string();
	wstring rid = json_collection.at(RESPONSE_RESOURCE_RID).as_string();
	unsigned long ts = json_collection.at(RESPONSE_RESOURCE_TS).as_integer();
	wstring self = json_collection.at(RESPONSE_RESOURCE_SELF).as_string();
	wstring etag = json_collection.at(RESPONSE_RESOURCE_ETAG).as_string();
	wstring attachments = json_collection.at(RESPONSE_RESOURCE_ATTACHMENTS).as_string();

	return make_shared<Document>(
		this->document_db_configuration(),
		id,
		rid,
		ts,
		self,
		etag,
		attachments,
		json_collection);
}

wstring Collection::GenerateGuid()
{
	UUID uuid;
	ZeroMemory(&uuid, sizeof(UUID));
	RPC_STATUS status = UuidCreate(&uuid);
	if (status != RPC_S_OK)
	{
		throw DocumentDBRuntimeException(L"Unable to create UUID");
	}

	wchar_t *str;
	status = UuidToStringW(&uuid, (RPC_WSTR*)&str);
	if (status == RPC_S_OUT_OF_MEMORY)
	{
		throw DocumentDBRuntimeException(L"Out of memory while creating UUID");
	}
	else if (status != RPC_S_OK)
	{
		throw DocumentDBRuntimeException(L"Unknown error while creating UUID");
	}

	wstring guid = str;
	RpcStringFreeW((RPC_WSTR*)&str);

	return guid;
}

Concurrency::task<shared_ptr<Document>> Collection::CreateDocumentAsync(
	const value& document) const
{
	value body = document;

	if (!body.has_field(DOCUMENT_ID))
	{
		body[DOCUMENT_ID] = value::string(GenerateGuid());
	}

	return this->CreateDocumentAsync(body.serialize());
}

Concurrency::task<shared_ptr<Document>> Collection::CreateDocumentAsync(
	const wstring& document) const
{
	http_request request = CreateRequest(
		methods::POST,
		RESOURCE_PATH_DOCS,
		this->resource_id(),
		this->document_db_configuration()->master_key());
	request.set_request_uri(this->self() + docs_);

	request.set_body(document);

	return this->document_db_configuration()->http_client().request(request).then([=](http_response response)
	{
		value json_response = response.extract_json().get();

		if (response.status_code() == status_codes::Created)
		{
			return DocumentFromJson(json_response);
		}

		ThrowExceptionFromResponse(response.status_code(), json_response);
	});
}

shared_ptr<Document> Collection::CreateDocument(
	const value& document) const
{
	return this->CreateDocumentAsync(document).get();
}

shared_ptr<Document> Collection::CreateDocument(
	const wstring& document) const
{
	return this->CreateDocumentAsync(document).get();
}

Concurrency::task<shared_ptr<Document>> Collection::GetDocumentAsync(
	const wstring& resource_id) const
{
	http_request request = CreateRequest(
		methods::GET,
		RESOURCE_PATH_DOCS,
		resource_id,
		this->document_db_configuration()->master_key());
	request.set_request_uri(this->self() + docs_ + resource_id);

	return this->document_db_configuration()->http_client().request(request).then([=](http_response response)
	{
		value json_response = response.extract_json().get();

		if (response.status_code() == status_codes::OK)
		{
			return DocumentFromJson(json_response);
		}

		ThrowExceptionFromResponse(response.status_code(), json_response);
	});
}

shared_ptr<Document> Collection::GetDocument(
	const wstring& resource_id) const
{
	return this->GetDocumentAsync(resource_id).get();
}

Concurrency::task<vector<shared_ptr<Document>>> Collection::ListDocumentsAsync() const
{
	http_request request = CreateRequest(
		methods::GET,
		RESOURCE_PATH_DOCS,
		this->resource_id(),
		this->document_db_configuration()->master_key());
	request.set_request_uri(this->self() + docs_);
	return this->document_db_configuration()->http_client().request(request).then([=](http_response response)
	{
		value json_response = response.extract_json().get();

		if (response.status_code() == status_codes::OK)
		{
			assert(this->resource_id() == json_response.at(RESPONSE_RESOURCE_RID).as_string());
			vector<shared_ptr<Document>> documents;
			documents.reserve(json_response.at(RESPONSE_BODY_COUNT).as_integer());
			value json_collections = json_response.at(RESPONSE_QUERY_DOCUMENTS);

			for (auto iter = json_collections.as_array().cbegin(); iter != json_collections.as_array().cend(); ++iter)
			{
				shared_ptr<Document> coll = DocumentFromJson(*iter);
				documents.push_back(coll);
			}
			return documents;
		}

		ThrowExceptionFromResponse(response.status_code(), json_response);
	});
}

vector<shared_ptr<Document>> Collection::ListDocuments() const
{
	return this->ListDocumentsAsync().get();
}

Concurrency::task<shared_ptr<Document>> Collection::ReplaceDocumentAsync(
	const wstring& resource_id,
	const value& document) const
{
	http_request request = CreateRequest(
		methods::PUT,
		RESOURCE_PATH_DOCS,
		resource_id,
		this->document_db_configuration()->master_key());
	request.set_request_uri(this->self() + docs_ + resource_id);

	value body = document;
	if (!body.has_field(DOCUMENT_ID))
	{
		body[DOCUMENT_ID] = value::string(GenerateGuid());
	}

	request.set_body(body);

	return this->document_db_configuration()->http_client().request(request).then([=](http_response response)
	{
		value json_response = response.extract_json().get();

		if (response.status_code() == status_codes::OK)
		{
			assert(resource_id == json_response.at(RESPONSE_RESOURCE_RID).as_string());
			return DocumentFromJson(json_response);
		}

		ThrowExceptionFromResponse(response.status_code(), json_response);
	});
}

shared_ptr<Document> Collection::ReplaceDocument(
	const wstring& resource_id,
	const value& document) const
{
	return this->ReplaceDocumentAsync(resource_id, document).get();
}

Concurrency::task<void> Collection::DeleteDocumentAsync(
	const shared_ptr<Document>& document) const
{
	return DeleteDocumentAsync(document->resource_id());
}

void Collection::DeleteDocument(
	const shared_ptr<Document>& document) const
{
	this->DeleteDocumentAsync(document).get();
}

Concurrency::task<void> Collection::DeleteDocumentAsync(
	const wstring& resource_id) const
{
	http_request request = CreateRequest(
		methods::DEL,
		RESOURCE_PATH_DOCS,
		resource_id,
		this->document_db_configuration()->master_key());
	request.set_request_uri(this->self() + docs_ + resource_id);

	return this->document_db_configuration()->http_client().request(request).then([=](http_response response)
	{
		if (response.status_code() == status_codes::NoContent)
		{
			return;
		}

		value json_response = response.extract_json().get();
		ThrowExceptionFromResponse(response.status_code(), json_response);
	});
}

void Collection::DeleteDocument(
	const wstring& resource_id) const
{
	this->DeleteDocumentAsync(resource_id).get();
}

Concurrency::task<shared_ptr<DocumentIterator>> Collection::QueryDocumentsAsync(
	const wstring& query,
	const int page_size) const
{
	http_request request = CreateQueryRequest(
		query,
		page_size,
		RESOURCE_PATH_DOCS,
		this->resource_id(),
		this->document_db_configuration()->master_key());
	const wstring requestUri = this->self() + docs_;
	request.set_request_uri(requestUri);

	return this->document_db_configuration()->http_client().request(request).then([=](http_response response)
	{
		wstring continuation_id = response.headers()[HEADER_MS_CONTINUATION];
		value json_response = response.extract_json().get();

		if (response.status_code() == status_codes::OK)
		{
			assert(this->resource_id() == json_response.at(RESPONSE_RESOURCE_RID).as_string());

			return make_shared<DocumentIterator>(
				shared_from_this(),
				query,
				page_size,
				requestUri,
				continuation_id,
				json_response.at(RESPONSE_QUERY_DOCUMENTS));
		}

		ThrowExceptionFromResponse(response.status_code(), json_response);
	});
}

shared_ptr<DocumentIterator> Collection::QueryDocuments(
	const wstring& query,
	const int page_size) const
{
	return this->QueryDocumentsAsync(query, page_size).get();
}
