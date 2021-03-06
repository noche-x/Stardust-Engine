#pragma once
#include <vector>
#include <iostream>
#include <fstream>
#include <Utilities/Timer.h>


namespace Stardust::Profiling {

	struct ProfileRun {
		double time;
		double fromStart;
	};

	struct ProfileData {
		double minTime;
		double avgTime;
		double maxTime;
		double medianTime;

		std::vector<ProfileRun> runs;
		std::string method;
		std::vector<ProfileData> subProfiles;
	};

	class Profiler {
	public:
		Profiler(std::string name = "APP");

		void beginProfileMethod();
		void endProfileMethod(std::string methodName);

		void beginProfileSubMethod();
		void endProfileSubMethod(std::string macroMethod, std::string subMethod);

		void outputStats(std::string fileName = "stardust_profiler.log");


	private:
		
		int findMethod(std::string name);

		std::vector<ProfileData> methodData;
		Utilities::Timer t;
		Utilities::Timer subT;
		std::string profilerName;
	};


	#define SDE_PROFILE(pf, funcCall, funcName) pf.beginProfileMethod(); func; pf.endProfileMethod(funcName)
}