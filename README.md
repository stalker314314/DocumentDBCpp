# DocumentDBCpp

## Introduction
DocumentDBCpp is C++ wrapper and object model for [DocumentDB ](http://azure.microsoft.com/en-us/services/documentdb/) NoSQL database. It is built on top of [C++ REST SDK](https://github.com/Microsoft/cpprestsdk) framework, so all the goodies like [Task Parallelism](https://msdn.microsoft.com/en-us/library/dd492427.aspx) are included. There is no other dependencies except C++ REST SDK. Library works with both Windows (both VS2013 and VS2015) and Linux, on both x86 and x64. There is also [associated Nuget](http://www.nuget.org/packages/DocumentDbCpp/) you can use.

## Usage

Perfect article to get you started is [here](https://azure.microsoft.com/en-us/documentation/articles/documentdb-cpp-get-started/). Some sample code as a teaser:
```cpp
	// Start by defining your account's configuration
	DocumentDBConfiguration conf (U("https://<account>.documents.azure.com"), U("<primary_key>"));
	// Create your client
	DocumentClient client (conf);
	// Create a new database
	shared_ptr<Database> db = client.CreateDatabase (U("db"));
	// Create a collection inside database
	shared_ptr<Collection> coll = db->CreateCollection (U("coll"));
	// Insert a document
	web::json::value doc;
	doc[U("foo")] = web::json::value::string (U("bar"));
	coll->CreateDocument (doc);
	// All of the above is also supported in async fashion
	coll->CreateDocumentAsync (doc).then ([=](shared_ptr<Document> doc)
	{
		ucout << U("Asynchronously done inserting document");
	});
```

For complete set of supported methods, look at test.cpp and/or browse a code.

### Installation

#### Windows

Installing this library is easiest with Nuget that you can find [here](http://www.nuget.org/packages/DocumentDbCpp/). If you get stuck, detailed explanation is [here](https://azure.microsoft.com/en-us/documentation/articles/documentdb-cpp-get-started/).

#### Linux

There is no support for Linux installation out-of-box, you will have to compile library yourself (take a look below).

### Compiling

#### Windows

Only VS compiler is supported on Windows. There are two solutions - one for VS2013 (toolset v120) and one for VS2015 (toolset v140). Pick one you would like, fire it and build.

#### Linux

There are no other dependencies, once you compile and install C++ REST SDK. To compile and install C++ REST SDK, refer to [this](https://github.com/Microsoft/cpprestsdk/wiki/How-to-build-for-Linux) article. Once you are done, start by cloning, creating build directory and configuring it:
```bash
git clone https://github.com/stalker314314/DocumentDBCpp.git DocumentDBCpp
mkdir docdb.build
cd docdb.build
cmake ../DocumentDBCpp
```

After that, compile and install as usual:

```bash
make
sudo make install
```
There is nothing magical about this process, this is regular CMake. If you want to use some IDE, let's say Eclipse, configure like this:
```bash
cmake -G "Eclipse CDT4 - Unix Makefiles" ../DocumentDBCpp
```

If you want to build, for example, Debug version without tests and sample, you can configure with something like this:
```bash
cmake -DBUILD_TESTS=OFF -DBUILD_SAMPLES=OFF -DCMAKE_BUILD_TYPE=Debug ../DocumentDBCpp
```

## Testing

Test your new changes by running documentdbtest. Couple of notes before you fire it up:

1. No support currently for proper unit testing
2. Note that you will have to put your own account in test.cpp on which tests will be run against
3. There is no automatic cleanup if tests are failing, so you will need to clean up after yourself
4. Tests are not accessing other databases in your account, nor deleting anything, but anyway...be careful

## Backlog

1. Extend object model with users, triggers, UDFs, conflicts and the rest of supported DocumentDB entities
2. Support for TCP protocol
3. Support working with secondary keys
