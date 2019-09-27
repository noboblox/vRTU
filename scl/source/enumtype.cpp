#include "enumtype.hpp"

#include <stdexcept>

namespace SCL
{

	EnumType::EnumType(const std::string& arName)
		: mTypeId(arName),
		mUniqueValues(),
		mUniqueNames() {}

	EnumType::~EnumType() {}

	void
		EnumType::Insert(ValueType aValue, const std::string& arValueName)
	{
		const auto it_name = mUniqueNames.find(arValueName);
		const auto it_value = mUniqueValues.find(aValue);

		if ((it_name != mUniqueNames.cend()) ||
			(it_value != mUniqueValues.cend()))
		{
			throw std::invalid_argument("The given definition contains already registered elements.");
		}

		mUniqueNames.insert(std::make_pair(arValueName, aValue));
		mUniqueValues.insert(std::make_pair(aValue, arValueName));
	}

	bool
		EnumType::IsValid(ValueType aValue) const
	{
		return (mUniqueValues.find(aValue) != mUniqueValues.cend());
	}

	const std::string&
		EnumType::ValueToName(ValueType aValue) const
	{
		const auto it = mUniqueValues.find(aValue);
		if (it == mUniqueValues.cend())
		{
			throw std::out_of_range("Invalid value for enum type.");
		}
		return it->second;
	}

	EnumType::ValueType
		EnumType::NameToValue(const std::string& arValueName) const
	{
		const auto it = mUniqueNames.find(arValueName);
		if (it == mUniqueNames.cend())
		{
			throw std::out_of_range("Invalid value name.");
		}
		return it->second;
	}

	const std::string&
		EnumType::GetTypeName() const
	{
		return mTypeId;
	}

}
