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

#include <cstdlib>
#include <ctime>
#include <fstream>

#include <cpprest/json.h>

#include "DocumentClient.h"
#include "exceptions.h"

using namespace std;
using namespace documentdb;
using namespace web::json;

wstring generate_random_string(
	size_t length)
{
	auto randchar = []() -> char
	{
		const char charset[] = "abcdefghijklmnopqrstuvwxyz";
		const size_t max_index = sizeof(charset) - 1;
		return charset[rand() % max_index];
	};
	wstring str(length, 0);
	generate_n(str.begin(), length, randchar);
	return str;
}

void compare_documents(
	const shared_ptr<Document>& doc1,
	const shared_ptr<Document>& doc2,
	bool skip_id = false)
{
	if (!skip_id)
	{
		assert(doc1->id() == doc2->id());
	}
	assert(doc1->resource_id() == doc2->resource_id());
	assert(doc1->self() == doc2->self());
	assert(doc1->attachments() == doc2->attachments());
}

void test_databases(
	const DocumentClient& client)
{
	int previous_db_count = client.ListDatabasesAsync().get().size();
	assert(previous_db_count == client.ListDatabases().size());

	// Create database with a random name
	wstring db_name = generate_random_string(8);
	shared_ptr<Database> db = client.CreateDatabaseAsync(wstring(db_name)).get();
	assert(db->id() == db_name);

	// We cannot create database with a same ID
	try
	{
		client.CreateDatabaseAsync(db->id()).get();
		assert(false);
	}
	catch (const ResourceAlreadyExistsException&)
	{
		// Pass
	}

	try
	{
		client.CreateDatabase(db->id());
		assert(false);
	}
	catch (const ResourceAlreadyExistsException&)
	{
		// Pass
	}

	// Check that newly created database is in list of all databases
	vector<shared_ptr<Database>> dbs = client.ListDatabasesAsync().get();
	assert((previous_db_count + 1) == dbs.size());
	assert(dbs.size() == client.ListDatabases().size());

	bool found_db = false;

	for (auto db_read : dbs)
	{
		if (db_read->id() == db_name)
		{
			found_db = true;
		}
	}
	assert(found_db);

	// Try getting newly created database
	shared_ptr<Database> read_db = client.GetDatabaseAsync(db->resource_id()).get();
	assert(db->resource_id() == read_db->resource_id());
	assert(db->id() == read_db->id());
	assert(db->colls() == read_db->colls());
	assert(db->etag() == read_db->etag());
	assert(db->self() == read_db->self());
	assert(db->users() == read_db->users());

	// Replace database
	wstring new_db_name = generate_random_string(8);
	db = client.ReplaceDatabaseAsync(db->resource_id(), wstring(new_db_name)).get();
	assert(db->id() == new_db_name);

	// Delete created database
	wstring resource_id = db->resource_id();
	client.DeleteDatabaseAsync(db->resource_id()).get();

	// We cannot delete database that does not exist
	try
	{
		client.DeleteDatabaseAsync(resource_id).get();
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}

	try
	{
		client.DeleteDatabase(resource_id);
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}

	// We cannot get database that does not exist
	try
	{
		client.GetDatabaseAsync(resource_id).get();
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}

	try
	{
		client.GetDatabase(resource_id);
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}
}

void test_collections(
	const DocumentClient& client)
{
	wstring db_name = generate_random_string(8);

	// Create a database on which we are going to test collections
	shared_ptr<Database> db = client.CreateDatabase(wstring(db_name));

	// There should be no collections at this point in this database
	vector<shared_ptr<Collection>> colls = db->ListCollectionsAsync().get();
	assert(colls.size() == 0);
	assert(colls.size() == db->ListCollections().size());

	// Create new test collection
	wstring coll_name = generate_random_string(8);
	shared_ptr<Collection> coll = db->CreateCollectionAsync(coll_name).get();
	assert(coll->id() == coll_name);

	// We cannot create collection with same resource ID
	try
	{
		db->CreateCollectionAsync(coll_name).get();
		assert(false);
	}
	catch (const ResourceAlreadyExistsException&)
	{
		// Pass
	}

	try
	{
		db->CreateCollection(coll_name);
		assert(false);
	}
	catch (const ResourceAlreadyExistsException&)
	{
		// Pass
	}

	// Try getting created collection
	shared_ptr<Collection> coll2 = db->GetCollectionAsync(coll->resource_id()).get();
	assert(coll->resource_id() == coll2->resource_id());
	assert(coll->id() == coll2->id());
	assert(coll->self() == coll2->self());
	assert(coll->conflicts() == coll2->conflicts());
	assert(coll->docs() == coll2->docs());
	assert(coll->sprocs() == coll2->sprocs());
	assert(coll->triggers() == coll2->triggers());
	assert(coll->udfs() == coll2->udfs());

	// Try reading all collections and make sure there is only test one in it
	colls = db->ListCollectionsAsync().get();
	assert(colls.size() == 1);
	assert(colls[0]->resource_id() == coll->resource_id());
	assert(colls[0]->id() == coll->id());
	assert(colls[0]->self() == coll->self());
	assert(colls[0]->conflicts() == coll->conflicts());
	assert(colls[0]->docs() == coll->docs());
	assert(colls[0]->sprocs() == coll->sprocs());
	assert(colls[0]->triggers() == coll->triggers());
	assert(colls[0]->udfs() == coll->udfs());

	// Delete created collection
	wstring resource_id = coll->resource_id();
	db->DeleteCollectionAsync(coll).get();

	// We cannot get collection that does not exist
	try
	{
		db->GetCollectionAsync(resource_id).get();
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}

	try
	{
		db->GetCollection(resource_id);
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}

	// We cannot delete collection that does not exist
	try
	{
		db->DeleteCollectionAsync(resource_id).get();
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}

	try
	{
		db->DeleteCollection(resource_id);
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}

	// Delete database now that we are done testing
	client.DeleteDatabase(db->resource_id());
}

void test_documents(
	const DocumentClient& client)
{
	wstring db_name = generate_random_string(8);

	// Create a database on which we are going to test collections
	shared_ptr<Database> db = client.CreateDatabase(wstring(db_name));

	// Create new test collection
	wstring coll_name = generate_random_string(8);
	shared_ptr<Collection> coll = db->CreateCollection(coll_name);

	// Collection should be empty
	assert(coll->ListDocumentsAsync().get().size() == 0);
	assert(coll->ListDocuments().size() == 0);

	shared_ptr<DocumentIterator> iter = coll->QueryDocumentsAsync(wstring(L"SELECT * FROM " + coll_name)).get();
	assert(!iter->HasMore());

	// Try insering one document with ID set
	value document1;
	document1[L"id"] = value::string(L"id");
	document1[L"foo"] = value::string(L"bar");
	shared_ptr<Document> doc = coll->CreateDocumentAsync(document1).get();
	assert(doc->id() == L"id");

	// Try inserting document with same ID
	value document_conflict;
	document_conflict[L"id"] = value::string(L"id");

	try
	{
		coll->CreateDocumentAsync(document_conflict).get();
		assert(false);
	}
	catch (const ResourceAlreadyExistsException&)
	{
		// Pass
	}

	try
	{
		coll->CreateDocumentAsync(document_conflict.serialize()).get();
		assert(false);
	}
	catch (const ResourceAlreadyExistsException&)
	{
		// Pass
	}

	try
	{
		coll->CreateDocument(document_conflict);
		assert(false);
	}
	catch (const ResourceAlreadyExistsException&)
	{
		// Pass
	}

	try
	{
		coll->CreateDocument(document_conflict.serialize());
		assert(false);
	}
	catch (const ResourceAlreadyExistsException&)
	{
		// Pass
	}

	// Get document
	shared_ptr<Document> doc_get = coll->GetDocumentAsync(doc->resource_id()).get();
	assert(doc_get->payload().at(L"foo").as_string() == L"bar");
	compare_documents(doc_get, doc);

	// Query for documents
	vector <shared_ptr<Document>> doc_list = coll->ListDocumentsAsync().get();
	assert(doc_list.size() == 1);
	assert(doc_list[0]->payload().at(L"foo").as_string() == L"bar");
	compare_documents(doc_list[0], doc);

	iter = coll->QueryDocumentsAsync(wstring(L"SELECT * FROM " + coll_name)).get();
	int count = 0;
	while (iter->HasMore())
	{
		shared_ptr<Document> iter_doc = iter->Next();
		assert(iter_doc->payload().at(L"foo").as_string() == L"bar");
		compare_documents(iter_doc, doc);

		count++;
	}
	assert(count == 1);

	// Delete document with document
	wstring resource_id = doc->resource_id();
	coll->DeleteDocumentAsync(doc).get();

	assert(coll->ListDocumentsAsync().get().size() == 0);

	iter = coll->QueryDocuments(wstring(L"SELECT * FROM " + coll_name));
	assert(!iter->HasMore());

	// Getting the document that does not exist results in exception
	try
	{
		coll->GetDocumentAsync(resource_id).get();
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}

	try
	{
		coll->GetDocument(resource_id);
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}

	// Deleting document that does not exist results in exception
	try
	{
		coll->DeleteDocumentAsync(resource_id).get();
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}

	try
	{
		coll->DeleteDocument(resource_id);
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}

	// Try inserting one document without ID set
	value document2;
	document2[L"foo2"] = value::string(L"bar2");
	doc = coll->CreateDocument(document2);

	// Query again for documents
	doc_list = coll->ListDocuments();
	assert(doc_list.size() == 1);
	assert(doc_list[0]->payload().at(L"foo2").as_string() == L"bar2");
	compare_documents(doc_list[0], doc);

	coll->DeleteDocument(doc->resource_id());

	// Try inserting document as JSON
	wstring id = generate_random_string(32);
	wstring json_object = L"{\"id\": \"" + id + L"\", \"foo3\": \"bar3\" }";
	doc = coll->CreateDocument(json_object);

	// Query again for documents
	iter = coll->QueryDocuments(wstring(L"SELECT * FROM " + coll_name));
	count = 0;
	while (iter->HasMore())
	{
		shared_ptr<Document> iter_doc = iter->Next();
		assert(doc->payload().at(L"foo3").as_string() == L"bar3");
		compare_documents(iter_doc, doc);

		count++;
	}
	assert(count == 1);

	// Replace document
	document2[L"foo3"] = value::string(L"bar4");
	shared_ptr<Document> replaced_doc = coll->ReplaceDocumentAsync(doc->resource_id(), document2).get();
	assert(replaced_doc->payload().at(L"foo3").as_string() == L"bar4");
	compare_documents(replaced_doc, doc, true);

	// Delete document with resource ID
	coll->DeleteDocumentAsync(doc->resource_id()).get();
	iter = coll->QueryDocuments(wstring(L"SELECT * FROM " + coll_name));
	assert(!iter->HasMore());

	// Delete collection now that we are done testing
	db->DeleteCollection(coll);

	// Delete database now that we are done testing
	client.DeleteDatabase(db->resource_id());
}

void test_users(
	const DocumentClient& client)
{
	wstring db_name = generate_random_string(8);

	// Create a database on which we are going to test users
	shared_ptr<Database> db = client.CreateDatabase(wstring(db_name));

	// There should be no users at this point in this database
	vector<shared_ptr<User>> users = db->ListUsersAsync().get();
	assert(users.size() == 0);
	assert(users.size() == db->ListUsers().size());

	// Create new test user
	wstring user_name = generate_random_string(8);
	shared_ptr<User> user = db->CreateUserAsync(user_name).get();
	assert(user->id() == user_name);

	// We cannot create user with same resource ID
	try
	{
		db->CreateUserAsync(user_name).get();
		assert(false);
	}
	catch (const ResourceAlreadyExistsException&)
	{
		// Pass
	}

	try
	{
		db->CreateUser(user_name);
		assert(false);
	}
	catch (const ResourceAlreadyExistsException&)
	{
		// Pass
	}

	// Try getting created user
	shared_ptr<User> user2 = db->GetUserAsync(user->resource_id()).get();
	assert(user->resource_id() == user2->resource_id());
	assert(user->id() == user2->id());
	assert(user->self() == user2->self());
	assert(user->permissions() == user2->permissions());
	

	// Try reading all users and make sure there is only test one in it
	users = db->ListUsersAsync().get();
	assert(users.size() == 1);
	assert(users[0]->resource_id() == user->resource_id());
	assert(users[0]->id() == user->id());
	assert(users[0]->self() == user->self());
	assert(users[0]->permissions() == user->permissions());
	
	// Replace user
	wstring new_user_name = generate_random_string(8);
	user = db->ReplaceUserAsync(user->resource_id(), wstring(new_user_name)).get();
	assert(user->id() == new_user_name);

	// Delete created user
	wstring resource_id = user->resource_id();
	db->DeleteUserAsync(user).get();

	// We cannot get user that does not exist
	try
	{
		db->GetUserAsync(resource_id).get();
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}

	try
	{
		db->GetUser(resource_id);
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}

	// We cannot delete user that does not exist
	try
	{
		db->DeleteUserAsync(resource_id).get();
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}

	try
	{
		db->DeleteUser(resource_id);
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}

	// Delete database now that we are done testing
	client.DeleteDatabase(db->resource_id());
}

void test_permissions(
	const DocumentClient& client) {
	wstring db_name = generate_random_string(8);

	// Create a database on which we are going to test permissions
	shared_ptr<Database> db = client.CreateDatabase(wstring(db_name));

	// There should be no users at this point in this database
	vector<shared_ptr<User>> users = db->ListUsersAsync().get();
	assert(users.size() == 0);
	assert(users.size() == db->ListUsers().size());

	wstring user_name = generate_random_string(8);

	// Create a user on which we are going to test permissions
	shared_ptr<User> user = db->CreateUser(wstring(user_name));

	// There should be no permissions at this point in this database
	vector<shared_ptr<Permission>> permissions = user->ListPermissionsAsync().get();
	assert(permissions.size() == 0);
	assert(permissions.size() == user->ListPermissions().size());

	wstring coll_name = generate_random_string(8);

	// Create a collection on which we are going to test permissions
	shared_ptr<Collection> coll = db->CreateCollection(wstring(coll_name));

	// Create new test permission
	wstring permission_name = generate_random_string(8);
	shared_ptr<Permission> permission = user->CreatePermissionAsync(permission_name, L"All", coll->self()).get();
	assert(permission->id() == permission_name);

	// We cannot create permission with same resource ID
	try
	{
		user->CreatePermissionAsync(permission_name, L"All", coll->self()).get();
		assert(false);
	}
	catch (const ResourceAlreadyExistsException&)
	{
		// Pass
	}

	try
	{
		user->CreatePermission(permission_name, L"All", coll->self());
		assert(false);
	}
	catch (const ResourceAlreadyExistsException&)
	{
		// Pass
	}

	// Try getting created permission
	shared_ptr<Permission> permission2 = user->GetPermissionAsync(permission->resource_id()).get();
	assert(permission->resource_id() == permission2->resource_id());
	assert(permission->id() == permission2->id());
	assert(permission->self() == permission2->self());
	assert(permission->permission_mode() == permission2->permission_mode());
	assert(permission->resource() == permission2->resource());

	// Try reading all permissions and make sure there is only test one in it
	permissions = user->ListPermissionsAsync().get();
	assert(permissions.size() == 1);
	assert(permissions[0]->resource_id() == permission->resource_id());
	assert(permissions[0]->id() == permission->id());
	assert(permissions[0]->self() == permission->self());
	assert(permissions[0]->permission_mode() == permission->permission_mode());
	assert(permissions[0]->resource() == permission->resource());

	// Replace permission
	wstring new_permission_name = generate_random_string(8);
	permission = user->ReplacePermissionAsync(permission->resource_id(), wstring(new_permission_name), L"Read", coll->self()).get();
	assert(permission->id() == new_permission_name);

	// Delete created permission
	wstring resource_id = permission->resource_id();
	user->DeletePermissionAsync(permission).get();

	// We cannot get permission that does not exist
	try
	{
		user->GetPermissionAsync(resource_id).get();
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}

	try
	{
		user->GetPermission(resource_id);
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}

	// We cannot delete permission that does not exist
	try
	{
		user->DeletePermissionAsync(resource_id).get();
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}

	try
	{
		user->DeletePermission(resource_id);
		assert(false);
	}
	catch (const ResourceNotFoundException&)
	{
		// Pass
	}

	// Delete user now that we are done testing
	db->DeleteUser(user->resource_id());

	// Delete collection now that we are done testing
	db->DeleteCollection(coll->resource_id());

	// Delete database now that we are done testing
	client.DeleteDatabase(db->resource_id());
}

int main()
{
	srand((unsigned int)time(nullptr));

	wifstream confFile("account_configuration.txt");
	wstring account, primaryKey;
	confFile >> account;
	confFile >> primaryKey;

	DocumentDBConfiguration conf (
		account,
		primaryKey);
	DocumentClient client(conf);

	test_databases(client);
	test_collections(client);
	test_documents(client);
	test_users(client);
	test_permissions(client);

	return 0;
}
