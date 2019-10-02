/*
 * enums.hpp
 *
 *  Created on: 15.09.2019
 *      Author: Rene
 */

#ifndef SCL_INCLUDE_ENUMS_HPP_
#define SCL_INCLUDE_ENUMS_HPP_

#include <string>

namespace SCL
{

  enum class FunctionalConstraint
  {
    undefined = -1,
    begin = 0,

    ST = begin,
    MX,
    CO,
    SP,
    SG,
    SE,
    SV,
    CF,
    DC,
    EX,
    SR,
    BL,
    OR,

    end
  };

  enum class PositionStateEnum
  {
    INTERMEDIATE = 0,
    OPEN = 1,
    CLOSED = 2,
    FAULTY = 3
  };

  struct BasicTypeId
  {
    enum Type
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

    BasicTypeId(BasicTypeId::Type aType)
       : mType(aType) {}
    virtual ~BasicTypeId() {}

    BasicTypeId(const BasicTypeId&) = default;
    BasicTypeId(BasicTypeId&&) = default;
    BasicTypeId& operator=(const BasicTypeId&) = default;
    BasicTypeId& operator=(BasicTypeId&&) = default;

    inline bool operator==(BasicTypeId::Type arOther) const {return mType == arOther; }
    inline bool operator<(BasicTypeId::Type arOther)  const {return mType < arOther; }
    inline bool operator<=(BasicTypeId::Type arOther) const {return  (*this == arOther)  ||  (*this <  arOther); }
    inline bool operator>(BasicTypeId::Type arOther)  const {return !(*this <  arOther)  && !(*this == arOther); }
    inline bool operator>=(BasicTypeId::Type arOther) const {return !(*this <  arOther); }
    inline bool operator!=(BasicTypeId::Type arOther) const {return !(*this == arOther); }
    inline bool IsValid() const { return (mType >= BasicType_begin) && (mType < BasicType_end);}

    static std::string GetString(const BasicTypeId& arType)
    {
      std::string out;

      switch (arType.mType)
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

    Type mType;

  };

}


#endif /* SCL_INCLUDE_ENUMS_HPP_ */
