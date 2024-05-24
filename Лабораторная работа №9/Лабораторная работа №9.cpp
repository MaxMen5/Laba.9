#include <iostream>
using namespace std;
const int SIZE = 100;

void instruction() {
    cout << "1 - Добавление элемента в хеш-множество\n";
    cout << "2 - Удаление элемента из хеш-множества\n";
    cout << "3 - Удаление всех элементов хеш-множества\n";
    cout << "4 - Поиск заданного элемента в хеш-множестве\n";
    cout << "5 - Количество элементов хеш-множества\n";
    cout << "6 - Вывод хеш-множества одним массивом\n";
    cout << "0 - завершение работы программы\n\n";
}

struct Node {
	int param = 0;
	Node* left = nullptr;
	Node* right = nullptr;
	Node* up = nullptr;
};
struct Tree {
	int counting = 0;
	Node* root = nullptr;
	enum Order { Prefix, Infix, Postfix };

	int* ToArray(Order order = Infix) {
		if (counting == 0) { return 0; }
		int* arr = new int[counting];
		input(arr, 0, order, root);
		return arr;
	}
	int input(int* arr, int index, Order order, Node* node) {
		if (node == nullptr) return index;
		if (order == Prefix) {
			arr[index] = node->param;
			index++;
		}
		index = input(arr, index, order, node->left);
		if (order == Infix) {
			arr[index] = node->param;
			index++;
		}
		index = input(arr, index, order, node->right);
		if (order == Postfix) {
			arr[index] = node->param;
			index++;
		}
	}

	Node* ToNode(int value) {
		Node* node = root;
		while (node->param != value) {
			if (value > node->param) { node = node->right; }
			else { node = node->left; }
		}
		return node;
	}
	void ToLeft(int value) {
		if (!contains(value)) { return; }
		Node* node = ToNode(value);
		if (node->right == nullptr) { return; }
		if (node->right->left == nullptr) {
			node->right->up = node->up;
			node->up = node->right;
			node->right = nullptr;
			node->up->left = node;
		}
		else {
			node->right = node->right->left;
			node->right->up->left = node;
			node->right->up->up = node->up;
			node->up = node->right->up;
			node->right->up = node;
		}
		if (node == root) { root = node->up; }
		else {
			if (node->up->up->right == node) { node->up->up->right = node->up; }
			else { node->up->up->left = node->up; }
		}
	}
	void ToRight(int value) {
		if (!contains(value)) { return; }
		Node* node = ToNode(value);
		if (node->left == nullptr) { return; }
		if (node->left->right == nullptr) {
			node->left->up = node->up;
			node->up = node->left;
			node->left = nullptr;
			node->up->right = node;
		}
		else {
			node->left = node->left->right;
			node->left->up->right = node;
			node->left->up->up = node->up;
			node->up = node->left->up;
			node->left->up = node;
		}
		if (node == root) { root = node->up; }
		else {
			if (node->up->up->left == node) { node->up->up->left = node->up; }
			else { node->up->up->right = node->up; }
		}
	}

	int count() { return counting; }

	void add(int value) {
		if (contains(value)) { return; }
		Node* newNode = new Node();
		newNode->param = value;
		counting++;
		if (counting == 1) {
			root = newNode;
			return;
		}
		Node* node = root;
		while (true) {
			if (value > node->param) {
				if (node->right == nullptr) {
					node->right = newNode;
					newNode->up = node;
					return;
				}
				else { node = node->right; }
			}
			else {
				if (node->left == nullptr) {
					node->left = newNode;
					newNode->up = node;
					return;
				}
				else { node = node->left; }
			}
		}
	}

	bool contains(int value) {
		Node* node = root;
		if (counting == 0) { return false; }
		while (true) {
			if (node->param == value) { return true; }
			if (value > node->param) {
				if (node->right == nullptr) { return false; }
				else { node = node->right; }
			}
			else {
				if (node->left == nullptr) { return false; }
				else { node = node->left; }
			}
		}
	}

	int minimum(Node* node) {
		if (node->left == nullptr) { return node->param; }
		else { minimum(node->left); }
	}
	void remove(int value, Node* node = nullptr) {
		if (node == nullptr) {
			node = root;
			if (!contains(value)) { return; }
			if (counting == 1) {
				clear();
				return;
			}
		}
		if (value > node->param) { remove(value, node->right); }
		else if (value < node->param) { remove(value, node->left); }
		else {
			if (node->right != nullptr && node->left != nullptr) {
				node->param = minimum(node->right);
				remove(node->param, node->right);
			}
			else {
				if (node == root) {
					if (node->left == nullptr) { root = node->right; }
					else { root = node->left; }
					root->up = nullptr;
				}
				else if (node->left == nullptr) {
					if (node->up->left == node) { node->up->left = node->right; }
					else { node->up->right = node->right; }
					if (node->right != nullptr) { node->right->up = node->up; }
				}
				else if (node->right == nullptr) {
					if (node->up->right == node) { node->up->right = node->left; }
					else { node->up->left = node->left; }
					node->left->up = node->up;
				}
				delete node;
				counting--;
			}
		}
	}

	void clear(Node* node = nullptr) {
		if (node == nullptr) {
			node = root;
			if (counting == 0) { return; }
		}
		if (node->left != nullptr) { clear(node->left); }
		if (node->right != nullptr) { clear(node->right); }
		if (node == root) {
			root = nullptr;
			counting = 0;
		}
		delete node;
	}
};

struct HashTree {
    int counting = 0;
    Tree hash[SIZE] = {};

    int count() { return counting; }

    int hashfoo(int value) {
        return value % SIZE;
    }

    void add(int value) {
        if (contains(value)) { return; }
        hash[hashfoo(value)].add(value);
        counting++;
    }

    bool contains(int value) {
        if (hash[hashfoo(value)].contains(value)) { return true; }
		else { return false; }
    }

    void remove(int value) {
        if (!contains(value)) { return; }
		hash[hashfoo(value)].remove(value);
		counting--;
    }

    void clear() {
        for (int i = 0; i < SIZE; i++) { hash[i].clear(); }
        counting = 0;
    }

    int* ToArray() {
        int* arr = new int[counting];
        int k = 0;
        for (int i = 0; i < SIZE; i++) {
			int* arr1 = hash[i].ToArray();
			for (int j = 0; j < hash[i].count(); j++, k++) {
				arr[k] = arr1[j];
			}
        }
        return arr;
    }
};

int main() {
    system("chcp 1251>NULL");
    instruction();
    HashTree hash;
    while (true) {
        int par, *arr;
        cout << "\nВведите команду: ";
        cin >> par;
        switch (par) {
        case 1:
            cout << "Введите число: ";
            cin >> par;
            hash.add(par);
            break;
        case 2:
            cout << "Введите число: ";
            cin >> par;
            hash.remove(par);
            break;
        case 3:
            hash.clear();
            break;
        case 4:
            cout << "Введите число: ";
            cin >> par;
            if (hash.contains(par)) { cout << "Число содержится в хеш-множестве\n"; }
            else { cout << "Число не содержится в хеш-множестве\n"; }
            break;
        case 5:
            cout << "Количество элементов: " << hash.count() << endl;
            break;
        case 6:
            arr = hash.ToArray();
            for (int i = 0; i < hash.count(); i++) { cout << arr[i] << " "; }
            cout << endl;
            break;
        case 0:
            return 0;
        default:
            cout << "Некорректная команда!";
        }
    }
}