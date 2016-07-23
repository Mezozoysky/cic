//
//  TargetSet.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 06.07.16.
//
//

#ifndef CICHECK_CICHECK__TASK_TARGET_SET_HPP
#define CICHECK_CICHECK__TASK_TARGET_SET_HPP

#include <CICheck/task/ATargetSet.hpp>
#include <CICheck/xmlu/LoadableFromXML.hpp>
#include <vector>
#include <string>

namespace cic
{


class TargetSet
: public task::ATargetSet
, public xmlu::LoadableFromXML
{
public:
	using Ptr = std::shared_ptr< TargetSet >;

	TargetSet() noexcept = default;
	virtual ~TargetSet() noexcept = default;

// 	virtual std::vector< std::string > calcSequenceFor( const std::string& targetName ) const override;
	virtual void loadFromXML( const xmlu::Node* root, indu::Industry* industry ) override;

// 	virtual const std::vector< std::string >& targets() const override;
// 	virtual const std::string& defaultTarget() const override;

protected:
	virtual void loadTargetFromXML( const xmlu::Node* root, indu::Industry* industry );
};


} // namespace cic

#endif /* CICHECK_CICHECK__TASK_TARGET_SET_HPP */
