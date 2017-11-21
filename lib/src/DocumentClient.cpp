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

#include "DocumentClient.h"

#ifdef _WIN32
#include <windows.h>
#include <Wincrypt.h>
#endif

#include <iostream>
#include <string>
#include <algorithm>

#include <cpprest/filestream.h>
#include <cpprest/json.h>

#include "ConnectionHelper.h"
#include "Database.h"
#include "DocumentDBConstants.h"
#include "exceptions.h"

using namespace documentdb;
using namespace std;
using namespace utility;
using namespace web::http;
using namespace web::json;
using namespace web::http::client;

DocumentClient::DocumentClient(
	const DocumentDBConfiguration& document_db_configuration)
{
	document_db_configuration_ = make_shared<DocumentDBConfiguration>(document_db_configuration);
}

shared_ptr<Database> DocumentClient::DatabaseFromJson(
	const value& json_database) const
{
	utility::string_t id = json_database.at(DOCUMENT_ID).as_string();
	utility::string_t rid = json_database.at(RESPONSE_RESOURCE_RID).as_string();
	unsigned long ts = json_database.at(RESPONSE_RESOURCE_TS).as_integer();
	utility::string_t self = json_database.at(RESPONSE_RESOURCE_SELF).as_string();
	utility::string_t etag = json_database.at(RESPONSE_RESOURCE_ETAG).as_string();
	utility::string_t colls = json_database.at(RESPONSE_RESOURCE_COLLS).as_string();
	utility::string_t users = json_database.at(RESPONSE_RESOURCE_USERS).as_string();

	return make_shared<Database>(document_db_configuration_, id, rid, ts, self, etag, colls, users);
}

pplx::task<shared_ptr<Database>> DocumentClient::CreateDatabaseAsync(
	const string_t& id) const
{
	http_request request = CreateRequest(
		methods::POST,
		RESOURCE_PATH_DBS,
		_XPLATSTR(""),
		document_db_configuration_->master_key());
	request.set_request_uri(RESOURCE_PATH_DBS);
	value body;
	body[DOCUMENT_ID] = value::string(id);
	request.set_body(body);

	return document_db_configuration_->http_client().request(request).then([=](http_response response)
	{
		value json_response = response.extract_json().get();

		if (response.status_code() == status_codes::Created)
		{
			return DatabaseFromJson(json_response);
		}

		ThrowExceptionFromResponse(response.status_code(), json_response);
	});
}

shared_ptr<Database> DocumentClient::CreateDatabase(
	const string_t& id) const
{
	return this->CreateDatabaseAsync(id).get();
}

pplx::task<void> DocumentClient::DeleteDatabaseAsync(
	const Database& database) const
{
	return DeleteDatabaseAsync(database.resource_id());
}

void DocumentClient::DeleteDatabase(
	const Database& database) const
{
	return this->DeleteDatabaseAsync(database).get();
}

pplx::task<void> DocumentClient::DeleteDatabaseAsync(
	const string_t& resource_id) const
{
	http_request request = CreateRequest(
		methods::DEL,
		RESOURCE_PATH_DBS,
		resource_id,
		document_db_configuration_->master_key());
	request.set_request_uri(string_t(RESOURCE_PATH_DBS) + _XPLATSTR("/") + resource_id);

	return document_db_configuration_->http_client().request(request).then([=](http_response response)
	{
		if (response.status_code() == status_codes::NoContent)
		{
			return;
		}

		value json_response = response.extract_json().get();
		ThrowExceptionFromResponse(response.status_code(), json_response);
	});
}

void DocumentClient::DeleteDatabase(
	const string_t& resource_id) const
{
	return this->DeleteDatabaseAsync(resource_id).get();
}

pplx::task<shared_ptr<Database>> DocumentClient::GetDatabaseAsync(
	const string_t& resource_id) const
{
	http_request request = CreateRequest(
		methods::GET,
		RESOURCE_PATH_DBS,
		resource_id,
		document_db_configuration_->master_key());
	request.set_request_uri(string_t(RESOURCE_PATH_DBS) + _XPLATSTR("/") + resource_id);

	return document_db_configuration_->http_client().request(request).then([=](http_response response)
	{
		value json_response = response.extract_json().get();

		if (response.status_code() == status_codes::OK)
		{
			assert(resource_id == json_response.at(RESPONSE_RESOURCE_RID).as_string());

			return DatabaseFromJson(json_response);
		}

		ThrowExceptionFromResponse(response.status_code(), json_response);
	});
}

shared_ptr<Database> DocumentClient::GetDatabase(
	const string_t& resource_id) const
{
	return this->GetDatabaseAsync(resource_id).get();
}

pplx::task<vector<shared_ptr<Database>>> DocumentClient::ListDatabasesAsync() const
{
	http_request request = CreateRequest(
		methods::GET,
		RESOURCE_PATH_DBS,
		_XPLATSTR(""),
		document_db_configuration_->master_key());
	request.set_request_uri(RESOURCE_PATH_DBS);

	return document_db_configuration_->http_client().request(request).then([=](http_response response)
	{
		value json_response = response.extract_json().get();

		if (response.status_code() == status_codes::OK)
		{
			assert(_XPLATSTR("") == json_response.at(RESPONSE_RESOURCE_RID).as_string());
			vector<shared_ptr<Database>> databases;
			databases.reserve(json_response.at(RESPONSE_BODY_COUNT).as_integer());
			value json_databases = json_response.at(RESPONSE_DATABASES);

			for (auto iter = json_databases.as_array().cbegin(); iter != json_databases.as_array().cend(); ++iter)
			{
				shared_ptr<Database> db = DatabaseFromJson(*iter);
				databases.push_back(db);
			}

			return databases;
		}

		ThrowExceptionFromResponse(response.status_code(), json_response);
		// TODO: document code
	});
}

vector<shared_ptr<Database>> DocumentClient::ListDatabases() const
{
	return this->ListDatabasesAsync().get();
}
