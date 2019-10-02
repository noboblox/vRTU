/*
 * basicvaluefactory.cpp
 *
 *  Created on: 28.09.2019
 *      Author: Rene Buettgen
 */

#include "basicvaluefactory.hpp"
#include "basicvalue.hpp"


#include <stdexcept>
#include <sstream>
#include <iostream>

namespace SCL
{

  BasicValueFactory::ValuePtr
  BasicValueFactory::Create(const BasicTypeId& aType)
  {

    switch (aType.mType)
    {
    case BasicTypeId::BOOLEAN:
      return std::make_unique<SCL::ValueBool>();
      break;
    case BasicTypeId::INT8:
      return std::make_unique<SCL::ValueSInt8>();
      break;
    case BasicTypeId::INT16:
      return std::make_unique<SCL::ValueSInt16>();
      break;
    case BasicTypeId::INT24:
      return std::make_unique<SCL::ValueSInt24>();
      break;
    case BasicTypeId::INT32:
      return std::make_unique<SCL::ValueSInt32>();
      break;
    case BasicTypeId::INT64:
      return std::make_unique<SCL::ValueSInt64>();
      break;
    case BasicTypeId::INT128:
      return std::make_unique<SCL::ValueSInt128>();
      break;
    case BasicTypeId::INT8U:
      return std::make_unique<SCL::ValueUInt8>();
      break;
    case BasicTypeId::INT16U:
      return std::make_unique<SCL::ValueUInt16>();
      break;
    case BasicTypeId::INT24U:
      return std::make_unique<SCL::ValueUInt24>();
      break;
    case BasicTypeId::INT32U:
      return std::make_unique<SCL::ValueUInt32>();
      break;
    case BasicTypeId::FLOAT32:
      return std::make_unique<SCL::ValueFloat32>();
      break;
    case BasicTypeId::FLOAT64:
      return std::make_unique<SCL::ValueFloat64>();
      break;
    case BasicTypeId::Dbpos:
      return std::make_unique<SCL::ValueDbPos>();
      break;
    case BasicTypeId::Tcmd:
      return std::make_unique<SCL::ValueTransCmd>();
      break;
    case BasicTypeId::Quality:
      return std::make_unique<SCL::ValueQuality>();
      break;
    case BasicTypeId::Timestamp:
      return std::make_unique<SCL::ValueTimeStamp>();
      break;
    case BasicTypeId::VisString32:
      break;
    case BasicTypeId::VisString64:
      break;
    case BasicTypeId::VisString129:
      break;
    case BasicTypeId::VisString255:
      break;
    case BasicTypeId::Octet64:
      break;
    case BasicTypeId::Unicode255:
      break;
    case BasicTypeId::EntryTime:
      break;
    case BasicTypeId::Check:
      break;
    case BasicTypeId::ObjRef:
      break;
    case BasicTypeId::Currency:
      break;
    case BasicTypeId::PhyComAddr:
      break;
    case BasicTypeId::TrgOps:
      break;
    case BasicTypeId::OptFlds:
      break;
    case BasicTypeId::SvOptFlds:
      break;

    default:
      break;
    }
    std::stringstream msg;
    msg << "Error, could not create instance for BasicValueType: " << aType.mType << " = "
        << BasicTypeId::GetString(aType) << std::endl;
    throw std::invalid_argument(msg.str());
  }



} /* namespace SCL */
