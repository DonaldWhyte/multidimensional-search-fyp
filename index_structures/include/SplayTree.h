#ifndef MDSEARCH_SPLAYTREE_H
#define MDSEARCH_SPLAYTREE_H

#include <functional>

namespace mdsearch
{

	template<typename Key, typename Value, typename Comp = std::less<Key> >
	class SplayTree
	{

	public:
		struct Node
		{
			Node* leftChild;
			Node* rightChild;
			Node* parent;

			Key key;
			Value value;

			Node(Node* parent, Node* leftChild = 0, Node* rightChild = 0)
				: parent(parent), leftChild(leftChild), rightChild(rightChild)
			{
			}
			virtual ~Node()
			{
				delete leftChild;
				delete rightChild;
			}
		};

		SplayTree();
		virtual ~SplayTree();

		bool insert(Key key, const Value& value);
		bool remove(Key key);
		bool update(Key key, const Value& newValue);
		bool exists(Key key) const;

		void splay(Node* nodeToSplay);

	private:
		Node* findNode(Key key) const;
		void leftRotate(Node* node);
		void rightRotate(Node* node);

		Node* root;
		Comp compare; // used to compare keys for (<, =, >)

	};

}

#endif