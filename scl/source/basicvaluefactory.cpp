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
		return std::make_unique<SCL::ValueString32>();
      break;
    case BasicTypeId::VisString64:
		return std::make_unique<SCL::ValueString64>();
      break;
    case BasicTypeId::VisString129:
		return std::make_unique<SCL::ValueString129>();
      break;
    case BasicTypeId::VisString255:
		return std::make_unique<SCL::ValueString255>();
      break;
    case BasicTypeId::Octet64:
		return std::make_unique<SCL::ValueOctet64>();
      break;
    case BasicTypeId::Unicode255:
		return std::make_unique<SCL::ValueUnicode255>();
      break;
    case BasicTypeId::EntryTime:
		return std::make_unique<SCL::ValueEntryTime>();
      break;
    case BasicTypeId::Check:
		return std::make_unique<SCL::ValueCheck>();
      break;
    case BasicTypeId::ObjRef:
		return std::make_unique<SCL::ValueObjRef>();
      break;
    case BasicTypeId::Currency:
		return std::make_unique<SCL::ValueCurrency>();
      break;
    case BasicTypeId::PhyComAddr:
		return std::make_unique<SCL::ValuePhyComAddr>();
      break;
    case BasicTypeId::TrgOps:
		return std::make_unique<SCL::ValueTrgOps>();
      break;
    case BasicTypeId::OptFlds:
		return std::make_unique<SCL::ValueOptFlds>();
      break;
    case BasicTypeId::SvOptFlds:
		return std::make_unique<SCL::ValueSvOptFlds>();
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
