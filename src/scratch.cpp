
/*******************************************************************************************
 *
 *   raylib-extras [ImGui] example - Simple Integration
 *
 *	This is a simple ImGui Integration
 *	It is done using C++ but with C style code
 *	It can be done in C as well if you use the C ImGui wrapper
 *	https://github.com/cimgui/cimgui
 *
 *   Copyright (c) 2021 Jeffery Myers
 *
 ********************************************************************************************/

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

static std::vector<std::string> stringIn = {};
static std::vector<std::string> stringOut = {};
static int bufIdx = {};

static void strReverse(std::vector<std::string> &vecIn,
                       std::vector<std::string> &vecOut, int &idx);

int main(int argc, char *argv[]) {
  stringIn.push_back("suck");
  stringIn.push_back("it");
  stringIn.push_back("dry");

  std::cout << stringIn.size() << "\n";
  for (int i = 0; i < stringIn.size(); i++) {
    strReverse(stringIn, stringOut, i);
    std::cout << stringOut[i] << "\n";
  }
}

static void strReverse(std::vector<std::string> &vecIn,
                       std::vector<std::string> &vecOut, int &idx) {
  int strInElems = stringIn.size();
  vecOut.clear();
  vecOut.shrink_to_fit();
  int len = strlen(vecIn[idx].data());
  char *msgIn = vecIn[idx].data();
  char msgOut[255] = {};
  for (int i = 0; i < strInElems; i++) {
    for (int j = 0; j < len; j++) {
      msgOut[j] = msgIn[len - j - 1];
    }
    vecOut.push_back(msgOut);
  }
}
