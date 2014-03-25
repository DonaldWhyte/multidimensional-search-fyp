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

			Node(Node* parent, Key key, const Value& value, Node* leftChild = 0, Node* rightChild = 0)
				: parent(parent), leftChild(leftChild), rightChild(rightChild), key(key), value(value)
			{
			}
			
			virtual ~Node()
			{
				delete leftChild;
				delete rightChild;
			}
		};

		SplayTree() : root(0)
		{
		}

		virtual ~SplayTree()
		{
			delete root;
		}

		void clear()
		{
			delete root;
			root = 0;
		}

		bool insert(Key key, const Value& value)
		{
			Node* nearestNode = findNearestNode(key);
			if (nearestNode && nearestNode->key == key) // if node with key already exists
			{
				return false;
			}
			else
			{
				// Create new node to store this key-value pair
				Node* newNode = new Node(nearestNode, key, value);
				if (!newNode->parent) // if new node is the root
					root = newNode;
				else if (compare(newNode->parent->key, newNode->key)) // newNode key > parent key, so it's right child
					newNode->parent->rightChild = newNode;
				else // newNode key < parent key, so it's left child
					newNode->parent->leftChild = newNode;

				splay(newNode);

				return true;
			}
		}

		bool remove(Key key)
		{
			Node* node = findNodeWithKey(key);
			if (node)
			{
				// Splay found node to the root of the tree
				splay(node);
				// TODO: comment
				if (!node->leftChild)
				{
					replace(node, node->rightChild);
				}
				else if (!node->rightChild)
				{
					replace(node, node->leftChild);
				}
				else
				{
					// TODO: comments
					Node* subtree = minimumSubtree(node->rightChild);
					if (subtree->parent != node)
					{
						replace(subtree, subtree->right);
						subtree->rightChild = node->rightChild;
						subtree->rightChild->parent = subtree;
					}
					replace(node, subtree);
					subtree->leftChild = node->leftChild;
					subtree->leftChild->parent = subtree;
				}

				// Finally, delete the node and return true to indicate success
				delete node;
				return true;
			}
			else
			{
				return false;
			}
		}

		bool update(Key oldKey, Key newKey, const Value& newValue)
		{
			if (remove(oldKey))
			{
				insert(newKey, newValue);
				return true;
			}
			else
			{
				return false;
			}
		}

		/* Return value stored in node with given key.
		 * If a node with the given key does not exist,
		 * then NULL is returned. */
		Value* getValue(Key key)
		{
			Node* node = findNodeWithKey(key);
			if (node)
				return &(node->value);
			else
				return 0;
		}

		void splay(Node* nodeToSplay)
		{
			// TODO
		}

		Node* rootNode()
		{
			return root;
		}

	private:
		Node* findNearestNode(Key key) const
		{
			Node* currentNode = root;
			Node* parent = 0;
			while (currentNode)
			{
				parent = currentNode;
				if(compare(currentNode->key, key))
					currentNode = currentNode->rightChild;
				else
					currentNode = currentNode->leftChild;
			}
			// 'currentNode' is now a null pointer, so we return the parent
			return parent;
		}

		Node* findNodeWithKey(Key key) const
		{
			Node* nearestNode = findNearestNode(key);
			if (nearestNode && nearestNode->key == key)
				return nearestNode;
			else
				return 0; // node with key not found
		}

		void replace(Node* x, Node* y)
		{
			if (!x->parent)
				root = y;
			else if (x == x->parent->leftChild)
				x->parent->leftChild = y;
			else
				x->parent->rightChild = y;
			// If y is not NULL (i.e. node x is not the root)
			if (y)
				y->parent = x->parent;
		}
 
		Node* minimumSubtree(Node* node)
		{
			while(node->leftChild)
				node = node->leftChild;
			return node;
		}

		Node* maximumSubtree(Node* node)
		{
			while(node->rightChild)
				node = node->rightChild;
			return node;
		}		

		void leftRotate(Node* node)
		{
			// TODO
		}

		void rightRotate(Node* node)
		{
			// TODO
		}

		Node* root;
		Comp compare; // used to compare keys for (<, =, >)

	};

}

#endif