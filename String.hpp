#ifndef _MYSTRING_HPP
#define _MYSTRING_HPP
#include <cstring>
#include <iostream>
class MyString
{
	static const long MAX_STATIC_SIZE = 16;
	union
	{
		char m_StaticBuffer[MAX_STATIC_SIZE];
		char* m_EndOfStorage;
	}; 
	char* m_DataStart;
	char* m_DataFinish;
public:

	void grow();
	bool IsMemAllocated() const;

	char* begin() const;
	char* end() const;
	char* c_str() const;
	void clear();
	bool empty() const;
	long length() const;
	long capacity() const;

	void reserve(long _value);
	void insert(long pos, const char* data);
	void erase(long pos, long len);
	MyString substring(long pos, long len);

	MyString();
	~MyString();

	MyString(const char* _string);
	MyString(const long _value);
	MyString(const MyString& _string);
	MyString(MyString && _string);

	bool operator == (const MyString & _string);
	bool operator != (const MyString & _string);
	MyString & operator = (const  MyString & _string);
	MyString & operator = (MyString && _string);
	MyString operator += (const MyString& _string);
	MyString MyString::operator+(const MyString _string) const;
	char operator[](long _value) const;
	char& operator[](long _index);

};


#endif //_MYSTRING_HPP