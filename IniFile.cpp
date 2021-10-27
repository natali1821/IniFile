#include "IniFile.h"

std::string immutableToLower(std::string s) {
	std::string str = s.substr(0, s.size());
	for (size_t i = 0; i < str.size(); ++i) {
		if (str[i] >= 'A' && str[i] <= 'Z') {
			str[i] += ('a' - 'A');
		}
	}
	return str;
}

IniFile::IniFile(std::string path) {
	std::string str;
	std::ifstream file(path);
	std::vector<std::string> strings;
	if (!file.is_open()) {
		throw std::invalid_argument("Failed to loaf file");
	}
	while (!file.eof()) {
		if (std::getline(file, str) && !str.empty()) {
			strings.push_back(str);
		}
	}
	file.close();

	std::string section;
	std::string key;
	std::string value;
	for (size_t i = 0; i < strings.size(); ++i) {
		if (strings[i][strings[i].size() - 1] == '\r') {
			strings[i].erase(strings[i].size() - 1);
		}
		size_t last = strings[i].size() - 1; // number of the last element of string
		size_t first = 0;
		if (strings[i][0] == '[' && strings[i][last] == ']') {
			section = strings[i].substr(1, last - 1);
			_data.insert(std::pair<std::string, std::map<std::string, std::string>>(section, {}));
		}
		else {
			last = strings[i].find('='); // number of the '='
			key = strings[i].substr(0, last);
			first = last;
			++first;
			last = strings[i].size() - 1;
			value = strings[i].substr(first, last);
			_data.at(section).insert(std::pair<std::string, std::string>(key, value));
		}
	}

}

void IniFile::save(std::string path) const {
	std::ofstream file(path);
	if (!file.is_open()) {
		throw std::invalid_argument("Failed to lad file");
	}
	for (std::map<std::string, std::map <std::string, std::string>> ::const_iterator ptr = _data.begin(); ptr != _data.end(); ++ptr) {
		file << "[" << ptr->first << "]" << std::endl;
		for (std::map<std::string, std::string>::const_iterator eptr = ptr->second.begin(); eptr != ptr->second.end(); ++eptr) {
			file << eptr->first << "=" << eptr->second << std::endl;
		}
	}
	file.close();
}

template<>
std::string IniFile::read(std::string section, std::string key, std::string defaultValue) const {
	if (sectionExists(section) && keyExists(section, key)) {
		return _data.at(section).at(key);
	}
	return defaultValue;
}

template<>
int IniFile::read(std::string section, std::string key, int defaultValue) const {
	if (sectionExists(section) && keyExists(section, key)) {
		return std::stoi(_data.at(section).at(key));
	}
	return defaultValue;
}

template<>
float IniFile::read(std::string section, std::string key, float defaultValue) const {
	if (sectionExists(section) && keyExists(section, key)) {
		return std::stof(_data.at(section).at(key));
	}
	return defaultValue;
}

template<>
bool IniFile::read(std::string section, std::string key, bool defaultValue) const {
	if (sectionExists(section) && keyExists(section, key)) {
		std::string str = immutableToLower(_data.at(section).at(key));
		for (size_t i = 0; i < _boolValues.size(); ++i) {
			if (_boolValues[i].compare(str) == 0) {
				return true;
			}
		}
	}
	return defaultValue;
}

template<>
void IniFile::write(std::string section, std::string key, std::string value) {
	if (sectionExists(section)) {
		if (keyExists(section, key)) {
			_data.at(section).at(key) = value;
		}
		else {
			_data.at(section).insert(std::pair<std::string, std::string>(key, value));
		}
	}
	else {
		_data.insert(std::pair<std::string, std::map<std::string, std::string>>(section, {}));
		_data.at(section).insert(std::pair<std::string, std::string>(key, value));
	}
}

template<>
void IniFile::write(std::string section, std::string key, int value) {
	if (sectionExists(section)) {
		if (keyExists(section, key)) {
			_data.at(section).at(key) = std::to_string(value);
		}
		else {
			_data.at(section).insert(std::pair<std::string, std::string>(key, std::to_string(value)));
		}
	}
	else {
		_data.insert(std::pair<std::string, std::map<std::string, std::string>>(section, {}));
		_data.at(section).insert(std::pair<std::string, std::string>(key, std::to_string(value)));
	}
}

template<>
void IniFile::write(std::string section, std::string key, float value) {
	if (sectionExists(section)) {
		if (keyExists(section, key)) {
			_data.at(section).at(key) = std::to_string(value);
		}
		else {
			_data.at(section).insert(std::pair<std::string, std::string>(key, std::to_string(value)));
		}
	}
	else {
		_data.insert(std::pair<std::string, std::map<std::string, std::string>>(section, {}));
		_data.at(section).insert(std::pair<std::string, std::string>(key, std::to_string(value)));
	}
}

template<>
void IniFile::write(std::string section, std::string key, bool value) {
	std::string str;
	if (value) {
		str = "true";
	}
	else {
		str = "false";
	}
	if (sectionExists(section)) {
		if (keyExists(section, key)) {
			_data.at(section).at(key) = str;
		}
		else {
			_data.at(section).insert(std::pair<std::string, std::string>(key, str));
		}
	}
	else {
		_data.insert(std::pair<std::string, std::map<std::string, std::string>>(section, {}));
		_data.at(section).insert(std::pair<std::string, std::string>(key, str));
	}
}



bool IniFile::sectionExists(std::string section) const {
	auto it = _data.find(section);
	return it != _data.end();
}

bool IniFile::keyExists(std::string section, std::string key) const {
	if (!sectionExists(section)) {
		return false;
	}
	auto it = _data.at(section).find(key);
	return it != _data.at(section).end();
}

std::vector<std::string> IniFile::sections() const {
	std::vector<std::string> sections;
	for (auto it = _data.begin(); it != _data.end(); ++it) {
		sections.push_back(it->first);
	}
	return sections;
}

std::vector<std::string> IniFile::keys(std::string section) const {
	std::vector<std::string> keys;
	if (sectionExists(section)) {
		auto it = _data.find(section);
		for (auto ptr = it->second.begin(); ptr != it->second.end(); ++ptr) {
			keys.push_back(ptr->first);
		}
	}
	return keys;
}
