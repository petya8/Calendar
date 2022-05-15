#include"String.h"
#pragma warning(disable:4996)
#include<cstring>
#include<exception>
using namespace std;
void String::free()
{
	/// wanted to write
	//delete[] str;
	/// but it throws an exception which i could not handle
	
	str[0] = '\0';
}

void String::copy(const String& other)
{
	str = new char[strlen(other.str) + 1];
	strcpy(str, other.str);
	size = other.size;
}

void String::deleteStr()
{
	free();
}

String::String()
{
	str = new char[1];
	str[0] = '\0';
	size = 0;
}

String::String(const char* string)
{
	if (string == nullptr)
	{
		str = new char[1];
		str[0] = '\0';
		size = 0;
	}
	else
	{
		size = strlen(string);
		str = new char[size + 1];
		strcpy(str, string);
	}
}

String::String(const String&other)
{
	copy(other);
}

String& String::operator=(const String& other)
{
	if (this != &other)
	{
		deleteStr();
		copy(other);
	}
	return *this;
}

char String::operator[](size_t index) const
{
	return str[index];
}

size_t String::getSize() const
{
	return size;
}

void String::concat(const String& other)
{
	char* temp = new char[getSize() + other.getSize() + 1];
	strcpy(temp, str);
	strcat(temp, other.str);

	delete[] str;
	str = temp;
	size = size + other.getSize();
}

bool String::hasSameString(const String nam) const
{
	size_t size = nam.getSize();
	size_t mysize = getSize();
	if (checkRow(nam) == 0) {
		return 0;
	}
	if (this == &nam) {
		return 1;
	}
	else if (size == mysize) {
		for (size_t i = 0; i <= size; i++) {
			if (str[i] != nam[i]) {
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

bool String::compareStrings(const String nam) const
{
	size_t size = nam.getSize();
	size_t mysize = getSize();
	if (checkRow(nam) == 0) {
		return 0;
	}
	if (hasSameString(nam)) {
		return 1;
	}
	else if (size < mysize) {
		size_t i = 0;
		while (i <= mysize) {
			if (str[i] == nam[0]) {
				size_t j = i;
				bool is = 1;
				while (is == 1 && j - i <= size) {
					if (str[j] != nam[j - i]) {
						is = 0;
					}
					else {
						j++;
					}
				}
				if (is == 1) {
					return 1;
				}
				else {
					i++;
				}
			}
			else {
				i++;
			}
		}
	}
	return 0;
}

const char* String::c_str() const
{
	return str;
}

String::~String()
{
	deleteStr();
}
