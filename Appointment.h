#pragma once
#include"String.h"
#include"Date.h"
#pragma warning(disable:4996)

struct Appointment {
	mutable Date date;
	String name ;
	String comments;
	mutable Time startTime;
	mutable Time endTime;
	void fr();
	void copyFrom(const Appointment& other);
	Appointment();
	String getComments()const;
	String getName()const;
	Time& getStartTime()const;
	Time& getEndTime()const;
	Date& getDate()const;
	Time& getDuration()const;
	void setDate(const Date& date);
	void setName(const String name);
	void setComments(const String com);
	void setAppointment(const String name,const String comments, const Date& newDate, const Time& beginning, const Time& ending);
	void setStartTime(const Time& newTime);
	void setEndTime(const Time& newTime);
	void operator=(const Appointment& other);
	bool compareNames(const String toCompare)const;
	bool compareComments(const String toCompare)const;
	bool hasSameName(const String toCompare)const;
	bool hasSameComment(const String toCompare)const;
	bool compareAppointment(const Appointment& app)const;
	void printName()const;
	void printComments()const;
	void printAppointment()const;
	void deleteAppontment();
	~Appointment();
};
