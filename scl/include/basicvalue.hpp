/*
 * basicvalue.hpp
 *
 *  Created on: 27.09.2019
 *      Author: Rene
 */

#ifndef SCL_INCLUDE_BASICVALUE_HPP_
#define SCL_INCLUDE_BASICVALUE_HPP_

#include <cstdint>

#include "enums.hpp"
#include "objectreference.hpp"

namespace SCL
{
  class BasicValueBase
  {
  public:
    virtual ~BasicValueBase() = 0;

    BasicValueBase(const BasicValueBase&) = delete;
    BasicValueBase(BasicValueBase&&) = delete;
    BasicValueBase& operator=(const BasicValueBase&) = delete;
    BasicValueBase& operator=(BasicValueBase&&) = delete;

    /**
     * @brief Gets the underlying value representation.
     * @code auto& number = basic_value.GetValueType<SCL::ValueSInt32>();
     *
     * @attention If the requested type does not match the real type of the class this function will throw (std::bad_cast).
     * @return Reference to the requested BasicValue
     */
    template <class ValueType>
	ValueType& GetValueType() { return dynamic_cast<ValueType&> (*this); }
    
	template <class ValueType>
	const ValueType& GetValueType() const { return dynamic_cast<const ValueType&> (*this); }

	inline BasicTypeId GetType() const { return mType; }

  protected:
    explicit BasicValueBase(BasicTypeId aType) // Creation via factory only
        : mType(aType) {}

    BasicTypeId mType;

  private:
    friend class BasicValueFactory;


  };




  // Classes with special API / initialization
  class ValueDbPos : public BasicValueBase
  {
  public:
    explicit ValueDbPos()
      : BasicValueBase(BasicTypeId::Dbpos),
        mValue(PositionStateEnum::FAULTY) {}

    const PositionStateEnum& Get() const {return mValue;}

  private:
    PositionStateEnum mValue;
  };

  class ValueTransCmd : public BasicValueBase
  {
  public:
    explicit ValueTransCmd()
      : BasicValueBase(BasicTypeId::Tcmd),
        mValue(),
        mIntermediate(false) {}

    inline int GetStep() const {return mValue;}
    inline bool IsIntermediatePosition() const {return mIntermediate;}

    inline void SetStep(int aStep) {mValue = aStep;}
    inline void SetIntermediateFlag(bool aFlag) {mIntermediate = aFlag;}

  private:
    int mValue;
    bool mIntermediate;
  };

  class ValueQuality : public BasicValueBase
  {
  public:
    enum class Validity
    {
      Good = 0,
      Invalid = 1,
      Questionable = 3
    };

    enum class Source
    {
      Process = 0,
      Substituted = 1
    };

    enum class Flags // According to bits in 61850-7-2
    {
      // Validity [0 - 1]
      Overflow        = 0x0004,
      OutOfRange      = 0x0008,
      BadReference    = 0x0010,
      Oscillatory     = 0x0020,
      Failure         = 0x0040,
      OldData         = 0x0080,
      Inconsistent    = 0x0100,
      Inaccurate      = 0x0200,
      // Source [10]
      Test            = 0x0800,
      OperatorBlocked = 0x1000
    };

  public:
    explicit ValueQuality()
      : BasicValueBase(BasicTypeId::Quality),
        mValidity(Validity::Good),
        mSource(Source::Process),
        mFlags(0) {}

    inline Validity GetValidity() const {return mValidity;}
    inline Source GetSource() const {return mSource;}
    inline bool GetFlag(Flags aFlag) const {return mFlags & static_cast<size_t>(aFlag);}

    inline void SetValidity(Validity aValidity) {mValidity = aValidity;}
    inline void SetSource(Source aSource) {mSource = aSource;}
    inline void RaiseFlag(Flags aFlag) {mFlags |= static_cast<size_t> (aFlag);}
    inline void ClearFlag(Flags aFlag) {mFlags ^= static_cast<size_t> (aFlag);}
    inline void ClearAllFlags() {mFlags = 0;}


  private:
    Validity mValidity;
    Source mSource;
    size_t mFlags;
  };

  class ValueTimeStamp : public BasicValueBase
  {
  public:
    explicit ValueTimeStamp()
      : BasicValueBase(BasicTypeId::Timestamp),
        mUnixSecondsUtc(),
        mNanoSeconds() {}

    inline uint_fast64_t GetSeconds() const {return mUnixSecondsUtc;};
    inline uint_fast32_t GetNanoSeconds() const {return mNanoSeconds;};

  private:
    uint_fast64_t mUnixSecondsUtc;
    uint_fast32_t mNanoSeconds;
  };

  // Template classes for generic types
  template <class CType, BasicTypeId::Type my_type>
  class BasicValue : public BasicValueBase
  {
  public:
    explicit BasicValue()
      : BasicValueBase(my_type),
        mValue() {}

    const CType& Get() const {return mValue;}
    void Set(const CType& arValue) {mValue = arValue;}

  private:
    CType mValue;
  };

  using ValueBool       = BasicValue<bool,                 BasicTypeId::BOOLEAN>;
  using ValueSInt8      = BasicValue<int_fast8_t,          BasicTypeId::INT8>;
  using ValueSInt16     = BasicValue<int_fast32_t,         BasicTypeId::INT16>;
  using ValueSInt24     = BasicValue<int_fast32_t,         BasicTypeId::INT24>;
  using ValueSInt32     = BasicValue<int_fast32_t,         BasicTypeId::INT32>;
  using ValueSInt64     = BasicValue<int_fast64_t,         BasicTypeId::INT64>;
  using ValueSInt128    = BasicValue<int_fast64_t,         BasicTypeId::INT128>; // Todo: Implement 128 bit.
  using ValueUInt8      = BasicValue<uint_fast32_t,        BasicTypeId::INT8U>;
  using ValueUInt16     = BasicValue<uint_fast32_t,        BasicTypeId::INT16U>;
  using ValueUInt24     = BasicValue<uint_fast32_t,        BasicTypeId::INT24U>;
  using ValueUInt32     = BasicValue<uint_fast32_t,        BasicTypeId::INT32U>;
  using ValueFloat32    = BasicValue<float,                BasicTypeId::FLOAT32>;
  using ValueFloat64    = BasicValue<double,               BasicTypeId::FLOAT64>;
  using ValueString32   = BasicValue<std::string,          BasicTypeId::VisString32>;
  using ValueString64   = BasicValue<std::string,          BasicTypeId::VisString64>;
  using ValueString129  = BasicValue<std::string,          BasicTypeId::VisString129>;
  using ValueString255  = BasicValue<std::string,          BasicTypeId::VisString255>;
  using ValueUnicode255 = BasicValue<std::string,          BasicTypeId::Unicode255>;
  using ValueObjRef     = BasicValue<SCL::ObjectReference, BasicTypeId::ObjRef>;

  // Generic class without special function
  using ValueOctet64    = BasicValue<std::string,          BasicTypeId::Octet64>;
  using ValueEntryTime  = BasicValue<std::string,          BasicTypeId::EntryTime>;
  using ValueCheck      = BasicValue<std::string,          BasicTypeId::Check>;
  using ValueCurrency   = BasicValue<std::string,          BasicTypeId::Currency>;
  using ValuePhyComAddr = BasicValue<std::string,          BasicTypeId::PhyComAddr>;
  using ValueTrgOps     = BasicValue<std::string,          BasicTypeId::TrgOps>;
  using ValueOptFlds    = BasicValue<std::string,          BasicTypeId::OptFlds>;
  using ValueSvOptFlds  = BasicValue<std::string,          BasicTypeId::SvOptFlds>;



} /* namespace SCL */

#endif /* SCL_INCLUDE_BASICVALUE_HPP_ */
