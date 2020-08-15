/*
 * startarg.hpp
 *
 *  Created on: Aug 11, 2020
 *      Author: blox
 */

#ifndef STARTARG_HPP_
#define STARTARG_HPP_

#include <iomanip>
#include <memory>
#include <vector>
#include <sstream>
#include <string>

namespace VRTU
{
  class StartArg
  {
  public:
    enum RequirementLevel
    {
      OPTIONAL,
      REQUIRED
    };

    StartArg(const std::string& arShortKey, const std::string& arLongKey, const std::string& arDescription,
             RequirementLevel aLevel);

    virtual ~StartArg();

    virtual bool Read(const std::vector<std::string>& arArgs, int& arCurrentIndex) = 0;
    virtual void Print(std::ostream& arStream) const = 0;

    inline bool IsRequired() const {return mLevel == REQUIRED;}
    inline bool IsSet() const {return mSet == true;}
    inline const std::string& GetShortKey() const {return mShortKey;}
    inline const std::string& GetLongKey() const {return mLongKey;}
    inline const std::string& GetDescription() const {return mDesc;}
    inline std::string GetRequiredString() const {if (IsRequired()) return "REQUIRED"; return "OPTIONAL";}

  protected:
    inline void SetStatus(bool aIsSet) {mSet = aIsSet;}
    inline bool IsMatch(const std::string& arKey) {return (arKey == mShortKey) || (arKey == mLongKey);}

  private:
    std::string mShortKey;
    std::string mLongKey;
    std::string mDesc;
    RequirementLevel mLevel;
    bool mSet;
  };

  std::ostream& operator<<(std::ostream& arStream, const StartArg& arArg);

  class FlagArg : public StartArg
  {
  public:
    FlagArg(const std::string& arShortKey, const std::string& arLongKey,
            bool& arDestination, RequirementLevel aLevel, const std::string& arDescription);

    virtual bool Read(const std::vector<std::string>& arArgs, int& arCurrentIndex) override;
    virtual void Print(std::ostream& arStream) const override;

  private:
    bool& mrDestination;
  };

  class IpArg : public StartArg
  {
  public:
    IpArg(const std::string& arShortKey, const std::string& arLongKey,
          std::string& arDestination, RequirementLevel aLevel, const std::string& arDescription);

    virtual bool Read(const std::vector<std::string>& arArgs, int& arCurrentIndex) override;
    virtual void Print(std::ostream& arStream) const override;

  private:
    std::string& mrDestination;
  };

  template<typename T>
  class ValueArg : public StartArg
  {
  public:
    using value_type = T;

    ValueArg(const std::string& arShortKey, const std::string& arLongKey,
             value_type& arDestination, RequirementLevel aLevel, const std::string& arDescription)
      : StartArg(arShortKey, arLongKey, arDescription, aLevel), mrDestination(arDestination) {}

    virtual bool Read(const std::vector<std::string>& arArgs, int& arCurrentIndex) override
    {
      if (IsMatch(arArgs[arCurrentIndex]))
      {
        int index = arCurrentIndex; // Modify reference on success only

        if ((index + 1) >= arArgs.size())
          return false;

        ++index;
        std::istringstream stream(arArgs[index]);
        value_type temp; // Modify reference on success only
        stream >> temp;

        if (!stream.eof() || stream.fail() || stream.bad())
          return false;

        mrDestination = temp;
        SetStatus(true);
        arCurrentIndex = ++index;
        return true;
      }
      return false;
    }

    virtual void Print(std::ostream& arStream) const override
    {
      std::string value_desc = GetLongKey();
      value_desc.append(" <value>");

      arStream << std::setw(10) << std::left << GetShortKey()
               << std::setw(30) << std::left << value_desc
               << std::setw(10) << std::left << GetRequiredString()
               << std::setw(50) << std::left << GetDescription();
    }

  private:
    value_type& mrDestination;
  };

  class StartArgParser
  {
  public:
    using SpStartArg = std::unique_ptr<StartArg>;
    using ArgVector = std::vector<SpStartArg>;

    void Register(SpStartArg&& arrSpArg);
    bool Parse(int argc, char* argv[]);

    bool FoundAllRequired() const;
    inline const ArgVector& GetRegisteredArgs() const
    {
      return mArgs;
    }

  private:
    ArgVector mArgs;
  };
} /* namespace VRTU */

#endif /* STARTARG_HPP_ */
