//  cic
//
//  cic - Copyright (C) 2017-2018 Stanislav Demyanovich <mezozoysky@gmail.com>
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
/// \brief DAGNode template
/// \author Stanislav Demyanovich <mezozoysky@gmail.com>
/// \date 2018
/// \copyright cic is released under the terms of zlib/png license


#ifndef CIC_PLAN__DAG_NODE_HPP
#define CIC_PLAN__DAG_NODE_HPP

#include <memory>
#include <vector>
#include <limits>

namespace cic
{
namespace plan
{

template < typename DerivedT >
class DAGNode : public std::enable_shared_from_this< DerivedT >
{
public:
    static constexpr std::size_t BAD_INDEX{ std::numeric_limits< std::size_t >::max() };

    using DAGType = DerivedT;
    using DAGBase = DAGNode< DAGType >;
    using DAGShared = std::shared_ptr< DAGType >;
    using ChildList = std::vector< DAGShared >;

    DAGNode() = default;
    DAGNode( const DAGNode& other ) = delete;
    DAGNode& operator=( const DAGNode& other ) = delete;
    virtual ~DAGNode() noexcept = default;

    inline std::size_t getChildrenSize() const noexcept;
    std::size_t addChild( const DAGShared& child );
    void addChildren( const ChildList& list );
    DAGShared getChild( std::size_t index ) const noexcept;
    std::size_t getChildIndex( const DAGShared& child ) const noexcept;
    inline const ChildList& getChildren() const noexcept;

    inline DAGShared getParent() const noexcept;

protected:
    inline ChildList& children() noexcept;

    void setParent( const DAGShared& parent );

    virtual void onAddChild( const DAGShared& child, std::size_t index );

private:
    ChildList mChildren;
    std::weak_ptr< DAGType > mParent;
};

template < typename DerivedT >
inline std::size_t DAGNode< DerivedT >::getChildrenSize() const noexcept
{
    return ( mChildren.size() );
}

template < typename DerivedT >
std::size_t DAGNode< DerivedT >::addChild( const DAGShared& child )
{
    std::size_t index{ getChildrenSize() };
    children().push_back( child );
    child->setParent( DAGBase::shared_from_this() );
    onAddChild( child, index );
    return ( index );
}

template < typename DerivedT >
void DAGNode< DerivedT >::addChildren( const ChildList& list )
{
    for ( const auto& child : list )
    {
        addChild( child );
    }
}

template < typename DerivedT >
typename DAGNode< DerivedT >::DAGShared DAGNode< DerivedT >::getChild( std::size_t index ) const noexcept
{
    if ( index >= getChildrenSize() )
    {
        return ( nullptr );
    }
    return ( getChildren()[ index ] );
}

template < typename DerivedT >
std::size_t DAGNode< DerivedT >::getChildIndex( const DAGShared& child ) const noexcept
{
    std::size_t index{ 0 };
    for ( ; index <= getChildrenSize(); ++index ) // '<=' matters
    {
        if ( index == getChildrenSize() )
        {
            return ( BAD_INDEX );
        }
        else
        {
            if ( getChildren()[ index ] == child )
            {
                break;
            }
        }
    }
    return ( index );
}

template < typename DerivedT >
inline const typename DAGNode< DerivedT >::ChildList& DAGNode< DerivedT >::getChildren() const noexcept
{
    return ( mChildren );
}

template < typename DerivedT >
inline typename DAGNode< DerivedT >::DAGShared DAGNode< DerivedT >::getParent() const noexcept
{
    DAGShared parent{ mParent.lock() };
    return ( parent );
}

template < typename DerivedT >
inline typename DAGNode< DerivedT >::ChildList& DAGNode< DerivedT >::children() noexcept
{
    return ( mChildren );
}

template < typename DerivedT >
void DAGNode< DerivedT >::setParent( const DAGShared& parent )
{
    mParent = parent;
}

template < typename DerivedT >
void DAGNode< DerivedT >::onAddChild( const DAGShared& child, std::size_t index )
{
}

} // namespace plan
} // namespace cic

#endif // CIC_PLAN__DAG_NODE_HPP
