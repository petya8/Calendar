#include"Appointment.h"
void Appointment:: fr() {
	date.setIfBusyPeriod(getStartTime(), getEndTime(), 0);
	if (name.c_str() != nullptr) {
		name.deleteStr();
	}
	size_t size = comments.getSize();
	if (comments.c_str() != nullptr) {
		comments.deleteStr();
	}
	
}
void Appointment:: copyFrom(const Appointment& other) {
	{
		if (checkRow(other.getComments()) && checkRow(other.getName()) && other.getDate().validDate() && other.getStartTime().isValidTime() && other.getEndTime().isValidTime()) {
			this->setComments(other.getComments());
			this->setName(other.getName());
			this->setDate(other.getDate());
			this->setStartTime(other.getStartTime());
			this->setEndTime(other.getEndTime());
			this->date.setIfBusyPeriod(this->getStartTime(), this->getEndTime(), 1);
		}
	}
}

Appointment::Appointment() {}

String Appointment::getComments() const
{
	return comments.c_str();
}

String Appointment::getName() const
{
	return name.c_str();
}

Time& Appointment::getStartTime() const
{
	return startTime;
}

Time& Appointment::getEndTime() const
{
	return endTime;
}

Date& Appointment::getDate() const
{
	return date;
}

Time& Appointment::getDuration() const
{
	return (getStartTime().difference(getEndTime()));
}

void Appointment::setDate(const Date& date)
{
	this->date = date;
}

void Appointment::setName(const String name)
{
	if (checkRow(name)) {
		this->name = name;
	}
}

void Appointment::setComments(const String com)
{
	if (checkRow(com)) {
		this->comments = com;
	}
}

void Appointment::setAppointment(const String name,const String comments, const Date& newDate, const Time& beginning, const Time& ending)
{
	if (checkRow(comments) && checkRow(name) && newDate.validDate() && beginning.isValidTime() && ending.isValidTime()) {
		this->setName(name);
		this->setComments(comments);
		this->setDate(newDate);
		this->setStartTime(beginning);
		this->setEndTime(ending);
		this->date.setIfBusyPeriod(this->getStartTime(), this->getEndTime(), 1);
	}
}

void Appointment::setStartTime(const Time& newTime)
{
	startTime = newTime;
}

void Appointment::setEndTime(const Time& newTime)
{
	endTime = newTime;
}

void Appointment::operator=(const Appointment& other)
{
	if (checkRow(other.getComments()) && checkRow(other.getName()) && other.getDate().validDate() && other.getStartTime().isValidTime() && other.getEndTime().isValidTime()) {
		if (this != &other) {
			this->setComments(other.getComments());
			this->setName(other.getName());
			this->setDate(other.getDate());
			this->setStartTime(other.getStartTime());
			this->setEndTime(other.getEndTime());
			this->date.setIfBusyPeriod(this->getStartTime(), this->getEndTime(), 1);
		}
	}
}

bool Appointment::compareNames(const String toCompare) const
{
	return name.compareStrings(toCompare);
}

bool Appointment::compareComments(const String toCompare) const
{
	return comments.compareStrings(toCompare);
}

bool Appointment::hasSameName(const String toCompare) const
{
	return comments.hasSameString(toCompare);
}

bool Appointment::hasSameComment(const String toCompare) const
{
	return (name.hasSameString(toCompare));
}

bool Appointment::compareAppointment(const Appointment& app)const {
	if (this->hasSameName(app.getName()) == 0) {
		return 0;
	}
	if (this->hasSameComment(app.getComments()) != 1) {
		return 0;
	}
	if (this->date.compareNoTime(app.getDate()) != 0) {
		return 0;
	}
	if (this->getStartTime().compare(app.getStartTime()) != 0) {
		return 0;
	}
	if (this->getEndTime().compare(app.getEndTime()) != 0) {
		return 0;
	}
	return 1;
}

void Appointment::printName() const
{
	size_t size = name.getSize();
	for (size_t i = 0; i < size+1; i++) {
		std::cout << name[i];
	}
	std::cout << std::endl;
}

void Appointment::printComments() const
{
	size_t sizeText = comments.getSize();
	for (size_t i = 0; i < sizeText+1; i++) {
		for (size_t j = 0; j <= sizeText; j++) {
			std::cout << comments[i];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void Appointment::printAppointment() const
{
	printName();
	getDate().printNoTime();
	getStartTime().print();
	getEndTime().print();
	printComments();
}

void Appointment::deleteAppontment()
{
	fr();
}

Appointment::~Appointment()
{
	fr();
}
