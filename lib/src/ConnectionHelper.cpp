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

#include <windows.h>
#include <Wininet.h>
#include <locale>
#include <codecvt>

#include <cpprest/json.h>

#include "hmac_bcrypt.h"
#include "DocumentDBConstants.h"

using namespace documentdb;
using namespace std;
using namespace web::http;
using namespace web::http::client;
using namespace web::json;

wstring GetCurrentRequestTime ()
{
	SYSTEMTIME st;
	GetSystemTime (&st);
	char bufferTime[1000];
	InternetTimeFromSystemTime (&st, INTERNET_RFC1123_FORMAT, bufferTime, 1000);
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes (bufferTime);
}

http_request CreateRequest (
	const method& method,
	const wstring& resource_type,
	const wstring& resource_id,
	const vector<unsigned char>& master_key)
{
	wstring requestTime = GetCurrentRequestTime ();

	wstring text = method + L"\n" + resource_type + L"\n" + resource_id + L"\n" + requestTime + L"\n\n";
	wstring textLowerCase (text);
	transform (text.begin (), text.end (), textLowerCase.begin (), towlower);

	BYTE buffer[1000];
	wcstombs_s (nullptr, (char*)buffer, 1000, textLowerCase.c_str (), textLowerCase.length ());
	DWORD hashSize = 0;
	BYTE* result = hmac ((BYTE*)&master_key[0], master_key.size (), buffer, strlen ((const char*)buffer), &hashSize);

	utility::string_t signature = utility::conversions::to_base64 (vector<BYTE> (result, result + hashSize));
	HeapFree (GetProcessHeap (), 0, result);

	http_request request (method);
	request.headers ().add (web::http::header_names::accept, MIME_TYPE_APPLICATION_JSON);
	request.headers ().add (web::http::header_names::user_agent, L"cpprestsdk/2.4.0.1");
	//request.headers ().add (web::http::header_names::cache_control, L"no-cache");
	request.headers ().add (
		web::http::header_names::authorization,
		uri::encode_data_string (L"type=master&ver=1.0&sig=" + signature));
	request.headers ().add (HEADER_MS_DATE, requestTime);
	request.headers ().add (HEADER_MS_VERSION, L"2014-08-21");

	return request;
}

http_request CreateQueryRequest (
	const wstring& query,
	const int page_size,
	const wstring& resource_type,
	const wstring& resource_id,
	const vector<unsigned char>& master_key,
	const wstring& continuation_id)
{
	http_request request = CreateRequest (methods::POST, resource_type, resource_id, master_key);
	request.headers ().add (web::http::header_names::content_type, MIME_TYPE_APPLICATION_SQL);
	request.headers ().add (HEADER_MS_DOCUMENTDB_IS_QUERY, true);
	request.headers ().add (HEADER_MS_MAX_ITEM_COUNT, page_size);

	if (!continuation_id.empty ())
	{
		request.headers ().add (HEADER_MS_CONTINUATION, continuation_id);
	}

	request.set_body (query);

	return request;
}

__declspec(noreturn)
void ThrowExceptionFromResponse (
	const status_code& status_code,
	const value& json_response)
{
	wstring message;
	if (json_response.has_field (RESPONSE_ERROR_MESSAGE))
	{
		message = json_response.at (RESPONSE_ERROR_MESSAGE).as_string ();
	}
	wstring code;
	if (json_response.has_field (RESPONSE_ERROR_CODE))
	{
		code = json_response.at (RESPONSE_ERROR_CODE).as_string ();
	}

	if (status_code == status_codes::NotFound)
	{
		throw ResourceNotFoundException (status_code, code, message);
	}
	else if (status_code == status_codes::Conflict)
	{
		throw ResourceAlreadyExistsException (status_code, code, message);
	}
	else if (status_code == status_codes::RequestEntityTooLarge)
	{
		throw DocumentTooLargeException (status_code, code, message);
	}
	else
	{
		throw DocumentDBResponseException (status_code, code, message);
	}
}