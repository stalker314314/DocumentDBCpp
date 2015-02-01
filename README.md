# DocumentDBCpp

## Introduction
DocumentDBCpp is C++ wrapper and object model for [DocumentDB ](http://azure.microsoft.com/en-us/services/documentdb/) NoSQL database. It is built on top of [Casablanca](https://casablanca.codeplex.com/) C++ Rest SDK framework, so all the goodies like [Task Parallelism](https://msdn.microsoft.com/en-us/library/dd492427.aspx) are included. There is no other dependencies except Casablanca.

## Usage

```cpp
	// Start by defining your account's configuration
	DocumentDBConfiguration conf (L"https://<account>.documents.azure.com", L"<primary_key>");
	// Create your client
	DocumentClient client (conf);
	// Create a new database
	shared_ptr<Database> db = client.CreateDatabase (L"db").get ();
	// Create a collection inside database
	shared_ptr<Collection> coll = db->CreateCollection (L"coll").get ();
	// Insert a document
	web::json::value doc;
	doc[L"foo"] = web::json::value::string (L"bar");
	coll->CreateDocument (doc).get ();
	// All of the above is also supported in async fashion
	coll->CreateDocument (doc).then ([=](shared_ptr<Document> doc)
	{
		wcout << L"Asynchronously done inserting document";
	});
```

For complete set of supported methods, look at test.cpp and/or browse a code.

## Testing

Test your new changes by running Test.exe. Couple of notes before you fire it up:

1. No support currently for proper unit testing
2. Note that you will have to put your own account in test.cpp on which tests will be run against
3. There is no automatic cleanup if tests are failing, so you will need to clean up after yourself
4. Tests are not accessing other databases in your account, nor deleting anything, but anyway...be careful

## Backlog

1. Extend object model with users, triggers, UDFs, conflicts and the rest of supported DocumentDB entities
2. Support for TCP protocol
3. Support working with secondary keys
4. Support Linux and all other platforms where Casablanca is supported
