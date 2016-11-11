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

#ifndef _DOCUMENTDB_DOCUMENT_CLIENT_H_
#define _DOCUMENTDB_DOCUMENT_CLIENT_H_

#include <string>
#include <vector>
#include <memory>

#include <cpprest/http_client.h>

#include "Database.h"
#include "DocumentDBConfiguration.h"

namespace documentdb {

	class DocumentClient
	{
	public:
		DocumentClient(const DocumentDBConfiguration& document_db_configuration);

		virtual ~DocumentClient() {}

		pplx::task<std::shared_ptr<Database>> CreateDatabaseAsync(
			const utility::string_t& id) const;

		std::shared_ptr<Database> CreateDatabase(
			const utility::string_t& id) const;

		pplx::task<std::shared_ptr<Database>> ReplaceDatabaseAsync(
			const utility::string_t& resource_id,
			const utility::string_t& new_id) const;

		std::shared_ptr<Database> ReplaceDatabase(
			const utility::string_t& resource_id,
			const utility::string_t& new_id) const;

		pplx::task<void> DeleteDatabaseAsync(
			const Database& database) const;

		void DeleteDatabase(
			const Database& database) const;

		pplx::task<void> DeleteDatabaseAsync(
			const utility::string_t& resource_id) const;

		void DeleteDatabase(
			const utility::string_t& resource_id) const;

		pplx::task<std::shared_ptr<Database>> GetDatabaseAsync(
			const utility::string_t& resource_id) const;

		std::shared_ptr<Database> GetDatabase(
			const utility::string_t& resource_id) const;

		pplx::task<std::vector<std::shared_ptr<Database>>> ListDatabasesAsync() const;

		std::vector<std::shared_ptr<Database>> ListDatabases() const;

	private:
		std::shared_ptr<DocumentDBConfiguration> document_db_configuration_;

		std::shared_ptr<Database> DatabaseFromJson(
			const web::json::value& json_database) const;
	};

}

#endif // !_DOCUMENTDB_DOCUMENT_CLIENT_H_
