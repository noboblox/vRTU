#ifndef SCL_DATA_ATTRIBUTE_HPP_
#define SCL_DATA_ATTRIBUTE_HPP_

/// This file is for the definition of the IEC 61850 runtime datatypes. These are defined inside the <DataTypeTemplates> section of the SCL.
/// @attention This classes are not to be confused with the data instances. Be aware that a DataTYPE contains Sub-DataINSTANCES

#include <string>
#include <vector>

#include "enums.hpp"

namespace SCL
{
  class EnumType;
  class DataAttributeType;
  
  class DataAttributeBase
  {
  public:
	  enum BasicType
	  {
		  BasicType_struct = -2, // invalid entry. do not use
		  BasicType_enum = -1,  // invalid entry. do not use

		  BasicType_begin = 0,

		  BOOLEAN = BasicType_begin,
		  INT8,
		  INT16,
		  INT24,
		  INT32,
		  INT64,
		  INT128,
		  INT8U,
		  INT16U,
		  INT24U,
		  INT32U,
		  FLOAT32,
		  FLOAT64,
		  Dbpos,
		  Tcmd,
		  Quality,
		  Timestamp,
		  VisString32,
		  VisString64,
		  VisString129,
		  VisString255,
		  Octet64,
		  Unicode255,
		  EntryTime,
		  Check,
		  ObjRef,
		  Currency,
		  PhyComAddr,
		  TrgOps,
		  OptFlds,
		  SvOptFlds,

		  BasicType_end
	  };

  public:
	  explicit DataAttributeBase(const std::string& arName, const EnumType& arEnum); // Enum constructor
	  explicit DataAttributeBase(const std::string& arName, const DataAttributeType& arStruct); // Struct constructor
	  explicit DataAttributeBase(const std::string& arName, DataAttributeBase::BasicType aBasicType); // Basic type constructor

	  inline const std::string& GetName() const { return mName; }
	  inline const std::string& GetTypeName() const { return mTypeName; }
	  virtual ~DataAttributeBase() = 0;

	  inline bool IsStruct() const { return (mBasicType == BasicType_struct); }
	  inline bool IsEnum() const { return (mBasicType == BasicType_enum); }
	  inline bool IsBasicType() const { return DataAttributeBase::IsBasicTypeValid(mBasicType); }

	  inline static bool IsBasicTypeValid(BasicType arType) { return (arType >= BasicType_begin) && (arType < BasicType_end);}
	  static std::string BasicTypeToName(BasicType arType);

  public: // Deep copy for data-instance
	  DataAttributeBase(const DataAttributeBase&) = default;
	  DataAttributeBase(DataAttributeBase&&) = default;
	  DataAttributeBase& operator=(const DataAttributeBase&) = default;
	  DataAttributeBase& operator=(DataAttributeBase&&) = default;


  private:
	  std::string mName;
	  BasicType mBasicType;
	  const std::string mTypeName;

  };

  class DataAttribute : public SCL::DataAttributeBase
  {
  public:
	  enum Trigger
	  {
		  no_triggers    = 0x0,
		  dchg           = 0x1,
		  qchg           = 0x2,
		  qchg_dchg      = 0x3,
		  dupd           = 0x4,
		  dupd_dchg      = 0x5,
		  dupd_qchg      = 0x6,
		  dupd_qchg_dchg = 0x7
	  };

  public:
	  explicit DataAttribute(const std::string& arName, const EnumType& arEnum, FunctionalConstraint aFc, Trigger aTrigger);
	  explicit DataAttribute(const std::string& arName, const DataAttributeType& arStruct, FunctionalConstraint aFc, Trigger aTrigger);
	  explicit DataAttribute(const std::string& arName, DataAttributeBase::BasicType aBasicType, FunctionalConstraint aFc, Trigger aTrigger);

	  virtual ~DataAttribute() {}

  public:
	  inline bool HasTriggerOnDataChange() const { return (mTriggerFlags & DataAttribute::dchg); }
	  inline bool HasTriggerOnDataUpdate() const { return (mTriggerFlags & DataAttribute::dupd); }
	  inline bool HasTriggerOnQualityChange() const { return (mTriggerFlags & DataAttribute::qchg); }
	  inline FunctionalConstraint GetConstraint() const { return mFC; }

  public: // deep-copy for data instances
	  DataAttribute(const DataAttribute&) = default;
	  DataAttribute(DataAttribute&&) = default;
	  DataAttribute& operator=(const DataAttribute&) = default;
	  DataAttribute& operator=(DataAttribute&&) = default;

  private:
	  void CheckInit() const;
	  FunctionalConstraint mFC;
	  int mTriggerFlags;
  };

  class BasicDataAttribute : public SCL::DataAttributeBase // No additional functionality. Just disallow DAs below a DAType
  {
  public:
	  explicit BasicDataAttribute(const std::string& arName, const EnumType& arEnum) // Enum constructor
		  : DataAttributeBase(arName, arEnum) {}

	  explicit BasicDataAttribute(const std::string& arName, const DataAttributeType& arStruct) // Struct constructor
		  : DataAttributeBase(arName, arStruct) {}

	  explicit BasicDataAttribute(const std::string& arName, DataAttributeBase::BasicType aBasicType) // Basic type constructor
		  : DataAttributeBase(arName, aBasicType) {}

	  virtual ~BasicDataAttribute() {}
  public: // deep copy for data instances
	  BasicDataAttribute(const BasicDataAttribute&) = default;
	  BasicDataAttribute(BasicDataAttribute&&) = default;
	  BasicDataAttribute& operator=(const BasicDataAttribute&) = default;
	  BasicDataAttribute& operator=(BasicDataAttribute&&) = default;

  };

  class DataAttributeType
  {
  public:
	  using SubElement = SCL::BasicDataAttribute;
	  explicit DataAttributeType(const std::string& arName) 
	  : mName(arName),
	    mBDAs() {}

	  virtual ~DataAttributeType() {}
	  virtual void Insert(const SubElement& arSubData);
	  inline const std::string& GetTypeName() const {return mName;}

  public:
	  DataAttributeType(const DataAttributeType& arOther) = delete;
	  DataAttributeType(DataAttributeType&& arOther) = delete;
	  DataAttributeType& operator=(const DataAttributeType& arOther) = delete;
	  DataAttributeType& operator=(DataAttributeType&& arOther) = delete;

  private:
	  std::string mName;
	  std::vector<SubElement> mBDAs;
  };
}

#endif // TYPES_HPP
