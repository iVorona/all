#include<iostream>
#include "MyString.hpp"

// ============================================================================================================================================

void MyString::grow()
{
	char* pNewData = new char[capacity() * 2 + 1];
	long temp_length = length();
	long temp_capacity = capacity();

	memcpy(pNewData, m_DataStart, sizeof(char)*capacity()*2);
	pNewData[capacity() * 2] = NULL;
	if (IsMemAllocated()) delete[] m_DataStart;
	m_DataStart = pNewData;
	m_DataFinish = m_DataStart + temp_length;
	m_EndOfStorage = m_DataStart + temp_capacity * 2;
};

bool MyString::IsMemAllocated() const
{
	return(m_DataStart != m_StaticBuffer);
}

char* MyString::c_str() const
{
	if (IsMemAllocated()) return (m_DataStart);
	else
	{
		char* returnString = new char[length()];
		memcpy(returnString, m_StaticBuffer, sizeof(char)*length());
		returnString[length()] = NULL;
		return returnString;
	}

}

long MyString::capacity() const
{
	if (IsMemAllocated())
	{
		return(m_EndOfStorage - m_DataStart);
	}
	else
	{
		return(MAX_STATIC_SIZE);
	}
};

void MyString::clear() 
{
	m_DataFinish = m_DataStart;
}

void MyString::reserve(long _value)
{
	if (_value < 0) throw "Incorrect size";
	while (capacity() < _value) grow();
}

bool MyString::empty() const
{
	return (m_DataStart == m_DataFinish);
}

long MyString::length() const
{
	return(m_DataFinish - m_DataStart);
};

char* MyString::begin() const
{
	return(m_DataStart);
};

char* MyString::end() const
{
	return(m_DataFinish);
};

char MyString::operator[](long _value) const
{
	if (_value > length() - 1) throw std::logic_error("Out of range");
	return begin()[_value];
}

char & MyString::operator[](long _value)
{
	if (_value > length() - 1) throw std::logic_error("Out of range");
	return begin()[_value];
}

void MyString::insert(long pos, const char* data)
{
	if (pos<0 || pos >capacity())throw std::logic_error("Out of range");
	reserve(strlen(data) + length());

	memcpy(m_DataStart + pos + strlen(data), m_DataStart + pos, sizeof(char)*(length() - pos + 1));
	memcpy(m_DataStart + pos, data, sizeof(char)*strlen(data));
	m_DataFinish = m_DataFinish + strlen(data);
	m_DataStart[length()] = NULL;
}

void MyString::erase(long pos, long len)
{
	if (pos<0 || pos + len >capacity()) throw std::logic_error("Out of range");
	memcpy(m_DataStart + pos, m_DataStart + pos + len, sizeof(char)*(m_DataFinish - m_DataStart - pos - len));
	m_DataFinish = m_DataFinish - len;
	m_DataStart[length()] = NULL;
}

MyString MyString::substring(long pos, long len)
{
	if (len == -1) len = length();
	if (pos + len>length() || pos<0 || len<0) throw std::logic_error("Out of range");

	char* tempString = new char[pos + len + 1];
	tempString[pos + len] = NULL;
	memcpy(tempString, m_DataStart + pos, len);
	MyString returnString{ tempString };
	delete[] tempString;
	return  returnString;
}


// ============================================================================================================================================

MyString::MyString()
{
	m_EndOfStorage = NULL;
	m_DataFinish = m_StaticBuffer;
	m_DataStart = m_StaticBuffer;
}

MyString::~MyString()
{
	if (IsMemAllocated()) delete[] m_DataStart;
};

MyString::MyString(const char* _string)
{
	if (strlen(_string)<MAX_STATIC_SIZE)
	{
		for (long i = 0; unsigned(i) < strlen(_string); i++)
		{
			m_StaticBuffer[i] = _string[i];
		}
		m_DataStart = m_StaticBuffer;
		m_DataFinish = m_StaticBuffer + strlen(_string);
	}
	else
	{
		m_EndOfStorage = NULL;
		m_DataFinish = m_StaticBuffer;
		m_DataStart = m_StaticBuffer;
		reserve(strlen(_string));
		memcpy(m_DataStart, _string, strlen(_string));
		m_DataStart[strlen(_string)] = NULL;
		m_DataFinish = m_DataStart + strlen(_string);
	}
};

MyString::MyString(const long _value)
{
	if(_value < 0L) throw std::exception("Incorrect size");
	m_EndOfStorage = NULL;
	m_DataFinish = m_StaticBuffer;
	m_DataStart = m_StaticBuffer;
	reserve(_value);

};

MyString::MyString(const MyString& __string)
{
	if (__string.m_DataStart != __string.m_StaticBuffer)
	{
		m_DataStart = new char[__string.capacity()];
		m_DataFinish = m_DataStart + __string.length();
		m_EndOfStorage = m_DataStart + __string.capacity();
		memcpy(m_DataStart, __string.m_DataStart, sizeof(char)*(__string.capacity()));
		m_DataStart[length()] = NULL;
	}
	else
	{
		for (long i = 0; i < __string.length(); i++) m_StaticBuffer[i] = __string.m_StaticBuffer[i];
		m_DataFinish = m_StaticBuffer + __string.length(); 
		m_DataStart = m_StaticBuffer;
	}

}

MyString::MyString(MyString && _string)
	:
	m_DataStart(_string.m_DataStart),
	m_EndOfStorage(_string.m_EndOfStorage),
	m_DataFinish(_string.m_DataFinish)
{

	if (!_string.IsMemAllocated())
	{
		memcpy(m_StaticBuffer, _string.m_StaticBuffer, strlen(_string.m_StaticBuffer) + 1);
		m_DataFinish = m_StaticBuffer + (m_DataFinish - m_DataStart);
		m_DataStart = m_StaticBuffer;
	}
	_string.m_EndOfStorage = _string.m_DataStart = _string.m_DataFinish = nullptr;
}
// =======================================================================ÎÏÅÐÀÒÎÐÛ==============================================================================

bool MyString::operator == (const MyString & _string)
{
	if (length() != _string.length()) return(false);
	for (long i = 0; i < length(); i++)
		if ((*this)[i] != _string[i]) return(false);
	return(true);
};

bool MyString::operator != (const MyString & _string)
{
	return(!(*this == _string));
};


MyString & MyString::operator = (const MyString & _string)
{
	if (&_string == this) {
		return *this;
	}
	reserve(_string.length());
	memcpy(m_DataStart, _string.m_DataStart, _string.length() + 1);
	m_DataFinish = m_DataStart +_string.length();
	return *this;
}

MyString & MyString::operator  = (MyString && _string)
{
	if (&_string == this) {
		return *this;
	}

	if (_string.IsMemAllocated() && IsMemAllocated())
	{
		std::swap(m_DataStart, _string.m_DataStart);
		std::swap(m_DataFinish, _string.m_DataFinish);
		std::swap(m_EndOfStorage, _string.m_EndOfStorage);
	}
	else
	if (!_string.IsMemAllocated() && !IsMemAllocated())
	{
		std::swap(m_StaticBuffer, _string.m_StaticBuffer);
		m_DataFinish = m_DataStart + strlen(m_DataStart);
		_string.m_DataFinish = _string.m_DataStart + strlen(_string.m_DataStart);
	}
	else
	{
		_string.reserve(length());
		reserve(_string.length());
		std::swap(m_DataStart, _string.m_DataStart);
		std::swap(m_DataFinish, _string.m_DataFinish);
		std::swap(m_EndOfStorage, _string.m_EndOfStorage);
	}
	return *this;
}

MyString MyString::operator += (const MyString& _string)
{
	reserve(length() + _string.length());
	memcpy(m_DataFinish, _string.m_DataStart, _string.length());
	m_DataFinish = m_DataFinish + _string.length();
	m_DataStart[length()] = NULL;
	return *this;
}

MyString MyString::operator+(const MyString _string) const 
{
	MyString resultString = *this;
	resultString += _string;
	return resultString;
}

MyString operator"" _s(const char * _string, std::size_t _size)
{
	return MyString(_string);
}