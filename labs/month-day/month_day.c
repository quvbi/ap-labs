#include <stdio.h>

/* month_day function's prototype*/
void month_day(int year, int yearday, int *pmonth, int *pday){


}

int leapyear(char cyear)
{
    int year = (int) cyear;
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int main(int argc, int **argv){
int year = atoi(argv[1]);
int dayear = atoi (argv[2]);
int month = 0;
int daymonth = 0;
    
        if(dayear <= 31){
             month = 1;
            daymonth = (int) dayear;

        } else  if(dayear <= (59 + leapyear(year))){
             month = 2;
            daymonth = dayear - 31;

        } else  if(dayear <= (90 + leapyear(year))){
             month = 3;
             daymonth = dayear - (59 + leapyear(year));
        } else  if(dayear <= (120 + leapyear(year))){
             month = 4;
             daymonth = dayear - (90 + leapyear(year));
        } else  if(dayear <= (151 + leapyear(year))){
             month = 5;
            daymonth =dayear - (120 + leapyear(year));

        } else  if(dayear <= (181 + leapyear(year))){
             month = 6;
             daymonth = dayear - (151 + leapyear(year));;
        } else  if(dayear <= (212 + leapyear(year))){
             month = 7;
             daymonth = dayear - (181 + leapyear(year));

        } else  if(dayear <= (243 + leapyear(year))){
             month = 8;
             daymonth = dayear - (212 + leapyear(year));

        } else  if(dayear <= (273 + leapyear(year))){
             month = 9;
             daymonth = dayear - (243 + leapyear(year));


        } else  if(dayear <= (304 + leapyear(year))){
             month = 10;
            daymonth = dayear - (273 + leapyear(year));

        } else  if(dayear <= (334 + leapyear(year))){
             month = 11;
         daymonth =  dayear - (304 + leapyear(year));

        } else  if(dayear <= (365 + leapyear(year))){
             month = 12;
        daymonth = dayear - (334 + leapyear(year));

        }  else {
            printf("No existe este dia");
            return 0;
        }
    char* monthstr;

      if(month == 1){
       monthstr = "Jan";
    } else if(month == 2){
       monthstr = "Feb";
    } else if(month == 3){
       monthstr = "Mar";
    }else if(month == 4){
       monthstr = "Apr";
    }else if(month == 5){
       monthstr = "May";
    }else if(month == 6){
       monthstr = "Jun";
    }else if(month == 7){
       monthstr = "Jul";
    }else if(month == 8){
       monthstr = "Agu";
    }else if(month == 9){
       monthstr = "Sep";
    }else if(month == 10){
       monthstr = "Oct";
    }else if(month == 11){
       monthstr = "Nov";
    }else if(month == 12){
       monthstr = "Dic";
    }

   if(daymonth < 10){
    printf("%s 0%d, %d", monthstr, daymonth, year); 
   } else {
       printf("%s %d, %d", monthstr, daymonth, year);    
   }

        return 0;
}
