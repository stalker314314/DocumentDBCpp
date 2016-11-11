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
			const utility::string_t& id,
			const utility::string_t& resource_id,
			unsigned long ts,
			const utility::string_t& self,
			const utility::string_t& etag,
			const utility::string_t& colls,
			const utility::string_t& users);

		virtual ~Database();

		//collections management

		pplx::task<std::shared_ptr<Collection>> CreateCollectionAsync(
			const utility::string_t& id) const;

		std::shared_ptr<Collection> CreateCollection(
			const utility::string_t& id) const;

		pplx::task<void> DeleteCollectionAsync(
			const utility::string_t& resource_id) const;

		void DeleteCollection(
			const utility::string_t& resource_id) const;

		pplx::task<void> DeleteCollectionAsync(
			const std::shared_ptr<Collection>& collection) const;

		void DeleteCollection(
			const std::shared_ptr<Collection>& collection) const;

		pplx::task<std::shared_ptr<Collection>> GetCollectionAsync(
			const utility::string_t& resource_id) const;

		std::shared_ptr<Collection> GetCollection(
			const utility::string_t& resource_id) const;

		pplx::task<std::vector<std::shared_ptr<Collection>>> ListCollectionsAsync() const;

		std::vector<std::shared_ptr<Collection>> ListCollections() const;

		utility::string_t colls() const
		{
			return colls_;
		}

		utility::string_t users() const
		{
			return users_;
		}

		//users management
		pplx::task<std::shared_ptr<User>> CreateUserAsync(
			const utility::string_t& id) const;

		std::shared_ptr<User> CreateUser(
			const utility::string_t& id) const;

		pplx::task<void> DeleteUserAsync(
			const utility::string_t& resource_id) const;

		void DeleteUser(
			const utility::string_t& resource_id) const;

		pplx::task<void> DeleteUserAsync(
			const std::shared_ptr<User>& user) const;

		void DeleteUser(
			const std::shared_ptr<User>& user) const;
			
		pplx::task<std::shared_ptr<User>> GetUserAsync(
			const utility::string_t& resource_id) const;

		std::shared_ptr<User> GetUser(
			const utility::string_t& resource_id) const;

		pplx::task<std::vector<std::shared_ptr<User>>> ListUsersAsync() const;

		std::vector<std::shared_ptr<User>> ListUsers() const;

		pplx::task<std::shared_ptr<User>> ReplaceUserAsync(
			const utility::string_t& resource_id,
			const utility::string_t& new_id) const;

		std::shared_ptr<User> ReplaceUser(
			const utility::string_t& resource_id,
			const utility::string_t& new_id) const;

	private:
		std::shared_ptr<Collection> CollectionFromJson(const web::json::value* json_collection) const;
		std::shared_ptr<User> UserFromJson(const web::json::value* json_user) const;

		utility::string_t colls_;
		utility::string_t users_;
	};

}

#endif // !_DOCUMENTDB_DATABASE_H_
