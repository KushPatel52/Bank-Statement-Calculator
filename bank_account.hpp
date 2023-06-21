#pragma once
#include <string>

void deposit(double & balance, double amount);

bool withdraw(double & balance, double amount);

void overdraft(double & balance, double amount);

double round_down_to_cent(double amount);

double interest_for_month(double balance, double apr);
void string_date_to_ints(const std::string & date,
                         int & year,
                         int & month,
                         int & day);

void string_date_to_int_ptrs(const std::string & date,
                             int * const year,
                             int * const month,
                             int * const day);

int number_of_first_of_months(const std::string & date_early,
                              const std::string & date_late);

double interest_earned(double balance, double apr,
    const std::string & date_early, const std::string & date_late);

std::string process_command(const std::string & line, std::string & previous_date,
    double & balance, double apr);


std::string process_commands(const std::string & input, double apr);
