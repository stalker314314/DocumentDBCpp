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

#ifndef _DOCUMENTDB_USER_H_
#define _DOCUMENTDB_USER_H_

#include <string>
#include <memory>

#include <cpprest/http_client.h>

#include "DocumentDBEntity.h"
#include "DocumentDBConfiguration.h"
#include "exceptions.h"
#include "Permission.h"

namespace documentdb {
	class User : public DocumentDBEntity 
	{
	public:
		User(
			const std::shared_ptr<const DocumentDBConfiguration>& document_db_configuration,
			const std::wstring& id,
			const std::wstring& resource_id,
			unsigned long ts,
			const std::wstring& self,
			const std::wstring& etag,
			const std::wstring& permissions);

		virtual ~User();

		Concurrency::task<std::shared_ptr<Permission>> CreatePermissionAsync(
			const std::wstring& id,
			const std::wstring& permissionMode,
			const std::wstring& resource) const;

		std::shared_ptr<Permission> CreatePermission(
			const std::wstring& id,
			const std::wstring& permissionMode,
			const std::wstring& resource) const;

		Concurrency::task<void> DeletePermissionAsync(
			const std::wstring& resource_id) const;

		void DeletePermission(
			const std::wstring& resource_id) const;

		Concurrency::task<void> DeletePermissionAsync(
			const std::shared_ptr<Permission>& permission) const;

		void DeletePermission(
			const std::shared_ptr<Permission>& permission) const;

		Concurrency::task<std::shared_ptr<Permission>> GetPermissionAsync(
			const std::wstring& resource_id) const;

		std::shared_ptr<Permission> GetPermission(
			const std::wstring& resource_id) const;

		Concurrency::task<std::vector<std::shared_ptr<Permission>>> ListPermissionsAsync() const;

		std::vector<std::shared_ptr<Permission>> ListPermissions() const;

		Concurrency::task<std::shared_ptr<Permission>> ReplacePermissionAsync(
			const std::wstring& resource_id,
			const std::wstring& new_id,
			const std::wstring& new_permissionMode,
			const std::wstring& new_resource) const;

		std::shared_ptr<Permission> ReplacePermission(
			const std::wstring& resource_id,
			const std::wstring& new_id,
			const std::wstring& new_permissionMode,
			const std::wstring& new_resource) const;

		std::wstring permissions() const
		{
			return permissions_;
		}
	private:
		std::shared_ptr<Permission> PermissionFromJson(const web::json::value* json_permission) const;

		std::wstring permissions_;
	};
}

#endif // !_DOCUMENTDB_USER_H_
