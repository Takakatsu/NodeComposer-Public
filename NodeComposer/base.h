#pragma once

////基本ライブラリ////
#include <algorithm>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <future>
#include <iostream>
#include <map>
#include <random>
#include <set>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <utility>
#include <variant>
#include <vector>

#include <boost/bimap.hpp>
#include <boost/tokenizer.hpp>

////各種必要ライブラリ////
//Qt系
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/qinputdialog.h>
#include <qdialogbuttonbox.h>
#include <qevent.h>
#include <qformlayout.h>
#include <qlabel.h>
#include <qmimedata.h>
#include <qpainter.h>
#include <qpushbutton.h>
#include <qspinbox.h>
//midi系
#include "library/RtMidi/RtMidi.h"
#include "library/midifile/MidiFile.h"
//json系
#include <nlohmann/json.hpp>
//Lua
#include <lua.hpp>

#include "util.h"

#include "Sound.h"

#include "NodeVariable.h"
#include "variable_lib.h"

#include "lua_additional_functions.h"

#include "Node.h"

#include "NodeComposer.h"
