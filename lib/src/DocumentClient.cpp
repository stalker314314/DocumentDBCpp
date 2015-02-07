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

#include <iostream>
#include <string.h>
#include <windows.h>
#include <Wincrypt.h>
#include <algorithm>

#include <cpprest/filestream.h>
#include <cpprest/json.h>

#include "ConnectionHelper.h"
#include "Database.h"
#include "DocumentDBConstants.h"
#include "exceptions.h"

using namespace documentdb;
using namespace std;
using namespace web::http;
using namespace web::json;
using namespace web::http::client;

DocumentClient::DocumentClient (
	const DocumentDBConfiguration& document_db_configuration)
{
	document_db_configuration_ = make_shared<DocumentDBConfiguration> (document_db_configuration);
}

shared_ptr<Database> DocumentClient::DatabaseFromJson (
	const value& json_database) const
{
	wstring id = json_database.at (DOCUMENT_ID).as_string ();
	wstring rid = json_database.at (RESPONSE_RESOURCE_RID).as_string ();
	unsigned long ts = json_database.at (RESPONSE_RESOURCE_TS).as_integer ();
	wstring self = json_database.at (RESPONSE_RESOURCE_SELF).as_string ();
	wstring etag = json_database.at (RESPONSE_RESOURCE_ETAG).as_string ();
	wstring colls = json_database.at (RESPONSE_RESOURCE_COLLS).as_string ();
	wstring users = json_database.at (RESPONSE_RESOURCE_USERS).as_string ();

	return make_shared<Database>(document_db_configuration_, id, rid, ts, self, etag, colls, users);
}

Concurrency::task<shared_ptr<Database>> DocumentClient::CreateDatabaseAsync (
	const wstring& id) const
{
	http_request request = CreateRequest (
		methods::POST,
		RESOURCE_PATH_DBS,
		L"",
		document_db_configuration_->master_key ());
	request.set_request_uri (RESOURCE_PATH_DBS);
	value body;
	body[DOCUMENT_ID] = value::string (id);
	request.set_body (body);

	return document_db_configuration_->http_client ().request (request).then ([=](http_response response)
	{
		value json_response = response.extract_json ().get ();

		if (response.status_code () == status_codes::Created)
		{
			return DatabaseFromJson (json_response);
		}

		ThrowExceptionFromResponse (response.status_code(), json_response);
	});
}

shared_ptr<Database> DocumentClient::CreateDatabase (
	const wstring& id) const
{
	return this->CreateDatabaseAsync (id).get ();
}

Concurrency::task<shared_ptr<Database>> DocumentClient::ReplaceDatabaseAsync (
	const wstring& resource_id,
	const wstring& new_id) const
{
	http_request request = CreateRequest (
		methods::PUT,
		RESOURCE_PATH_DBS,
		resource_id,
		document_db_configuration_->master_key ());
	request.set_request_uri (wstring (RESOURCE_PATH_DBS) + L"/" + resource_id);

	value body;
	body[DOCUMENT_ID] = value::string (new_id);
	request.set_body (body);

	return document_db_configuration_->http_client ().request (request).then ([=](http_response response)
	{
		value json_response = response.extract_json ().get ();

		if (response.status_code () == status_codes::OK)
		{
			return DatabaseFromJson (json_response);
		}

		ThrowExceptionFromResponse (response.status_code(), json_response);
	});
}

shared_ptr<Database> DocumentClient::ReplaceDatabase (
	const wstring& resource_id,
	const wstring& new_id) const
{
	return this->ReplaceDatabaseAsync (resource_id, new_id).get ();
}

Concurrency::task<void> DocumentClient::DeleteDatabaseAsync (
	const Database& database) const
{
	return DeleteDatabaseAsync (database.resource_id());
}

void DocumentClient::DeleteDatabase (
	const Database& database) const
{
	return this->DeleteDatabaseAsync (database).get ();
}

Concurrency::task<void> DocumentClient::DeleteDatabaseAsync (
	const wstring& resource_id) const
{
	http_request request = CreateRequest (
		methods::DEL,
		RESOURCE_PATH_DBS,
		resource_id,
		document_db_configuration_->master_key ());
	request.set_request_uri (wstring (RESOURCE_PATH_DBS) + L"/" + resource_id);

	return document_db_configuration_->http_client ().request (request).then ([=](http_response response)
	{
		if (response.status_code () == status_codes::NoContent)
		{
			return;
		}

		value json_response = response.extract_json ().get ();
		ThrowExceptionFromResponse (response.status_code (), json_response);
	});
}

void DocumentClient::DeleteDatabase (
	const wstring& resource_id) const
{
	return this->DeleteDatabaseAsync (resource_id).get ();
}

Concurrency::task<shared_ptr<Database>> DocumentClient::GetDatabaseAsync (
	const wstring& resource_id) const
{
	http_request request = CreateRequest (
		methods::GET,
		RESOURCE_PATH_DBS,
		resource_id,
		document_db_configuration_->master_key ());
	request.set_request_uri (wstring (RESOURCE_PATH_DBS) + L"/" + resource_id);

	return document_db_configuration_->http_client ().request (request).then ([=](http_response response)
	{
		value json_response = response.extract_json ().get ();

		if (response.status_code () == status_codes::OK)
		{
			assert (resource_id == json_response.at (RESPONSE_RESOURCE_RID).as_string ());

			return DatabaseFromJson (json_response);
		}

		ThrowExceptionFromResponse (response.status_code (), json_response);
	});
}

shared_ptr<Database> DocumentClient::GetDatabase (
	const wstring& resource_id) const
{
	return this->GetDatabaseAsync (resource_id).get ();
}

Concurrency::task<vector<shared_ptr<Database>>> DocumentClient::ListDatabasesAsync () const
{
	http_request request = CreateRequest (
		methods::GET,
		RESOURCE_PATH_DBS,
		L"",
		document_db_configuration_->master_key ());
	request.set_request_uri (RESOURCE_PATH_DBS);

	return document_db_configuration_->http_client ().request (request).then ([=](http_response response)
	{
		value json_response = response.extract_json ().get ();

		if (response.status_code () == status_codes::OK)
		{
			assert (L"" == json_response.at (RESPONSE_RESOURCE_RID).as_string ());
			vector<shared_ptr<Database>> databases;
			databases.reserve (json_response.at (RESPONSE_BODY_COUNT).as_integer ());
			value json_databases = json_response.at (RESPONSE_DATABASES);

			for (auto iter = json_databases.as_array ().cbegin (); iter != json_databases.as_array ().cend (); ++iter)
			{
				shared_ptr<Database> db = DatabaseFromJson (*iter);
				databases.push_back (db);
			}

			return databases;
		}

		ThrowExceptionFromResponse (response.status_code (), json_response);
		// TODO: document code
	});
}

vector<shared_ptr<Database>> DocumentClient::ListDatabases () const
{
	return this->ListDatabasesAsync ().get ();
}
