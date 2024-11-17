#include <utility>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <tuple>
#include "Conf_Parser.h"
using namespace std;
Conf_Parser::Conf_Parser(string config_name) {
    this->config_name = config_name;
}

vector<ConfigTuple> Conf_Parser::Parse() {
    ifstream file(this->config_name);
    if (!file.is_open()) {
        throw runtime_error("Error opening file!");
    }

    vector<ConfigTuple> parsed;
    for (string line; getline(file, line);) {
        // Убираем лишние пробелы в начале и в конце строки
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        // Пропускаем пустые строки и строки, начинающиеся с #
        if (line.empty() || line[0] == '#') {
            continue;
        }

        istringstream s(line);
        vector<string> args;
        string arg;

        // Разделяем строку по пробелам
        while (getline(s, arg, ' ')) {
            if (!arg.empty()) { // Исключаем пустые аргументы
                args.push_back(arg);
            }
        }

        if (!args.empty()) { // Проверяем, что остались непустые аргументы
            string convect_name = args[0];
            ConfigTuple tuple(convect_name, args);
            parsed.push_back(tuple);
        }
    }

    return parsed;
};