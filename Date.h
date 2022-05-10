#pragma once
#include"Time.h"
#include<cstring>
#pragma warning(disable:4996)
class Date {
	unsigned year;
	unsigned month;
	unsigned day;
	Time time;
	bool isLeapYear() const {
		if (year <= 1916)
			return year % 4 == 0;
		return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
	}
	void ifNineteenSixteen() {
		{
			if (year == 1916 && month == 4 && day >= 1 && day <= 13)
				day = 15;
		}
	}
	unsigned daysInMonth;
public:
	void changeDaysInMonth();
	Date();
	Date(const Date& date);
	Date(unsigned year, unsigned month, unsigned day, const Time& time);
	bool isValidDay(unsigned number)const;
	bool validDate()const;
	unsigned getYear()const;
	unsigned getMonth()const;
	unsigned getDay()const;
	unsigned getDaysInMonth()const;
	Time& getTime()const;
	void setDayThirtyOne(unsigned);
	void setLater(unsigned);
	void setYear(unsigned);
	void setMonth(unsigned);
	void setDay(unsigned);
	void setTime(const Time& time);
	bool checkBusyPeriod(const Time& st, const Time& ed)const;
	void setIfBusyPeriod(const Time& st, const Time& ed, bool is);
	Date& timeLater(const Time& time)const;
	Date& daysLater(unsigned number)const;
	Date& monthsLater(unsigned number)const;
	Date& yearsLater(unsigned number)const;
	void operator=(const Date& date);
	int compare(const Date& other)const;
	int compareNoTime(const Date& other) const;
	const char* dayOfWeek()const;
	void print()const;
	void printNoTime()const;
};
bool isValidMonth(unsigned number)
{
	return (number > 0 && number <= 12);
}

bool isValidYear(unsigned number) {
	return (number > 0);
}

void takeName(char* day, const char* name) {
	int size = strlen(name) + 1;
	for (int i = 0; i < size; i++) {
		day[i] = name[i];
	}
	day[size] = '\0';
}
