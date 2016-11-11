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
#include <cctype>
#include <ctype.h>
#include <cpprest/json.h>

#include "exceptions.h"
#include "TriggerOperation.h"
#include "TriggerType.h"

using namespace std;
using namespace utility;
namespace documentdb
{

	bool comparei(
		string_t string1,
		string_t string2)
	{
		transform(string1.begin(), string1.end(), string1.begin(), ::toupper);
		transform(string2.begin(), string2.end(), string2.begin(), ::toupper);

		return (string1 == string2);
	}

	string_t triggerOperationToWstring(const TriggerOperation& trigger_operation)
	{
		switch (trigger_operation) {
		case TriggerOperation::ALL:
			return _XPLATSTR("ALL");
		case TriggerOperation::CREATE:
			return _XPLATSTR("CREATE");
		case TriggerOperation::UPDATE:
			return _XPLATSTR("UPDATE");
		case TriggerOperation::REPLACE:
			return _XPLATSTR("REPLACE");
		case TriggerOperation::DEL:
			return _XPLATSTR("DELETE");
		default:
			throw DocumentDBRuntimeException(_XPLATSTR("Unsupported trigger operation."));
		}
	}

	TriggerOperation wstringToTriggerOperation(const string_t& trigger_operation_str)
	{
		TriggerOperation triggerOperation;
		if (comparei(trigger_operation_str, _XPLATSTR("ALL")))
		{
			triggerOperation = TriggerOperation::ALL;
		}
		else if (comparei(trigger_operation_str, _XPLATSTR("UPDATE")))
		{
			triggerOperation = TriggerOperation::UPDATE;
		}
		else if (comparei(trigger_operation_str, _XPLATSTR("CREATE")))
		{
			triggerOperation = TriggerOperation::CREATE;
		}
		else if (comparei(trigger_operation_str, _XPLATSTR("REPLACE")))
		{
			triggerOperation = TriggerOperation::REPLACE;
		}
		else if (comparei(trigger_operation_str, _XPLATSTR("DELETE")))
		{
			triggerOperation = TriggerOperation::DEL;
		}
		else
		{
			throw DocumentDBRuntimeException(_XPLATSTR("Unsupported trigger operation: ") + trigger_operation_str);
		}
		return triggerOperation;
	}

	string_t triggerTypeToWstring(const TriggerType& trigger_type)
	{
		switch (trigger_type) {
		case TriggerType::PRE:
			return _XPLATSTR("Pre");
		case TriggerType::POST:
			return _XPLATSTR("POST");
		default:
			throw DocumentDBRuntimeException(_XPLATSTR("Unsupported trigger type."));
		}
	}

	TriggerType wstringToTriggerType(const string_t& trigger_type_str)
	{
		TriggerType triggerType;
		if (comparei(trigger_type_str, _XPLATSTR("PRE")))
		{
			triggerType = TriggerType::PRE;
		}
		else if (comparei(trigger_type_str, _XPLATSTR("POST")))
		{
			triggerType = TriggerType::POST;
		}
		else
		{
			throw DocumentDBRuntimeException(_XPLATSTR("Unsupported trigger type: ") + trigger_type_str);
		}
		return triggerType;
	}
}
