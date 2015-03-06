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

#ifndef _DOCUMENTDB_COLLECTION_H_
#define _DOCUMENTDB_COLLECTION_H_

#include <string>
#include <memory>

#include <cpprest/http_client.h>

#include "DocumentDBEntity.h"
#include "DocumentDBConfiguration.h"
#include "IndexingPolicy.h"
#include "DocumentIterator.h"
#include "TriggerIterator.h"
#include "StoredProcedureIterator.h"
#include "UserDefinedFunctionIterator.h"
#include "Document.h"
#include "Trigger.h"
#include "TriggerOperation.h"
#include "TriggerType.h"
#include "StoredProcedure.h"
#include "UserDefinedFunction.h"

namespace documentdb {
	std::wstring triggerOperationToWstring(
		const TriggerOperation& trigger_operation);
	TriggerOperation wstringToTriggerOperation(
		const std::wstring& trigger_operation_str);

	std::wstring triggerTypeToWstring(
		const TriggerType& trigger_type);
	TriggerType wstringToTriggerType
		(const std::wstring& trigger_type_str);

	bool comparei(
		std::wstring string1,
		std::wstring string2);

	class Collection : public DocumentDBEntity, public std::enable_shared_from_this < Collection >
	{
		friend class DocumentIterator;
		friend class TriggerIterator;
		friend class StoredProcedureIterator;
		friend class UserDefinedFunctionIterator;

	public:
		Collection(
			const std::shared_ptr<const DocumentDBConfiguration>& document_db_configuration,
			const std::wstring& id,
			const std::wstring& resource_id,
			const unsigned long ts,
			const std::wstring& self,
			const std::wstring& etag,
			const std::wstring& docs,
			const std::wstring& sprocs,
			const std::wstring& triggers,
			const std::wstring& udfs,
			const std::wstring& conflicts,
			const IndexingPolicy& indexing_policy);

		virtual ~Collection();

		Concurrency::task<std::shared_ptr<Document>> CreateDocumentAsync(
			const web::json::value& document) const;

		std::shared_ptr<Document> CreateDocument(
			const web::json::value& document) const;

		Concurrency::task<std::shared_ptr<Document>> CreateDocumentAsync(
			const std::wstring& document) const;

		std::shared_ptr<Document> CreateDocument(
			const std::wstring& document) const;

		Concurrency::task<std::shared_ptr<Document>> GetDocumentAsync(
			const std::wstring& resource_id) const;

		std::shared_ptr<Document> GetDocument(
			const std::wstring& resource_id) const;

		Concurrency::task<std::vector<std::shared_ptr<Document>>> ListDocumentsAsync() const;

		std::vector<std::shared_ptr<Document>> ListDocuments() const;

		Concurrency::task<std::shared_ptr<Document>> ReplaceDocumentAsync(
			const std::wstring& resource_id,
			const web::json::value& document) const;

		std::shared_ptr<Document> ReplaceDocument(
			const std::wstring& resource_id,
			const web::json::value& document) const;

		Concurrency::task<void> DeleteDocumentAsync(
			const std::shared_ptr<Document>& document) const;

		void DeleteDocument(
			const std::shared_ptr<Document>& document) const;

		Concurrency::task<void> DeleteDocumentAsync(
			const std::wstring& resource_id) const;

		void DeleteDocument(
			const std::wstring& resource_id) const;

		Concurrency::task<std::shared_ptr<DocumentIterator>> QueryDocumentsAsync(
			const std::wstring& query,
			const int page_size = 10) const;

		std::shared_ptr<DocumentIterator> QueryDocuments(
			const std::wstring& query,
			const int page_size = 10) const;

		//triggers management
		Concurrency::task<std::shared_ptr<Trigger>> CreateTriggerAsync(
			const std::wstring& id,
			const std::wstring& body,
			const TriggerOperation& triggerOperation,
			const TriggerType& triggerType) const;

		std::shared_ptr<Trigger> CreateTrigger(
			const std::wstring& id,
			const std::wstring& body,
			const TriggerOperation& triggerOperation,
			const TriggerType& triggerType) const;

		Concurrency::task<std::shared_ptr<Trigger>> GetTriggerAsync(
			const std::wstring& resource_id) const;

		std::shared_ptr<Trigger> GetTrigger(
			const std::wstring& resource_id) const;

		Concurrency::task<std::vector<std::shared_ptr<Trigger>>> ListTriggersAsync() const;

		std::vector<std::shared_ptr<Trigger>> ListTriggers() const;

		Concurrency::task<std::shared_ptr<Trigger>> ReplaceTriggerAsync(
			const std::wstring& id,
			const std::wstring& new_id,
			const std::wstring& body,
			const TriggerOperation& triggerOperation,
			const TriggerType& triggerType) const;

		std::shared_ptr<Trigger> ReplaceTrigger(
			const std::wstring& id,
			const std::wstring& new_id,
			const std::wstring& body,
			const TriggerOperation& triggerOperation,
			const TriggerType& triggerType) const;

		Concurrency::task<void> DeleteTriggerAsync(
			const std::shared_ptr<Trigger>& trigger) const;

		void DeleteTrigger(
			const std::shared_ptr<Trigger>& trigger) const;

		Concurrency::task<void> DeleteTriggerAsync(
			const std::wstring& resource_id) const;

		void DeleteTrigger(
			const std::wstring& resource_id) const;

		Concurrency::task<std::shared_ptr<TriggerIterator>> QueryTriggersAsync(
			const std::wstring& query,
			const int page_size = 10) const;

		std::shared_ptr<TriggerIterator> QueryTriggers(
			const std::wstring& query,
			const int page_size = 10) const;

		//stored procedures management
		Concurrency::task<std::shared_ptr<StoredProcedure>> CreateStoredProcedureAsync(
			const std::wstring& id,
			const std::wstring& body) const;

		std::shared_ptr<StoredProcedure> CreateStoredProcedure(
			const std::wstring& id,
			const std::wstring& body) const;

		Concurrency::task<std::shared_ptr<StoredProcedure>> GetStoredProcedureAsync(
			const std::wstring& resource_id) const;

		std::shared_ptr<StoredProcedure> GetStoredProcedure(
			const std::wstring& resource_id) const;

		Concurrency::task<std::vector<std::shared_ptr<StoredProcedure>>> ListStoredProceduresAsync() const;

		std::vector<std::shared_ptr<StoredProcedure>> ListStoredProcedures() const;

		Concurrency::task<std::shared_ptr<StoredProcedure>> ReplaceStoredProcedureAsync(
			const std::wstring& id,
			const std::wstring& new_id,
			const std::wstring& body) const;

		std::shared_ptr<StoredProcedure> ReplaceStoredProcedure(
			const std::wstring& id,
			const std::wstring& new_id,
			const std::wstring& body) const;

		Concurrency::task<void> DeleteStoredProcedureAsync(
			const std::shared_ptr<StoredProcedure>& storedProcedure) const;

		void DeleteStoredProcedure(
			const std::shared_ptr<StoredProcedure>& storedProcedure) const;

		Concurrency::task<void> DeleteStoredProcedureAsync(
			const std::wstring& resource_id) const;

		void DeleteStoredProcedure(
			const std::wstring& resource_id) const;

		Concurrency::task<std::shared_ptr<StoredProcedureIterator>> QueryStoredProceduresAsync(
			const std::wstring& query,
			const int page_size = 10) const;

		std::shared_ptr<StoredProcedureIterator> QueryStoredProcedures(
			const std::wstring& query,
			const int page_size = 10) const;

		Concurrency::task<void> ExecuteStoredProcedureAsync(
			const std::wstring& resource_id,
			const web::json::value& input) const;

		void ExecuteStoredProcedure(
			const std::wstring& resource_id,
			const web::json::value& input) const;

		// User defined functions management
		Concurrency::task<std::shared_ptr<UserDefinedFunction>> CreateUserDefinedFunctionAsync(
			const std::wstring& id,
			const std::wstring& body) const;

		std::shared_ptr<UserDefinedFunction> CreateUserDefinedFunction(
			const std::wstring& id,
			const std::wstring& body) const;

		Concurrency::task<std::shared_ptr<UserDefinedFunction>> GetUserDefinedFunctionAsync(
			const std::wstring& resource_id) const;

		std::shared_ptr<UserDefinedFunction> GetUserDefinedFunction(
			const std::wstring& resource_id) const;

		Concurrency::task<std::vector<std::shared_ptr<UserDefinedFunction>>> ListUserDefinedFunctionsAsync() const;

		std::vector<std::shared_ptr<UserDefinedFunction>> ListUserDefinedFunctions() const;

		Concurrency::task<std::shared_ptr<UserDefinedFunction>> ReplaceUserDefinedFunctionAsync(
			const std::wstring& id,
			const std::wstring& new_id,
			const std::wstring& body) const;

		std::shared_ptr<UserDefinedFunction> ReplaceUserDefinedFunction(
			const std::wstring& id,
			const std::wstring& new_id,
			const std::wstring& body) const;

		Concurrency::task<void> DeleteUserDefinedFunctionAsync(
			const std::shared_ptr<UserDefinedFunction>& userDefinedFunction) const;

		void DeleteUserDefinedFunction(
			const std::shared_ptr<UserDefinedFunction>& userDefinedFunction) const;

		Concurrency::task<void> DeleteUserDefinedFunctionAsync(
			const std::wstring& resource_id) const;

		void DeleteUserDefinedFunction(
			const std::wstring& resource_id) const;

		Concurrency::task<std::shared_ptr<UserDefinedFunctionIterator>> QueryUserDefinedFunctionsAsync(
			const std::wstring& query,
			const int page_size = 10) const;

		std::shared_ptr<UserDefinedFunctionIterator> QueryUserDefinedFunctions(
			const std::wstring& query,
			const int page_size = 10) const;

		std::wstring docs() const
		{
			return docs_;
		}

		std::wstring sprocs() const
		{
			return sprocs_;
		}

		std::wstring triggers() const
		{
			return triggers_;
		}

		std::wstring udfs() const
		{
			return udfs_;
		}

		std::wstring conflicts() const
		{
			return conflicts_;
		}

		IndexingPolicy indexing_policy() const
		{
			return indexing_policy_;
		}

	private:
		std::shared_ptr<Document> DocumentFromJson(
			const web::json::value& json_collection) const;

		std::shared_ptr<Trigger> TriggerFromJson(
			const web::json::value* json_trigger) const;

		std::shared_ptr<StoredProcedure> StoredProcedureFromJson(
			const web::json::value* json_sproc) const;

		std::shared_ptr<UserDefinedFunction> UserDefinedFunctionFromJson(
			const web::json::value* json_udf) const;

		static std::wstring GenerateGuid();

		std::wstring docs_;
		std::wstring sprocs_;
		std::wstring triggers_;
		std::wstring udfs_;
		std::wstring conflicts_;
		IndexingPolicy indexing_policy_;
	};
}

#endif // !_DOCUMENTDB_COLLECTION_H_
