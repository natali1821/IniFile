#include <string>
#include <map>
#include <vector>
#include <cassert>
#include <fstream>
#include <iostream>

std::string immutableToLower(std::string str); // for realisation function read for bool

class IniFile {
	std::map<std::string, std::map<std::string, std::string>> _data;
	std::vector<std::string> _boolValues = { "true", "on", "yes", "y", "1" }; // for realisation function read for bool
public:
	IniFile(std::string path);

	void save(std::string path) const;

	/**
	 * ��������� �� ������ section
	 * �������� ����� key
	 * ���� �� ������� ������� - ���������� default value
	 *
	 * ���������� ��� std::string, int, float, bool
	 */
	template<typename T>
	T read(std::string section, std::string key, T defaultValue = T{}) const;

	/**
	 * � ���� key �� ������ section
	 * ���������� �������� value
	 *
	 * ���������� ��� std::string, int, float, bool
	 */
	template<typename T>
	void write(std::string section, std::string key, T value);

	/**
	 * ���������, ���������� �� ������ section
	 */
	bool sectionExists(std::string section) const;

	/**
	 * ���������, ���������� �� ���� key � ������ section
	 */
	bool keyExists(std::string section, std::string key) const;

	/**
	 * ���������� ������������ ������
	 */
	std::vector<std::string> sections() const;

	/**
	 * ���������� ������������ ����� � ������ section
	 */
	std::vector<std::string> keys(std::string section) const;
};


// true -  true, True, TRUE, on, On, ON, yes, Yes, YES, y, Y, 1
// false - ���������
