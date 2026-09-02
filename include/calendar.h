#include <bits/stdc++.h>
#include <ctime>
#include <vector>
#include "cstdio"

int dayNumber(int day, int month, int year);
std::string getMonthName(int monthNumber);
int numberOfDays(int monthNumber, int year);
void printCalendar(int year);
void printCurrentMonth();
std::vector<std::string> get_month_vector(const std::string& color = "\033[38;2;158;72;68m",
                                           const std::string& today_color = "\033[36m");