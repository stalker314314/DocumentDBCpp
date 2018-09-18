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

#define DOCUMENT_ID (_XPLATSTR("id"))
#define ATTACHMENT_ID (_XPLATSTR("id"))
#define PERMISSION_MODE (_XPLATSTR("permissionMode"))
#define RESOURCE (_XPLATSTR("resource"))
#define BODY (_XPLATSTR("body"))
#define TRIGGER_OPERATION (_XPLATSTR("triggerOperation"))
#define TRIGGER_TYPE (_XPLATSTR("triggerType"))
#define CONTENT_TYPE (_XPLATSTR("contentType"))
#define MEDIA (_XPLATSTR("media"))

// Resource paths
#define RESOURCE_PATH_DBS (_XPLATSTR("dbs"))
#define RESOURCE_PATH_COLLS (_XPLATSTR("colls"))
#define RESOURCE_PATH_DOCS (_XPLATSTR("docs"))
#define RESOURCE_PATH_USERS (_XPLATSTR("users"))
#define RESOURCE_PATH_PERMISSIONS (_XPLATSTR("permissions"))
#define RESOURCE_PATH_TRIGGERS (_XPLATSTR("triggers"))
#define RESOURCE_PATH_SPROCS (_XPLATSTR("sprocs"))
#define RESOURCE_PATH_UDFS (_XPLATSTR("udfs"))
#define RESOURCE_PATH_ATTACHMENTS (_XPLATSTR("attachments"))

// MIME types
#define MIME_TYPE_APPLICATION_JSON (_XPLATSTR("application/json"))
#define MIME_TYPE_APPLICATION_SQL (_XPLATSTR("application/sql"))

// Headers
#define HEADER_MS_CONTINUATION (_XPLATSTR("x-ms-continuation"))
#define HEADER_MS_DATE (_XPLATSTR("x-ms-date"))
#define HEADER_MS_VERSION (_XPLATSTR("x-ms-version"))
#define HEADER_MS_DOCUMENTDB_IS_QUERY (_XPLATSTR("x-ms-documentdb-isquery"))
#define HEADER_MS_MAX_ITEM_COUNT (_XPLATSTR("x-ms-max-item-count"))

// Response body
#define RESPONSE_DATABASES (_XPLATSTR("Databases"))
#define RESPONSE_QUERY_DOCUMENTS (_XPLATSTR("Documents"))
#define RESPONSE_INDEXING_POLICY (_XPLATSTR("indexingPolicy"))
#define RESPONSE_DOCUMENT_COLLECTIONS (_XPLATSTR("DocumentCollections"))
#define RESPONSE_INDEX_KIND (_XPLATSTR("kind"))
#define RESPONSE_INDEX_INDEX_TYPE (_XPLATSTR("kind"))
#define RESPONSE_INDEX_DATA_TYPE (_XPLATSTR("dataType"))
#define RESPONSE_INDEX_PRECISION (_XPLATSTR("precision"))
#define RESPONSE_INDEX_NUMERIC_PRECISION (_XPLATSTR("precision"))
#define RESPONSE_INDEX_STRING_PRECISION (_XPLATSTR("StringPrecision"))
#define RESPONSE_INDEX_PATH (_XPLATSTR("path"))
#define RESPONSE_INDEXING_POLICY_AUTOMATIC (_XPLATSTR("automatic"))
#define RESPONSE_INDEXING_POLICY_INDEXING_MODE (_XPLATSTR("indexingMode"))
#define RESPONSE_INDEXING_POLICY_INCLUDED_PATHS (_XPLATSTR("includedPaths"))
#define RESPONSE_INDEXING_POLICY_INCLUDED_PATHS_INDEXES (_XPLATSTR("indexes"))
#define RESPONSE_INDEXING_POLICY_EXCLUDED_PATHS (_XPLATSTR("excludedPaths"))
#define RESPONSE_INDEXING_POLICY_PATH (_XPLATSTR("Path"))
#define RESPONSE_BODY_COUNT (_XPLATSTR("_count"))
#define RESPONSE_USERS (_XPLATSTR("Users"))
#define RESPONSE_PERMISSIONS (_XPLATSTR("Permissions"))
#define RESPONSE_QUERY_TRIGGERS (_XPLATSTR("Triggers"))
#define RESPONSE_QUERY_SPROCS (_XPLATSTR("StoredProcedures"))
#define RESPONSE_QUERY_UDFS (_XPLATSTR("UserDefinedFunctions"))
#define RESPONSE_QUERY_ATTACHMENTS (_XPLATSTR("Attachments"))

// Response keys
#define RESPONSE_RESOURCE_RID (_XPLATSTR("_rid"))
#define RESPONSE_RESOURCE_TS (_XPLATSTR("_ts"))
#define RESPONSE_RESOURCE_SELF (_XPLATSTR("_self"))
#define RESPONSE_RESOURCE_ETAG (_XPLATSTR("_etag"))
#define RESPONSE_RESOURCE_COLLS (_XPLATSTR("_colls"))
#define RESPONSE_RESOURCE_USERS (_XPLATSTR("_users"))
#define RESPONSE_RESOURCE_ATTACHMENTS (_XPLATSTR("_attachments"))
#define RESPONSE_RESOURCE_DOCS (_XPLATSTR("_docs"))
#define RESPONSE_RESOURCE_SPROCS (_XPLATSTR("_sprocs"))
#define RESPONSE_RESOURCE_TRIGGERS (_XPLATSTR("_triggers"))
#define RESPONSE_RESOURCE_UDFS (_XPLATSTR("_udfs"))
#define RESPONSE_RESOURCE_CONFLICTS (_XPLATSTR("_conflicts"))
#define RESPONSE_RESOURCE_PERMISSIONS (_XPLATSTR("_permissions"))
#define RESPONSE_RESOURCE_PERMISSION_MODE (_XPLATSTR("permissionMode"))
#define RESPONSE_RESOURCE_RESOURCE (_XPLATSTR("resource"))
#define RESPONSE_RESOURCE_TOKEN (_XPLATSTR("_token"))
#define RESPONSE_RESOURCE_BODY (_XPLATSTR("body"))
#define RESPONSE_RESOURCE_TRIGGER_OPERATION (_XPLATSTR("triggerOperation"))
#define RESPONSE_RESOURCE_TRIGGER_TYPE (_XPLATSTR("triggerType"))
#define RESPONSE_RESOURCE_CONTENT_TYPE (_XPLATSTR("contentType"))
#define RESPONSE_RESOURCE_MEDIA (_XPLATSTR("media"))

// Response error keys
#define RESPONSE_ERROR_MESSAGE (_XPLATSTR("message"))
#define RESPONSE_ERROR_CODE (_XPLATSTR("code"))

#endif // !_DOCUMENTDB_DOCUMENT_DB_CONSTANTS_H_
