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
	const shared_ptr<const DocumentDBConfiguration>& document_db_configuration,
	const wstring& id,
	const wstring& resource_id,
	unsigned long ts,
	const wstring& self,
	const wstring& etag,
	const wstring& permissions)
	: DocumentDBEntity(document_db_configuration, id, resource_id, ts, self, etag)
	, permissions_(permissions)
{
	//TODO : deal with permissions...
}

User::~User()
{
}

shared_ptr<Permission> User::PermissionFromJson(
	const web::json::value* json_permission) const
{
	wstring id = json_permission->at(DOCUMENT_ID).as_string();
	wstring rid = json_permission->at(RESPONSE_RESOURCE_RID).as_string();
	unsigned long ts = json_permission->at(RESPONSE_RESOURCE_TS).as_integer();
	wstring self = json_permission->at(RESPONSE_RESOURCE_SELF).as_string();
	wstring etag = json_permission->at(RESPONSE_RESOURCE_ETAG).as_string();
	wstring mode = json_permission->at(RESPONSE_RESOURCE_PERMISSION_MODE).as_string();
	wstring resource = json_permission->at(RESPONSE_RESOURCE_RESOURCE).as_string();
	wstring token = json_permission->at(RESPONSE_RESOURCE_TOKEN).as_string();

	IndexingPolicy indexing_policy;
	if (json_permission->has_field(RESPONSE_INDEXING_POLICY))
	{
		value indexing_policy_json = json_permission->at(RESPONSE_INDEXING_POLICY);
		indexing_policy = IndexingPolicy::FromJson(indexing_policy_json);
	}

	return make_shared<Permission>(
		this->document_db_configuration(),
		id,
		mode,
		resource,
		rid,
		ts,
		self,
		etag,
		token);
}

Concurrency::task<shared_ptr<Permission>> User::CreatePermissionAsync(
	const wstring& id,
	const wstring& permissionMode,
	const wstring& resource) const
{
	http_request request = CreateRequest(
		methods::POST,
		RESOURCE_PATH_PERMISSIONS,
		this->resource_id(),
		this->document_db_configuration()->master_key());
	request.set_request_uri(this->self() + permissions_);

	value body;
	body[DOCUMENT_ID] = value::string(id);
	body[PERMISSION_MODE] = value::string(permissionMode);
	body[RESOURCE] = value::string(resource);
	request.set_body(body);

	return this->document_db_configuration()->http_client().request(request).then([=](http_response response)
	{
		value json_response = response.extract_json().get();

		if (response.status_code() == status_codes::Created)
		{
			return PermissionFromJson(&json_response);
		}

		ThrowExceptionFromResponse(response.status_code(), json_response);
	});
}

shared_ptr<Permission> User::CreatePermission(
	const wstring& id,
	const wstring& permissionMode,
	const wstring& resource) const
{
	return CreatePermissionAsync(id, permissionMode, resource).get();
}

Concurrency::task<void> User::DeletePermissionAsync(
	const wstring& resource_id) const
{
	http_request request = CreateRequest(
		methods::DEL,
		RESOURCE_PATH_PERMISSIONS,
		resource_id,
		this->document_db_configuration()->master_key());
	request.set_request_uri(this->self() + permissions_ + resource_id);

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

void User::DeletePermission(
	const wstring& resource_id) const
{
	this->DeletePermissionAsync(resource_id).get();
}

Concurrency::task<void> User::DeletePermissionAsync(
	const shared_ptr<Permission>& permission) const
{
	return DeletePermissionAsync(permission->resource_id());
}

void User::DeletePermission(
	const shared_ptr<Permission>& permission) const
{
	this->DeletePermissionAsync(permission->resource_id()).get();
}

Concurrency::task<shared_ptr<Permission>> User::GetPermissionAsync(
	const wstring& resource_id) const
{
	http_request request = CreateRequest(
		methods::GET,
		RESOURCE_PATH_PERMISSIONS,
		resource_id,
		this->document_db_configuration()->master_key());
	request.set_request_uri(this->self() + permissions_ + resource_id);

	return this->document_db_configuration()->http_client().request(request).then([=](http_response response)
	{
		value json_response = response.extract_json().get();

		if (response.status_code() == status_codes::OK)
		{
			return PermissionFromJson(&json_response);
		}

		ThrowExceptionFromResponse(response.status_code(), json_response);
	});
}

shared_ptr<Permission> User::GetPermission(
	const wstring& resource_id) const
{
	return GetPermissionAsync(resource_id).get();
}

Concurrency::task<vector<shared_ptr<Permission>>> User::ListPermissionsAsync() const
{
	http_request request = CreateRequest(
		methods::GET,
		RESOURCE_PATH_PERMISSIONS,
		this->resource_id(),
		this->document_db_configuration()->master_key());
	request.set_request_uri(this->self() + permissions_);
	return this->document_db_configuration()->http_client().request(request).then([=](http_response response)
	{
		value json_response = response.extract_json().get();

		if (response.status_code() == status_codes::OK)
		{
			assert(this->resource_id() == json_response.at(RESPONSE_RESOURCE_RID).as_string());
			vector<shared_ptr<Permission>> permissions;
			permissions.reserve(json_response.at(RESPONSE_BODY_COUNT).as_integer());
			value json_permissions = json_response.at(RESPONSE_PERMISSIONS);

			for (auto iter = json_permissions.as_array().cbegin(); iter != json_permissions.as_array().cend(); ++iter)
			{
				shared_ptr<Permission> permission = PermissionFromJson(&(*iter));
				permissions.push_back(permission);
			}
			return permissions;
		}

		ThrowExceptionFromResponse(response.status_code(), json_response);
	});
}

vector<shared_ptr<Permission>> User::ListPermissions() const
{
	return this->ListPermissionsAsync().get();
}

Concurrency::task<shared_ptr<Permission>> User::ReplacePermissionAsync(
	const wstring& resource_id,
	const wstring& new_id,
	const wstring& new_permissionMode,
	const wstring& new_resource) const
{
	http_request request = CreateRequest(
		methods::PUT,
		RESOURCE_PATH_PERMISSIONS,
		resource_id,
		document_db_configuration()->master_key());
	request.set_request_uri(this->self() + permissions_ + resource_id);

	value body;
	body[DOCUMENT_ID] = value::string(new_id);
	body[PERMISSION_MODE] = value::string(new_permissionMode);
	body[RESOURCE] = value::string(new_resource);
	request.set_body(body);

	return document_db_configuration()->http_client().request(request).then([=](http_response response)
	{
		value json_response = response.extract_json().get();

		if (response.status_code() == status_codes::OK)
		{
			return PermissionFromJson(&json_response);
		}

		ThrowExceptionFromResponse(response.status_code(), json_response);
	});
}

shared_ptr<Permission> User::ReplacePermission(
	const wstring& resource_id,
	const wstring& new_id,
	const wstring& new_permissionMode,
	const wstring& new_resource) const
{
	return this->ReplacePermissionAsync(resource_id, new_id, new_permissionMode, new_resource).get();
}
