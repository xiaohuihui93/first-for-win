#include <iostream>
using namespace std;
/*
    365 days in a year
    LEAP YEAR: any year divisible by 4 --> LEAP
    EXCEPT:   if year divisible by 100 --> NOT LEAP
    EXCEPT:   if year divisible by 400 --> LEAP
    year = 365.2425 days
    2000 LEAP
    2004 LEAP
    2100 NOT LEAP
    J2000 = Jan. 1, 2000 = 0
    Jan. 2, 2000 = 1
    Jan. 3, 2000 = 2
https://en.wikipedia.org/wiki/Julian_day
http://aa.usno.navy.mil/faq/docs/JD_Formula.php
Look it up, cite it, feel free to use it.  Just make it object oriented.
 */
class Juliandate{
private : int year,month,day,hour,minute,second;
  double jday;
public:
  static int base;
    Juliandate(int year=2017,int month=1,int day=1,int h=20,int min=0,int sec=0):year(year),month(month),day(day),hour(h),minute(min),second(sec){
        jday=0;
                for(int i=0;i<year-2000;i++)
                jday+=testyear(2000+i);

                 for(int j=0;j<month-1;j++)
                     jday+=testmonth(year,month);
                 double hours=sec/3600+min/60.0+h;
                 jday=jday+day-1+hours/24.0;


    }
    int testyear(int testyear){
        if ((testyear%100==0&&testyear%400==0)||(testyear%100!=0&&testyear%4==0))
            return 366;
        else
            return 365;
        }
    int testmonth(int testyear,int testmonth){
        int days_month;
        switch (testmonth) {
        case 1:
            days_month=31;
           break;
        case 2:
            if(Juliandate::testyear(testyear)==366)
                days_month=29;
            else
                days_month=28;
        case 3:
            days_month=31;
        case 4:
            days_month=30;
        case 5:
            days_month=31;
        case 6:
            days_month=30;
        case 7:
            days_month=31;
        case 8:
            days_month=31;
        case 9:
            days_month=30;
        case 10:
            days_month=31;
        case 11:
            days_month=30;
        case 12:
            days_month=31;
        }
        return days_month;

    }


    friend Juliandate operator +(Juliandate a,int b){
         int y=a.year,m=a.month,d=a.day;
         for(int i=0;i<b;i++){
      if(d<a.testmonth(y,m))
              d++;
      else{
          d=1;
         if (m<12)
             m+=1;

         else
         {m=1;
             y+=1;}}}

          return Juliandate(y,m,d,a.hour,a.minute,a.second);
    }
    friend double operator -(Juliandate a,Juliandate b){
        return a.jday-b.jday;
    }
  int  getYear()const{
        return year;
    }
   int getMonth()const{
        return month;
    }
    int getDay()const{
        return day;
    }
    int getHour()const{
        return hour;
    }
     int getMin()const{
        return minute;
    }
    int getSec()const{
        return second;
    }
    friend ostream& operator <<(ostream& s,Juliandate b){
        s<<b.year<<" "<<b.month<<" "<<b.day<<" "<<b.hour<<" "<<b.minute<<" "<<b.second<<endl;
        return s;
    }

};





int main() {
    const Juliandate d1(2017, 2, 13, 20, 00, 00);
    const Juliandate d1b(2017, 2, 13, 22, 00, 00);

    cout << d1b - d1 << '\n'; // 2 hours = 2/24 day

    const Juliandate tomorrow = d1 + 1;
    cout << tomorrow; // 2017 2 14 20 00 00
    cout << tomorrow - d1 << '\n';
    // hh:mm:ss  20/24  20/24.0
    const Juliandate d2(2017, 2, 19); // 00:00:00
  const Juliandate d3 = d2 + 3; // what is it 3 days later?
    double days   = d3 - d1;

    int year = d3.getYear();
    int month = d3.getMonth();
    int day = d3.getDay();
    int hour = d3.getHour();
    int min = d3.getMin();
    int sec = d3.getSec();
    cout << d1 << '\n';
    cout << d2 << '\n';
    cout << d3 << '\n';
    cout << year << " " << month << " " << day <<
        hour << " " << min << " " << sec << '\n';
    // Not every operation makes sense!
    //	JulianDate d5 = d3 * d1;
    Juliandate torture(2017, 2, 27); // 00:00:00
    for (int i = 0; i < 3; i++) {
        cout << torture.getYear() << " " << torture.getMonth() << " " << torture.getDay() << '\n';
        torture = torture + 1;
    }
}
