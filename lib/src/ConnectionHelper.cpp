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

#include "ConnectionHelper.h"

#ifdef _WIN32
#include <windows.h>
#include <Wininet.h>
#endif

#include <stdlib.h>
#include <locale>
#include <codecvt>
#include <time.h>

#include <cpprest/json.h>

#include "hmac_bcrypt.h"
#include "DocumentDBConstants.h"

using namespace documentdb;
using namespace std;
using namespace utility;
using namespace web::http;
using namespace web::http::client;
using namespace web::json;

static const char *DAY_NAMES[] =
  { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };
static const char *MONTH_NAMES[] =
  { "Jan", "Feb", "Mar", "Apr", "May", "Jun",
    "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };

const int RFC1123_TIME_LEN = 29;

string_t GetCurrentRequestTime()
{
	time_t t;
	struct tm tm;
	char* buf = new char[RFC1123_TIME_LEN+1];

	time(&t);
#ifdef _WIN32
	gmtime_s(&tm, &t);
#else
	gmtime_r(&t, &tm);
#endif

	strftime(buf, RFC1123_TIME_LEN+1, "---, %d --- %Y %H:%M:%S GMT", &tm);
	memcpy(buf, DAY_NAMES[tm.tm_wday], 3);
	memcpy(buf+8, MONTH_NAMES[tm.tm_mon], 3);

#ifdef _UTF16_STRINGS
	string_t timeAsString(strlen(buf) + 1, U(' '));
	mbstowcs_s(nullptr, &timeAsString[0], timeAsString.size(), buf, strlen(buf));
	timeAsString.resize(wcslen(timeAsString.c_str()));
#else
	string_t timeAsString = string_t(buf);
#endif
	delete[] buf;
	return timeAsString;
}

http_request CreateRequest(
	const method& method,
	const string_t& resource_type,
	const string_t& resource_id,
	const vector<unsigned char>& master_key)
{
	string_t requestTime = GetCurrentRequestTime();

	string_t text = method + _XPLATSTR("\n") + resource_type + _XPLATSTR("\n") + resource_id + _XPLATSTR("\n") + requestTime + _XPLATSTR("\n\n");
	string_t textLowerCase(text);
	transform(text.begin(), text.end(), textLowerCase.begin(), ::tolower);

	unsigned char buffer[1000];
#ifdef _UTF16_STRINGS
	wcstombs_s(nullptr, (char*)buffer, 1000, textLowerCase.c_str(), textLowerCase.size());
#else
	strcpy((char*)buffer, textLowerCase.c_str());
#endif
	int hashSize = 0;
	unsigned char* result = hmac((unsigned char*)&master_key[0], master_key.size(), buffer, strlen((const char*)buffer), &hashSize);

	utility::string_t signature = utility::conversions::to_base64(vector<unsigned char>(result, result + hashSize));
#ifdef _WIN32
	HeapFree(GetProcessHeap(), 0, result);
#else
	delete[] result;
#endif

	http_request request(method);
	request.headers().add(web::http::header_names::accept, MIME_TYPE_APPLICATION_JSON);
	request.headers().add(web::http::header_names::user_agent, _XPLATSTR("cpprestsdk/2.4.0.1"));
	//request.headers ().add (web::http::header_names::cache_control, _XPLATSTR("no-cache"));
	request.headers().add(
		web::http::header_names::authorization,
		uri::encode_data_string(_XPLATSTR("type=master&ver=1.0&sig=") + signature));
	request.headers().add(HEADER_MS_DATE, requestTime);
	request.headers().add(HEADER_MS_VERSION, _XPLATSTR("2017-02-22"));

	return request;
}

http_request CreateQueryRequest(
	const string_t& query,
	const int page_size,
	const string_t& resource_type,
	const string_t& resource_id,
	const vector<unsigned char>& master_key,
	const string_t& continuation_id)
{
	http_request request = CreateRequest(methods::POST, resource_type, resource_id, master_key);
	request.headers().add(web::http::header_names::content_type, MIME_TYPE_APPLICATION_SQL);
	request.headers().add(HEADER_MS_DOCUMENTDB_IS_QUERY, true);
	request.headers().add(HEADER_MS_MAX_ITEM_COUNT, page_size);

	if (!continuation_id.empty())
	{
		request.headers().add(HEADER_MS_CONTINUATION, continuation_id);
	}

	request.set_body(query);

	return request;
}

__declspec(noreturn)
void ThrowExceptionFromResponse(
const status_code& status_code,
const value& json_response)
{
	string_t message;
	if (json_response.has_field(RESPONSE_ERROR_MESSAGE))
	{
		message = json_response.at(RESPONSE_ERROR_MESSAGE).as_string();
	}
	string_t code;
	if (json_response.has_field(RESPONSE_ERROR_CODE))
	{
		code = json_response.at(RESPONSE_ERROR_CODE).as_string();
	}

	if (status_code == status_codes::NotFound)
	{
		throw ResourceNotFoundException(status_code, code, message);
	}
	else if (status_code == status_codes::Conflict)
	{
		throw ResourceAlreadyExistsException(status_code, code, message);
	}
	else if (status_code == status_codes::RequestEntityTooLarge)
	{
		throw DocumentTooLargeException(status_code, code, message);
	}
	else
	{
		throw DocumentDBResponseException(status_code, code, message);
	}
}
