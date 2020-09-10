/////////////////////////////////////////////////////////////////////////////////////////
// JobBase.h
// 
// Abstract interface that all jobs for the Job Executer must implement
/////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <memory>

#include "Results.h"

namespace WordCounter {

	class JobBase {
	public:
        typedef std::shared_ptr<JobBase> SPtr;
        typedef std::weak_ptr<JobBase> WPtr;

        virtual ~JobBase(void) {};

		// This function must implement the job functionality. 
		// 'results' must be filled out with the computed results.
		virtual bool execute(const Results::WPtr& results) = 0;
	};

}
