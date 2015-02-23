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

#include "User.h"

#include <cpprest/filestream.h>
#include <cpprest/json.h>


#include "ConnectionHelper.h"
#include "DocumentDBConstants.h"
#include "IndexingPolicy.h"
#include "exceptions.h"

using namespace documentdb;
using namespace std;
using namespace web::http;
using namespace web::json;
using namespace web::http::client;

User::User(
	const std::shared_ptr<const DocumentDBConfiguration>& document_db_configuration,
	const std::wstring& id,
	const std::wstring& resource_id,
	unsigned long ts,
	const std::wstring& self,
	const std::wstring& etag,
	const std::wstring& permissions)
	: DocumentDBEntity(document_db_configuration, id, resource_id, ts, self, etag)
	, permissions_(permissions)
{
	//TODO : deal with permissions...
}

User::~User()
{
}

//std::shared_ptr<Permission> UserPermissionFromJson(
//	const web::json::value* json_user) const
//{
//	wstring id = json_user->at(DOCUMENT_ID).as_string();
//	wstring rid = json_user->at(RESPONSE_RESOURCE_RID).as_string();
//	unsigned long ts = json_user->at(RESPONSE_RESOURCE_TS).as_integer();
//	wstring self = json_user->at(RESPONSE_RESOURCE_SELF).as_string();
//	wstring etag = json_user->at(RESPONSE_RESOURCE_ETAG).as_string();
//	wstring docs = json_user->at(RESPONSE_RESOURCE_DOCS).as_string();
//	wstring mode = json_user->at(RESPONSE_RESOURCE_PERMISSION_MODE).as_string();
//	wstring resource = json_user->at(RESPONSE_RESOURCE_RESOURCE).as_string();
//	wstring token = json_user->at(RESPONSE_RESOURCE_TOKEN).as_string();
//
//	IndexingPolicy indexing_policy;
//	if (json_user->has_field(RESPONSE_INDEXING_POLICY))
//	{
//		value indexing_policy_json = json_collection->at(RESPONSE_INDEXING_POLICY);
//		indexing_policy = IndexingPolicy::FromJson(indexing_policy_json);
//	}
//
//	return make_shared<Permission>(
//		this->document_db_configuration(),
//		id,
//		mode,
//		resource
//		rid,
//		ts,
//		self,
//		etag,
//		token);
//}
//
//Concurrency::task<std::shared_ptr<Permission>> User::createPermissionAsync(
//	const std::wstring& id) const
//{
//	http_request request = CreateRequest(
//		methods::POST,
//		RESOURCE_PATH_PERMISSIONS,
//		this->resource_id(),
//		this->document_db_configuration()->master_key());
//	request.set_request_uri(this->self() + permissions_);
//
//	value body;
//	body[DOCUMENT_ID] = value::string(id);
//	request.set_body(body);
//
//	return this->document_db_configuration()->http_client().request(request).then([=](http_response response)
//	{
//		value json_response = response.extract_json().get();
//
//		if (response.status_code() == status_codes::Created)
//		{
//			return PermissionFromJson(&json_response);
//		}
//
//		ThrowExceptionFromResponse(response.status_code(), json_response);
//	});
//}
//
//std::shared_ptr<Permission> User::CreatePermission(
//	const std::wstring& id) const
//{
//
//}