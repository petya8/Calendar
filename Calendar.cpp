#include"Calendar.h"

size_t takeRowLenght(char* row) {
	size_t counter = 0;
	while (row[counter] != '\0' && row[counter] != '\n') {
		counter++;
	}
	return counter;
}

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
		newch[j] = ch[i - j ];
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

void sortAppointmentsPeriodApps(Appointment*& app, size_t startIndex, size_t number) {
	for (size_t i = startIndex; i < number; i++) {
		for (size_t j = i + 1; j <= number; j++) {
			if (app[i].getDuration().compare(app[j].getDuration()) < 0) {
				swapAppointments(app[i], app[j]);
			}
		}
	}
}

void writeAppFile(std::fstream& myfile, Appointment& app) {
	myfile.write((char*)&app.date, sizeof(Date));
	size_t sizeOfName = 0;
	myfile.write((char*)&sizeOfName, sizeof(sizeOfName));
	app.name = new char[sizeOfName + 1];
	myfile.write((char*)&app.name, sizeOfName);
	app.name.concat("\0");
	size_t sizeCom = 0;
	myfile.write((char*)&sizeCom, sizeof(sizeCom));
	app.comments [sizeCom + 1];
	myfile.write((char*)&app.comments, sizeCom);
	app.comments.concat("\0");
	myfile.write((char*)&app.startTime, sizeof(Time));
	myfile.write((char*)&app.endTime, sizeof(Time));
}

void writeAppFileOfstr(std::ofstream& myfile, Appointment& app) {
	myfile.write((char*)&app.date, sizeof(Date));
	size_t sizeOfName = 0;
	myfile.write((char*)&sizeOfName, sizeof(sizeOfName));
	app.name[sizeOfName + 1];
	myfile.write((char*)&app.name, sizeOfName);
	app.name.concat("\0");
	size_t sizeCom = 0;
	myfile.write((char*)&sizeCom, sizeof(sizeCom));
	app.comments[sizeCom + 1];
	myfile.write((char*)&app.comments, sizeCom);
	app.comments.concat("\0");
	myfile.write((char*)&app.startTime, sizeof(Time));
	myfile.write((char*)&app.endTime, sizeof(Time));
}

void rewriteFile(std::fstream& myfile, size_t position, Appointment& appToAdd) {
	int currSize = 100;
	Appointment* apps = new Appointment[currSize];
	int numberApps = 0;
	myfile.seekg(position, std::ios::beg);
	while (!myfile.eof()) {
		Appointment app;
		myfile.read((char*)&app.date, sizeof(Date));
		size_t sizeOfName = 0;
		myfile.read((char*)&sizeOfName, sizeof(sizeOfName));
		app.name = new char[sizeOfName + 1];
		myfile.read((char*)&app.name, sizeOfName);
		app.name.concat("\0");
		size_t sizeCom = 0;
		myfile.read((char*)&sizeCom, sizeof(sizeCom));
		app.comments[sizeCom + 1];
		myfile.read((char*)&app.comments, sizeCom);
		app.comments.concat("\0");
		myfile.read((char*)&app.startTime, sizeof(Time));
		myfile.read((char*)&app.endTime, sizeof(Time));
		if (numberApps > currSize) {
			resize(apps, numberApps, currSize);
		}
		apps[numberApps] = app;
		numberApps++;
		if (appToAdd.getDate().compareNoTime(app.getDate()) == 0 && appToAdd.getStartTime().compare(app.getEndTime()) >= 0) {
			apps[numberApps] = appToAdd;
			numberApps++;
		}
	}
	sortAppointments(apps, 0, numberApps);
	myfile.seekp(position, std::ios::beg);
	for (int i = 0; i < numberApps; i++) {
		writeAppFile(myfile, apps[i]);
	}
}

void Calendar::createFile() {
	std::ofstream myfile(myFile);
	if (!myfile.is_open()) {
		std::cout << "No such file";
	}

	myfile.close();
}

Date& calculateWeekWanted(size_t number, const Date& startingDate) {
	return startingDate.daysLater(number * 7);
}////it was for the function before the change that was made for the conditions it calculates number of week from a date on

Calendar::Calendar()
{
	createFile();
	Time st;
	st.createTime(0, 0, 8, 0);
	workBegins = st;
	Time ed;
	ed.createTime(0, 0, 18, 0);
	workEnds = ed;
}

Time& Calendar::getWorkBegins() const
{
	return workBegins;
}

Time& Calendar::getWorkEnds() const
{
	return workEnds;
}

const char* Calendar::getNameOfFile() const
{
	return myFile;
}

void Calendar::setWorkBegins(const Time& time)
{
	workBegins = time;
}

void Calendar::setWorkEnds(const Time& time)
{
	workEnds = time;
}

bool Calendar::tooEarly(const Time& time) const
{
	return (getWorkBegins().compare(time)>0);
}

bool Calendar::tooLate(const Time& time) const
{
	return (getWorkEnds().compare(time) < 0);
}

bool Calendar::isInTime(const Time& st, const Time& ed) const
{
	return(tooEarly(st)==0&& tooLate(ed)==0);
}

bool Calendar::isBusy(const Date& date, const Time& st, const Time& ed) const
{
	if (isInTime(st, ed)==0) {
		return 1;
	}
	Time s = st;
	const int MAX = 100;
	Appointment* apps = new Appointment[MAX];
	apps = findByDate(date);
	size_t size = sizeof(apps) / sizeof(Appointment);
	size_t i = 0;
	while (i <= size) {
		while (s.compare(ed) < 0) {
			if (apps[i].getStartTime().compare(st) == 0 && apps[i].getStartTime().isTBusy()) {
				return 1;
			}
			s.timePasses();
		}
		if (s.compare(ed) >= 0) {
			return 0;
		}
		else {
			i++;
		}
	}
	return 0;
}

Date& Calendar::findByTime(const Date& d, const Time& st, const Time& ed)const
{
	Date copy(d);
	if (isInTime(st, ed)) {
		while (isBusy(copy, st, ed)) {
			copy = copy.daysLater(1);
		}
		return copy;
	}
	else {
		copy.createInvalidDate(0, 0, 0,st);
		return copy;
	}
	
}

Time& Calendar::findStartingHour(const Date& date, const Time& period) const
{
	Appointment* apps = new Appointment[100];
	apps = findByDate(date);
	size_t size = sizeof(apps) / sizeof(Appointment);
	Time st = this->getWorkBegins();
	bool bsy=0;
	size_t i = 0;
	while (i <= size) {
		Time ed = st.sum(period);
		while (st.compare(ed) < 0) {
			if (apps[i].getStartTime().compare(st) == 0&&apps[i].getStartTime().isTBusy()) {
				bsy = 1;
				st = apps[i].getEndTime();
			}
			st.timePasses();
		}
		if (bsy == 0) {
			st = st.difference(period);
			i = size + 1;
		}
		else {
			st = apps[i].getEndTime();
			i++;
		}
	}
	if (bsy == 1) {
		st.timelater(-1, -1, -1);
	}
	return st;
}

Date& Calendar::findByPeriod(const Date& date, const Time& period)const
{
	Date res (date);
	Time st = findStartingHour(res,period);
	while (st.isValidTime() == 0) {
		res = res.daysLater(1);
		st = findStartingHour(res, period);
	}
	res.setTime(st);
	return res;
}

Appointment*& Calendar::findByDate(const Date& data) const
{
	Appointment* apps = new Appointment[100];
	std::ifstream myfile(myFile);
	size_t currg = myfile.tellg();
	myfile.seekg(0, std::ios::end);
	size_t sizeOfFile = myfile.tellg();
	int numberApps = 0;
	myfile.seekg(0, std::ios::beg);
	while (!myfile.eof()) {
		Appointment app;
		myfile.read((char*)&app.date, sizeof(Date));
		size_t sizeOfName = 0;
		myfile.read((char*)&sizeOfName, sizeof(sizeOfName));
		app.name = new char[sizeOfName + 1];
		myfile.read((char*)&app.name, sizeOfName);
		app.name.concat("\0");
		size_t sizeCom = 0;
		myfile.read((char*)&sizeCom, sizeof(sizeCom));
		app.comments[sizeCom + 1];
		myfile.read((char*)&app.comments, sizeCom);
		app.comments.concat("\0");
		myfile.read((char*)&app.startTime, sizeof(Time));
		myfile.read((char*)&app.endTime, sizeof(Time));
		if (app.getDate().compareNoTime(data) == 0) {
			apps[numberApps] = app;
			numberApps++;
		}
		else if (app.getDate().compareNoTime(data.daysLater(1)) == 0) {
			myfile.seekg(0, std::ios::end);
		}
	}
	sortAppointments(apps, 0, numberApps - 1);
	myfile.seekg(currg, std::ios::beg);
	myfile.close();
	return apps;
}

Appointment*& Calendar::findAppointmentsByName(const char* name) const
{
	Appointment* apps = new Appointment[100];
	std::ifstream myfile(myFile);
	size_t sizeOfFile = getFileSize(myfile);
	size_t currg = myfile.tellg();
	int numberApps = 0;
	myfile.seekg(0, std::ios::beg);
	while (!myfile.eof()) {
		Appointment app;
		myfile.read((char*)&app.date, sizeof(Date));
		size_t sizeOfName = 0;
		myfile.read((char*)&sizeOfName, sizeof(sizeOfName));
		app.name = new char[sizeOfName + 1];
		myfile.read((char*)&app.name, sizeOfName);
		app.name.concat("\0");
		size_t sizeCom = 0;
		myfile.read((char*)&sizeCom, sizeof(sizeCom));
		app.comments[sizeCom + 1];
		myfile.read((char*)&app.comments, sizeCom);
		app.comments.concat("\0");
		myfile.read((char*)&app.startTime, sizeof(Time));
		myfile.read((char*)&app.endTime, sizeof(Time));
		if (app.compareNames(name) == 1) {
			apps[numberApps] = app;
			numberApps++;
		}
	}
	myfile.seekg(currg, std::ios::beg);
	myfile.close();
	return apps;
}

Appointment*& Calendar::findAppointmentsByCom(const char* com) const
{
	Appointment* apps = new Appointment[100];
	std::ifstream myfile(myFile);
	size_t sizeOfFile = getFileSize(myfile);
	size_t currg = myfile.tellg();
	int numberApps = 0;
	myfile.seekg(0, std::ios::beg);
	while (!myfile.eof()) {
		Appointment app;
		myfile.read((char*)&app.date, sizeof(Date));
		size_t sizeOfName = 0;
		myfile.read((char*)&sizeOfName, sizeof(sizeOfName));
		app.name = new char[sizeOfName + 1];
		myfile.read((char*)&app.name, sizeOfName);
		app.name.concat("\0");
		size_t sizeCom = 0;
		myfile.read((char*)&sizeCom, sizeof(sizeCom));
		app.comments[sizeCom + 1];
		myfile.read((char*)&app.comments, sizeCom);
		app.comments.concat("\0");
		myfile.read((char*)&app.startTime, sizeof(Time));
		myfile.read((char*)&app.endTime, sizeof(Time));
		if (app.compareComments(com) == 1) {
			apps[numberApps] = app;
			numberApps++;
		}
	}
	myfile.seekg(currg, std::ios::beg);
	myfile.close();
	return apps;
}

Appointment* findAppointments(Calendar& calendar)
{
	char* comment = new char[100];
	std::cout << "Enter comments";
	std::cin.ignore();
	std::cin.getline(comment, '\0');
	Appointment* apps = new Appointment[100];
	Appointment* app = new Appointment[100];
	Appointment* res = new Appointment[100];
	app = calendar.findAppointmentsByCom(comment);
	size_t currentSize = sizeof(apps) / sizeof(Appointment);
	apps = calendar.findAppointmentsByName(comment);
	size_t sizeToAdd = sizeof(app) / sizeof(Appointment);
	for (size_t i = 0; i <= currentSize; i++) {
		res[i] = apps[i];
	}
	delete[] apps;
	for (size_t i = currentSize + 1; i <= currentSize + 1 + sizeToAdd; i++) {
		res[i] = app[i];
	}
	delete[] app;
	return res;
}

void Calendar::workload(const Date& beginning, const Date& ending)const {//////READ from file write to newfile
	char* name = new char[100];
	int size = strlen("stats");
	char st[] = "stats";
	for (int i = 0; i < size; i++) {
		name[i] = st[i];
	}
	name[size] = '-';
	size++;
	char* year = intToChar(beginning.getYear());
	int newSize = takeRowLenght(year) + size;
	for (int i = size; i <= newSize; i++) {
		name[i] = year[newSize - i];
	}
	delete[] year;
	name[++newSize] = '-';
	newSize++;
	char* month = intToChar(beginning.getMonth());
	size = takeRowLenght(month) + newSize;
	for (int i = newSize; i <= size; i++) {
		name[i] = month[size - i];
	}
	delete[] month;
	name[++size] = '-';
	size++;
	char* day = intToChar(beginning.getDay());
	newSize = takeRowLenght(day) + size;
	for (int i = size; i <= newSize; i++) {
		name[i] = day[newSize - i];
	}
	delete[] day;
	name[++newSize] = '.';
	name[++newSize] = 't';
	name[++newSize] = 'x';
	name[++newSize] = 't';
	name[++newSize] = '\0';
	char* taken = new char[newSize];
	for (int i = 0; i <= newSize; i++) {
		taken[i] = name[i];
	}
	delete[]name;
	std::ifstream myfile(myFile);
	Appointment* apps = new Appointment[100];
	Date d;
	d = beginning;
	size_t currg = myfile.tellg();
	size_t numberApps = 0;
	myfile.seekg(0, std::ios::beg);
	size_t sizeOfFile = getFileSize(myfile);
	while (!myfile.eof()) {
		Appointment app;
		myfile.read((char*)&app.date, sizeof(Date));
		size_t sizeOfName = 0;
		myfile.read((char*)&sizeOfName, sizeof(sizeOfName));
		app.name = new char[sizeOfName + 1];
		myfile.read((char*)&app.name, sizeOfName);
		app.name.concat("\0");
		size_t sizeCom = 0;
		myfile.read((char*)&sizeCom, sizeof(sizeCom));
		app.comments[sizeCom + 1];
		myfile.read((char*)&app.comments, sizeCom);
		app.comments.concat("\0");
		myfile.read((char*)&app.startTime, sizeof(Time));
		myfile.read((char*)&app.endTime, sizeof(Time));
		if ((app.getDate().compareNoTime(beginning) >= 0) && (app.getDate().compareNoTime(ending) <= 0) && app.getStartTime().isTBusy()) {
			apps[numberApps] = app;
			numberApps++;
		}
		if (app.getDate().compareNoTime(ending.daysLater(1)) == 0) {
			myfile.seekg(sizeOfFile, std::ios::beg);
		}
	}
	myfile.seekg(currg, std::ios::beg);
	myfile.close();
	sortAppointmentsPeriodApps(apps, 0, numberApps);
	std::cout << "These are the appointments for the week" << std::endl;
	for (size_t k = 0; k <= numberApps; k++) {
		apps[k].printAppointment();
	}
	std::cout << std::endl;
	std::ofstream file((const char*)taken, std::ios::trunc);
	if (!file.is_open()) {
		std::cout << "Error opening the file";
	}
	file.seekp(0, std::ios::beg);
	int j = 0; bool change = 1;
	const char* weekday = apps[0].getDate().dayOfWeek();
	while (j != numberApps) {
		if (change == 1) {
			file.write(weekday, strlen(weekday));
			change = 0;
		}
		writeAppFileOfstr(file, apps[j]);
		if (apps[j].getDate().compareNoTime(apps[j + 1].getDate()) != 0) {
			change = 1;
			weekday = apps[j + 1].getDate().dayOfWeek();
		}
		j++;
	}
	file.close();
}

void Calendar::getWorkload(const Date& date) const {
	std::cout << "Appointments for ";
	date.printD();
	std::cout << std::endl;
	Appointment* app = new Appointment[100];
	app = findByDate(date);
	size_t size = sizeof(app) / sizeof(Appointment);
	for (size_t i = 0; i <= size; i++) {
		app[i].printAppointment();
	}
	std::cout << std::endl;
}

void Calendar::changeAppName(const char* oldName,const char* newName)
{
	std::fstream myfile(myFile);
	size_t currg = myfile.tellg();
	int numberApps = 0;
	myfile.seekg(0, std::ios::beg);
	while (!myfile.eof()) {
		size_t position = myfile.tellg();
		Appointment app;
		myfile.read((char*)&app.date, sizeof(Date));
		size_t sizeOfName = 0;
		myfile.read((char*)&sizeOfName, sizeof(sizeOfName));
		app.name = new char[sizeOfName + 1];
		myfile.read((char*)&app.name, sizeOfName);
		app.name.concat("\0");
		size_t sizeCom = 0;
		myfile.read((char*)&sizeCom, sizeof(sizeCom));
		app.comments[sizeCom + 1];
		myfile.read((char*)&app.comments, sizeCom);
		app.comments.concat("\0");
		myfile.read((char*)&app.startTime, sizeof(Time));
		myfile.read((char*)&app.endTime, sizeof(Time));
		if (app.hasSameName(oldName) == 1) {
			myfile.seekp(position, std::ios::beg);
			app.setName(newName);
			writeAppFile(myfile, app);
		}
	}
	myfile.seekg(currg, std::ios::beg);
	myfile.close();
}

void Calendar::changeAppComm(const char* oldCom, const char* newComm)
{
	std::fstream myfile(myFile);
	size_t currg = myfile.tellg();
	int numberApps = 0;
	myfile.seekg(0, std::ios::beg);
	while (!myfile.eof()) {
		size_t position = myfile.tellg();
		Appointment app;
		myfile.read((char*)&app.date, sizeof(Date));
		size_t sizeOfName = 0;
		myfile.read((char*)&sizeOfName, sizeof(sizeOfName));
		app.name = new char[sizeOfName + 1];
		myfile.read((char*)&app.name, sizeOfName);
		app.name.concat("\0");
		size_t sizeCom = 0;
		myfile.read((char*)&sizeCom, sizeof(sizeCom));
		app.comments[sizeCom + 1];
		myfile.read((char*)&app.comments, sizeCom);
		app.comments.concat("\0");
		myfile.read((char*)&app.startTime, sizeof(Time));
		myfile.read((char*)&app.endTime, sizeof(Time));
		if (app.hasSameComment(oldCom) == 1) {
			myfile.seekp(position, std::ios::beg);
			app.setComments(newComm);
			writeAppFile(myfile, app);
		}
	}
	myfile.seekg(currg, std::ios::beg);
	myfile.close();
}

void Calendar::changeAppDate(const char* name, const Date& newDate, const Date& oldDate)
{
	if (oldDate.validDate()) {
		std::fstream myfile(myFile);
		size_t currg = myfile.tellg();
		int numberApps = 0;
		myfile.seekg(0, std::ios::beg);
		Appointment copy;
		while (!myfile.eof()) {
			size_t position = myfile.tellg();
			Appointment app;
			myfile.read((char*)&app.date, sizeof(Date));
			size_t sizeOfName = 0;
			myfile.read((char*)&sizeOfName, sizeof(sizeOfName));
			app.name = new char[sizeOfName + 1];
			myfile.read((char*)&app.name, sizeOfName);
			app.name.concat("\0");
			size_t sizeCom = 0;
			myfile.read((char*)&sizeCom, sizeof(sizeCom));
			app.comments[sizeCom + 1];
			myfile.read((char*)&app.comments, sizeCom);
			app.comments.concat("\0");
			myfile.read((char*)&app.startTime, sizeof(Time));
			myfile.read((char*)&app.endTime, sizeof(Time));
			if (app.getDate().compareNoTime(oldDate) && app.hasSameName(name) == 1) {
				Time st = findStartingHour(newDate, app.getDuration());
				if (st.isValidTime()) {
					myfile.seekp(position, std::ios::beg);
					copy = app;
					app.deleteAppontment();
					writeAppFile(myfile, app);
					copy.setDate(newDate);
					copy.setStartTime(st);
					copy.setEndTime(st.sum(app.getDuration()));
					myfile.seekg(0, std::ios::end);
				}
				else {
					std::cout << "There is not time for that appointment for this date";
					myfile.seekg(0, std::ios::end);
				}
			}
		}
		if (copy.compareNames(name) == 0) {
			myfile.seekg(0, std::ios::beg);
			while (!myfile.eof()) {
				Appointment app;
				myfile.read((char*)&app.date, sizeof(Date));
				size_t sizeOfName = 0;
				myfile.read((char*)&sizeOfName, sizeof(sizeOfName));
				app.name = new char[sizeOfName + 1];
				myfile.read((char*)&app.name, sizeOfName);
				app.name.concat("\0");
				size_t sizeCom = 0;
				myfile.read((char*)&sizeCom, sizeof(sizeCom));
				app.comments[sizeCom + 1];
				myfile.read((char*)&app.comments, sizeCom);
				app.comments.concat("\0");
				myfile.read((char*)&app.startTime, sizeof(Time));
				myfile.read((char*)&app.endTime, sizeof(Time));
				size_t position = myfile.tellg();
				if (app.getDate().compareNoTime(copy.getDate()) == 0 && app.getEndTime().compare(copy.getStartTime()) <= 0) {
					rewriteFile(myfile, position + 1, copy);
					myfile.seekg(0, std::ios::end);
				}
			}
		}
		myfile.seekg(currg, std::ios::beg);
		myfile.close();
	}
	else {
		std::cout << "Invalid date";
	}
}

void Calendar::changeAppStT(const Date& date, const Time& oldTime, const Time& newTime)
{
	if (newTime.isValidTime()) {
		std::fstream myfile(myFile);
		size_t currg = myfile.tellg();
		int numberApps = 0;
		myfile.seekg(0, std::ios::beg);
		while (!myfile.eof()) {
			size_t position = myfile.tellg();
			Appointment app;
			myfile.read((char*)&app.date, sizeof(Date));
			size_t sizeOfName = 0;
			myfile.read((char*)&sizeOfName, sizeof(sizeOfName));
			app.name = new char[sizeOfName + 1];
			myfile.read((char*)&app.name, sizeOfName);
			app.name.concat("\0");
			size_t sizeCom = 0;
			myfile.read((char*)&sizeCom, sizeof(sizeCom));
			app.comments[sizeCom + 1];
			myfile.read((char*)&app.comments, sizeCom);
			app.comments.concat("\0");
			myfile.read((char*)&app.startTime, sizeof(Time));
			myfile.read((char*)&app.endTime, sizeof(Time));
			if (app.getDate().compareNoTime(date) &&app.getStartTime().compare(oldTime)&& isInTime(newTime,app.getDuration().sum(newTime)) && isBusy(date, newTime,newTime.sum(app.getDuration()))==0) {
				myfile.seekp(position, std::ios::beg);
				app.setStartTime(newTime);
				Time period = app.getDuration();
				app.setEndTime(newTime.sum(period));
				writeAppFile(myfile, app);
			}
		}
		myfile.seekg(currg, std::ios::beg);
		myfile.close();
	}
}

void Calendar::changeAppEdT(const Date& date, const Time& oldTime, const Time& newTime) {
	if (newTime.isValidTime()) {
		std::fstream myfile(myFile);
		size_t currg = myfile.tellg();
		int numberApps = 0;
		myfile.seekg(0, std::ios::beg);
		while (!myfile.eof()) {
			size_t position = myfile.tellg();
			Appointment app;
			myfile.read((char*)&app.date, sizeof(Date));
			size_t sizeOfName = 0;
			myfile.read((char*)&sizeOfName, sizeof(sizeOfName));
			app.name = new char[sizeOfName + 1];
			myfile.read((char*)&app.name, sizeOfName);
			app.name.concat("\0");
			size_t sizeCom = 0;
			myfile.read((char*)&sizeCom, sizeof(sizeCom));
			app.comments[sizeCom + 1];
			myfile.read((char*)&app.comments, sizeCom);
			app.comments.concat("\0");
			myfile.read((char*)&app.startTime, sizeof(Time));
			myfile.read((char*)&app.endTime, sizeof(Time));
			if (app.getDate().compareNoTime(date) && app.getEndTime().compare(oldTime) && isInTime(newTime, app.getDuration().sum(newTime)) && isBusy(date, app.getEndTime(), newTime) == 0) {
				myfile.seekp(position, std::ios::beg);
				app.setEndTime(newTime);
				writeAppFile(myfile, app);
			}
		}
		myfile.seekg(currg, std::ios::beg);
		myfile.close();
	}
}

void Calendar::changeApp()
{
	std::cin.ignore();
	std::cout << "Which component do you want to change?"<<std::endl;
	char* change = new char[30];
	std::cin.getline(change, '\n');
	if (strcmp(change, "name") == 0) {
		std::cout << "Enter old name " << std::endl;
		char* oldName = new char[30];
		std::cin.getline(oldName, '\n');
		std::cout << "Enter the new name" << std::endl;
		std::cin.getline(change, '\n');
		changeAppName(oldName, change);
		delete[] oldName;
	}
	else if (strcmp(change, "comment") == 0) {
		std::cout << "Enter old coment " << std::endl;
		char* oldCom = new char [100];
		std::cin.getline(oldCom, '\0');
		std::cout << "Enter new coment " << std::endl;
		char* newCom = new char [100];
		std::cin.getline(newCom, '\0');
		changeAppComm(oldCom, newCom);
		delete[] oldCom;
	}
	else if (strcmp(change, "date") == 0) {
		std::cout << "This function changes only if the date has free time and will change the start time. If you want to be the same try change start time later. If you do not see free time at that date than do not change at all."<<std::endl<<" Enter old name " << std::endl;
		std::cin.getline(change, '\n');
		std::cout << "Enter the new day,month,year" << std::endl;
		unsigned day = 0, month = 0, year = 0;
		std::cin >> day;
		std::cin >> month;
		std::cin >> year;
		Time t;
		t.createTime(0, 0, 0, 0);
		Date d(year, month, day, t);
		std::cout << "Enter the old day,month,year" << std::endl;
		std::cin >> day;
		std::cin >> month;
		std::cin >> year;
		Date dOld(year, month, day, t);
		changeAppDate(change, d, dOld);
	}
	else if (strcmp(change, "start time") == 0) {
		std::cout << "This function changes only if possible!"<<std::endl<<"Enter  day, month, year " << std::endl;
		unsigned day = 0, month = 0, year = 0;
		std::cin >> day;
		std::cin >> month;
		std::cin >> year;
		Time t;
		t.createTime(0, 0, 0, 0);
		Date d(year, month, day, t);
		std::cout << "Enter the old start time: hours,minutes,seconds" << std::endl;
		size_t hours = 0, minutes = 0, seconds = 0;
		std::cin >> hours;
		std::cin >> minutes;
		std::cin >> seconds;
		t.setHours(hours);
		t.setMinutes(minutes);
		t.setSeconds(seconds);
		d.setTime(t);
		std::cout << "Enter the new start time: hours,minutes,seconds" << std::endl;
		std::cin >> hours;
		std::cin >> minutes;
		std::cin >> seconds;
		t.setHours(hours);
		t.setMinutes(minutes);
		t.setSeconds(seconds);
		changeAppStT(d, d.getTime(), t);
	}
	else if (strcmp(change, "end time") == 0) {
		std::cout << "This function changes the duration of the appointment, if you do not want enter -1 for date and time." << std::endl << "Enter day, month, year " << std::endl;
		unsigned day = 0, month = 0, year = 0;
		std::cin >> day;
		std::cin >> month;
		std::cin >> year;
		Time t;
		t.createTime(0, 0, 0, 0);
		Date d(year, month, day, t);
		std::cout << "Enter the old end time: hours,minutes,seconds";
		size_t hours = 0, minutes = 0, seconds = 0;
		std::cin >> hours;
		std::cin >> minutes;
		std::cin >> seconds;
		t.setHours(hours);
		t.setMinutes(minutes);
		t.setSeconds(seconds);
		d.setTime(t);
		std::cout << "Enter the new end time: hours,minutes,seconds";
		std::cin >> hours;
		std::cin >> minutes;
		std::cin >> seconds;
		t.setHours(hours);
		t.setMinutes(minutes);
		t.setSeconds(seconds);
		changeAppEdT(d, d.getTime(), t);
	}
	else {
		std::cout << "Wrong input";
	}
	delete[] change;
}

void Calendar::createAPP( Appointment& appNew)
{
	std::fstream myfile(myFile);
	size_t currg = myfile.tellg();
	int numberApps = 0;
	myfile.seekg(0, std::ios::beg);
	while (!myfile.eof()) {
		Appointment app;
		myfile.read((char*)&app.date, sizeof(Date));
		size_t sizeOfName = 0;
		myfile.read((char*)&sizeOfName, sizeof(sizeOfName));
		app.name = new char[sizeOfName + 1];
		myfile.read((char*)&app.name, sizeOfName);
		app.name.concat("\0");
		size_t sizeCom = 0;
		myfile.read((char*)&sizeCom, sizeof(sizeCom));
		app.comments[sizeCom + 1];
		myfile.read((char*)&app.comments, sizeCom);
		app.comments.concat("\0");
		myfile.read((char*)&app.startTime, sizeof(Time));
		myfile.read((char*)&app.endTime, sizeof(Time));
		size_t position = myfile.tellg();
		if (appNew.getDate().compareNoTime(app.date) && app.getEndTime().compare(appNew.getStartTime()) <= 0) {
			myfile.seekp(position, std::ios::beg);
			rewriteFile(myfile, position + 1, appNew);
			myfile.seekg(0, std::ios::end);
		}
	}
	myfile.seekg(currg, std::ios::beg);
	myfile.close();
}

void Calendar::createNewApp()
{
	std::cout << "Enter date you want (day.month.year)" << std::endl;
	unsigned day = 0, month = 0, year = 0;
	std::cin >> day;
	std::cin >> month;
	std::cin >> year;
	Time t, p;
	p.createTime(0, 0, 0, 0);
	Date d(year, month, day, t);
	if (d.validDate() ==0) {
		std::cout << "Invalid date" << std::endl;
		return;
	}
	std::cout << "Enter duration of appointment" << std::endl;
	size_t hours = 0, minutes = 0, seconds = 0;
	std::cin >> hours;
	std::cin >> minutes;
	std::cin >> seconds;
	p.setHours(hours);
	p.setMinutes(minutes);
	p.setSeconds(seconds);
	if (p.isValidTime() == 0) {
		std::cout << "Invalid time" << std::endl;
		return;
	}
	std::cout << "Enter starting hour you want (hour:minutes:seconds)" << std::endl;
	std::cin >> hours;
	std::cin >> minutes;
	std::cin >> seconds;
	t.setHours(hours);
	t.setMinutes(minutes);
	t.setSeconds(seconds);
	if (t.isValidTime() == 0) {
		std::cout << "Invalid time" << std::endl;
		return;
	}
	if (isBusy(d, t, p.sum(t))) {
		std::cout << "There is another appointment at that time you can delete it with another function" << std::endl;
		return;
	}
	std::cout << "Enter name " << std::endl;
	char* tm=new char[50];
	std::cin.getline(tm, '\0');
	Appointment appNew;
	appNew.setDate(d);
	appNew.setStartTime(t);
	appNew.setEndTime(t.sum(p));
	appNew.setName(tm);
	char* txt = new char[1000];
	std::cin.getline(txt, '\0');
	appNew.setComments(txt);
	delete[] tm;
	delete[] txt;
	createAPP(appNew);
}

void Calendar:: chooseTimeAndAddApp()
{
	bool choise = 0;
	std::cout << "If you are sure that the time is free press 0 if it is busy or invalid the program will not make an appointment." << std::endl;
	std::cin >> choise;
	if (choise == 0) {
		createNewApp();
		return;
	}
	else {
		int want = 1;
		std::cout << "Enter date you want (day.month.year)" << std::endl;
		Date d;
		unsigned nb = 0;
		std::cin >> nb;
		d.setDay(nb);
		std::cin >> nb;
		d.setMonth(nb);
		std::cin >> nb;
		d.setYear(nb);
		if (d.validDate() == 0) {
			std::cout << "Invalid date" << std::endl;
			return;
		}
		std::cout << "Enter duration of appointment" << std::endl;
		Time p;
		size_t hours = 0, minutes = 0, seconds = 0;
		std::cin >> hours;
		std::cin >> minutes;
		std::cin >> seconds;
		p.setHours(hours);
		p.setMinutes(minutes);
		p.setSeconds(seconds);
		if (p.isValidTime() == 0) {
			std::cout << "Invalid time" << std::endl;
			return;
		}
		Time aTry;
		aTry = findStartingHour(d,p);
		if (aTry.isValidTime() == 0) {
			std::cout << "The day you entered is busy if you want you can put it in that date:" << std::endl;
			d = findByPeriod(d, p);
			d.printNoTime();
			std::cout << "If this date is not suitable for you and want to end this procedure press 0" << std::endl;
			std::cout << "If this date is suitable for you press 1" << std::endl;
			std::cout << "If this date is not suitable for you but you want to continue searching press 2" << std::endl;
			std::cin >> want;
			if (want == 0) {
				std::cout << "The appointment could not be created" << std::endl;
				return;
			}
			if (want == 2) {
				d=d.daysLater(1);
				d = findByPeriod(d, p);
				d.printNoTime();
			}
		}
		if (want != 1) {
			std::cout << "Possible time for appointment is" << std::endl;
			aTry = d.getTime();
			d.printD();
			std::cout << "If starting hour is suitable for you press 1" << std::endl;
			std::cout << "If starting hour is not suitable for you press 0" << std::endl;
			std::cin >> want;
			if (want == 0) {
				std::cout << "Enter starting hour you want (hour:minutes:seconds)" << std::endl;
				Time t;
				std::cin >> hours;
				std::cin >> minutes;
				std::cin >> seconds;
				t.setHours(hours);
				t.setMinutes(minutes);
				t.setSeconds(seconds);
				if (t.isValidTime() == 0) {
					std::cout << "Invalid time" << std::endl;
					return;
				}
				if (isBusy(d, t, p.sum(t))) {
					std::cout << "There is another appointment at that time you can delete it with another function. The appointment could not be created." << std::endl;
					return;
				}
				aTry = t;
			}
		}
		std::cout << "Enter name ";
		char* tm = new char[50];
		std::cin.getline(tm, '\0');
		Appointment appNew;
		appNew.setStartTime(aTry);
		appNew.setEndTime(aTry.sum(p));
		aTry.createTime(0, 0, 0, 0);
		d.setTime(aTry);
		appNew.setDate(d);
		appNew.setName(tm);
		char* txt = new char[1000];
		std::cin.getline(txt,'\0');
		appNew.setComments(txt);
		delete[] tm;
		delete[] txt;
		createAPP(appNew);
	}	
}

void Calendar::removeApp(Appointment& oldApp)
{
	if (oldApp.getDate().validDate()&&tooEarly(oldApp.getStartTime())==0&& tooLate(oldApp.getEndTime()) == 0) {
		std::fstream myfile(myFile);
		size_t currg = myfile.tellg();
		int numberApps = 0;
		myfile.seekg(0, std::ios::beg);
		Appointment copy;
		while (!myfile.eof()) {
			size_t position = myfile.tellg();
			Appointment app;
			myfile.read((char*)&app.date, sizeof(Date));
			size_t sizeOfName = 0;
			myfile.read((char*)&sizeOfName, sizeof(sizeOfName));
			app.name = new char[sizeOfName + 1];
			myfile.read((char*)&app.name, sizeOfName);
			app.name.concat("\0");
			size_t sizeCom = 0;
			myfile.read((char*)&sizeCom, sizeof(sizeCom));
			app.comments[sizeCom + 1];
			myfile.read((char*)&app.comments, sizeCom);
			app.comments.concat("\0");
			myfile.read((char*)&app.startTime, sizeof(Time));
			myfile.read((char*)&app.endTime, sizeof(Time));
			if (app.getDate().compareNoTime(oldApp.getDate()) && app.hasSameName(oldApp.getName()) == 1) {
				oldApp.deleteAppontment();
				oldApp.setDate(app.getDate());
				rewriteFile(myfile, position + 1, oldApp);
				myfile.seekg(0, std::ios::end);
			}
		}
		myfile.seekg(currg, std::ios::beg);
		myfile.close();
	}
	else {
		std::cout << "Invalid input" << std::endl;;
	}
}

void Calendar::interface() {
	int choice = 0;
	Time st(getWorkBegins());
	std::cout << "The start of working day is" << std::endl; st.print(); std::cout << std::endl;
	st = getWorkEnds();
	std::cout << "The end of working day is" << std::endl; st.print(); std::cout << std::endl;
	std::cout << "If you want to add appointment enter 1" << std::endl;
	std::cout << "If you want to remove existing appointment enter 2 and be sure everything you add is correct " << std::endl;
	std::cout << "If you want the workload for a given day enter 3" << std::endl;
	std::cout << "If you want to change appointment enter 4" << std::endl;
	std::cout << "If you want to search for an appointment enter 5" << std::endl;
	std::cout << "If you want the workload for a given week enter 6" << std::endl;
	std::cout << "If you want to find suitable time for a date enter 7" << std::endl;
	std::cout << "If you want to continue enter 8 otherwise the program will be closed authomatically" << std::endl;
	std::cout << "If you want to change starting hour of working time enter 9" << std::endl;
	std::cout << "If you want to change starting hour of working time enter 10" << std::endl;
	std::cin >> choice;
	if (choice == 1) {
		chooseTimeAndAddApp();
		std::cout<<"If you wish to continue using the program press 8" << std::endl;
		std::cin >> choice;
	}
	if (choice == 2) {
		int way = 0;
		std::cout << "If you want to point appointment by date and starting time press1" << std::endl;
		std::cout << "If you want to point appointment by name press 2 and be sure the name is the same" << std::endl;
		std::cout << "If you want to point appointment by comment and be sure the comment is the same press 3" << std::endl;
		std::cin >> way;
		if (way == 1) {
			std::cin.ignore();
			std::cout << "Enter date appointment was (day.month.year)" << std::endl;
			Date d;
			unsigned nb = 0;
			std::cin >> nb;
			d.setDay(nb);
			std::cin >> nb;
			d.setMonth(nb);
			std::cin >> nb;
			d.setYear(nb);
			if (d.validDate() == 1) {
				std::cout << "Invalid date" << std::endl;
			}
			else {
				char* tm = new char[25];
				size_t st = 0;
				std::cout << "Enter starting hour you want (hour:minutes:seconds)" << std::endl;
				Time t;
				size_t hours = 0, minutes = 0, seconds = 0;
				std::cin >> hours;
				std::cin >> minutes;
				std::cin >> seconds;
				t.setHours(hours);
				t.setMinutes(minutes);
				t.setSeconds(seconds);
				if (t.isValidTime() == 0) {
					std::cout << "Invalid time" << std::endl;
				}
				else {
					Appointment* tapps = findByDate(d);
					size_t sz = sizeof(tapps) / sizeof(Appointment);
					for (size_t j = 0; j < sz; j++) {
						if (tapps[j].getDate().compareNoTime(d) == 0 && tapps[j].getStartTime().compare(t) == 0) {
							removeApp(tapps[j]);
						}
					}
				}
			}
		}
		else if (way == 2) {
			char* tm = new char[25];
			std::cout << "Enter name " << std::endl;
			std::cin.getline(tm, '\n');
			Appointment* apps = findAppointmentsByName(tm);
			removeApp(apps[0]);
			delete[] tm;
		}
		else if (way == 3) {
			int i = 0, j = 0;
			char* tm = new char[25];
			char* txt = new char [1000];
			std::cin.getline(txt, '\0');
			Appointment* apps = findAppointmentsByCom(txt);
			removeApp(apps[0]);
			delete[] tm;
			delete[] txt;
		}
		else {
			std::cout << "Wrong input" << std::endl;
		}
		std::cout << "If you wish to continue using the program press 8" << std::endl;
		std::cin >> choice;
	}
	if (choice == 3) {
		std::cout << "Enter date  (day.month.year)" << std::endl;
		Date d;
		unsigned nb = 0;
		std::cin >> nb;
		d.setDay(nb);
		std::cin >> nb;
		d.setMonth(nb);
		std::cin >> nb;
		d.setYear(nb);
		if (d.validDate() == 1) {
			std::cout << "Invalid date" << std::endl;
		}
		else {
			getWorkload(d);
		}
		std::cout << "If you wish to continue using the program press 8" << std::endl;
		std::cin >> choice;
	}
	if (choice == 4) {
		changeApp();
		std::cout << "If you wish to continue using the program press 8" << std::endl;
		std::cin >> choice;
	}
	if (choice == 5) {
		findAppointments(*this);
		std::cout << "If you wish to continue using the program press 8" << std::endl;
		std::cin >> choice;
	}
	if (choice == 6) {
		std::cout << "Enter the date from wich the weeks to be calculated on(day.month.year)" << std::endl;
		Date d;
		unsigned nb = 0;
		std::cin >> nb;
		d.setDay(nb);
		std::cin >> nb;
		d.setMonth(nb);
		std::cin >> nb;
		d.setYear(nb);
		if (d.validDate() == 0) {
			std::cout << "Invalid date" << std::endl;
		}
		else {
			std::cout << "Enter the date to wich the weeks to be calculated (day.month.year)" << std::endl;
			Date e;
			std::cin >> nb;
			e.setDay(nb);
			std::cin >> nb;
			e.setMonth(nb);
			std::cin >> nb;
			e.setYear(nb);
			if (e.validDate() == 0) {
				std::cout << "Invalid date" << std::endl;
			}
			else {
				workload(d, e);
			}
		}
		std::cout << "If you wish to continue using the program press 8" << std::endl;
		std::cin >> choice;
	}
	if (choice == 7) {
		std::cout << "Enter date  (day.month.year)" << std::endl;
		Date d;
		unsigned nb = 0;
		std::cin >> nb;
		d.setDay(nb);
		std::cin >> nb;
		d.setMonth(nb);
		std::cin >> nb;
		Time p;
		size_t hours = 0, minutes = 0, seconds = 0;
		std::cin >> hours;
		std::cin >> minutes;
		std::cin >> seconds;
		p.setHours(hours);
		p.setMinutes(minutes);
		p.setSeconds(seconds);
		if (p.isValidTime() == 0 && d.validDate() == 0) {
			std::cout << "Invalid time or date";
		}
		else {
			d = findByPeriod(d,p);
			d.printD();
		}
		std::cout << "If you wish to continue using the program press 8" << std::endl;
		std::cin >> choice;
	}
	if (choice == 9) {
		std::cout << "Changing the start of the working day" << std::endl;
		size_t nm = 0;
		std::cout << "Eneter hour" << std::endl;
		std::cin >> nm;
		st.setHours(nm);
		std::cout << "Eneter minutes" << std::endl;
		std::cin >> nm;
		st.setMinutes(nm);
		std::cout << "Eneter seconds" << std::endl;
		std::cin >> nm;
		st.setSeconds(nm);
		setWorkBegins(st);
		std::cout << "If you wish to continue using the program press 8" << std::endl;
		std::cin >> choice;
	}
	if (choice == 10) {
		std::cout << "Changing the end of the working day" << std::endl;
		size_t nm = 0;
		std::cout << "Eneter hour" << std::endl;
		std::cin >> nm;
		st.setHours(nm);
		std::cout << "Eneter minutes" << std::endl;
		std::cin >> nm;
		st.setMinutes(nm);
		std::cout << "Eneter seconds" << std::endl;
		std::cin >> nm;
		st.setSeconds(nm);
		setWorkEnds(st);
		std::cout << "If you wish to continue using the program press 8" << std::endl;
		std::cin >> choice;
	}
	if (choice == 8) {
		interface();
	}
}
