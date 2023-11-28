#pragma once

#include <iostream>

class BTree {
	private:
	public:
		BTree();
		void insert(const std::string& key);
		void search(const std::string& key);
		void display();
};
