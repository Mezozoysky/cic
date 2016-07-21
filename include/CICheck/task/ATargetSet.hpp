//
//  ATargetSet.hpp
//  CICheck
//
//  Created by Stanislav Demyanovich on 28.06.16.
//
//

#ifndef CICHECK_TASK__A_TARGET_SET_HPP
#define CICHECK_TASK__A_TARGET_SET_HPP

#include <CICheck/task/TaskUnit.hpp>
#include <CICheck/task/ARule.hpp>
#include <vector>
#include <string>
#include <map>

namespace cic
{
namespace task
{

struct TgtData
{
	std::string successTrigger{ "" };
	std::string failureTrigger{ "" };
	std::vector< std::string > deps{};
	std::vector< ARule::Ptr > rules{};
};


class ATargetSet
: public TaskUnit
{
public:
	using Ptr = std::shared_ptr< ATargetSet >;
	using TgtMap = std::map< std::string, TgtData >;
	using Seq = std::vector< std::string >;

// 	static const std::size_t BAD_INDEX;


public:
    ATargetSet() noexcept;
	virtual ~ATargetSet() noexcept = default;

	virtual bool check( const std::string& tgtName );
	std::size_t getSize() const noexcept;

	const std::string& defaultTarget() const noexcept;

protected:
// 	virtual void addTarget( const ATarget::Ptr& tgt );
// 	virtual void removeTarget( const std::string& tgtName );
// 	virtual void clearTargets();

	void buildSequence( const std::string& tgtName, Seq& seq ) const;
	bool isADependsOnB( const std::string& tgtA, const std::string& tgtB ) const;
protected:
	TgtMap mTargets;
	std::string mDefault;
};


} // namespace task
} // namespace cic

#endif /* CICHECK_TASK__A_TARGET_SET_HPP */
