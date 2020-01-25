#include "iec104properties.hpp"

#include <stdexcept>

  TC::TypeIdEnum::EnumDefinition const TC::TypeIdEnum::msDefinition
  {
    {"M_SP_NA_1", M_SP_NA_1},
    {"M_SP_TA_1", M_SP_TA_1},
    {"M_DP_NA_1", M_DP_NA_1},
    {"M_DP_TA_1", M_DP_TA_1},
    {"M_ST_NA_1", M_ST_NA_1},
    {"M_ST_TA_1", M_ST_TA_1},
    {"M_BO_NA_1", M_BO_NA_1},
    {"M_BO_TA_1", M_BO_TA_1},
    {"M_ME_NA_1", M_ME_NA_1},
    {"M_ME_TA_1", M_ME_TA_1},
    {"M_ME_NB_1", M_ME_NB_1},
    {"M_ME_TB_1", M_ME_TB_1},
    {"M_ME_NC_1", M_ME_NC_1},
    {"M_ME_TC_1", M_ME_TC_1},
    {"M_IT_NA_1", M_IT_NA_1},
    {"M_IT_TA_1", M_IT_TA_1},
    {"M_EP_TA_1", M_EP_TA_1},
    {"M_EP_TB_1", M_EP_TB_1},
    {"M_EP_TC_1", M_EP_TC_1},
    {"M_PS_NA_1", M_PS_NA_1},
    {"M_ME_ND_1", M_ME_ND_1},
    {"M_SP_TB_1", M_SP_TB_1},
    {"M_DP_TB_1", M_DP_TB_1},
    {"M_ST_TB_1", M_ST_TB_1},
    {"M_BO_TB_1", M_BO_TB_1},
    {"M_ME_TD_1", M_ME_TD_1},
    {"M_ME_TE_1", M_ME_TE_1},
    {"M_ME_TF_1", M_ME_TF_1},
    {"M_IT_TB_1", M_IT_TB_1},
    {"M_EP_TD_1", M_EP_TD_1},
    {"M_EP_TE_1", M_EP_TE_1},
    {"M_EP_TF_1", M_EP_TF_1},
    {"S_IT_TC_1", S_IT_TC_1},
    {"C_SC_NA_1", C_SC_NA_1},
    {"C_DC_NA_1", C_DC_NA_1},
    {"C_RC_NA_1", C_RC_NA_1},
    {"C_SE_NA_1", C_SE_NA_1},
    {"C_SE_NB_1", C_SE_NB_1},
    {"C_SE_NC_1", C_SE_NC_1},
    {"C_BO_NA_1", C_BO_NA_1},
    {"C_SC_TA_1", C_SC_TA_1},
    {"C_DC_TA_1", C_DC_TA_1},
    {"C_RC_TA_1", C_RC_TA_1},
    {"C_SE_TA_1", C_SE_TA_1},
    {"C_SE_TB_1", C_SE_TB_1},
    {"C_SE_TC_1", C_SE_TC_1},
    {"C_BO_TA_1", C_BO_TA_1},
    {"M_EI_NA_1", M_EI_NA_1},
    {"S_CH_NA_1", S_CH_NA_1},
    {"S_RP_NA_1", S_RP_NA_1},
    {"S_AR_NA_1", S_AR_NA_1},
    {"S_KR_NA_1", S_KR_NA_1},
    {"S_KS_NA_1", S_KS_NA_1},
    {"S_KC_NA_1", S_KC_NA_1},
    {"S_ER_NA_1", S_ER_NA_1},
    {"S_US_NA_1", S_US_NA_1},
    {"S_UQ_NA_1", S_UQ_NA_1},
    {"S_UR_NA_1", S_UR_NA_1},
    {"S_UK_NA_1", S_UK_NA_1},
    {"S_UA_NA_1", S_UA_NA_1},
    {"S_UC_NA_1", S_UC_NA_1},
    {"C_IC_NA_1", C_IC_NA_1},
    {"C_CI_NA_1", C_CI_NA_1},
    {"C_RD_NA_1", C_RD_NA_1},
    {"C_CS_NA_1", C_CS_NA_1},
    {"C_TS_NA_1", C_TS_NA_1},
    {"C_RP_NA_1", C_RP_NA_1},
    {"C_CD_NA_1", C_CD_NA_1},
    {"C_TS_TA_1", C_TS_TA_1},
    {"P_ME_NA_1", P_ME_NA_1},
    {"P_ME_NB_1", P_ME_NB_1},
    {"P_ME_NC_1", P_ME_NC_1},
    {"P_AC_NA_1", P_AC_NA_1},
    {"F_FR_NA_1", F_FR_NA_1},
    {"F_SR_NA_1", F_SR_NA_1},
    {"F_SC_NA_1", F_SC_NA_1},
    {"F_LS_NA_1", F_LS_NA_1},
    {"F_AF_NA_1", F_AF_NA_1},
    {"F_SG_NA_1", F_SG_NA_1},
    {"F_DR_TA_1", F_DR_TA_1},
    {"F_SC_NB_1", F_SC_NB_1}
  };

namespace TC
{



  constexpr const char* Iec104DataDefinition::csTypeId;
  constexpr const char* Iec104DataDefinition::csDataId;
   
  Iec104DataDefinition::Iec104DataDefinition(long aTypeId, long aDataId)
    : BasePropertyList(),
      mProperties{ {csTypeId, std::to_string(aTypeId)},
                   {csDataId, std::to_string(aDataId)} }
  {}
  
  Iec104DataDefinition::~Iec104DataDefinition() {}


  const bool
  Iec104DataDefinition::HasExplicitProperty(const std::string& arKey) const
  {
    return mProperties.find(arKey) != mProperties.cend();
  }

  const std::string&
  Iec104DataDefinition::GetExplicitProperty(const std::string arKey) const
  {
    if (!HasExplicitProperty(arKey))
      throw std::invalid_argument("Requested property does not exist");
    return mProperties.find(arKey)->second;
  }

  const bool
  Iec104DataDefinition::HasDefaultProperty(const std::string& arKey) const
  {
    return false;
  }
  
  const std::string&
  Iec104DataDefinition::GetDefaultProperty(const std::string arKey) const
  {
    throw std::invalid_argument("Key has no default argument.");
  }
}