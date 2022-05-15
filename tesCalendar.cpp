//#include<iostream>
//#include<ctime>
//#pragma warning (disable:4996)
//int main() {
//	std::cout << "Current date is:";
//	time_t now = time(0);
//	char* dt = ctime(&now);
//	std::cout << dt << std::endl;
//	tm* gmtm = gmtime(&now);
//	dt = asctime(gmtm);
//	std::cout << "The UTC date and time is:" << dt;
//	Calendar c;
//	c.interface();
//}



#include"Calendar.h"

int main() {
	Calendar c;
	c.interface();
}