#pragma once
#include<fstream>
#include"Appointment.h"
const int MAXS = 100;

class Calendar {
	const char* myFile="Calendar.txt";
	mutable Time workBegins;
	mutable Time workEnds;
	void createFile() {
		std::ofstream myfile(myFile);
		if (!myfile.is_open()) {
			std::cout << "No such file";
		}
		
		myfile.close();
	}
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
	Date& findByTime(const Date& d, const Time& st, const Time& ed);/////namira data tarsi po startov i kraen chas   
	Time& findStartingHour(const Date& date, const Time& period)const;//7!// tarsi dali v tazi data ima vreme za saotv sreshta da polzva use is busy
	Date& findByPeriod(const Date& date, const Time& period);/////namira data i vremeto v neq e startoviq chas tarsi po period na sreshtause findstarting hour
	//////tarsi ot fail chete
	Appointment*& findByDate(const Date& data)const;
	Appointment*& findAppointmentsByName(char* name)const;
	Appointment*& findAppointmentsBytextInCom(char* com)const;
	Appointment*& findAppointmentsByCom(char** com)const;
	friend Appointment*& findAppointments(Calendar& calendar);///5
	void workload(const Date& beginning, const Date& ending)const;///6 writes in new file and printsthem
	void getWorkload(const Date& date) const;////3
	void changeAppName(char* oldName, char* newName);
	void changeAppComm(char** oldCom, char** newComm);
	void changeAppDate(char* name,const Date& oldDate, const Date& newDate);
	void changeAppStT(const Date& date, const Time& oldTime, const Time& newTime);
	void changeAppEdT(const Date& date, const Time& oldTime, const Time& newTime);
	void changeApp();
	void createNewApp();
	void interface();
};
size_t getFileSize(std::ifstream& file)
{
	size_t currentPosition = file.tellg();
	file.seekg(0, std::ios::end);
	size_t result = file.tellg();
	file.seekg(currentPosition);
	return result;
}
void swapAppointments(Appointment& app1, Appointment& app2) {
	Appointment temp(app1);
	app1 = app2;
	app2 = temp;
}
void sortAppointments(Appointment*& app, size_t startIndex, size_t number) {
	for (size_t i = startIndex; i < number; i++) {
		for (size_t j = i + 1; j <= number; j++) {
			while (app[i].getDate().compareNoTime(app[j].getDate()) == 0) {
				if (app[i].getStartTime().compare(app[j].getStartTime()) < 0) {
					swapAppointments(app[i], app[j]);
				}
			}
		}
	}
}

char* intToChar(int n) {
	char* ch = new char[10]; int i = 0;
	while (n > 0) {
		ch[i++] = ((int)n % 10) + '0';
		n /= 10;
	}
	char* newch = new char[i];
	for (int j = 0; j < i; j++) {
		newch[j] = ch[i - j - 1];
	}
	newch[i] = '\0';
	delete[] ch;
	return newch;
}
void resize(Appointment*& app, int numberApps, int& maxSize) {
	Appointment* newApp = new Appointment[maxSize * 2];
	for (int i = 0; i <= numberApps; i++) {
		newApp[i] = app[i];
	}
	delete[] app;
	app = newApp;
	maxSize *= 2;
	for (int i = 0; i <= numberApps; i++)
	{
		app[i] = newApp[i];
	}
}

void writeAppFile(std::fstream& myfile, Appointment& app) {
	myfile.write((char*)&app.date, sizeof(Date));
	size_t sizeOfName = 0;
	myfile.write((char*)&sizeOfName, sizeof(sizeOfName));
	app.name = new char[sizeOfName + 1];
	myfile.write(app.name, sizeOfName);
	app.name[sizeOfName + 1] = '\0';
	size_t sizeCom = 0;
	myfile.write((char*)&sizeCom, sizeof(sizeCom));
	app.comments = new char* [sizeCom + 1];
	myfile.write((char*)&app.comments, sizeCom);
	size_t endSize = takeRowLenght(app.comments[sizeCom + 1]);
	app.comments[sizeCom + 1][endSize] = '\0';
	myfile.write((char*)&app.startTime, sizeof(Time));
	myfile.write((char*)&app.endTime, sizeof(Time));
}
void writeAppFileOfstr(std::ofstream& myfile, Appointment& app) {
	myfile.write((char*)&app.date, sizeof(Date));
	size_t sizeOfName = 0;
	myfile.write((char*)&sizeOfName, sizeof(sizeOfName));
	app.name = new char[sizeOfName + 1];
	myfile.write(app.name, sizeOfName);
	app.name[sizeOfName + 1] = '\0';
	size_t sizeCom = 0;
	myfile.write((char*)&sizeCom, sizeof(sizeCom));
	app.comments = new char* [sizeCom + 1];
	myfile.write((char*)&app.comments, sizeCom);
	size_t endSize = takeRowLenght(app.comments[sizeCom + 1]);
	app.comments[sizeCom + 1][endSize] = '\0';
	myfile.write((char*)&app.startTime, sizeof(Time));
	myfile.write((char*)&app.endTime, sizeof(Time));
}
void rewriteFile(std::fstream& myfile,size_t position,Appointment& appToAdd ) {
	int currSize = MAXS;
	Appointment* apps = new Appointment[currSize];
	int numberApps = 0;
	myfile.seekg(position, std::ios::beg);
	while (!myfile.eof()) {
		Appointment app;
		myfile.read((char*)&app.date, sizeof(Date));
		size_t sizeOfName = 0;
		myfile.read((char*)&sizeOfName, sizeof(sizeOfName));
		app.name = new char[sizeOfName + 1];
		myfile.read(app.name, sizeOfName);
		app.name[sizeOfName + 1] = '\0';
		size_t sizeCom = 0;
		myfile.read((char*)&sizeCom, sizeof(sizeCom));
		app.comments = new char* [sizeCom + 1];
		myfile.read((char*)&app.comments, sizeCom);
		size_t endSize = takeRowLenght(app.comments[sizeCom + 1]);
		app.comments[sizeCom + 1][endSize] = '\0';
		myfile.read((char*)&app.startTime, sizeof(Time));
		myfile.read((char*)&app.endTime, sizeof(Time));
		if (numberApps > currSize) {
			resize(apps, numberApps, currSize);
		}
		apps[numberApps] = app;
		numberApps++;
	}
	myfile.seekp(position, std::ios::beg);
	writeAppFile(myfile, appToAdd);
	for (int i = 0; i < numberApps; i++) {
		writeAppFile(myfile, apps[i]);
	}
}