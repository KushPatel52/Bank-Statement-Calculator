#include <iostream>
using std::endl;
#include <iomanip>
#include <string>
using std::string;
#include <cmath>
#include <sstream>
#include <cassert>

#include "bank_account.hpp"

void deposit(double & balance, double amount) {
  balance += amount;
}

bool withdraw(double & balance, double amount) {
  bool can_withdraw = balance > amount;
  if (can_withdraw) {
    balance -= amount;
  }
  return can_withdraw;
}

void overdraft(double & balance, double amount) {
  const double overdraft_fee = 35;
  balance -= overdraft_fee;
  balance -= amount;
}

double round_down_to_cent(double amount) {
  amount = floor(amount * 100);
  return amount / 100;
}

double interest_for_month(double balance, double apr) {
  if (balance < 0) return 0;
  double month_apr = (apr / 100) / 12;
  double interest = balance * month_apr;
  return round_down_to_cent(interest);
}

void string_date_to_ints(const string & date,
                         int & year,
                         int & month,
                         int & day) {
  year = std::stoi(date.substr(0, 4));
  month = std::stoi(date.substr(5, 2));
  day = std::stoi(date.substr(8, 2));
}

void string_date_to_int_ptrs(const string & date,
                             int * const year,
                             int * const month,
                             int * const day) {
  string_date_to_ints(date, *year, *month, *day);
}

int number_of_first_of_months(const string & date_early,
                              const string & date_late) {
  int year_early, month_early, day_early;
  int year_late, month_late, day_late;
  string_date_to_ints(date_early, year_early, month_early, day_early);
  string_date_to_ints(date_late, year_late, month_late, day_late);

  int first_of_months = 0;

  first_of_months += 12 * (year_late - year_early);
  first_of_months += month_late - month_early;

  return first_of_months;
}

double interest_earned(double balance, double apr,
    const string & date_early, const string & date_late) {
  int compounded_months = number_of_first_of_months(date_early, date_late);
  double new_balance = balance;
  for (int i = 0; i < compounded_months; ++i) {
    new_balance += interest_for_month(new_balance, apr);
  }
  return new_balance - balance;
}

string process_command(const string & line, string & previous_date,
    double & balance, double apr) {
  std::stringstream stream;
  stream << std::fixed << std::setprecision(2);
  string current_date = line.substr(0, 10);
  string instruction = line.substr(11);

  stream << "On " << current_date << ": Instructed to perform \""
         << instruction << "\"" << endl;

  if (previous_date != "") {
    int first_months = number_of_first_of_months(previous_date, current_date);
    if (first_months) {
      stream << "Since " << previous_date << ", interest has accrued "
             << first_months << " times." << endl;
      double interest = interest_earned(balance, apr,
          previous_date, current_date);
      balance += interest;
      stream << '$' << interest << " interest has been earned." << endl;
    }
  }

  string::size_type space_pos = instruction.find(' ');
  assert(space_pos != string::npos);
  string command(instruction, 0, space_pos);
  string amount_str(instruction, space_pos + 2);
  double amount = std::stod(amount_str);

  if (command == "Deposit") {
    deposit(balance, amount);
  } else if (command == "Withdraw") {
    bool success = withdraw(balance, amount);
    if (!success) {
      stream << "Overdraft!" << endl;
      overdraft(balance, amount);
    }
  }

  stream << "Balance: " << balance << endl;

  previous_date.assign(current_date);
  return stream.str();
}


string process_commands(const string & input, double apr) {
  double balance = 0;
  string previous_date;
  string result;
  string copy = input;
  string::size_type pos = 0;
  string line;

  while ((pos = copy.find('\n')) != string::npos) {
    line = copy.substr(0, pos);
    copy.erase(0, pos + 1);
    result += process_command(line, previous_date, balance, apr);
  }
  return result;
}
