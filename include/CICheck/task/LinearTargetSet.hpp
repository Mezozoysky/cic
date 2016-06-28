//
//  LinearTargetSet.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 29.06.16.
//
//

#ifndef CICHECK_TASK__LINEAR_TARGET_SET_HPP
#define CICHECK_TASK__LINEAR_TARGET_SET_HPP

#include <CICheck/task/AbstractTargetSet.hpp>

namespace cic
{
namespace task
{

class LinearTargetSet
: public AbstractTargetSet
{
public:
	LinearTargetSet( const Sequence& targetNames ) noexcept;
	virtual ~LinearTargetSet() noexcept;

	virtual const Sequence calcSequenceFor( const std::string& targetName ) const override;
};

} // namespace task
} // namespace cic

#endif /* CICHECK_TASK__LINEAR_TARGET_SET_HPP */
