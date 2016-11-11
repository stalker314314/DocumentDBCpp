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

#include "Document.h"

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <cpprest/json.h>

#include "DocumentDBConstants.h"
#include "ConnectionHelper.h"
#include "exceptions.h"

using namespace documentdb;
using namespace std;
using namespace utility;
using namespace web::http;
using namespace web::json;
using namespace web::http::client;

Document::Document(
		const shared_ptr<const DocumentDBConfiguration>& document_db_configuration,
		const string_t& id,
		const string_t& resource_id,
		const unsigned long ts,
		const string_t& self,
		const string_t& etag,
		const string_t& attachments,
		const value& payload)
	: DocumentDBEntity(document_db_configuration, id, resource_id, ts, self, etag)
	, attachments_(attachments)
	, payload_(payload)
{
}

Document::~Document()
{
}

shared_ptr<Attachment> Document::AttachmentFromJson(
	const value& json_attachment) const
{
	string_t id = json_attachment.at(DOCUMENT_ID).as_string();
	string_t rid = json_attachment.at(RESPONSE_RESOURCE_RID).as_string();
	unsigned long ts = json_attachment.at(RESPONSE_RESOURCE_TS).as_integer();
	string_t self = json_attachment.at(RESPONSE_RESOURCE_SELF).as_string();
	string_t etag = json_attachment.at(RESPONSE_RESOURCE_ETAG).as_string();
	string_t contentType = json_attachment.at(RESPONSE_RESOURCE_CONTENT_TYPE).as_string();
	string_t media = json_attachment.at(RESPONSE_RESOURCE_MEDIA).as_string();

	return make_shared<Attachment>(
		this->document_db_configuration(),
		id,
		rid,
		ts,
		self,
		etag,
		contentType,
		media);
}

pplx::task<shared_ptr<Attachment>> Document::CreateAttachmentAsync(
	const string_t& id,
	const string_t& contentType,
	const string_t& media) const
{
	http_request request = CreateRequest(
		methods::POST,
		RESOURCE_PATH_ATTACHMENTS,
		this->resource_id(),
		this->document_db_configuration()->master_key());
	request.set_request_uri(this->self() + attachments_);

	value body;
	body[ATTACHMENT_ID] = value::string(id);
	body[CONTENT_TYPE] = value::string(contentType);
	body[MEDIA] = value::string(media);
	request.set_body(body);

	return this->document_db_configuration()->http_client().request(request).then([=](http_response response)
	{
		value json_response = response.extract_json().get();

		if (response.status_code() == status_codes::Created)
		{
			return AttachmentFromJson(json_response);
		}

		ThrowExceptionFromResponse(response.status_code(), json_response);
	});
}

pplx::task<shared_ptr<Attachment>> Document::CreateAttachmentAsync(
	const string_t& id,
	const string_t& contentType,
	const vector<unsigned char>& raw_media) const
{
	http_request request = CreateRequest(
		methods::POST,
		RESOURCE_PATH_ATTACHMENTS,
		this->resource_id(),
		this->document_db_configuration()->master_key());
	request.set_request_uri(this->self() + attachments_);

	request.headers().add(web::http::header_names::content_type, contentType);
	request.headers().add(_XPLATSTR("Slug"), id);
	request.set_body(raw_media);

	return this->document_db_configuration()->http_client().request(request).then([=](http_response response)
	{
		value json_response = response.extract_json().get();

		if (response.status_code() == status_codes::Created)
		{
			return AttachmentFromJson(json_response);
		}

		ThrowExceptionFromResponse(response.status_code(), json_response);
	});
}

shared_ptr<Attachment> Document::CreateAttachment(
	const string_t& id,
	const string_t& contentType,
	const string_t& media) const
{
	return CreateAttachmentAsync(id, contentType, media).get();
}

shared_ptr<Attachment> Document::CreateAttachment(
	const string_t& id,
	const string_t& contentType,
	const vector<unsigned char>& raw_media) const
{
	return CreateAttachmentAsync(id, contentType, raw_media).get();
}

pplx::task<shared_ptr<Attachment>> Document::GetAttachmentAsync(
	const string_t& resource_id) const
{
	http_request request = CreateRequest(
		methods::GET,
		RESOURCE_PATH_ATTACHMENTS,
		resource_id,
		this->document_db_configuration()->master_key());
	request.set_request_uri(this->self() + attachments_ + resource_id);

	return this->document_db_configuration()->http_client().request(request).then([=](http_response response)
	{
		value json_response = response.extract_json().get();

		if (response.status_code() == status_codes::OK)
		{
			return AttachmentFromJson(json_response);
		}

		ThrowExceptionFromResponse(response.status_code(), json_response);
	});
}

shared_ptr<Attachment> Document::GetAttachment(
	const string_t& resource_id) const
{
	return GetAttachmentAsync(resource_id).get();
}

pplx::task<vector<shared_ptr<Attachment>>> Document::ListAttachmentsAsync() const
{
	http_request request = CreateRequest(
		methods::GET,
		RESOURCE_PATH_ATTACHMENTS,
		this->resource_id(),
		this->document_db_configuration()->master_key());
	request.set_request_uri(this->self() + attachments_);
	return this->document_db_configuration()->http_client().request(request).then([=](http_response response)
	{
		value json_response = response.extract_json().get();

		if (response.status_code() == status_codes::OK)
		{
			assert(this->resource_id() == json_response.at(RESPONSE_RESOURCE_RID).as_string());
			vector<shared_ptr<Attachment>> attachments;
			attachments.reserve(json_response.at(RESPONSE_BODY_COUNT).as_integer());
			value json_attachments = json_response.at(RESPONSE_QUERY_ATTACHMENTS);

			for (auto iter = json_attachments.as_array().cbegin(); iter != json_attachments.as_array().cend(); ++iter)
			{
				shared_ptr<Attachment> coll = AttachmentFromJson(*iter);
				attachments.push_back(coll);
			}
			return attachments;
		}

		ThrowExceptionFromResponse(response.status_code(), json_response);
	});
}

vector<shared_ptr<Attachment>> Document::ListAttachments() const
{
	return ListAttachmentsAsync().get();
}

pplx::task<shared_ptr<Attachment>> Document::ReplaceAttachmentAsync(
	const string_t& id,
	const string_t& new_id,
	const string_t& contentType,
	const string_t& media) const
{
	http_request request = CreateRequest(
		methods::PUT,
		RESOURCE_PATH_ATTACHMENTS,
		id,
		this->document_db_configuration()->master_key());
	request.set_request_uri(this->self() + attachments_ + id);

	value body_;
	body_[DOCUMENT_ID] = value::string(new_id);
	body_[MEDIA] = value::string(media);
	body_[CONTENT_TYPE] = value::string(contentType);

	request.set_body(body_);

	return this->document_db_configuration()->http_client().request(request).then([=](http_response response)
	{
		value json_response = response.extract_json().get();

		if (response.status_code() == status_codes::OK)
		{
			return AttachmentFromJson(json_response);
		}

		ThrowExceptionFromResponse(response.status_code(), json_response);
	});
}

shared_ptr<Attachment> Document::ReplaceAttachment(
	const string_t& id,
	const string_t& new_id,
	const string_t& contentType,
	const string_t& media) const
{
	return ReplaceAttachmentAsync(id, new_id, contentType, media).get();
}

pplx::task<void> Document::DeleteAttachmentAsync(
	const shared_ptr<Attachment>& attachment) const
{
	return DeleteAttachmentAsync(attachment->resource_id());
}

void Document::DeleteAttachment(
	const shared_ptr<Attachment>& attachment) const
{
	DeleteAttachmentAsync(attachment->resource_id()).get();
}

pplx::task<void> Document::DeleteAttachmentAsync(
	const string_t& resource_id) const
{
	http_request request = CreateRequest(
		methods::DEL,
		RESOURCE_PATH_ATTACHMENTS,
		resource_id,
		this->document_db_configuration()->master_key());
	request.set_request_uri(this->self() + attachments_ + resource_id);

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

void Document::DeleteAttachment(
	const string_t& resource_id) const
{
	DeleteAttachmentAsync(resource_id).get();
}

pplx::task<shared_ptr<AttachmentIterator>> Document::QueryAttachmentsAsync(
	const string_t& query,
	const int page_size) const
{
	http_request request = CreateQueryRequest(
		query,
		page_size,
		RESOURCE_PATH_ATTACHMENTS,
		this->resource_id(),
		this->document_db_configuration()->master_key());
	const string_t requestUri = this->self() + attachments_;
	request.set_request_uri(requestUri);

	return this->document_db_configuration()->http_client().request(request).then([=](http_response response)
	{
		string_t continuation_id = response.headers()[HEADER_MS_CONTINUATION];
		value json_response = response.extract_json().get();

		if (response.status_code() == status_codes::OK)
		{
			assert(this->resource_id() == json_response.at(RESPONSE_RESOURCE_RID).as_string());
			
			return make_shared<AttachmentIterator>(
				shared_from_this(),
				query,
				page_size,
				requestUri,
				continuation_id,
				json_response.at(RESPONSE_QUERY_ATTACHMENTS));
		}

		ThrowExceptionFromResponse(response.status_code(), json_response);
	});
}

shared_ptr<AttachmentIterator> Document::QueryAttachments(
	const string_t& query,
	const int page_size) const
{
	return QueryAttachmentsAsync(query, page_size).get();
}
