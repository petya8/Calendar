#include"Date.h"
bool isValidMonth(unsigned number)
{
	return (number > 0 && number <= 12);
}

bool isValidYear(unsigned number) {
	return (number > 0);
}

bool Date::isLeapYear() const {
	if (year <= 1916)
		return year % 4 == 0;
	return ((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0);
}

void Date::ifNineteenSixteen() {
	{
		if (year == 1916 && month == 4 && day >= 1 && day <= 13)
			day = 15;
	}
}

void takeName(char* day, const char* name) {
	int size = strlen(name) + 1;
	for (int i = 0; i < size; i++) {
		day[i] = name[i];
	}
	day[size] = '\0';
} 

void Date::changeDaysInMonth()
{
	if (getMonth() == 2) {
		if (isLeapYear()) {
			daysInMonth = 29;
		}
		daysInMonth = 28;
	}
	if (month <= 7 && month != 2) {
		if (month % 2 == 0) {
			daysInMonth = 30;
		}
		daysInMonth = 31;
	}
	if (month > 7) {
		if (month % 2 == 0) {
			daysInMonth = 31;
		}
		daysInMonth = 30;
	}
}
Date::Date():year(1), month(1), day(1),daysInMonth(31) {}

void Date::copyFromOther(const Date& date)
{
	if (date.validDate()==1) {
		this->setYear(date.getYear());
		this->setMonth(date.getMonth());
		this->setDay(date.getDay());
		this->setTime(date.getTime());
	}
}

Date::Date(unsigned year, unsigned month, unsigned day, const Time& time)
{
	this->setYear(1); this->setMonth(1); this->setDay(day);
	std::cout << isValidDay(day) << ' ' << day << std::endl;
	if(isValidYear(year)&&isValidMonth(month)&&isValidDay(day)&&time.isValidTime()) {
		setMonth(month);
		setYear(year);
		setDay(day);
		setTime(time);
	}
}

bool Date::isValidDay(unsigned number) const
{
	if ( getMonth() == 2) {
		if (isLeapYear()) {
			return(number > 0 && number <= 29);
		}
		else {
			return (number > 0 && number <= 28);
		}
		
	}
	else {
		if (getMonth() >= 1 && getMonth() <= 7) {
			if (getMonth() % 2 == 0) {
				return (number > 0 && number <= 30);
			}
			else {
				return (number > 0 && number <= 31);
			}
		}
		else if (getMonth() > 7 && getMonth() <= 12) {
			if (getMonth() % 2 == 0) {
				return (number > 0 && number <= 31);
			}
			else {
				return (number > 0 && number <= 30);
			}
		}
		else {
			return false;
		}

	}
}

bool Date::validDate() const
{
	return (isValidYear(getYear()) && isValidMonth(getMonth()) && isValidDay(getDay()));
}

unsigned Date::getYear() const
{
	return year;
}

unsigned Date::getMonth() const
{
	return month;
}

unsigned Date::getDay() const
{
	return day;
}

unsigned Date::getDaysInMonth() const
{
	return daysInMonth;
}

Time& Date::getTime() const
{
	Time copy(this->time);
	return copy;
}

void Date::setDayThirtyOne(unsigned day)
{
	if (day > 0 && day <= 31) {
		changeDaysInMonth();
		if (day > getDaysInMonth()) {
			if (isLeapYear() && getMonth() == 2 && day > 29) {
				setMonth(3);
				this->day = day - 29;
			}
			else {
				if (getMonth() == 2 && day > 28) {
					setMonth(3);
					this->day = day - 28;
				}
				else if (getMonth() >= 1 && getMonth() <= 7 && getMonth() % 2 == 0 && day > 30) {
					setMonth(getMonth() + 1);
					this->day = day - 30;
				}
				else if (getMonth() > 7 && getMonth() <= 12 && getMonth() % 2 != 0 && day > 30) {
					setMonth(getMonth() + 1);
					this->day = day - 30;
				}
			}
			ifNineteenSixteen();
			changeDaysInMonth();
		}else{ 
			setDay(day);
		}
	}
}

void Date::setLater(unsigned day)
{
	if (day > 0 && day <= 31) {
		changeDaysInMonth();
		if (day > getDaysInMonth()) {
			this->day = getDaysInMonth();
			ifNineteenSixteen();
		}
		else {
			setDay(day);
		}
	}
}

void Date::setYear(unsigned year)
{
	if (isValidYear(year)) {
		this->year = year;
		setLater(getDay());
	}
}

void Date::setMonth(unsigned month)
{
	if (isValidMonth(month)) {
		this->month=month;
		setLater(getDay());
	}
}

void Date::setDay(unsigned day)
{
	if (isValidDay(day)) {
		this->day = day;
		ifNineteenSixteen();
		changeDaysInMonth();
	}
}

void Date::setTime(const Time& time)
{
	if (time.isValidTime() == 1) {
		this->time = time;
	}
}

bool Date::checkBusyPeriod(const Time& st, const Time& ed) const
{
	Time res(st);
	bool bsy = 0;
	while (bsy == 0 && res.compare(ed) != 0) {
		bsy = res.isTBusy();
		res.timePasses();
	}
	return bsy;
}

void Date::setIfBusyPeriod(const Time& st, const Time& ed, bool is)
{
	Time current = getTime();
	if (is == 0) {
		setTime(st);
		while (getTime().compare(ed)<=0) {
			time.setIfBusy(is);
			time.timePasses();
		}
		setTime(current);
	}
	else {
		if (checkBusyPeriod(st, ed) == 0) {
			setTime(st);
			while (getTime().compare(ed) <= 0) {
				time.setIfBusy(is);
				time.timePasses();
			}
			setTime(current);
		}
	}
}

Date& Date::timeLater(const Time& other) const
{
	Date res(*this);
	Time copy;	
	copy.timelater(other.getSeconds(),other.getMinutes(),other.getHours());
	while (copy.getSeconds() >= 60) {
		copy.timelater((copy.getSeconds() - 60), copy.getMinutes()+1, copy.getHours());
	}
	while (copy.getMinutes() >= 60) {
		copy.timelater(copy.getSeconds(), copy.getMinutes() - 60, copy.getHours() + 1);
	}
	while (copy.getHours() >= 24) {
		res=res.daysLater(1);
		copy.timelater(copy.getSeconds(), copy.getMinutes(), copy.getHours() - 24);
	}
	res.setTime(copy);
	return res;
}

Date& Date::daysLater(unsigned number) const
{
	Date copy(*this);
	copy.day = 1;
	unsigned dayToAdd= number + copy.getDay();
	if (copy.getDaysInMonth() < dayToAdd) {
		dayToAdd = dayToAdd - (copy.getDaysInMonth() - this->getDay())-1;
		copy=copy.monthsLater(1);
		copy.changeDaysInMonth();
		while (copy.getDaysInMonth() < dayToAdd) {
			copy=monthsLater(1);
			dayToAdd -= copy.getDaysInMonth();
			copy.changeDaysInMonth();
		}
	}
	dayToAdd += copy.getDay();
	copy.setDayThirtyOne(dayToAdd);
	return copy;
}

Date& Date::monthsLater(unsigned number) const
{
	Date copy(*this);
	unsigned initDay = getDay();
	unsigned initMonth = getMonth();
	unsigned monthsToAdd = getMonth()+number;
	if (copy.isLeapYear() && (initMonth < 2 || (initMonth == 2 && initDay < 29))) {
		initDay++;
	}
	copy.setDay(1);
	copy.setMonth(1);
	if (monthsToAdd >= 12) {
		monthsToAdd= monthsToAdd-(12-this->getMonth())-1;
		copy = copy.yearsLater(1);
		if (copy.isLeapYear()) {
			initDay++;
		}
		while (monthsToAdd>=12) {
			copy = copy.yearsLater(1);
			if (copy.isLeapYear()) {
				initDay++;
			}
			monthsToAdd -= 12;
		}
	}
	copy.setMonth(monthsToAdd);
	if (copy.getMonth() <= 2 && copy.isLeapYear()) {
		initDay--;
	}
	copy.setDayThirtyOne(initDay);
	return copy;
}

Date& Date::yearsLater(unsigned number) const
{
	Date copy(*this);
	unsigned initDay = getDay();
	unsigned initMonth = getMonth();
	if (copy.isLeapYear() && (initMonth < 2 || (initMonth == 2 && initDay < 29))) {
		initDay++;
	}
	copy.setDay(1);
	copy.setMonth(1);
	while (number != 0) {
		copy.setYear(getYear()+1);
		if (copy.isLeapYear()) {
			initDay++;
		}
		number--;
	}
	copy.setMonth(initMonth);
	if (copy.getMonth() <= 2 && copy.isLeapYear()) {
		initDay--;
	}
	copy.setDayThirtyOne(initDay);
	return copy;
}

void Date::operator=(const Date& date)
{
	if (date.validDate()) {
		this->setTime(date.getTime());
		this->setDay(date.getDay());
		this->setMonth(date.getMonth());
		this->setYear(date.getYear());
		this->changeDaysInMonth();
	}
}

int Date::compareD(const Date& other) const
{
	if (this->getYear() > other.getYear()) {
		return 1;
	}
	else if (this->getYear() < other.getYear()) {
		return -1;
	}
	else {
		if (this->getMonth() > other.getMonth()) {
			return 1;
		}
		else if (this->getMonth() < other.getMonth()) {
			return -1;
		}
		else {
			if (this->getDay() > other.getDay()) {
				return 1;
			}
			else if (this->getDay() < other.getDay()) {
				return -1;
			}
			else {
				return (this->time.compare(other.time));
			}
		}
	}
}

int Date::compareNoTime(const Date& other) const
{
	if (this->getYear() > other.getYear()) {
		return 1;
	}
	else if (this->getYear() < other.getYear()) {
		return -1;
	}
	else {
		if (this->getMonth() > other.getMonth()) {
			return 1;
		}
		else if (this->getMonth() < other.getMonth()) {
			return -1;
		}
		else {
			if (this->getDay() > other.getDay()) {
				return 1;
			}
			else if (this->getDay() < other.getDay()) {
				return -1;
			}
			else {
				return 0;
			}
		}
	}
}

const char* Date::dayOfWeek() const
{
	char* takeDay = new char[10];
	Date copy(1, 1, 1, getTime());
	int weekDay = 5;
	while (this->compareNoTime(copy) != 0) {
		copy = copy.daysLater(1);
		(weekDay += 1) %= 7;
	}
	switch (weekDay) {
	case 0: takeName(takeDay, "Sunday"); break;
	case 1: takeName(takeDay, "Monday"); break;
	case 2: takeName(takeDay, "Tuesday"); break;
	case 3:takeName(takeDay, "Wednesday"); break;
	case 4:takeName(takeDay, "Thirsday"); break;
	case 5:takeName(takeDay, "Friday");	break;
	case 6:takeName(takeDay, "Saturday"); break;
	default: takeName(takeDay, "Wrong!");
	}
	try {
		if (strcmp(takeDay, "Wrong")) {
			throw false;
		}
	}
	catch (bool) {
		throw "Wrong ";
	}
	return takeDay;
}

void Date::printD() const
{
	std::cout << getYear() << '.';
	std::cout << getMonth() <<'.';
	std::cout << getDay() ;
	std::cout<<std::endl;
	time.print();
}

void Date::printNoTime() const
{
	std::cout << getYear() << '.';
	std::cout << getMonth() << '.';
	std::cout << getDay() ;
	std::cout <<  std::endl;
}

void Date::createInvalidDate(unsigned year, unsigned month, unsigned day, const Time& time)
{
	this->year = year;
	this->month = month;
	this->day = day;
	this->time = time;
}





