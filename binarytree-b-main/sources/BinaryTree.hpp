#pragma once
#include <iostream>
#include <stack>
#include <iterator>
#include <string>
#include <vector>
#include <array>


namespace ariel
{
	enum {
		POSTORDER = 0,
		PREORDER,
		INORDER,
		ORDERS
	};

	template <typename T>
	class Node
	{
	public:
		T* value;
		Node<T>* left;
		Node<T>* right;

		Node(T val) : left(nullptr), right(nullptr), value(new T(val)) {};
		~Node() 
		{
			if(value)
			{
				delete value;
			}
		}

		Node(const Node& other) {
			this->value = new T(other.value);
			this->left = new Node<T>(other.left);
			this->right = new Node<T>(other.right);
		}

		Node& operator =(const Node& other) {
			if (this == &other) 
			{
				return *this;
			}

			this->value = new T(other.value);
			this->left = new Node<T>(other.left);
			this->right = new Node<T>(other.right);
			return *this;
		}

		Node(Node&& source) noexcept 
		{
			this->value = source.value;
			this->left = source.left;
			this->right = source.right; 
		}
		Node& operator =(Node&& other) noexcept{
			if (this == &other) {
				return *this;
			}

			this->value = other.value;
			this->left = other.left;
			this->right = other.right;

			return *this;
		}



	};
	template <typename T>

	class BinaryTree
	{
	public:

		std::array<std::vector<T*>, ORDERS> the_nodes;
		Node<T>* root = nullptr;

		struct Iterator
		{
		private:
			std::vector<T*> vec;
			T* m_ptr;
		public:
			Iterator(T* ptr, std::vector<T*> v) : m_ptr(ptr), vec(v) {}

			T& operator*() const { return *m_ptr; }
			T* operator->() { return m_ptr; }
			Iterator& operator++() {
				unsigned int index = 0;

				for (unsigned int i = 0; i < vec.size(); i++) 
				{
					if (m_ptr == vec.at(i))
					{
						index = i + 1;
						break;
					}
				}

				m_ptr = vec.at(index);

				return *this; 
			}
			Iterator operator++(int) { Iterator tmp = *this; ++(*this); return tmp; }
			friend bool operator== (const Iterator& a, const Iterator& b) { return a.m_ptr == b.m_ptr; };
			friend bool operator!= (const Iterator& a, const Iterator& b) { return a.m_ptr != b.m_ptr; };
		};

		Iterator begin() 
		{
			create_inorder(root);

			the_nodes[INORDER].push_back(nullptr);

			return Iterator(the_nodes[INORDER].at(0), the_nodes[INORDER]);
		}

		Iterator begin_preorder() 
		{
			create_preorder(root);

			the_nodes[PREORDER].push_back(nullptr);

			return Iterator(the_nodes[PREORDER].at(0), the_nodes[PREORDER]);
		}
		
		Iterator begin_inorder() 
		{
			return begin();
		}

		Iterator begin_postorder() 
		{
			create_postorder(root);

			the_nodes[POSTORDER].push_back(nullptr);

			return Iterator(the_nodes[POSTORDER].at(0), the_nodes[POSTORDER]);
		}

		Iterator end_postorder() 
		{
			return Iterator(nullptr, the_nodes[POSTORDER]);
		}
		
		Iterator end_inorder() 
		{
			return end();
		}

		Iterator end() 
		{
			return Iterator(nullptr, the_nodes[INORDER]);
		}

		Iterator end_preorder() 
		{ 
			return Iterator(nullptr, the_nodes[PREORDER]);
		}

		BinaryTree() {};

		BinaryTree& add_root(T val)
		{
			if (this->root) 
			{
				*this->root->value = val;
			}
			else 
			{
				this->root = new Node<T>(val);
			}

			return *this;
		}

		BinaryTree& add_left(T node1, T node2)
		{
			Node<T>* temp = search_node(node1);

			if (temp)
			{
				if (temp->left)
				{
					*(temp->left->value) = node2;
				}

				else
				{
					Node<T>* temp2 = new Node<T>(node2);
					temp->left = temp2;
				}
			}

			else
			{
				throw std::invalid_argument("no such node");
			}

			return *this;
		}

		BinaryTree& add_right(T node1, T node2)
		{
			Node<T>* temp = search_node(node1);

			if (temp)
			{
				if (temp->right != nullptr)
				{
					*(temp->right->value) = node2;
				}

				else
				{
					Node<T>* temp2 = new Node<T>(node2);
					temp->right = temp2;
				}
			}

			else
			{
				throw std::invalid_argument("no such node");
			}

			return *this;
		}

		Node<T>* search_node(T node)
		{
			std::vector<Node<T>*> v;
			v.push_back(root);

			while (!v.empty())
			{
				Node<T>* temp = v.back();

				v.pop_back();

				if (!temp) {continue;}
				if (*(temp->value) == node)
				{
					return temp;
				}
				v.push_back(temp->left);
				v.push_back(temp->right);
			}

			return nullptr;
		}

		void create_inorder(Node<T>* root, bool first = true)
		{
			if (first)
			{
				while (!the_nodes[INORDER].empty())
				{
					the_nodes[INORDER].pop_back();
				}

				first = false;
			}

			if (root) 
			{
				create_inorder(root->left, first);

				the_nodes[INORDER].push_back(root->value);

				create_inorder(root->right, first);
			}

		}

		void create_preorder(Node<T>* root, bool first = true)
		{
			if (first)
			{
				while (!the_nodes[PREORDER].empty())
				{
					the_nodes[PREORDER].pop_back();
				}

				first = false;
			}

			if (root) 
			{
				the_nodes[PREORDER].push_back(root->value);

				create_preorder(root->left, first);
				create_preorder(root->right, first);
			}

		}

		void create_postorder(Node<T>* root, bool first = true)
		{
			if (first)
			{
				while (!the_nodes[POSTORDER].empty())
				{
					the_nodes[POSTORDER].pop_back();
				}

				first = false;
			}

			if (root) {
				create_postorder(root->left, first);
				create_postorder(root->right, first);

				the_nodes[POSTORDER].push_back(root->value);
			}

		}

	};

	template <typename T>
	std::ostream& operator<<(std::ostream& os, const BinaryTree<T>& tree)
	{
		std::cout << std::endl;
		std::vector<Node<T>*> v;
		v.push_back(tree.root);

		while (!v.empty())
		{
			Node<T>* temp = v.back();
			v.pop_back();

			if (!temp) { continue; }

			os << *temp->value << "->(";
			
			if (temp->left)
			{
				os << *temp->left->value << ",";
			}

			else
			{
				os << "null,";
			}

			if (temp->right)
			{
				os << *temp->right->value << ")" << std::endl;
			}

			else
			{
				os << "null)" << std::endl;
			}

			v.push_back(temp->left);
			v.push_back(temp->right);
		}

		return os;
	}

	
}
