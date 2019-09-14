#include "dataattribute.hpp"

namespace SCL
{
	void
	DataAttributeType::Insert(const SubElement& arSubData)
	{
		mBDAs.push_back(arSubData);
	}
}

