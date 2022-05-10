#pragma once
#include"Date.h"
const int MAXNAME = 30;
const int MAXCOM = 100;
size_t takelenghtOfText(char** text) {////////DA SE TESTVA
	size_t rows = 0, j = 0;
	while (text[rows][j] != '\0') {
		while (text[rows][j] != '\n' || text[rows][j] != '\0') {
			j++;
		}
		if (text[rows][j] != '\0') {
			j = 0;
			rows++;
		}
	}
	return rows;
}
size_t takeRowLenght(char* row) {
	size_t counter = 0;
	while (row[counter] != '\0' && row[counter] != '\n') {
		counter++;
	}
	return counter;
}
bool isValidSymbol(char ch) {
	return ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch == ',') || (ch == '.') || (ch == '!') || (ch == ';') || (ch == ':') || (ch == '-') || (ch == '(') || (ch == ')') || (ch == '"') || (ch == ' ') || (ch == '\n') || (ch == '\0'));
}
bool checkRow(char* row) {
	size_t size = takeRowLenght(row);
	for (size_t i = 0; i < size; i++) {
		if (isValidSymbol(row[i]) == 0) {
			return 0;
		}
	}
	return 1;
}
bool checkComm(char** com) {
	size_t size = takelenghtOfText(com);
	for (size_t i = 0; i <= size; i++) {
		size_t takesize = takeRowLenght(com[i]);
		for (size_t j = 0; j < takesize; j++) {
			if (isValidSymbol(com[i][j]) == 0) {
				return 0;
			}
		}
	}
	return 1;
}
struct Appointment {
	mutable Date date;
	char* name = new char[MAXNAME];
	char** comments = new char* [MAXCOM];
	mutable Time startTime;
	mutable Time endTime;
	void free() {
		date.setIfBusyPeriod(getStartTime(), getEndTime(), 0);
		delete[]name;
		size_t size = takelenghtOfText(comments);
		for (size_t i = 0; i < size; i++) {
			delete[] comments[i];
		}
		delete[] comments;
		name = nullptr;
		comments = nullptr;
	}
	void copyFrom(const Appointment& other) {
		{
			if (checkComm(other.getComments()) && checkRow(other.getName()) && other.getDate().validDate() && other.getStartTime().isValidTime() && other.getEndTime().isValidTime()) {
				this->setComments(other.getComments());
				this->setName(other.getName());
				this->setDate(other.getDate());
				this->setStartTime(other.getStartTime());
				this->setEndTime(other.getEndTime());
				this->date.setIfBusyPeriod(this->getStartTime(), this->getEndTime(), 1);
			}
		}
	}
	Appointment();
	char** getComments()const;
	char* getName()const;
	Time& getStartTime()const;
	Time& getEndTime()const;
	Date& getDate()const;
	Time& getDuration()const;
	void setDate(const Date& date);
	void setName(char* name);
	void setComments(char** com);
	void setAppointment(char* name, char** comments, const Date& newDate, const Time& beginning, const Time& ending);
	void setStartTime(const Time& newTime);
	void setEndTime(const Time& newTime);
	void operator=(const Appointment& other);
	bool hasSameName(char* nam) const;
	bool compareNames(char* nam) const;
	bool hasSameComment(char** com) const;
	bool compareComments(char** com) const;
	bool hasCommentInComment(char* com) const;
	bool compareAppointment(const Appointment& app)const;
	void printName()const;
	void printComments()const;
	void printAppointment()const;
	void deleteAppontment();
	~Appointment();
};

Appointment::Appointment()
{
	name = nullptr;
	comments = nullptr;
}

char** Appointment::getComments() const
{
	return comments;
}

char* Appointment::getName() const
{
	return name;
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

void Appointment::setName(char* name)
{
	if (checkRow(name)) {
		if (&this->name != &name) {
			delete[] this->name;
			size_t size = takeRowLenght(name);
			this->name = new char[size + 1];
			for (size_t i = 0; i <= size; i++) {
				this->name[i] = name[i];
			}
			this->name[size + 1] = '\0';
		}
	}
}

void Appointment::setComments(char** com)
{
	if (checkComm(com)) {
		if (&this->comments != &com) {
			size_t newSize = takelenghtOfText(com);
			size_t oldSize = takelenghtOfText(this->getComments());
			for (size_t i = 0; i < oldSize; i++) {
				delete[] this->comments[i];
			}
			delete[]this->comments;
			this->comments = new char* [newSize];
			for (size_t i = 0; i <= newSize; i++) {
				size_t size = takeRowLenght(com[i]) + 1;
				this->comments[i] = new char[size];
				for (size_t j = 0; j <= size; j++) {
					this->comments[i][j] = com[i][j];
				}
			}
		}
	}
}

void Appointment::setAppointment(char* name, char** comments, const Date& newDate, const Time& beginning, const Time& ending)
{
	if (checkComm(comments) && checkRow(name) && newDate.validDate() && beginning.isValidTime() && ending.isValidTime()) {
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
	if (checkComm(other.getComments()) && checkRow(other.getName()) && other.getDate().validDate() && other.getStartTime().isValidTime() && other.getEndTime().isValidTime()) {
		this->setComments(other.getComments());
		this->setName(other.getName());
		this->setDate(other.getDate());
		this->setStartTime(other.getStartTime());
		this->setEndTime(other.getEndTime());
		this->date.setIfBusyPeriod(this->getStartTime(), this->getEndTime(), 1);
	}
}

bool Appointment::hasSameName(char* nam) const
{
	size_t size = takeRowLenght(nam);
	size_t mysize = takeRowLenght(this->getName());
	if (checkRow(nam) == 0) {
		return 0;
	}
	if (&this->name == &nam) {
		return 1;
	}
	else if (size == mysize) {
		for (size_t i = 0; i <= size; i++) {
			if (this->getName()[i] != nam[i]) {
				return 0;
			}
		}
		return 1;
	}
	return 0;
}

bool Appointment::compareNames(char* nam) const
{
	size_t size = takeRowLenght(nam);
	size_t mysize = takeRowLenght(this->getName());
	if (checkRow(nam) == 0) {
		return 0;
	}
	if (hasSameName(name)) {
		return 1;
	}
	else if (size < mysize) {
		size_t i = 0;
		while (i <= mysize) {
			if (this->getName()[i] == nam[0]) {
				size_t j = i;
				bool is = 1;
				while (is == 1 && j - i <= size) {
					if (this->getName()[j] != nam[j - i]) {
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

bool Appointment::hasSameComment(char** com) const
{
	if (checkComm(com) == 0) {
		return 0;
	}
	size_t size = takelenghtOfText(com);
	if (&this->comments == &com) {
		return 1;
	}
	else if (size == takelenghtOfText(this->getComments())) {
		for (size_t i = 0; i <= size; i++) {
			size_t sizeRow = takeRowLenght(com[i]);
			for (size_t j = 0; j <= sizeRow; j++) {
				if (this->getComments()[i][j] != com[i][j]) {
					return 0;
				}
			}
		}
		return 1;
	}
	return 0;
}

bool Appointment::compareComments(char** com) const
{
	if (checkComm(com) == 0) {
		return 0;
	}
	size_t size = takelenghtOfText(com);
	size_t myCom = takelenghtOfText(this->getComments());
	if (hasSameComment(com)) {
		return 1;
	}
	else if (size < myCom) {
		bool is = 0;
		size_t i = 0;
		while (i <= myCom) {
			size_t k = 0;
			if (this->getComments()[i][k] == '\n') {
				i++; k = 0;
			}
			if ((this->getComments()[i][k] == com[0][0]) || is == 1) {
				is = 1;
				size_t j = 0;
				size_t w = 1;
				k++;
				while (is == 1 && j <= size && com[j][w] != '\0') {
					if (this->getComments()[i][k] == '\0' && com[j][w] == '\0') {
						is = 1;
						j = size + 1;
					}
					if (this->getComments()[i][k] == '\0' && com[j][w] != '\0') {
						is = 0;
						j = size + 1;
					}
					if (this->getComments()[i][k] == '\n') {
						i++; k = 0;
					}
					if (com[j][w] == '\n') {
						j++; w = 0;
					}
					if (this->getComments()[i][k] != com[j][w]) {
						is = 0;
						j = size + 1;
					}
					w++;
					k++;
					if (this->getComments()[i][k] == '\0' && com[j][w] == '\0') {
						is = 1;
						j = size + 1;
					}
				}
				if (is == 1) {
					return 1;
				}
			}
			else {
				k++;
			}
		}
	}
	return 0;
}


bool Appointment::hasCommentInComment(char* com) const
{
	if (checkRow(com) == 0) {
		return 0;
	}
	bool is = 0;
	size_t size = takeRowLenght(com);
	size_t mySize = takelenghtOfText(this->getComments());
	size_t i = 0;
	while (i <= mySize) {
		size_t row = takeRowLenght(getComments()[i]);
		size_t j = 0;
		while (j <= row) {
			if (this->getComments()[i][j] == '\n') {
				i++; j = 0;
			}
			if (this->getComments()[i][j] == com[0]) {
				is = 1;
				size_t count = 1;
				j++;;
				while (is == 1 && count <= size) {
					if (this->getComments()[i][j] == '\0' && com[count] != '\0') {
						is = 0;
					}
					if (this->getComments()[i][j] == '\n') {
						i++; j = 0;
					}
					if (this->getComments()[i][j] != com[count]) {
						count = 0;
						is == 0;
					}
					else {
						count++;
						j++;
					}
				}
				if (is == 1) {
					return 1;
				}
			}
			i++;
		}
	}
	return 0;
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
	size_t size = takeRowLenght(name);
	for (int i = 0; i <= size; i++) {
		std::cout << name[i];
	}
	std::cout << std::endl;
}

void Appointment::printComments() const
{
	size_t sizeText = takelenghtOfText(comments);
	for (int i = 0; i <= sizeText; i++) {
		size_t sizeRow = takeRowLenght(comments[i]);
		for (size_t j = 0; j <= sizeRow; j++) {
			std::cout << comments[i][j];
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
	free();
}

Appointment::~Appointment()
{
	free();
}

