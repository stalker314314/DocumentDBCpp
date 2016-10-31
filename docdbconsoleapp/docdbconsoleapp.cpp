// hellodocumentdbcpp.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cpprest/json.h>
#include "DocumentClient.h"
#include "exceptions.h"
#include "TriggerOperation.h"
#include "TriggerType.h"

using namespace std;
using namespace documentdb;
using namespace web::json;

shared_ptr<Database> createdbandcoll(const DocumentClient& client, const wstring dbname, const wstring collname) {
	try {
		// Create a new database (say dbname=FamilyRegistry)
		shared_ptr<Database> db = client.CreateDatabase(dbname);
		// Create a collection inside database (say collname=FamilyCollection)
		shared_ptr<Collection> coll = db->CreateCollection(collname);
		return db;
	}
	catch (DocumentDBRuntimeException ex) {
		wcout << ex.message();
	}

	return nullptr;
}

void replacedocument(const DocumentClient& client, const wstring dbresourceid, const wstring collresourceid,
	const wstring docresourceid) {
	try {
		client.GetDatabase(dbresourceid).get();
		shared_ptr<Database> db = client.GetDatabase(dbresourceid);
		shared_ptr<Collection> coll = db->GetCollection(collresourceid);
		value newdoc;
		newdoc[L"id"] = value::string(L"WakefieldFamily");
		newdoc[L"FirstName"] = value::string(L"Lucy");
		newdoc[L"LastName"] = value::string(L"Smith Wakefield");
		coll->ReplaceDocumentAsync(docresourceid, newdoc);
	}
	catch (DocumentDBRuntimeException ex) {
		throw;
	}
}

void deletedocument(const DocumentClient& client, const wstring dbresourceid, const wstring collresourceid,
	const wstring docresourceid) {
	try {
		client.GetDatabase(dbresourceid).get();
		shared_ptr<Database> db = client.GetDatabase(dbresourceid);
		shared_ptr<Collection> coll = db->GetCollection(collresourceid);
		coll->DeleteDocumentAsync(docresourceid).get();
	}
	catch (DocumentDBRuntimeException ex) {
		throw;
	}
}

void deletedb(const DocumentClient& client, const wstring dbresourceid) {
	try {
		client.DeleteDatabase(dbresourceid);
	}
	catch (DocumentDBRuntimeException ex) {
		throw;
	}
}

void executesimplequery(const DocumentClient& client, const wstring dbresourceid, const wstring collresourceid) {
	try {
		client.GetDatabase(dbresourceid).get();
		shared_ptr<Database> db = client.GetDatabase(dbresourceid);
		shared_ptr<Collection> coll = db->GetCollection(collresourceid);
		wstring coll_name = coll->id();
		shared_ptr<DocumentIterator> iter = coll->QueryDocumentsAsync(wstring(L"SELECT * FROM " + coll_name)).get();
		wcout << "\n\nQuerying collection:";
		while (iter->HasMore())
		{
			shared_ptr<Document> doc = iter->Next();
			wstring doc_name = doc->id();
			wcout << "\n\t" << doc_name << "\n";
			wcout << "\t" << "[{\"FirstName\":" << doc->payload().at(U("FirstName")).as_string() << ",\"LastName\":"
				<< doc->payload().at(U("LastName")).as_string() << "}]";
		}
	}
	catch (DocumentDBRuntimeException ex) {
		wcout << ex.message();
	}
}

int main()
{
	try {
		// Start by defining your account's configuration
		DocumentDBConfiguration conf(L"https://cppdocuments.documents.azure.com:443/", L"Add your key here");
		// Create your client
		DocumentClient client(conf);
		// Create a new database
		try {
			shared_ptr<Database> db = client.CreateDatabase(L"FamilyDB");
			wcout << "\nCreating database:\n" << db->id();
			// Create a collection inside database
			shared_ptr<Collection> coll = db->CreateCollection(L"FamilyColl");
			wcout << "\n\nCreating collection:\n" << coll->id();

			value document_family;

			document_family[L"id"] = value::string(L"AndersenFamily");
			document_family[L"FirstName"] = value::string(L"Thomas");
			document_family[L"LastName"] = value::string(L"Andersen");
			shared_ptr<Document> doc = coll->CreateDocumentAsync(document_family).get();
			wcout << "\n\nCreating document:\n" << doc->id();

			document_family[L"id"] = value::string(L"WakefieldFamily");
			document_family[L"FirstName"] = value::string(L"Lucy");
			document_family[L"LastName"] = value::string(L"Wakefield");
			doc = coll->CreateDocumentAsync(document_family).get();
			wcout << "\n\nCreating document:\n" << doc->id();

			executesimplequery(client, db->resource_id(), coll->resource_id());
			replacedocument(client, db->resource_id(), coll->resource_id(), doc->resource_id());
			executesimplequery(client, db->resource_id(), coll->resource_id());
			deletedocument(client, db->resource_id(), coll->resource_id(), doc->resource_id());
			wcout << "\n\nDeleted document:\n" << doc->id();
			deletedb(client, db->resource_id());
			wcout << "\n\nDeleted db:\n" << db->id();
		}
		catch (ResourceAlreadyExistsException ex) {
			wcout << ex.message();
		}
	}
	catch (DocumentDBRuntimeException ex) {
		wcout << ex.message();
	}
}


