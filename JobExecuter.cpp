/////////////////////////////////////////////////////////////////////////////////////////
// JobExecuter.cpp
/////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "JobExecuter.h"

namespace WordCounter {

	JobExecuter::SPtr JobExecuter::create(const uint32_t& no_parallel_jobs) {
		SPtr instance(new JobExecuter(no_parallel_jobs));
		return instance;
	}

	void JobExecuter::add_job(const JobBase::SPtr& job) {
		jobs_.push_back({job, JobDescription::NotStarted });
	}

	void JobExecuter::start(const Results::WPtr& results) {
		// std::cout << "JobExecuter::start" << std::endl;

		bool jobs_to_execute = !jobs_.empty();
		while (jobs_to_execute) {
			// std::cout << "Loop through jobs" << std::endl;

			// Loop through all jobs and find those not started yet
			uint32_t no_started_jobs = 0;
			bool jobs_not_started_yet = false;
			for (auto& job_entry : jobs_) {
				// std::cout << "Job: Current status - " << job_entry.status  << std::endl;

				if (job_entry.status == JobDescription::NotStarted) {
					// This is a job not started yet
					jobs_not_started_yet = true;

					// Have we started the allowed number at one time
					if (no_started_jobs >= no_parallel_jobs_) {
						// std::cout << "Too many jobs started now: " << no_started_jobs << std::endl;
						break;
					}

					// Start the job
					// std::cout << "Start job" << std::endl;
					job_entry.status = JobDescription::Running;

					// For testing sequential execution
					// job_entry.job->execute(results);

					currently_running_threads_.push_back(std::thread(&JobBase::execute, job_entry.job.get(), results));

					no_started_jobs++;
				}
			}

			// Wait for the jobs to finish
			if (jobs_not_started_yet) {
				// std::cout << "Wait for jobs to finish" << std::endl;
				for (auto& thread_item : currently_running_threads_) {
					thread_item.join();
					// std::cout << "Thread joined" << std::endl;
				}
				currently_running_threads_.clear();
			}

			// Mark all running jobs as finished
			for (auto& job_entry : jobs_) {
				if (job_entry.status == JobDescription::Running) {
					// std::cout << "Job marked finished" << std::endl;
					job_entry.status = JobDescription::Finished;
				}
			}

			jobs_to_execute = jobs_not_started_yet;
			// std::cout << "Iterate main loop again: " << jobs_to_execute  << std::endl;
		}
	}

}
