#include "dataattribute.hpp"
#include "enumtype.hpp"

#include <sstream>
#include <stdexcept>

namespace SCL
{

  DataAttributeBase::DataAttributeBase(const std::string& arName, const EnumType& arEnum)
    : mName(arName), mBasicType(BasicType_enum), mTypeName(arEnum.GetTypeName()) {}

  DataAttributeBase::DataAttributeBase(const std::string& arName, const DataAttributeType& arStruct)
    : mName(arName), mBasicType(BasicType_struct), mTypeName(arStruct.GetTypeName()) {}

  DataAttributeBase::DataAttributeBase(const std::string& arName, DataAttributeBase::BasicType aBasicType)
    : mName(arName), mBasicType(aBasicType), mTypeName(BasicDataAttribute::BasicTypeToName(aBasicType))
  {
    if (!IsBasicTypeValid(mBasicType))
    {
      std::stringstream msg;

      msg << "Cannot initialize DataAttributeBase with basic type: " << static_cast<int>(mBasicType) << " = "
          << DataAttributeBase::BasicTypeToName(mBasicType) << "- Use dedicated constructors for enum/struct types." << std::endl;
      throw std::invalid_argument(msg.str());
    }
  }

  std::string
  DataAttributeBase::BasicTypeToName(BasicType arType)
  {
    std::string out;

    switch (arType)
    {
    case BOOLEAN:
      out = "bool";
      break;
    case INT8:
      out = "int8";
      break;
    case INT16:
      out = "int16";
      break;
    case INT24:
      out = "int24";
      break;
    case INT32:
      out = "int32";
      break;
    case INT64:
      out = "int64";
      break;
    case INT128:
      out = "int128";
      break;
    case INT8U:
      out = "int8u";
      break;
    case INT16U:
      out = "int16u";
      break;
    case INT24U:
      out = "int24u";
      break;
    case INT32U:
      out = "int32u";
      break;
    case FLOAT32:
      out = "float32";
      break;
    case FLOAT64:
      out = "float64";
      break;
    case Dbpos:
      out = "Dbpos";
      break;
    case Tcmd:
      out = "Tcmd";
      break;
    case Quality:
      out = "Quality";
      break;
    case Timestamp:
      out = "Timestamp";
      break;
    case VisString32:
      out = "VisString32";
      break;
    case VisString64:
      out = "VisString64";
      break;
    case VisString129:
      out = "VisString129";
      break;
    case VisString255:
      out = "VisString255";
      break;
    case Octet64:
      out = "Octet64";
      break;
    case Unicode255:
      out = "Unicode255";
      break;
    case EntryTime:
      out = "EntryTime";
      break;
    case Check:
      out = "Check";
      break;
    case ObjRef:
      out = "ObjRef";
      break;
    case Currency:
      out = "Currency";
      break;
    case PhyComAddr:
      out = "PhyComAddr";
      break;
    case TrgOps:
      out = "TrgOps";
      break;
    case OptFlds:
      out = "OptFlds";
      break;
    case SvOptFlds:
      out = "SvOptFlds";
      break;

    // Invalid values
    case BasicType_enum:
      out = "INVALID: Enum";
      break;
    case BasicType_struct:
      out = "INVALID: Struct";
      break;
    case BasicType_end:
      out = "INVALID: End";
      break;

    // Anything else unknown
    default:
      out = "unknown type";
      break;
    }

    return out;
  }


  void
  DataAttributeType::Insert(const SubElement& arSubData)
  {
      mBDAs.push_back(arSubData);
  }

  DataAttributeBase::~DataAttributeBase()
  {
  }

  DataAttribute::DataAttribute(const std::string& arName, const EnumType& arEnum, FunctionalConstraint aFc, Trigger aTrigger)
      : DataAttributeBase(arName, arEnum),
      mFC(aFc),
      mTriggerFlags(static_cast<int>(aTrigger))
  {

  }

  DataAttribute::DataAttribute(const std::string& arName, const DataAttributeType& arStruct, FunctionalConstraint aFc, Trigger aTrigger)
      : DataAttributeBase(arName, arStruct),
      mFC(aFc),
      mTriggerFlags(static_cast<int>(aTrigger))
  {

  }

  DataAttribute::DataAttribute(const std::string& arName, DataAttributeBase::BasicType aBasicType, FunctionalConstraint aFc, Trigger aTrigger)
      : DataAttributeBase(arName, aBasicType),
      mFC(aFc),
      mTriggerFlags(static_cast<int>(aTrigger))
  {

  }

  void
  DataAttribute::CheckInit() const
  {
    if ((mFC < FunctionalConstraint::begin) || (mFC >= FunctionalConstraint::end))
    {
      throw std::invalid_argument("Error: invalid functional constraint provided to data attriubte.");
    }
  }


}

