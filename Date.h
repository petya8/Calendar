#pragma once
#include<cstring>
#include"Time.h"
#pragma warning(disable:4996)
class Date {
	unsigned year;
	unsigned month;
	unsigned day;
	Time time;
	bool isLeapYear() const; 
	void ifNineteenSixteen(); 
	unsigned daysInMonth;
public:
	void changeDaysInMonth();
	Date();
	void copyFromOther(const Date& date);
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
	int compareD(const Date& other)const;
	int compareNoTime(const Date& other) const;
	const char* dayOfWeek()const;
	void printD()const;
	void printNoTime()const;
	void createInvalidDate(unsigned year, unsigned month, unsigned day, const Time& time);
};

