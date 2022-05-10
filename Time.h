#pragma once
#include<iostream>

class Time {
	size_t seconds;
	size_t minutes;
	size_t hours;
	bool busy;
	bool changeDay;
public:
	Time();
	bool isValidTime()const;
	bool isTBusy()const;
	Time(const Time& other);
	Time(size_t seconds, size_t minutes, size_t hours, bool busy);
	size_t getSeconds()const;
	size_t getMinutes()const;
	size_t getHours()const;
	void setIfBusy(bool is);
	void setSeconds(size_t seconds);
	void setMinutes(size_t minutes);
	void setHours(size_t hours);
	void operator =(const Time& other);
	int compare(const Time&time)const;
	void checkTime();
	Time& difference(const Time& other)const;
	Time& sum(const Time& other)const;
	void timePasses();
	void print()const;
	void timelater(size_t sec,size_t min,size_t hour);
};
bool areValidSecMin(size_t number) {
	return (number >= 0 && number <= 60);
}
bool isValidHour(size_t number) {
	return (number >= 0 && number <= 24);
}