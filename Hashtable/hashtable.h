#pragma once
#include <string>
#include <vector>

class HashTable
{
public:
	HashTable(size_t initial_size);
	~HashTable();
	bool Add(const std::string& key);
	bool Remove(const std::string& key);
	bool Has(const std::string& key) const;
	//запрещаем копирование, присваивание
	HashTable(const std::string& key) = delete;
	HashTable(std::string&& key) = delete;
	HashTable& operator=(const std::string& key) = delete;
	HashTable& operator=(std::string&& key) = delete;

private:
	size_t size;
	int n_elements;
	void ResizeTable();
	std::vector< std::pair<std::string, bool> > table;
};