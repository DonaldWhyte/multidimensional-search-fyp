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
				// Swap node with its right child
				if (!node->leftChild)
				{
					replace(node, node->rightChild);
				}
				// ..or its left child if it doesn't have a right child
				else if (!node->rightChild)
				{
					replace(node, node->leftChild);
				}
				// If node has two children, we need to push the node down to
				// the bottom of the tree before removing it
				else
				{
					Node* minNode = minimumNodeInSubtree(node->rightChild);
					if (minNode->parent != node)
					{
						replace(minNode, minNode->rightChild);
						minNode->rightChild = node->rightChild;
						minNode->rightChild->parent = minNode;
					}
					replace(node, minNode);
					minNode->leftChild = node->leftChild;
					minNode->leftChild->parent = minNode;
				}

				// Finally, delete the node and return true to indicate success
				// Ensure node to delete's children are NULL, because it's a leaf
				// and we don't want to delete any other nodes
				node->leftChild = node->rightChild = 0;
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

		/* Move given node to the top of the tree. */
		void splay(Node* nodeToSplay)
		{
			// Keep performing tree rotations until desired node is the root
			while (nodeToSplay->parent)
			{
				if (!nodeToSplay->parent->parent) // ZIG
				{
					if (nodeToSplay->parent->leftChild == nodeToSplay)
						rightRotate(nodeToSplay->parent);
					else
						leftRotate(nodeToSplay->parent);
				}
				// ZIG-ZIG (LEFT CHILDREN)
				else if (nodeToSplay->parent->leftChild == nodeToSplay && nodeToSplay->parent->parent->leftChild == nodeToSplay->parent)
				{
					rightRotate(nodeToSplay->parent->parent);
					rightRotate(nodeToSplay->parent);
				}
				// ZIG-ZIG (RIGHT CHILDREN)
				else if (nodeToSplay->parent->rightChild == nodeToSplay && nodeToSplay->parent->parent->rightChild == nodeToSplay->parent)
				{
					leftRotate(nodeToSplay->parent->parent);
					leftRotate(nodeToSplay->parent);
				}
				// ZIG-ZAG (RIGHT-LEFT CHILDREN)
				else if (nodeToSplay->parent->leftChild == nodeToSplay && nodeToSplay->parent->parent->rightChild == nodeToSplay->parent)
				{
					rightRotate(nodeToSplay->parent);
					leftRotate(nodeToSplay->parent);
				}
				// ZIG-ZAG (LEFT-RIGHT CHILDREN)
				else
				{
					leftRotate(nodeToSplay->parent);
					rightRotate(nodeToSplay->parent);
				}
			}
		}

		Node* rootNode()
		{
			return root;
		}

		std::string toString() const
		{
			return nodeToString(root);
		}

	private:
		Node* findNearestNode(Key key) const
		{
			Node* currentNode = root;
			Node* parent = 0;
			while (currentNode)
			{
				parent = currentNode;
				if (currentNode->key == key) // if we've reached the node with the given key
					return currentNode;
				else if (compare(currentNode->key, key)) // current node's key is LESS THAN desired key
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
 
 		/* Return node which has the smallest key in the subtre
 		 * rooted at the given node. */
		Node* minimumNodeInSubtree(Node* node)
		{
			while(node->leftChild)
				node = node->leftChild;
			return node;
		}

		void leftRotate(Node* node)
		{
			Node* other = node->rightChild;
			node->rightChild = other->leftChild;
			if (other->leftChild)
				other->leftChild->parent = node;

			other->parent = node->parent;
			if (!node->parent) // if original node was root
				root = other;
			else if (node == node->parent->leftChild) // original node is left child
				node->parent->leftChild = other;
			else // if it's right child
				node->parent->rightChild = other;

			other->leftChild = node;
			node->parent = other;
		}

		void rightRotate(Node* node)
		{
			Node* other = node->leftChild;
			node->leftChild = other->rightChild;
			if (other->rightChild)
				other->rightChild->parent = node;

			other->parent = node->parent;
			if (!node->parent)
				root = other;
			else if (node == node->parent->leftChild)
				node->parent->leftChild = other;
			else
				node->parent->rightChild = other;

			other->rightChild = node;
			node->parent = other;
		}

		std::string nodeToString(Node* node, unsigned int level = 0) const
		{
			std::stringstream ss;
			for (unsigned int i = 0; (i < level); i++)
				ss << "\t";			
			if (!node)
			{
				ss << "()\n";
			}
			else
			{
				ss << "(Key: " << node->key << ", Value: " << node->value << ")\n";
				ss << nodeToString(node->leftChild, level + 1);
				ss << nodeToString(node->rightChild, level + 1);
			}

			return ss.str();
		}

		Node* root;
		Comp compare; // used to compare keys for (<, =, >)

	};

}

#endif