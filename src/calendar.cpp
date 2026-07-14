#include "../include/calendar.h"


/*

  Index     Day
  0         Sunday
  1         Monday
  2         Tuesday
  3         Wednesday
  4         Thursday
  5         Friday
  6         Saturday*/
int dayNumber(int day, int month, int year) {

    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1,
                      4, 6, 2, 4};
    year -= month < 3;
    return (year + year / 4 - year / 100 +
            year / 400 + t[month - 1] + day) %
           7;
}

/*
  A Function that returns the name of the month
  with a given month number

  Month Number     Name
  0                January
  1                February
  2                March
  3                April
  4                May
  5                June
  6                July
  7                August
  8                September
  9                October
  10               November
  11               December */
std::string getMonthName(int monthNumber) {
    std::string months[] = {
        "╔════════════════ January ════════════════╗",
        "╔════════════════ February ═══════════════╗",
        "╔═════════════════ March ═════════════════╗",
        "╔═════════════════ April ═════════════════╗",
        "╔══════════════════ May ══════════════════╗",
        "╔══════════════════ June ═════════════════╗",
        "╔══════════════════ July ═════════════════╗",
        "╔═════════════════ August ════════════════╗",
        "╔═══════════════ September ═══════════════╗",
        "╔════════════════ October ════════════════╗",
        "╔════════════════ November ═══════════════╗",
        "╔════════════════ December ═══════════════╗"};

    return (months[monthNumber]);
}

/* A Function to return the number of days in
   a month

  Month Number     Name        Number of Days
  0                January     31
  1                February    28 (non-leap) / 29 (leap)
  2                March       31
  3                April       30
  4                May         31
  5                June        30
  6                July        31
  7                August      31
  8                September   30
  9                October     31
  10               November    30
  11               December    31

*/
int numberOfDays(int monthNumber, int year) {
    // January
    if (monthNumber == 0)
        return (31);

    // February
    if (monthNumber == 1) {
        // If the year is leap then February has
        // 29 days
        if (year % 400 == 0 ||
            (year % 4 == 0 && year % 100 != 0))
            return (29);
        else
            return (28);
    }

    // March
    if (monthNumber == 2)
        return (31);

    // April
    if (monthNumber == 3)
        return (30);

    // May
    if (monthNumber == 4)
        return (31);

    // June
    if (monthNumber == 5)
        return (30);

    // July
    if (monthNumber == 6)
        return (31);

    // August
    if (monthNumber == 7)
        return (31);

    // September
    if (monthNumber == 8)
        return (30);

    // October
    if (monthNumber == 9)
        return (31);

    // November
    if (monthNumber == 10)
        return (30);

    // December
    if (monthNumber == 11)
        return (31);
    return 0;
}

void printCalendar(int year) {
    printf("         Calendar - %d\n\n", year);
    int days;


    int current = dayNumber(1, 1, year);


    for (int i = 0; i < 12; i++) {
        days = numberOfDays(i, year);

        printf("\n%s\n", getMonthName(i).c_str());

        printf("╟─────┬─────┬─────┬─────┬─────┬─────┬─────╢\n");
        printf("║ Sun │ Mon │ Tue │ Wed │ Thu │ Fri │ Sat ║\n");
        printf("╟─────┼─────┼─────┼─────┼─────┼─────┼─────╢\n");

        int k = current;
        
        printf("║");


        for (int p = 0; p < k; p++) {
            printf("     │");
        }

        for (int j = 1; j <= days; j++) {

            printf(" %2d  ", j);
            k++;

            if (k > 6) {

                printf("║\n"); 

                if (j < days) {
                    printf("╟─────┼─────┼─────┼─────┼─────┼─────┼─────╢\n");
                    printf("║"); 
                }
                k = 0;
            } else {

                printf("│"); 
            }
        }

        if (k > 0) {
            for (int p = k; p <= 6; p++) {
                printf("     ");
                if (p < 6) printf("│");
            }
            printf("║\n"); 
        }

  
        printf("╚═════╧═════╧═════╧═════╧═════╧═════╧═════╝\n");

        current = k;
    }

    return;
}
void printCurrentMonth() {

    time_t t = time(0);
    tm* now = localtime(&t);
    

    int year = now->tm_year + 1900;
    int monthIndex = now->tm_mon; 
    int day = now->tm_mday;

    


    int days = numberOfDays(monthIndex, year);


    int current = dayNumber(1, monthIndex + 1, year);


    printf("\n%s\n", getMonthName(monthIndex).c_str());


    printf("╟─────┬─────┬─────┬─────┬─────┬─────┬─────╢\n");
    printf("║ Sun │ Mon │ Tue │ Wed │ Thu │ Fri │ Sat ║\n");
    printf("╟─────┼─────┼─────┼─────┼─────┼─────┼─────╢\n");

    int k = current;
    

    printf("║");

  
    for (int p = 0; p < k; p++) {
        printf("     │");
    }


    for (int j = 1; j <= days; j++) {

        if(j == day){
            printf("\033[36m %2d  \033[39m", j);
        }else{
            printf(" %2d  ", j);
        }
        k++;

        if (k > 6) {
            printf("║\n"); 
            

            if (j < days) {
                printf("╟─────┼─────┼─────┼─────┼─────┼─────┼─────╢\n");
                printf("║"); 
            }
            k = 0;
        } else {

            printf("│"); 
        }
    }


    if (k > 0) {
        for (int p = k; p <= 6; p++) {
            printf("     ");
            if (p < 6) printf("│");
        }
        printf("║\n"); 
    }

    printf("╚═════╧═════╧═════╧═════╧═════╧═════╧═════╝\n");
}

std::vector<std::string> get_month_vector() {
    time_t t = time(0);
    tm* now = localtime(&t);
    std::vector<std::string> result;

    int year = now->tm_year + 1900;
    int monthIndex = now->tm_mon; 
    int day = now->tm_mday;

    int days = numberOfDays(monthIndex, year);
    int current = dayNumber(1, monthIndex + 1, year);

    // Top headers
    result.push_back(getMonthName(monthIndex));
    result.push_back("╟─────┬─────┬─────┬─────┬─────┬─────┬─────╢");
    result.push_back("║ Sun │ Mon │ Tue │ Wed │ Thu │ Fri │ Sat ║");
    result.push_back("╟─────┼─────┼─────┼─────┼─────┼─────┼─────╢");

    int k = current;
    std::string current_line = "║";

    // Initial padding for the first week
    for (int p = 0; p < k; p++) {
        current_line += "     │";
    }

    // Days loop
    for (int j = 1; j <= days; j++) {
        char buf[64];
        if (j == day) {
            // ANSI cyan for current day
            snprintf(buf, sizeof(buf), "\033[36m %2d  \033[38;2;158;72;68m", j);
        } else {

            snprintf(buf, sizeof(buf), "\033[38;2;158;72;68m %2d  ", j);
        }
        
        current_line += buf;
        k++;
 
        if (k > 6) {
            current_line += "║";
            result.push_back(current_line); // Push the completed week
            
            if (j < days) {
                result.push_back("╟─────┼─────┼─────┼─────┼─────┼─────┼─────╢");
                current_line = "║";// start the next week
            }
            k = 0;
        } else {
            current_line += "│"; 
        }
    }

    if (k > 0) {
        for (int p = k; p <= 6; p++) {
            current_line += "     ";
            if (p < 6) current_line += "│";
        }
        current_line += "║";
        result.push_back(current_line);
    }

    // Bottom border
    result.push_back("╚═════╧═════╧═════╧═════╧═════╧═════╧═════╝");

    return result;
}
