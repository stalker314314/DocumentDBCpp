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

#include <string>
#include <cpprest/json.h>

#include "exceptions.h"
#include "TriggerOperation.h"
#include "TriggerType.h"

using namespace std;
namespace documentdb
{

	bool comparei(
		wstring string1,
		wstring string2)
	{
		transform(string1.begin(), string1.end(), string1.begin(), toupper);
		transform(string2.begin(), string2.end(), string2.begin(), toupper);

		return (string1 == string2);
	}

	wstring triggerOperationToWstring(const TriggerOperation& trigger_operation)
	{
		switch (trigger_operation) {
		case TriggerOperation::ALL:
			return L"ALL";
		case TriggerOperation::CREATE:
			return L"CREATE";
		case TriggerOperation::UPDATE:
			return L"UPDATE";
		case TriggerOperation::REPLACE:
			return L"REPLACE";
		case TriggerOperation::DEL:
			return L"DELETE";
		default:
			throw DocumentDBRuntimeException(L"Unsupported trigger operation.");
		}
	}

	TriggerOperation wstringToTriggerOperation(const std::wstring& trigger_operation_str)
	{
		TriggerOperation triggerOperation;
		if (comparei(trigger_operation_str, L"ALL"))
		{
			triggerOperation = TriggerOperation::ALL;
		}
		else if (comparei(trigger_operation_str, L"UPDATE"))
		{
			triggerOperation = TriggerOperation::UPDATE;
		}
		else if (comparei(trigger_operation_str, L"CREATE"))
		{
			triggerOperation = TriggerOperation::CREATE;
		}
		else if (comparei(trigger_operation_str, L"REPLACE"))
		{
			triggerOperation = TriggerOperation::REPLACE;
		}
		else if (comparei(trigger_operation_str, L"DELETE"))
		{
			triggerOperation = TriggerOperation::DEL;
		}
		else
		{
			throw DocumentDBRuntimeException(L"Unsupported trigger operation: " + trigger_operation_str);
		}
		return triggerOperation;
	}

	wstring triggerTypeToWstring(const TriggerType& trigger_type)
	{
		switch (trigger_type) {
		case TriggerType::PRE:
			return L"Pre";
		case TriggerType::POST:
			return L"POST";
		default:
			throw DocumentDBRuntimeException(L"Unsupported trigger type.");
		}
	}

	TriggerType wstringToTriggerType(const wstring& trigger_type_str)
	{
		TriggerType triggerType;
		if (comparei(trigger_type_str, L"PRE"))
		{
			triggerType = TriggerType::PRE;
		}
		else if (comparei(trigger_type_str, L"POST"))
		{
			triggerType = TriggerType::POST;
		}
		else
		{
			throw DocumentDBRuntimeException(L"Unsupported trigger type: " + trigger_type_str);
		}
		return triggerType;
	}
}
