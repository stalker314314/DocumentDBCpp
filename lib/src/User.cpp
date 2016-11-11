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
using namespace utility;
using namespace web::http;
using namespace web::json;
using namespace web::http::client;

User::User(
	const shared_ptr<const DocumentDBConfiguration>& document_db_configuration,
	const string_t& id,
	const string_t& resource_id,
	unsigned long ts,
	const string_t& self,
	const string_t& etag,
	const string_t& permissions)
	: DocumentDBEntity(document_db_configuration, id, resource_id, ts, self, etag)
	, permissions_(permissions)
{
	//TODO : deal with permissions...
}

User::~User()
{
}

shared_ptr<Permission> User::PermissionFromJson(
	const value* json_permission) const
{
	string_t id = json_permission->at(DOCUMENT_ID).as_string();
	string_t rid = json_permission->at(RESPONSE_RESOURCE_RID).as_string();
	unsigned long ts = json_permission->at(RESPONSE_RESOURCE_TS).as_integer();
	string_t self = json_permission->at(RESPONSE_RESOURCE_SELF).as_string();
	string_t etag = json_permission->at(RESPONSE_RESOURCE_ETAG).as_string();
	string_t mode = json_permission->at(RESPONSE_RESOURCE_PERMISSION_MODE).as_string();
	string_t resource = json_permission->at(RESPONSE_RESOURCE_RESOURCE).as_string();
	string_t token = json_permission->at(RESPONSE_RESOURCE_TOKEN).as_string();

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

pplx::task<shared_ptr<Permission>> User::CreatePermissionAsync(
	const string_t& id,
	const string_t& permissionMode,
	const string_t& resource) const
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
	const string_t& id,
	const string_t& permissionMode,
	const string_t& resource) const
{
	return CreatePermissionAsync(id, permissionMode, resource).get();
}

pplx::task<void> User::DeletePermissionAsync(
	const string_t& resource_id) const
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
	const string_t& resource_id) const
{
	this->DeletePermissionAsync(resource_id).get();
}

pplx::task<void> User::DeletePermissionAsync(
	const shared_ptr<Permission>& permission) const
{
	return DeletePermissionAsync(permission->resource_id());
}

void User::DeletePermission(
	const shared_ptr<Permission>& permission) const
{
	this->DeletePermissionAsync(permission->resource_id()).get();
}

pplx::task<shared_ptr<Permission>> User::GetPermissionAsync(
	const string_t& resource_id) const
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
	const string_t& resource_id) const
{
	return GetPermissionAsync(resource_id).get();
}

pplx::task<vector<shared_ptr<Permission>>> User::ListPermissionsAsync() const
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

pplx::task<shared_ptr<Permission>> User::ReplacePermissionAsync(
	const string_t& resource_id,
	const string_t& new_id,
	const string_t& new_permissionMode,
	const string_t& new_resource) const
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
	const string_t& resource_id,
	const string_t& new_id,
	const string_t& new_permissionMode,
	const string_t& new_resource) const
{
	return this->ReplacePermissionAsync(resource_id, new_id, new_permissionMode, new_resource).get();
}
