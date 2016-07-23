//
//  CheckMap.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 06.07.16.
//
//

#ifndef CICHECK_CICHECK__TASK_CHECK_MAP_HPP
#define CICHECK_CICHECK__TASK_CHECK_MAP_HPP

#include <CICheck/task/ACheckMap.hpp>
#include <CICheck/xmlu/LoadableFromXML.hpp>
#include "RuleSet.hpp"
#include <map>

namespace cic
{

class CheckMap
: public task::ACheckMap
, public xmlu::LoadableFromXML
{
public:
	using Ptr = std::shared_ptr< CheckMap >;

	virtual ~CheckMap() noexcept = default;

	virtual bool check( const std::vector< std::string >& sequence ) override;
	virtual void loadFromXML( const xmlu::Node* root, indu::Industry* industry ) override;

	virtual std::size_t getSize() const override;
	virtual std::vector< std::string >&& getKeys() const override;
	virtual bool hasKey( const std::string& key ) const override;
	virtual task::ARuleSet::Ptr getByKey( const std::string& key ) const override;

protected:
	std::map< std::string, RuleSet::Ptr > mRulesets;
};



} // namespace cic

#endif /* CICHECK_CICHECK__TASK_CHECK_MAP_HPP */
