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
#include <pplx/pplxtasks.h>
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
	utility::string_t triggerOperationToWstring(
		const TriggerOperation& trigger_operation);
	TriggerOperation wstringToTriggerOperation(
		const utility::string_t& trigger_operation_str);

	utility::string_t triggerTypeToWstring(
		const TriggerType& trigger_type);
	TriggerType wstringToTriggerType
		(const utility::string_t& trigger_type_str);

	bool comparei(
		utility::string_t string1,
		utility::string_t string2);

	class Collection : public DocumentDBEntity, public std::enable_shared_from_this < Collection >
	{
		friend class DocumentIterator;
		friend class TriggerIterator;
		friend class StoredProcedureIterator;
		friend class UserDefinedFunctionIterator;

	public:
		Collection(
			const std::shared_ptr<const DocumentDBConfiguration>& document_db_configuration,
			const utility::string_t& id,
			const utility::string_t& resource_id,
			const unsigned long ts,
			const utility::string_t& self,
			const utility::string_t& etag,
			const utility::string_t& docs,
			const utility::string_t& sprocs,
			const utility::string_t& triggers,
			const utility::string_t& udfs,
			const utility::string_t& conflicts,
			const IndexingPolicy& indexing_policy);

		virtual ~Collection();

		pplx::task<std::shared_ptr<Document>> CreateDocumentAsync(
			const web::json::value& document) const;

		std::shared_ptr<Document> CreateDocument(
			const web::json::value& document) const;

		pplx::task<std::shared_ptr<Document>> CreateDocumentAsync(
			const utility::string_t& document) const;

		std::shared_ptr<Document> CreateDocument(
			const utility::string_t& document) const;

		pplx::task<std::shared_ptr<Document>> GetDocumentAsync(
			const utility::string_t& resource_id) const;

		std::shared_ptr<Document> GetDocument(
			const utility::string_t& resource_id) const;

		pplx::task<std::vector<std::shared_ptr<Document>>> ListDocumentsAsync() const;

		std::vector<std::shared_ptr<Document>> ListDocuments() const;

		pplx::task<std::shared_ptr<Document>> ReplaceDocumentAsync(
			const utility::string_t& resource_id,
			const web::json::value& document) const;

		std::shared_ptr<Document> ReplaceDocument(
			const utility::string_t& resource_id,
			const web::json::value& document) const;

		pplx::task<void> DeleteDocumentAsync(
			const std::shared_ptr<Document>& document) const;

		void DeleteDocument(
			const std::shared_ptr<Document>& document) const;

		pplx::task<void> DeleteDocumentAsync(
			const utility::string_t& resource_id) const;

		void DeleteDocument(
			const utility::string_t& resource_id) const;

		pplx::task<std::shared_ptr<DocumentIterator>> QueryDocumentsAsync(
			const utility::string_t& query,
			const int page_size = 10) const;

		std::shared_ptr<DocumentIterator> QueryDocuments(
			const utility::string_t& query,
			const int page_size = 10) const;

		//triggers management
		pplx::task<std::shared_ptr<Trigger>> CreateTriggerAsync(
			const utility::string_t& id,
			const utility::string_t& body,
			const TriggerOperation& triggerOperation,
			const TriggerType& triggerType) const;

		std::shared_ptr<Trigger> CreateTrigger(
			const utility::string_t& id,
			const utility::string_t& body,
			const TriggerOperation& triggerOperation,
			const TriggerType& triggerType) const;

		pplx::task<std::shared_ptr<Trigger>> GetTriggerAsync(
			const utility::string_t& resource_id) const;

		std::shared_ptr<Trigger> GetTrigger(
			const utility::string_t& resource_id) const;

		pplx::task<std::vector<std::shared_ptr<Trigger>>> ListTriggersAsync() const;

		std::vector<std::shared_ptr<Trigger>> ListTriggers() const;

		pplx::task<std::shared_ptr<Trigger>> ReplaceTriggerAsync(
			const utility::string_t& id,
			const utility::string_t& new_id,
			const utility::string_t& body,
			const TriggerOperation& triggerOperation,
			const TriggerType& triggerType) const;

		std::shared_ptr<Trigger> ReplaceTrigger(
			const utility::string_t& id,
			const utility::string_t& new_id,
			const utility::string_t& body,
			const TriggerOperation& triggerOperation,
			const TriggerType& triggerType) const;

		pplx::task<void> DeleteTriggerAsync(
			const std::shared_ptr<Trigger>& trigger) const;

		void DeleteTrigger(
			const std::shared_ptr<Trigger>& trigger) const;

		pplx::task<void> DeleteTriggerAsync(
			const utility::string_t& resource_id) const;

		void DeleteTrigger(
			const utility::string_t& resource_id) const;

		pplx::task<std::shared_ptr<TriggerIterator>> QueryTriggersAsync(
			const utility::string_t& query,
			const int page_size = 10) const;

		std::shared_ptr<TriggerIterator> QueryTriggers(
			const utility::string_t& query,
			const int page_size = 10) const;

		//stored procedures management
		pplx::task<std::shared_ptr<StoredProcedure>> CreateStoredProcedureAsync(
			const utility::string_t& id,
			const utility::string_t& body) const;

		std::shared_ptr<StoredProcedure> CreateStoredProcedure(
			const utility::string_t& id,
			const utility::string_t& body) const;

		pplx::task<std::shared_ptr<StoredProcedure>> GetStoredProcedureAsync(
			const utility::string_t& resource_id) const;

		std::shared_ptr<StoredProcedure> GetStoredProcedure(
			const utility::string_t& resource_id) const;

		pplx::task<std::vector<std::shared_ptr<StoredProcedure>>> ListStoredProceduresAsync() const;

		std::vector<std::shared_ptr<StoredProcedure>> ListStoredProcedures() const;

		pplx::task<std::shared_ptr<StoredProcedure>> ReplaceStoredProcedureAsync(
			const utility::string_t& id,
			const utility::string_t& new_id,
			const utility::string_t& body) const;

		std::shared_ptr<StoredProcedure> ReplaceStoredProcedure(
			const utility::string_t& id,
			const utility::string_t& new_id,
			const utility::string_t& body) const;

		pplx::task<void> DeleteStoredProcedureAsync(
			const std::shared_ptr<StoredProcedure>& storedProcedure) const;

		void DeleteStoredProcedure(
			const std::shared_ptr<StoredProcedure>& storedProcedure) const;

		pplx::task<void> DeleteStoredProcedureAsync(
			const utility::string_t& resource_id) const;

		void DeleteStoredProcedure(
			const utility::string_t& resource_id) const;

		pplx::task<std::shared_ptr<StoredProcedureIterator>> QueryStoredProceduresAsync(
			const utility::string_t& query,
			const int page_size = 10) const;

		std::shared_ptr<StoredProcedureIterator> QueryStoredProcedures(
			const utility::string_t& query,
			const int page_size = 10) const;

		pplx::task<void> ExecuteStoredProcedureAsync(
			const utility::string_t& resource_id,
			const web::json::value& input) const;

		void ExecuteStoredProcedure(
			const utility::string_t& resource_id,
			const web::json::value& input) const;

		void ExecuteStoredProcedureWithOutput(
			const utility::string_t& resource_id,
			const web::json::value& input,
			web::json::value& output) const;

		// User defined functions management
		pplx::task<std::shared_ptr<UserDefinedFunction>> CreateUserDefinedFunctionAsync(
			const utility::string_t& id,
			const utility::string_t& body) const;

		std::shared_ptr<UserDefinedFunction> CreateUserDefinedFunction(
			const utility::string_t& id,
			const utility::string_t& body) const;

		pplx::task<std::shared_ptr<UserDefinedFunction>> GetUserDefinedFunctionAsync(
			const utility::string_t& resource_id) const;

		std::shared_ptr<UserDefinedFunction> GetUserDefinedFunction(
			const utility::string_t& resource_id) const;

		pplx::task<std::vector<std::shared_ptr<UserDefinedFunction>>> ListUserDefinedFunctionsAsync() const;

		std::vector<std::shared_ptr<UserDefinedFunction>> ListUserDefinedFunctions() const;

		pplx::task<std::shared_ptr<UserDefinedFunction>> ReplaceUserDefinedFunctionAsync(
			const utility::string_t& id,
			const utility::string_t& new_id,
			const utility::string_t& body) const;

		std::shared_ptr<UserDefinedFunction> ReplaceUserDefinedFunction(
			const utility::string_t& id,
			const utility::string_t& new_id,
			const utility::string_t& body) const;

		pplx::task<void> DeleteUserDefinedFunctionAsync(
			const std::shared_ptr<UserDefinedFunction>& userDefinedFunction) const;

		void DeleteUserDefinedFunction(
			const std::shared_ptr<UserDefinedFunction>& userDefinedFunction) const;

		pplx::task<void> DeleteUserDefinedFunctionAsync(
			const utility::string_t& resource_id) const;

		void DeleteUserDefinedFunction(
			const utility::string_t& resource_id) const;

		pplx::task<std::shared_ptr<UserDefinedFunctionIterator>> QueryUserDefinedFunctionsAsync(
			const utility::string_t& query,
			const int page_size = 10) const;

		std::shared_ptr<UserDefinedFunctionIterator> QueryUserDefinedFunctions(
			const utility::string_t& query,
			const int page_size = 10) const;

		utility::string_t docs() const
		{
			return docs_;
		}

		utility::string_t sprocs() const
		{
			return sprocs_;
		}

		utility::string_t triggers() const
		{
			return triggers_;
		}

		utility::string_t udfs() const
		{
			return udfs_;
		}

		utility::string_t conflicts() const
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

		static utility::string_t GenerateGuid();

		utility::string_t docs_;
		utility::string_t sprocs_;
		utility::string_t triggers_;
		utility::string_t udfs_;
		utility::string_t conflicts_;
		IndexingPolicy indexing_policy_;
	};
}

#endif // !_DOCUMENTDB_COLLECTION_H_
