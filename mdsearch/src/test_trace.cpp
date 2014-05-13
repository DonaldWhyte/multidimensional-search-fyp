#include "kdtree.hpp"
#include "pyramidtree.hpp"
#include "bithash.hpp"
#include "operationlist.hpp"
#include "timing.hpp"
#include <string>

using namespace mdsearch;

int main(int argc, char* argv[])
{
	// Load operation list containing trace of real operations
	OperationList<3> operationList;
	operationList.load("/media/Maxtor/DonaldFYP/test_data/operation_lists/traces/active-w4.ops");
	const std::vector<Operation<3> >& operations = operationList.getOperations();

	KDTree<3> kdTree;
	OperationListExecutor<KDTree<3>, 3> kdTreeExecutor;
	BitHash<3> bitHash;
	OperationListExecutor<BitHash<3>, 3> bitHashExecutor;
	Boundary<3> initialBoundary = { 0 };
	PyramidTree<3> pyramidTree(initialBoundary);
	OperationListExecutor<PyramidTree<3>, 3> pyramidTreeExecutor;

	double start = getTime();
	kdTreeExecutor.execute(&kdTree, operationList);
	std::cout << "Point KD-tree took " << getTime() - start << " seconds" << std::endl;
	start = getTime();
	bitHashExecutor.execute(&bitHash, operationList);
	std::cout << "Bit Hash took " << getTime() - start << " seconds" << std::endl;
	start = getTime();
	pyramidTreeExecutor.executeWithBoundary(&pyramidTree, operationList);
	std::cout << "Pyramid Tree took " << getTime() - start << " seconds" << std::endl;
}