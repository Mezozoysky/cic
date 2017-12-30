//  cic
//
//  cic - Copyright (C) 2017 Stanislav Demyanovich <mezozoysky@gmail.com>
//
//  This software is provided 'as-is', without any express or
//  implied warranty. In no event will the authors be held
//  liable for any damages arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute
//  it freely, subject to the following restrictions:
//
//  1.  The origin of this software must not be misrepresented;
//      you must not claim that you wrote the original software.
//      If you use this software in a product, an acknowledgment
//      in the product documentation would be appreciated but
//      is not required.
//
//  2.  Altered source versions must be plainly marked as such,
//      and must not be misrepresented as being the original software.
//
//  3.  This notice may not be removed or altered from any
//      source distribution.


/// \file
/// \brief ARule - XMLed version of rule abstraction from cic-goal library
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2017
/// \copyright cic is released under the terms of zlib/png license


#ifndef CIC_PLAN__SERIALIZABLE_HPP
#define CIC_PLAN__SERIALIZABLE_HPP

#include <string>
#include <cic/industry/Industry.hpp>

namespace Poco
{
namespace XML
{
class Element;
}
} // namespace Poco

namespace cic
{
namespace plan
{

#define CLASSINFO( className )                                                                               \
public:                                                                                                      \
    static const std::string& getClassNameStatic()                                                           \
    {                                                                                                        \
        static const std::string staticClassName{ #className };                                              \
        return ( staticClassName );                                                                          \
    };                                                                                                       \
    virtual const std::string& getClassName() const noexcept override                                        \
    {                                                                                                        \
        return ( getClassNameStatic() );                                                                     \
    }

class ClassInfo
{
public:
    ClassInfo() = default;
    virtual ~ClassInfo() noexcept = default;

    virtual const std::string& getClassName() const noexcept = 0;
};


class Serializable : public ClassInfo
{
public:
    Serializable() = default;
    virtual ~Serializable() noexcept = default;

    // virtual const std::string& getClassName() const noexcept = 0;

    virtual void loadFromXML( Poco::XML::Element* root, industry::Industry* industry ) = 0;
    virtual void saveToXML( Poco::XML::Element* root ) const = 0;
};

} // namespace plan
} // namespace cic

#endif // CIC_PLAN__SERIALIZABLE_HPP
