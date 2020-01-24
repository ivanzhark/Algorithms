#include <iostream>
#include "string"
#include <vector>

size_t M_hash = 1 << 16;

//’эш-таблица
//разрешение коллизий методо цепочек
size_t Gorner(const char * key)
{
	size_t value = 0;
	while (*key) {
		value += ( (value)*17 + (*key) ) % M_hash;
		key++;
	}
	return value;
}

struct HashTableNode
{
	HashTableNode(std::string key_) { key = std::move(key_); next = nullptr; };
	std::string key;
	HashTableNode* next;
};

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
	std::vector<HashTableNode*> table;
};

HashTable::HashTable(size_t initial_size) : table(initial_size, nullptr) { size = initial_size; n_elements = 0; }

HashTable::~HashTable()
{
	for (int i = 0; i < table.size(); i++) {
		HashTableNode* head = table[i];
		while (head) {
			HashTableNode* next = head->next;
			delete head;
			head = next;
		}
	}
}

void HashTable::ResizeTable()
{
	std::vector<HashTableNode*> old_table = table;
	size = size * 2;

	table = std::vector<HashTableNode*>(size, nullptr);

	for (int i = 0; i < old_table.size(); i++) {
		HashTableNode* head = old_table[i];
		while (head) {
			std::string key = head->key;
			Add(key);
			HashTableNode* next=head->next;
			delete head;
			head = next;
		}
	}
	old_table.clear();
}



bool HashTable::Add(const std::string& key)
{
	if ( n_elements / (size+0.0) > 0.75){
		ResizeTable();
	}
	const size_t hash = Gorner(key.c_str())% size;
	if (table[hash]) {
		HashTableNode* head = table[hash];
		while (head->next != nullptr)
		{
			if (head->key == key) return false;
			head = head->next;
		}
		if (head->key == key) return false;
		HashTableNode* next_node = new HashTableNode(key);
		head->next = next_node;
	}
	else
	{
		table[hash] = new HashTableNode(key);
	}
	++n_elements;
	return true;
}

bool HashTable::Remove(const std::string& key)
{
	const size_t hash = Gorner(key.c_str()) % size;
	HashTableNode* &head = table[hash];
	if (!head) return false;

	if (head->key == key)
	{
		HashTableNode* next = head;
		head = head->next;
		delete next;
		--n_elements;
		return true;
	}

	HashTableNode* prev = head;
	while (prev->next!=nullptr) {
		if (prev->next->key == key)
		{
			HashTableNode* next;
			next = prev->next;
			prev->next = prev->next->next;
			delete next;
			--n_elements;
			return true;
		}
		prev = prev->next;
	}
	return false;
}

bool HashTable::Has(const std::string& key) const
{
	const size_t hash = Gorner(key.c_str()) % size;
	//если не пустой
	if (table[hash]) {
		HashTableNode* head = table[hash];
		if (head->key == key) return true;
		while (head != nullptr)
		{
			if (head->key == key) return true;
			head = head->next;
		}
	}
	return false;
}

int main()
{
	HashTable table(2);
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