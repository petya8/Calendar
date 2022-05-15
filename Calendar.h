#pragma once
#include<fstream>
#include"Appointment.h"
///const int MAXS = 100;

class Calendar {
	const char* myFile="Calendar.txt";
	mutable Time workBegins;
	mutable Time workEnds;
	void createFile();
public:
	Calendar();
	Time& getWorkBegins()const;
	Time& getWorkEnds()const;
	const char* getNameOfFile()const;
	void setWorkBegins(const Time& time);
	void setWorkEnds(const Time& time);
	bool tooEarly(const Time& time)const;
	bool tooLate(const Time& time)const;
	bool isInTime(const Time& st,const Time& ed)const;////gleda dali st i ed sa v worend/beg 
	bool isBusy(const Date& date, const Time& st, const Time& ed)const;////dali e zaeto ot do na dadena data use sisintime
	/// tarsi da zapishe
	Date& findByTime(const Date& d, const Time& st, const Time& ed)const ;/////namira data tarsi po startov i kraen chas   
	Time& findStartingHour(const Date& date, const Time& period)const;//7!// tarsi dali v tazi data ima vreme za saotv sreshta da polzva use is busy
	Date& findByPeriod(const Date& date, const Time& period)const ;/////namira data i vremeto v neq e startoviq chas tarsi po period na sreshtause findstarting hour
	//////tarsi ot fail chete
	Appointment*& findByDate(const Date& data)const;
	Appointment*& findAppointmentsByName(const char* name)const;
	Appointment*& findAppointmentsByCom(const char* com)const;
	friend Appointment* findAppointments(Calendar& calendar);///5
	void workload(const Date& beginning, const Date& ending)const;///6 writes in new file and printsthem
	void getWorkload(const Date& date) const;////3
	void changeAppName(const char* oldName,const char* newName);
	void changeAppComm(const char* oldCom,const char* newComm);
	void changeAppDate(const char* name,const Date& oldDate, const Date& newDate);
	void changeAppStT(const Date& date, const Time& oldTime, const Time& newTime);
	void changeAppEdT(const Date& date, const Time& oldTime, const Time& newTime);
	void changeApp();////4
	void createAPP(Appointment& appNew);
	void createNewApp();
	void chooseTimeAndAddApp();////1
	void removeApp(Appointment& app);///2
	void interface();
};
