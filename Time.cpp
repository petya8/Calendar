#include"Time.h"

Time::Time():hours(0),minutes(0),seconds(0),busy(0),changeDay(0){}

bool Time::isValidTime() const
{
	return (isValidHour(hours) && areValidSecMin(seconds) && areValidSecMin(minutes));
}

bool Time::isTBusy() const
{
	return busy;
}

Time::Time(const Time& other)
{
	if (other.isValidTime()==1) {
		this->setHours(other.getHours());
		this->setMinutes(other.getMinutes());
		this->setSeconds(other.getSeconds());
		this->setIfBusy(other.isTBusy());
	}
}

Time::Time(size_t seconds, size_t minutes, size_t hours, bool busy)
{
	if ((isValidHour(hours) && areValidSecMin(seconds) && areValidSecMin(minutes)) == 1) {
		this->setSeconds(seconds);
		this->setMinutes(minutes);
		this->setHours(hours);
		this->setIfBusy(busy);
	}
}

size_t Time::getSeconds() const
{
	return seconds;
}

size_t Time::getMinutes() const
{
	return minutes;
}

size_t Time::getHours() const
{
	return hours;
}

void Time::setIfBusy(bool is)
{
	busy = is;
}

void Time::setSeconds(size_t seconds)
{
	if (areValidSecMin(seconds) == 1) {
		if (seconds == 60) {
			this->seconds = 0;
			setMinutes(getMinutes() + 1);
		}
		else {
			this->seconds = seconds;
		}
	}
	checkTime();
}

void Time::setMinutes(size_t minutes)
{
	if (areValidSecMin(minutes) == 1) {
		if (minutes == 60) {
			this->minutes = 0;
			setHours(getHours() + 1);
		}
		else {
			this->minutes = minutes;
		}
	}
	checkTime();
}

void Time::setHours(size_t hours)
{
	if (isValidHour(hours) == 1) {
		if (hours == 24) {
			this->hours = 0;
			changeDay = 1;
		}
		else {
			this->hours = hours;
		}
	}
	checkTime();
}

void Time::operator=(const Time& other)
{
	if (other.isValidTime() == 1) {
		this->setHours(other.getHours());
		this->setMinutes(other.getMinutes());
		this->setSeconds(other.getSeconds());
		this->setIfBusy(other.isTBusy());
	}
}

int Time::compare(const Time& other) const
{
	if (other.isValidTime() == 1) {
		if (other.isValidTime() == 1) {
			if (this->getHours() > other.getHours()) {
				return 1;
			}
			else if (this->getHours() < other.getHours()) {
				return -1;
			}
			else {
				if (this->getMinutes() > other.getMinutes()) {
					return 1;
				}
				else if (this->getMinutes() < other.getMinutes()) {
					return -1;
				}
				else {
					if (this->getSeconds() > other.getSeconds()) {
						return 1;
					}
					else if (this->getSeconds() < other.getSeconds()) {
						return -1;
					}
					else {
						return 0;
					}
				}
			}
		}
	}
	else {
		return -3;
	}
}

void Time::checkTime()
{
	size_t min = getMinutes();
	if (getSeconds() == 60) {
		setSeconds(1);
		min++;
	}
	if (min >= 60) {
		setMinutes(min-60);
		if (min == 60) {
			setMinutes(0);
		}
		setHours(getHours() + 1);
	}
	if (hours == 24) {
		setHours(0);
		changeDay = 1;
	}
}

Time& Time::difference(const Time& other) const
{ 
	Time res;
	int sec = 0;
	int min = 0;
	int hour = 0;
	if (other.isValidTime() == 1) {
		if (this->compare(other) >= 0) {
			sec = this->getSeconds() - other.getSeconds();
			min = this->getMinutes() - other.getMinutes();
			hour = this->getHours() - other.getHours();
			if (sec < 0) {
				sec = abs(sec);
				min--;
			}
			if (min < 0) {
				min = abs(min);
				hour--;
			}
		}
		else {
			sec = other.getSeconds()- this->getSeconds();
			min = other.getMinutes()- this->getMinutes();
			hour = other.getHours()- this->getHours();
			if (sec < 0) {
				sec = abs(sec);
				min--;
			}
			if (min < 0) {
				min = abs(min);
				hour--;
			}
		}
	}
	else {
		res.timelater(-1, -1, -1);
	}
	return res;
}

Time& Time::sum(const Time& other) const
{
	Time res;
	if (other.isValidTime() == 1) {
		size_t sec = this->getSeconds() + other.getSeconds();
		size_t min = this->getMinutes() + other.getMinutes();
		size_t hour = this->getHours() + other.getHours();
		if (sec > 60) {
			min++;
			sec = sec - 60;
		}
		if (min > 60) {
			hour++;
			min = min - 60;
		}
		if (hour > 24) {
			res.changeDay = 1;
			hour = hour - 24;
		}
		res.setHours(hour);
		res.setMinutes(min);
		res.setSeconds(sec);
	}
	else {
		res.timelater(-1, -1, -1);
	}
	return res;
}

void Time::timePasses()
{
	Time sec(1, 0, 0, 0);
	*this = this->sum(sec);
}

void Time::print() const
{
	std::cout << getHours() << ':' << getMinutes() << ':' << getSeconds();
	std::cout << std::endl;
}

void Time::timelater(size_t sec, size_t min, size_t hour)
{
	this->seconds = sec;
	this->minutes = min;
	this->hours = hour;
	this->busy = 0;
	this->changeDay = 0;
}

