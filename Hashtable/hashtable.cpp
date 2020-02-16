#include "hashtable.h"
#include <iostream>

#define MIN(a,b) ((a) < (b) ? ( a) : (b))
size_t proba_max = 10;

//Хэш-таблица
//разрешение коллизий методом открытой адресации
//метод Горнера 

size_t Gorner(const char* key, const size_t M)
{
	size_t value = 0;
	while (*key) {
		value += ((value) * 7 + (*key)) % M;
		key++;
	}
	return value % M;
}

size_t Gorner_2(const char* key, const size_t M)
{
	size_t value = 0;
	while (*key) {
		value += ((value) * 11 + (*key)) % M;
		key++;
	}
	return value;
}



HashTable::HashTable(size_t initial_size) : table(initial_size, std::make_pair("", false)) { size = initial_size; n_elements = 0; }

HashTable::~HashTable()
{
	table.clear();
}

void HashTable::ResizeTable()
{
	std::vector< std::pair<std::string, bool> > old_table(table);
	table.clear();
	size = size * 2;
	table = std::vector< std::pair<std::string, bool> >(size, std::make_pair("", false));
	n_elements = 0;

	for (int i = 0; i < old_table.size(); i++) {
		Add(std::move(old_table[i].first));
	}
	old_table.clear();
}

bool HashTable::Add(const std::string& key)
{
	if (n_elements / (size + 0.0) > 0.75) {
		ResizeTable();
	}
	const size_t hash = Gorner(key.c_str(), size);

	if (table[hash].first.empty()) {
		table[hash].first = key;
		table[hash].second = true;
		++n_elements;
		return true;
	}

	if (table[hash].first == key) return false;

	const size_t hash_2 = Gorner_2(key.c_str(), size);
	size_t ind_key = 0xffffffff;
	for (size_t i = 1; i < MIN(size, proba_max); i++)
	{
		size_t k = (hash + i * hash_2) % size;
		if (!table[k].first.empty()) {
			if (table[k].first == key) return false;
		}
		else if (!table[k].second) {
			if (ind_key == 0xffffffff) ind_key = k;
			table[ind_key].first = key;
			++n_elements;
			table[k].second = true;
			return true;
		}
		else {
			if (ind_key == 0xffffffff)
				ind_key = k;
		}
	}
	ResizeTable();
	Add(key);
}

bool HashTable::Remove(const std::string& key)
{
	const size_t hash = Gorner(key.c_str(), size);
	if (table[hash].first == key) {
		table[hash].first.clear();
		--n_elements;
		return true;
	}

	const size_t hash_2 = Gorner_2(key.c_str(), size);
	size_t k;
	for (size_t i = 1; i < size; i++)
	{
		k = (hash + i * hash_2) % size;
		if (table[k].first == key) {
			table[k].first.clear();
			--n_elements;
			return true;
		}
		else if (!table[k].second)
		{
			return false;
		}
	}
	return false;
}

bool HashTable::Has(const std::string& key) const
{
	const size_t hash = Gorner(key.c_str(), size);
	if (table[hash].first == key) return true;
	const size_t hash_2 = Gorner_2(key.c_str(), size);
	size_t k;
	for (size_t i = 1; i < MIN(size, proba_max); i++)
	{
		k = (hash + i * hash_2) % size;
		if (table[k].first == key) return true;
		if (!table[k].second) return false;
	}

	return false;
}

int main()
{
	HashTable table(256);
	char c;
	std::string key;

	while (std::cin >> c)
	{
		std::cin >> key;
		bool result;
		switch (c)
		{
		case '+': result = table.Add(key); break;
		case '?': result = table.Has(key); break;
		case '-': result = table.Remove(key); break;
		}
		if (result)
			std::cout << "OK\n";
		else std::cout << "FAIL\n";
	}

	return 0;
}