#include"Calendar.h"

Calendar::Calendar()
{
	createFile();
	Time st(0, 0, 8, 0);
	workBegins = st;
	Time ed(0, 0, 18, 0);
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
	return (getWorkBegins().compare(time)<0);
}

bool Calendar::tooLate(const Time& time) const
{
	return (getWorkEnds().compare(time) > 0);
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
	Appointment* apps = new Appointment[MAXS];
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

Date& Calendar::findByTime(const Date& d, const Time& st, const Time& ed)
{
	if (isInTime(st, ed)) {
		Date copy(d);
		while (isBusy(copy, st, ed)) {
			copy = copy.daysLater(1);
		}
		return copy;
	}
}

Time& Calendar::findStartingHour(const Date& date, const Time& period) const
{
	Appointment* apps = new Appointment[MAXS];
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

Date& Calendar::findByPeriod(const Date& date, const Time& period)
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
	Appointment* apps = new Appointment[MAXS];
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

Appointment*& Calendar::findAppointmentsByName(char* name) const
{
	Appointment* apps = new Appointment[MAXS];
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
		if (app.compareNames(name) == 1) {
			apps[numberApps] = app;
			numberApps++;
		}
	}
	myfile.seekg(currg, std::ios::beg);
	myfile.close();
	return apps;
}

Appointment*& Calendar::findAppointmentsBytextInCom(char* com) const
{
	Appointment* apps = new Appointment[MAXS];
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
		if (app.hasCommentInComment(com) == 1) {
			apps[numberApps] = app;
			numberApps++;
		}
	}
	myfile.seekg(currg, std::ios::beg);
	myfile.close();
	return apps;
}

Appointment*& Calendar::findAppointmentsByCom(char** com) const
{
	Appointment* apps = new Appointment[MAXS];
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
		if (app.compareComments(com) == 1) {
			apps[numberApps] = app;
			numberApps++;
		}
	}
	myfile.seekg(currg, std::ios::beg);
	myfile.close();
	return apps;
}

Appointment*& findAppointments(Calendar& calendar)
{
	char* comment = new char[MAXS];
	std::cout << "Enter comments";
	std::cin.ignore();
	std::cin.getline(comment, '\0');
	Appointment* apps = new Appointment[MAXS];
	Appointment* app = new Appointment[MAXS];
	Appointment* res = new Appointment[MAXS];
	app = calendar.findAppointmentsBytextInCom(comment);
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
	char* name = new char[MAXS];
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
	Appointment* apps = new Appointment[MAXS];
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
	sortAppointments(apps, 0, numberApps);
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
	date.print();
	std::cout << std::endl;
	Appointment* app = new Appointment[MAXS];
	app = findByDate(date);
	size_t size = sizeof(app) / sizeof(Appointment);
	for (size_t i = 0; i <= size; i++) {
		app[i].printAppointment();
	}
	std::cout << std::endl;
}

void Calendar::changeAppName(char* oldName,char* newName)
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
		if (app.hasSameName(oldName) == 1) {
			myfile.seekp(position, std::ios::beg);
			app.setName(newName);
			writeAppFile(myfile, app);
		}
	}
	myfile.seekg(currg, std::ios::beg);
	myfile.close();
	////for interface
	/*std::cout << "Please enter the whole correct name that you want to change";*/
}

void Calendar::changeAppComm(char** oldCom, char** newComm)
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
		if (app.hasSameComment(oldCom) == 1) {
			myfile.seekp(position, std::ios::beg);
			app.setComments(newComm);
			writeAppFile(myfile, app);
		}
	}
	myfile.seekg(currg, std::ios::beg);
	myfile.close();
	////interface to nenter the correct full oldcomments
}

void Calendar::changeAppDate(char* name, const Date& newDate, const Date& oldDate)
{
	////interface if you change the date i will assign the convinient starting time for the  date 
	////if you do not want to change the other starfting time use another function
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
	/// for interface eneter date and old and newstart time
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
	///this makes the app longer insert date and its old endtime and the newtime you want to be the end
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
	size_t choise = 0;
	std::cout << "Which component do you want to change?";
	char* change = new char[MAXNAME];
	std::cin.getline(change, '\n');
	if (strcmp(change, "name") == 0) {
		std::cout << "Enter old name ";
		char* oldName = new char[MAXNAME];
		std::cin.getline(oldName, '\n');
		std::cout << "Enter the new name";
		std::cin.getline(change, '\n');
		changeAppName(oldName, change);
		delete[] oldName;
	}
	else if (strcmp(change, "comment") == 0) {
		std::cout << "Enter old coment ";
		char** oldCom = new char* [MAXCOM];
		char ch = 1;
		size_t line = 0;
		while (ch != '\0' && isValidSymbol(ch)) {
			size_t row = 0;
			while (ch != '\n') {
				oldCom[line][row++] = ch;
			}
			line++;
		}
		std::cout << "Enter old coment ";
		char** newCom = new char* [MAXCOM];
		ch = 1;
		line = 0;
		while (ch != '\0' && isValidSymbol(ch)) {
			size_t row = 0;
			while (ch != '\n') {
				newCom[line][row++] = ch;
			}
			line++;
		}
		changeAppComm(oldCom, newCom);
		delete[] oldCom;
	}
	else if (strcmp(change, "Date") == 0) {
		std::cout << "This function changes only if the date has free time and will change the start time. If you want to be the same try change start time later. If you do not see free time at that date than do not change at all."<<std::endl<<" Enter old name ";
		std::cin.getline(change, '\n');
		std::cout << "Enter the new day,month,year";
		unsigned day = 0, month = 0, year = 0;
		std::cin >> day;
		std::cin >> month;
		std::cin >> year;
		Time t(0, 0, 0, 0);
		Date d(year, month, day,t);
		std::cout << "Enter the old day,month,year";
		std::cin >> day;
		std::cin >> month;
		std::cin >> year;
		Date dOld(year, month, day, t);
		changeAppDate(change, d, dOld);
	}
	else if (strcmp(change, "start day") == 0) {
		std::cout << "This function changes only if possible!"<<std::endl<<"Enter  day, month, year ";
		unsigned day = 0, month = 0, year = 0;
		std::cin >> day;
		std::cin >> month;
		std::cin >> year;
		Time t(0, 0, 0, 0);
		Date d(year, month, day, t);
		std::cout << "Enter the old start time: hours,minutes,seconds";
		size_t hours = 0, minutes = 0, seconds = 0;
		std::cin >> hours;
		std::cin >> minutes;
		std::cin >> seconds;
		t.setHours(hours);
		t.setMinutes(minutes);
		t.setSeconds(seconds);
		d.setTime(t);
		std::cout << "Enter the new start time: hours,minutes,seconds";
		std::cin >> hours;
		std::cin >> minutes;
		std::cin >> seconds;
		t.setHours(hours);
		t.setMinutes(minutes);
		t.setSeconds(seconds);
		changeAppStT(d, d.getTime(), t);
	}
	else if (strcmp(change, "start day") == 0) {
		std::cout << "This function changes the duration of the appointment, if you do not want enter -1 for date and time." << std::endl << "Enter day, month, year " << std::endl;
		unsigned day = 0, month = 0, year = 0;
		std::cin >> day;
		std::cin >> month;
		std::cin >> year;
		Time t(0, 0, 0, 0);
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
}

//void Calendar::createNewApp()
//{
//	std::cin.ignore();
//	std::cout << "Enter date you want (day,month,year)";
//	char* date = new char[25];
//	while()
//
//}









