/*
 * resourcepath.hpp
 *
 *  Created on: Aug 22, 2020
 *      Author: blox
 */

#ifndef TC_REST_RESOURCEPATH_HPP_
#define TC_REST_RESOURCEPATH_HPP_

#include <string>
#include <vector>

namespace TC
{
  namespace REST
  {
    class ResourcePath
    {
    public:
      enum ComparePolicy
      {
        CP_IMMEDIATE, //!< Only direct parent or child
        CP_ANY        //!< Any parent or child
      };

      static constexpr char PATH_SEPARATOR = '/';
      static constexpr char ALLOWED_PARSER_CHARS[] =
          "/.%abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

      /// Special URI characters are percent encoded
      using EscapedString = std::string;
      /// ASCII string. May contains special characters
      using UnescapedString = std::string;

      explicit ResourcePath();
      /// Construct from escaped URI path string
      explicit ResourcePath(const EscapedString& arString);

      ~ResourcePath() = default;
      ResourcePath(const ResourcePath& arOther) = default;
      ResourcePath(ResourcePath&& arrOther) = default;
      ResourcePath& operator=(const ResourcePath& arOther) = default;
      ResourcePath& operator=(ResourcePath&& arrOther) = default;


      inline const UnescapedString& operator[](size_t aPos) const {return mComponents.at(aPos);}
      void AddComponent(const UnescapedString& arString);
      void PopComponent();

      inline bool operator==(const ResourcePath& arOther) const {return mComponents == arOther.mComponents;}
      inline bool operator< (const ResourcePath& arOther) const {return mComponents <  arOther.mComponents;}
      inline bool operator!=(const ResourcePath& arOther) const {return !operator==(arOther);}
      inline bool operator> (const ResourcePath& arOther) const {return !operator<=(arOther);}
      inline bool operator>=(const ResourcePath& arOther) const {return !operator< (arOther);}
      inline bool operator<=(const ResourcePath& arOther) const {return !operator> (arOther);}

      /// Number of path components
      inline size_t GetSize() const {return mComponents.size();}

      /**
       * @brief Is the current ResourcePath a child of the provided path?
       * @param arOther The other path
       * @param aPolicy Consider any valid parent or the direct parent only?
       *
       * @note Equal paths are neither parent nor child to each other
       */
      bool IsChild(const ResourcePath& arOther, ComparePolicy aPolicy = CP_ANY) const;

      /**
       * @brief Is the current ResourcePath a parent of the provided path?
       * @param arOther The other path
       * @param aPolicy Consider any valid child or the direct child only?
       *
       * @note Equal paths are neither parent nor child to each other
       */
      bool IsParent(const ResourcePath& arOther, ComparePolicy aPolicy = CP_ANY) const;

      /// Get the path as escaped ASCII string
      EscapedString ToString() const;

    private:
      void ParseString(const EscapedString& arString);
      EscapedString Escape(const UnescapedString& arString) const;
      UnescapedString Unescape(const EscapedString& arString) const;
      bool IsSpecialCharacter(char aChar) const;

      std::vector<UnescapedString> mComponents;
    };

    std::ostream& operator<<(std::ostream& arOut, const TC::REST::ResourcePath& arPath);

  } /* namespace REST */
} /* namespace TC */


#endif /* TC_REST_RESOURCEPATH_HPP_ */
