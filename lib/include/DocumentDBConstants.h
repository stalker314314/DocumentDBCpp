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

#ifndef _DOCUMENTDB_DOCUMENT_DB_CONSTANTS_H_
#define _DOCUMENTDB_DOCUMENT_DB_CONSTANTS_H_

#define DOCUMENT_ID (L"id")
#define ATTACHMENT_ID (L"id")
#define PERMISSION_MODE (L"permissionMode")
#define RESOURCE (L"resource")
#define BODY (L"body")
#define TRIGGER_OPERATION (L"triggerOperation")
#define TRIGGER_TYPE (L"triggerType")
#define CONTENT_TYPE (L"contentType")
#define MEDIA (L"media")

// Resource paths
#define RESOURCE_PATH_DBS (L"dbs")
#define RESOURCE_PATH_COLLS (L"colls")
#define RESOURCE_PATH_DOCS (L"docs")
#define RESOURCE_PATH_USERS (L"users")
#define RESOURCE_PATH_PERMISSIONS (L"permissions")
#define RESOURCE_PATH_TRIGGERS (L"triggers")
#define RESOURCE_PATH_SPROCS (L"sprocs")
#define RESOURCE_PATH_UDFS (L"udfs")
#define RESOURCE_PATH_ATTACHMENTS (L"attachments")

// MIME types
#define MIME_TYPE_APPLICATION_JSON (L"application/json")
#define MIME_TYPE_APPLICATION_SQL (L"application/sql")

// Headers
#define HEADER_MS_CONTINUATION (L"x-ms-continuation")
#define HEADER_MS_DATE (L"x-ms-date")
#define HEADER_MS_VERSION (L"x-ms-version")
#define HEADER_MS_DOCUMENTDB_IS_QUERY (L"x-ms-documentdb-isquery")
#define HEADER_MS_MAX_ITEM_COUNT (L"x-ms-max-item-count")

// Response body
#define RESPONSE_DATABASES (L"Databases")
#define RESPONSE_QUERY_DOCUMENTS (L"Documents")
#define RESPONSE_INDEXING_POLICY (L"indexingPolicy")
#define RESPONSE_DOCUMENT_COLLECTIONS (L"DocumentCollections")
#define RESPONSE_INDEX_INDEX_TYPE (L"IndexType")
#define RESPONSE_INDEX_NUMERIC_PRECISION (L"NumericPrecision")
#define RESPONSE_INDEX_STRING_PRECISION (L"StringPrecision")
#define RESPONSE_INDEX_PATH (L"Path")
#define RESPONSE_INDEXING_POLICY_AUTOMATIC (L"automatic")
#define RESPONSE_INDEXING_POLICY_INDEXING_MODE (L"indexingMode")
#define RESPONSE_INDEXING_POLICY_INCLUDED_PATHS (L"IncludedPaths")
#define RESPONSE_INDEXING_POLICY_EXCLUDED_PATHS (L"ExcludedPaths")
#define RESPONSE_INDEXING_POLICY_PATH (L"Path")
#define RESPONSE_BODY_COUNT (L"_count")
#define RESPONSE_USERS (L"Users")
#define RESPONSE_PERMISSIONS (L"Permissions")
#define RESPONSE_QUERY_TRIGGERS (L"Triggers")
#define RESPONSE_QUERY_SPROCS (L"StoredProcedures")
#define RESPONSE_QUERY_UDFS (L"UserDefinedFunctions")
#define RESPONSE_QUERY_ATTACHMENTS (L"Attachments")

// Response keys
#define RESPONSE_RESOURCE_RID (L"_rid")
#define RESPONSE_RESOURCE_TS (L"_ts")
#define RESPONSE_RESOURCE_SELF (L"_self")
#define RESPONSE_RESOURCE_ETAG (L"_etag")
#define RESPONSE_RESOURCE_COLLS (L"_colls")
#define RESPONSE_RESOURCE_USERS (L"_users")
#define RESPONSE_RESOURCE_ATTACHMENTS (L"_attachments")
#define RESPONSE_RESOURCE_DOCS (L"_docs")
#define RESPONSE_RESOURCE_SPROCS (L"_sprocs")
#define RESPONSE_RESOURCE_TRIGGERS (L"_triggers")
#define RESPONSE_RESOURCE_UDFS (L"_udfs")
#define RESPONSE_RESOURCE_CONFLICTS (L"_conflicts")
#define RESPONSE_RESOURCE_PERMISSIONS (L"_permissions")
#define RESPONSE_RESOURCE_PERMISSION_MODE (L"permissionMode")
#define RESPONSE_RESOURCE_RESOURCE (L"resource")
#define RESPONSE_RESOURCE_TOKEN (L"_token")
#define RESPONSE_RESOURCE_BODY (L"body")
#define RESPONSE_RESOURCE_TRIGGER_OPERATION (L"triggerOperation")
#define RESPONSE_RESOURCE_TRIGGER_TYPE (L"triggerType")
#define RESPONSE_RESOURCE_CONTENT_TYPE (L"contentType")
#define RESPONSE_RESOURCE_MEDIA (L"media")

// Response error keys
#define RESPONSE_ERROR_MESSAGE (L"message")
#define RESPONSE_ERROR_CODE (L"code")

#endif // !_DOCUMENTDB_DOCUMENT_DB_CONSTANTS_H_
