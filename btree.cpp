#include "btree.h"

/*
	Class Constructors
*/

template <typename T>
Node<T>::Node(bool _isLeaf) {
	isLeaf = _isLeaf;
}
template <typename T>
Node<T>::~Node() {

}

template <typename T>
BTree<T>::BTree() {
	t = 3;
}
template <typename T>
BTree<T>::BTree(int _t) {
	t = _t;
}
template <typename T>
BTree<T>::~BTree() {

}

/*
	Private Functions
*/

/*
	Public Functions
*/

template <typename T>
void Node<T>::traverse() {

}

template <typename T>
void BTree<T>::insert(T key) {
	std::cout << key << std::endl;
}

template <typename T>
void BTree<T>::search(T key) {
	
}

template <typename T>
void BTree<T>::display() {

}