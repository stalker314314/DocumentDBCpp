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

#ifndef _DOCUMENTDB_DATABASE_H_
#define _DOCUMENTDB_DATABASE_H_

#include <string>
#include <vector>
#include <memory>

#include <cpprest/http_client.h>

#include "DocumentDBEntity.h"
#include "Collection.h"
#include "User.h"
#include "DocumentDBConfiguration.h"
#include "exceptions.h"

namespace documentdb {

	class Database : public DocumentDBEntity
	{
	public:
		Database(
			const std::shared_ptr<const DocumentDBConfiguration>& document_db_configuration,
			const std::wstring& id,
			const std::wstring& resource_id,
			unsigned long ts,
			const std::wstring& self,
			const std::wstring& etag,
			const std::wstring& colls,
			const std::wstring& users);

		virtual ~Database();

		//collections management

		Concurrency::task<std::shared_ptr<Collection>> CreateCollectionAsync(
			const std::wstring& id) const;

		std::shared_ptr<Collection> CreateCollection(
			const std::wstring& id) const;

		Concurrency::task<void> DeleteCollectionAsync(
			const std::wstring& resource_id) const;

		void DeleteCollection(
			const std::wstring& resource_id) const;

		Concurrency::task<void> DeleteCollectionAsync(
			const std::shared_ptr<Collection>& collection) const;

		void DeleteCollection(
			const std::shared_ptr<Collection>& collection) const;

		Concurrency::task<std::shared_ptr<Collection>> GetCollectionAsync(
			const std::wstring& resource_id) const;

		std::shared_ptr<Collection> GetCollection(
			const std::wstring& resource_id) const;

		Concurrency::task<std::vector<std::shared_ptr<Collection>>> ListCollectionsAsync() const;

		std::vector<std::shared_ptr<Collection>> ListCollections() const;

		std::wstring colls() const
		{
			return colls_;
		}

		std::wstring users() const
		{
			return users_;
		}

		//users management
		Concurrency::task<std::shared_ptr<User>> CreateUserAsync(
			const std::wstring& id) const;

		std::shared_ptr<User> CreateUser(
			const std::wstring& id) const;

		Concurrency::task<void> DeleteUserAsync(
			const std::wstring& resource_id) const;

		void DeleteUser(
			const std::wstring& resource_id) const;

		Concurrency::task<void> DeleteUserAsync(
			const std::shared_ptr<User>& user) const;

		void DeleteUser(
			const std::shared_ptr<User>& user) const;
			
		Concurrency::task<std::shared_ptr<User>> GetUserAsync(
			const std::wstring& resource_id) const;

		std::shared_ptr<User> GetUser(
			const std::wstring& resource_id) const;

		Concurrency::task<std::vector<std::shared_ptr<User>>> ListUsersAsync() const;

		std::vector<std::shared_ptr<User>> ListUsers() const;

		Concurrency::task<std::shared_ptr<User>> ReplaceUserAsync(
			const std::wstring& resource_id,
			const std::wstring& new_id) const;

		std::shared_ptr<User> ReplaceUser(
			const std::wstring& resource_id,
			const std::wstring& new_id) const;

	private:
		std::shared_ptr<Collection> CollectionFromJson(const web::json::value* json_collection) const;
		std::shared_ptr<User> UserFromJson(const web::json::value* json_user) const;

		std::wstring colls_;
		std::wstring users_;
	};

}

#endif // !_DOCUMENTDB_DATABASE_H_
