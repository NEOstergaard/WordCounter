/////////////////////////////////////////////////////////////////////////////////////////
// JobExecuter.h
// 
// Implements functionality to execute a number of jobs in parallel
// The number of parallel jobs running at one time can be configured
/////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>
#include <thread>

#include "JobBase.h"

namespace WordCounter {

	class JobExecuter {
	public:
		typedef std::shared_ptr<JobExecuter> SPtr;
		typedef std::weak_ptr<JobExecuter> WPtr;

		JobExecuter(const JobExecuter&) = delete;
		JobExecuter& operator=(const JobExecuter&) = delete;

		virtual ~JobExecuter() {}

		// Create an object instance
		static SPtr create(const uint32_t& no_parallel_jobs);

		// Add a 'job' to be executed later.
		void add_job(const JobBase::SPtr& job);

		// Execute all collected jobs. Returns when all jobs has finished.
		// Each job must update 'results' with the results computed.
		void start(const Results::WPtr& results);

	private:
		JobExecuter(const uint32_t& no_parallel_jobs) :
			no_parallel_jobs_(no_parallel_jobs) {
		}

		uint32_t no_parallel_jobs_;

		typedef struct {
			JobBase::SPtr job;
			enum {
				NotStarted,
				Running,
				Finished
			} status;
		} JobDescription;

		// Stores all the jobs added
		std::vector<JobDescription> jobs_;

		// Currently running threads
		std::vector<std::thread> currently_running_threads_;
	};

}
