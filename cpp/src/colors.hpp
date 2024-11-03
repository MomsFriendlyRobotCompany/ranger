/**************************************************\
* The MIT License (MIT)
* Copyright (c) 2014 Kevin Walchko
* see LICENSE for full details
\**************************************************
* This is based off the python model colorama syntax.
* It should only work on Unix/Linux/macOS
*
* Examples:
* cout << Fore.RED << "hello" << Fore.RESET << endl;
* printf("%shello%s", Fore.RED.c_str(), Fore.RESET.c_str());
*/

#pragma once

// #include <string>

// foreground colors
constexpr char FG_BLACK[]   = "\x1b[30m";
constexpr char FG_RED[]     = "\x1b[31m";
constexpr char FG_GREEN[]   = "\x1b[32m";
constexpr char FG_YELLOW[]  = "\x1b[33m";
constexpr char FG_BLUE[]    = "\x1b[34m";
constexpr char FG_MAGENTA[] = "\x1b[35m";
constexpr char FG_CYAN[]    = "\x1b[36m";
constexpr char FG_WHITE[]   = "\x1b[37m";
constexpr char FG_RESET[]   = "\x1b[0m";

// background colors
constexpr char BG_BLACK[]   = "\x1b[40m";
constexpr char BG_RED[]     = "\x1b[41m";
constexpr char BG_GREEN[]   = "\x1b[42m";
constexpr char BG_YELLOW[]  = "\x1b[43m";
constexpr char BG_BLUE[]    = "\x1b[44m";
constexpr char BG_MAGENTA[] = "\x1b[45m";
constexpr char BG_CYAN[]    = "\x1b[46m";
constexpr char BG_WHITE[]   = "\x1b[47m";
constexpr char BG_RESET[]   = "\x1b[0m";

// class ForeColor {
// public:
//   std::string BLACK   = "\x1b[30m";
//   std::string RED     = "\x1b[31m";
//   std::string GREEN   = "\x1b[32m";
//   std::string YELLOW  = "\x1b[33m";
//   std::string BLUE    = "\x1b[34m";
//   std::string MAGENTA = "\x1b[35m";
//   std::string CYAN    = "\x1b[36m";
//   std::string WHITE   = "\x1b[37m";
//   std::string RESET   = "\x1b[0m";
// };

// class BackColor {
// public:
//   std::string BLACK   = "\x1b[40m";
//   std::string RED     = "\x1b[41m";
//   std::string GREEN   = "\x1b[42m";
//   std::string YELLOW  = "\x1b[43m";
//   std::string BLUE    = "\x1b[44m";
//   std::string MAGENTA = "\x1b[45m";
//   std::string CYAN    = "\x1b[46m";
//   std::string WHITE   = "\x1b[47m";
//   std::string RESET   = "\x1b[0m";
// };

// // do I want to do this?
// static ForeColor Fore;
// static BackColor Back;