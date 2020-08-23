/*
 * resourcepath.cpp
 *
 *  Created on: Aug 22, 2020
 *      Author: blox
 */

#include "resourcepath.hpp"

#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>

namespace TC
{
  namespace REST
  {
    constexpr char ResourcePath::PATH_SEPARATOR;
    constexpr char ResourcePath::ALLOWED_PARSER_CHARS[];

    std::ostream&
    operator<<(std::ostream& arOut, const TC::REST::ResourcePath& arPath)
    {
      arOut << arPath.ToString();
      return arOut;
    }

    ResourcePath::ResourcePath()
      : mComponents()
    {
    }

    ResourcePath::ResourcePath(const ResourcePath::EscapedString& arString)
     : mComponents()
    {
      ParseString(arString);
    }

    bool
    ResourcePath::IsChild(const ResourcePath& arOther, ComparePolicy aPolicy) const
    {
      return arOther.IsParent(*this, aPolicy);
    }

    bool
    ResourcePath::IsParent(const ResourcePath& arOther, ComparePolicy aPolicy) const
    {
      if (GetSize() >= arOther.GetSize())
        return false;

      if ((aPolicy == CP_IMMEDIATE) &&
          (GetSize() != (arOther.GetSize() - 1)))
        return false; // Not the immediate parent

      size_t i = 0;
      for (; i < GetSize(); ++i)
        if (mComponents[i] != arOther.mComponents[i])
          return false;

      return true;
    }

    ResourcePath::EscapedString
    ResourcePath::ToString() const
    {
      EscapedString result;
      for (const auto& dir : mComponents)
      {
        result.push_back(PATH_SEPARATOR);
        result.append(Escape(dir));
      }

      return result;
    }

    void
    ResourcePath::AddComponent(const UnescapedString& arString)
    {
      mComponents.push_back(arString);
    }

    void
    ResourcePath::PopComponent()
    {
      mComponents.pop_back();
    }

    void
    ResourcePath::ParseString(const ResourcePath::EscapedString& arString)
    {
      mComponents.clear();

      if (arString.empty() ||
          ((arString.size() == 1) && (arString[0] == PATH_SEPARATOR))) // "/" is an empty path
        return;

      if (arString[0] != PATH_SEPARATOR) // A valid path has the form "/x"
        throw std::invalid_argument("The path should start with a separator");


      if (arString.find_first_not_of(ALLOWED_PARSER_CHARS) != EscapedString::npos)
        throw std::invalid_argument("Invalid character detected");

      size_t component_begin = 1;
      size_t component_end = 0;

      std::vector<UnescapedString> buffer;

      while (component_begin < arString.size() && component_end != EscapedString::npos)
      {
        component_end = arString.find(PATH_SEPARATOR, component_begin);

        if (component_begin == component_end)
          throw std::invalid_argument("Empty path component detected.");

        const size_t count = component_end - component_begin;
        buffer.push_back(Unescape(arString.substr(component_begin, count)));

        // Next possible component starts after the "/"
        component_begin = component_end + 1;
      }

      mComponents = std::move(buffer); // No exception thrown until here
    }

    ResourcePath::EscapedString
    ResourcePath::Escape(const ResourcePath::UnescapedString& arString) const
    {
      std::ostringstream result;

      for(const auto& c : arString)
      {
        if (!IsSpecialCharacter(c))
        {
          result << c;
        }
        else
        {
          result << "%" << std::uppercase << std::setfill('0') << std::setw(2) << std::hex << static_cast<int>(c);
        }
      }
      return result.str();
    }

    ResourcePath::UnescapedString
    ResourcePath::Unescape(const ResourcePath::EscapedString& arString) const
    {
      ResourcePath::UnescapedString result;
      result.reserve(arString.size());

      size_t pos = 0;

      while (pos < arString.size())
      {
        const char check = arString[pos];

        if (check != '%')
        {
          result.push_back(check);
          ++pos;
          continue;
        }

        if (pos + 2 >= arString.size())
          throw std::invalid_argument("Not enough characters left for decoding.");

        static constexpr int BASE_HEX = 16;
        const char target_char = std::stoi(arString.substr(pos + 1, 2), nullptr, BASE_HEX);

        if (!IsSpecialCharacter(target_char))
          throw std::invalid_argument("Invalid encoded character");

        result.push_back(target_char);
        pos += 3; // "%xx"
      }

      return result;
    }

    bool
    ResourcePath::IsSpecialCharacter(char aChar) const
    {
      switch (aChar)
      {
        case ':':
        case '/':
        case '?':
        case '#':
        case '[':
        case ']':
        case '@':
        case '!':
        case '$':
        case '&':
        case '\'':
        case '(':
        case ')':
        case '*':
        case '+':
        case ',':
        case ';':
        case '=':
        case '%':
        case ' ':
          return true;

        default:
          return false;
      }
    }
  } /* namespace REST */
} /* namespace TC */
