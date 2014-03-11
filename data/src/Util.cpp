#include "Util.h"
#include <cstdlib>

namespace mdsearch
{

	Real generateRandomNumber(Real minimum, Real maximum)
	{
		return minimum + static_cast<float>(rand()) / static_cast<float>(RAND_MAX / (maximum - minimum));
	}
	
}