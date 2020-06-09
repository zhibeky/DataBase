//DataBase
#include <iostream>
#include <set>
#include <map>
#include <algorithm>
#include <iomanip>
#include <sstream>
#include <exception>
using namespace std;

class Date {
  int year;
  int month;
  int day;
public:
  explicit Date() {
    year = 0;
    month = 0;
    day = 0;
  }
  explicit Date(int new_year, int new_month, int new_day) {
    year = new_year;
    if(new_month < 1 || new_month > 12) {
      throw runtime_error("Month value is invalid: " + to_string(new_month));
    }
    month = new_month;
    if(new_day < 1 || new_day > 31) {
      throw runtime_error("Day value is invalid: " + to_string(new_day));
    }
    day = new_day;
  }
  int GetYear() const { return year; }
  int GetMonth() const { return month; }
  int GetDay() const { return day; }
};
set<string> FindSet( map<Date, set<string>> &DataBase, Date &date );
void PrintSet( const set<string> &s );
istream& operator>>(istream& stream, Date& date );
ostream& operator<<(ostream& stream, const Date& data );
bool operator<(const Date& date1, const Date& date2 );


ostream &operator<<(ostream& stream, const Date& date) {
  stream << setw(4) << setfill('0') << date.GetYear() << '-'
         << setw(2) << setfill('0') << date.GetMonth() << '-'
         << setw(2) << setfill('0') << date.GetDay();
  return stream;
}

bool operator<(const Date& date1, const Date& date2) {
  if(date1.GetYear() == date2.GetYear()) {
    if(date1.GetMonth() == date2.GetMonth()) {
      return (date1.GetDay() < date2.GetDay());
    }
    return (date1.GetMonth() < date2.GetMonth());
  }
  return (date1.GetYear() < date2.GetYear());
}

bool EnsureAndIgnore(istream& stream, string &date_str) {
  if(stream.peek() != '-') {
    return false;
  }
  stream.ignore(1);
  return true;
}

void PrintSet( const set<string> &s ) {

  if(!s.empty()){
    for(const auto& str : s) {
      cout << str << endl;
    }
  }
}
void PrintPairs(const Date &date, const set<string> &s) {
  for(const auto &i : s){
    cout << date << " " << i << endl;
  }
}
Date ParseDate(string date_str) {
  stringstream s(date_str);
  int year, month, day;
  bool b = true;
    if(s >> year && b) {
      b = true;
      if(!EnsureAndIgnore(s, date_str)) {
        throw runtime_error("Wrong date format: " + date_str);
      }
    }
    else {
      b = false;
    }
    if(s.peek() == EOF) {
      throw runtime_error("Wrong date format: " + date_str);
    }
    if(s >> month && b) {
      b = true;
      if(!EnsureAndIgnore(s, date_str)){
        throw runtime_error("Wrong date format: " + date_str);
      }
    }
    else {
      b = false;
    }
    if(s.peek() == EOF) {
      throw runtime_error("Wrong date format: " + date_str);
    }
    if(s >> day && b)
      b = true;
    if(!s.eof()) throw runtime_error("Wrong date format: " + date_str);
    if(!b) throw runtime_error("Wrong date format: " + date_str);

    return Date{year, month, day};
}

class DataBase {
  map<Date, set<string>> DB;
public:

  DataBase() {}

  void AddEvent(const Date& date, const string& event) { DB[date].insert(event); }

  int DeleteDate(Date &date) {
    int count = 0;
    for(const auto& i : DB[date]) {
      count++;
    }
    DB[date].clear();
    return count;
  }

  set<string> FindDate(const Date &date) {
    if(DB.count(date) != 0) return DB[date];
    else {
      set<string> s;
      return s;
    }
  }

  void Print() {
    for(const auto& data : DB)
      if(DB.count(data.first) != 0 && !DB[data.first].empty()) {
        PrintPairs(data.first, data.second);
      }
  }

  bool DeleteEvent(const Date& date, const string& event) {
    if(DB.count(date) == 0 || DB.at(date).empty())
      return false;
    else {
      for(const auto& i : DB[date]) {
        if(i == event) {
          DB[date].erase(event);
          return true;
        }
      }
      return false;
    }
  }
};

void ParseCommands(const string& command, DataBase& db) {
  try {
  stringstream stream(command);
  Date date;
  string com = "", event = "", date_str = "";
  if(stream >> com) {
    if(com == "Add") {
      if(stream >> date_str) {
        bool t = false;
        try {
        date = ParseDate(date_str);
      } catch (runtime_error &ex) {
        cout << ex.what() << endl;
        t = true;
      }
      if(t == false){
          if(stream >> event) db.AddEvent(date, event);
          else {
            stringstream ss;
            ss << "Wrong date format: " << date;
            throw runtime_error(ss.str());
          }
        }
      }
    }
    else if(com == "Find") {
      if(stream >> date_str) {
        date = ParseDate(date_str);
        PrintSet(db.FindDate(date));
      }
    }
    else if(com == "Print") db.Print();

    else if(com == "Del") {
      if(stream >> date_str) {
        date = ParseDate(date_str);
        if(stream >> event) {
          cout << (db.DeleteEvent(date, event) ? "Deleted successfully" : "Event not found");
          cout << endl;
        }
        else cout << "Deleted " << db.DeleteDate(date) << " events" << endl;
      }
    }
    else {
      stringstream ss;
      ss << "Unknown command: " << com;
      throw logic_error(ss.str());
    }
  }
} catch(runtime_error& ex) {
  cout << ex.what() << endl;
}
}

int main() {
  try {
    DataBase db;
    string command;
    while(getline(cin, command))
      ParseCommands(command, db);
  } catch(exception& ex) {
    cout << ex.what() << endl;
  }
  return 0;
}
