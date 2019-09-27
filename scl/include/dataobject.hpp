#ifndef SCL_DATA_OBJECT_HPP_
#define SCL_DATA_OBJECT_HPP_

#include <stdexcept>
#include <string>
#include <vector>

namespace SCL
{
  class DataObjectType;
  class DataAttribute;
  
  class DataObjectBase
  {
  public:
	  explicit DataObjectBase(const std::string& arName, const DataObjectType& arType); // Struct constructor

	  inline const std::string& GetName() const { return mName; }
	  virtual ~DataObjectBase() = 0;

  private:
	  std::string mName;
	  std::string mType;

  };

  class SubDataObject : public SCL::DataObjectBase
  {
  public:
	  explicit SubDataObject(const std::string& arName, const DataObjectType& arStruct)
		  : DataObjectBase(arName, arStruct) {}

	  virtual ~SubDataObject() {}
  public: // deep copy for data instances
	  SubDataObject(const SubDataObject&) = default;
	  SubDataObject(SubDataObject&&) = default;
	  SubDataObject& operator=(const SubDataObject&) = default;
	  SubDataObject& operator=(SubDataObject&&) = default;

  };

  class DataObjectType
  {
  public:
	  enum CommonDataClass
	  {
		  SPS,
		  DPS, 
		  INS, 
		  ACT, 
		  ACD, 
		  SEC, 
		  BCR, 
		  MV,
		  CMV, 
		  SAV, 
		  WYE, 
		  DEL, 
		  SEQ, 
		  HMV, 
		  HWYE,
		  HDEL,
		  SPC,
		  DPC, 
		  INC, 
		  BSC, 
		  ISC, 
		  APC, 
		  SPG, 
		  ING, 
		  ASG, 
		  CURVE,
		  DPL,
		  LPL, 
		  CSD, 
		  ENS, 
		  ENC, 
		  ENG, 
		  CTS, 
		  UTS, 
		  BTS, 
		  LTS, 
		  OTS, 
		  GTS, 
		  MTS, 
		  NTS, 
		  STS, 
		  BAC, 
		  ORG, 
		  TSG, 
		  CUG,  
		  HST, 
		  CSG, 
		  VSS, 
		  VSG 
	  };

  public:
	  explicit DataObjectType(const std::string& arName, CommonDataClass aCdc) 
	  : mName(arName),
	    mCDC(aCdc),
	    mSDOs(),
	    mDAs() {}

	  virtual ~DataObjectType() {}
	  virtual void Insert(const SubDataObject& arSDO);
	  virtual void Insert(const DataAttribute& arDA);
	  inline const std::string& GetTypeName() const {return mName;}

  public:
	  DataObjectType(const DataObjectType& arOther) = delete;
	  DataObjectType(DataObjectType&& arOther) = delete;
	  DataObjectType& operator=(const DataObjectType& arOther) = delete;
	  DataObjectType& operator=(DataObjectType&& arOther) = delete;

  private:
	  std::string mName;
	  CommonDataClass mCDC;
	  std::vector<SubDataObject> mSDOs;
	  std::vector<DataAttribute> mDAs;
  };
}

#endif // SCL_DATA_OBJECT_HPP_
