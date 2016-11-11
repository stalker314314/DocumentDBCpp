// hellodocumentdb.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cpprest/json.h>

#ifdef USUNG_NUGET
// In VS case, this sample is built using Nuget and these are location of headers
#include <documentdbcpp\DocumentClient.h>
#include <documentdbcpp\exceptions.h>
#include <documentdbcpp\TriggerOperation.h>
#include <documentdbcpp\TriggerType.h>
#else
// In Linux/CMake case, we are getting headers locally
#include <DocumentClient.h>
#include <exceptions.h>
#include <TriggerOperation.h>
#include <TriggerType.h>
#endif

using namespace documentdb;
using namespace web::json;
using namespace std;
using namespace utility;

#ifndef U
#define U(x) _XPLATSTR(x)
#endif

void executesimplequery(const DocumentClient &client,
	const string_t dbresourceid,
	const string_t collresourceid) {
	try {
		client.GetDatabase(dbresourceid).get();
		shared_ptr<Database> db = client.GetDatabase(dbresourceid);
		shared_ptr<Collection> coll = db->GetCollection(collresourceid);
		string_t coll_name = coll->id();
		shared_ptr<DocumentIterator> iter =
			coll->QueryDocumentsAsync(U("SELECT * FROM ") + coll_name).get();
		ucout << "\n\nQuerying collection:";
		while (iter->HasMore()) {
			shared_ptr<Document> doc = iter->Next();
			string_t doc_name = doc->id();
			ucout << "\n\t" << doc_name << "\n";
			ucout << "\t"
				<< "[{\"FirstName\":"
				<< doc->payload().at(U("FirstName")).as_string()
				<< ",\"LastName\":" << doc->payload().at(U("LastName")).as_string()
				<< "}]";
		}
	}
	catch (DocumentDBRuntimeException ex) {
		ucout << ex.message();
	}
}

void replacedocument(const DocumentClient &client, const string_t dbresourceid,
	const string_t collresourceid,
	const string_t docresourceid) {
	try {
		client.GetDatabase(dbresourceid).get();
		shared_ptr<Database> db = client.GetDatabase(dbresourceid);
		shared_ptr<Collection> coll = db->GetCollection(collresourceid);
		value newdoc;
		newdoc[U("id")] = value::string(U("WakefieldFamily"));
		newdoc[U("FirstName")] = value::string(U("Lucy"));
		newdoc[U("LastName")] = value::string(U("Smith Wakefield"));
		coll->ReplaceDocument(docresourceid, newdoc);
	}
	catch (DocumentDBRuntimeException ex) {
		throw;
	}
}


void deletedocument(const DocumentClient &client, const string_t dbresourceid,
	const string_t collresourceid, const string_t docresourceid) {
	try {
		client.GetDatabase(dbresourceid).get();
		shared_ptr<Database> db = client.GetDatabase(dbresourceid);
		shared_ptr<Collection> coll = db->GetCollection(collresourceid);
		coll->DeleteDocumentAsync(docresourceid).get();
	}
	catch (DocumentDBRuntimeException ex) {
		ucout << ex.message();
	}
}

void deletedb(const DocumentClient &client, const string_t dbresourceid) {
	try {
		client.DeleteDatabase(dbresourceid);
	}
	catch (DocumentDBRuntimeException ex) {
		ucout << ex.message();
	}
}

int main() {
	try {
		// Start by defining your account's configuration
		DocumentDBConfiguration conf(
			U("https://cppdocuments.documents.azure.com:443/"), U("Add your key here"));
		// Create your client
		DocumentClient client(conf);
		// Create a new database
		try {
			shared_ptr<Database> db = client.CreateDatabase(U("FamilyDB"));
			ucout << "\nCreating database:\n" << db->id();
			// Create a collection inside database
			shared_ptr<Collection> coll = db->CreateCollection(U("FamilyColl"));
			ucout << "\n\nCreating collection:\n" << coll->id();
			value document_family;
			document_family[U("id")] = value::string(U("AndersenFamily"));
			document_family[U("FirstName")] = value::string(U("Thomas"));
			document_family[U("LastName")] = value::string(U("Andersen"));
			shared_ptr<Document> doc =
				coll->CreateDocumentAsync(document_family).get();
			ucout << "\n\nCreating document:\n" << doc->id();
			document_family[U("id")] = value::string(U("WakefieldFamily"));
			document_family[U("FirstName")] = value::string(U("Lucy"));
			document_family[U("LastName")] = value::string(U("Wakefield"));
			doc = coll->CreateDocumentAsync(document_family).get();
			ucout << "\n\nCreating document:\n" << doc->id();			executesimplequery(client, db->resource_id(), coll->resource_id());
			executesimplequery(client, db->resource_id(), coll->resource_id());
			replacedocument(client, db->resource_id(), coll->resource_id(),
				doc->resource_id());
			ucout << "\n\nReplaced document:\n" << doc->id();
			executesimplequery(client, db->resource_id(), coll->resource_id());
			deletedocument(client, db->resource_id(), coll->resource_id(),
				doc->resource_id());
			ucout << "\n\nDeleted document:\n" << doc->id();
			deletedb(client, db->resource_id());
			ucout << "\n\nDeleted db:\n" << db->id();
			cin.get();
		}
		catch (ResourceAlreadyExistsException ex) {
			ucout << ex.message();
		}
	}
	catch (DocumentDBRuntimeException ex) {
		ucout << ex.message();
	}
	cin.get();
}
