#pragma once
class String
{
	char* str;
	size_t size;
	void free();
	void copy(const String& other);
public:
	void deleteStr();
	String();
	String(const char*);
	String(const String&);
	char operator[](size_t index)const;
	size_t getSize() const;
	void concat(const String& other);
	bool hasSameString(const String nam) const;
	bool compareStrings(const String nam) const;
	const char* c_str() const;
	String& operator=(const String& other);
	~String();
};
static bool isValidSymbol(char ch) {
	return ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == ',') || (ch == '.') || (ch == '!') || (ch == ';') || (ch == ':') || (ch == '-') || (ch == '(') || (ch == ')') || (ch == '"') || (ch == ' ') || (ch == '\n') || (ch == '\0'));
}
static bool checkRow(const String row) {
	size_t size = row.getSize();
	for (size_t i = 0; i < size; i++) {
		if (isValidSymbol(row[i]) == 0) {
			return 0;
		}
	}
	return 1;
}
