#include "kdtree.h"
#include "pyramidtree.h"
#include "hashtable.h"
#include "dataset.h"

using namespace mdsearch;

void func()
{
	Point<1> p;
	Boundary<1> b;
	Dataset<1> dataset;

	KDTree<1> kdTree;
	PyramidTree<1> pyramidTree(b);
	HashTable<1> hashTable;
}