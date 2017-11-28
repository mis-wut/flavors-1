#include "multiConfigMasks.h"

#include <fstream>
#include <iostream>

using namespace Flavors;

namespace FlavorsBenchmarks
{
	std::string MultiConfigMasksBenchmark::Label = "Count;Seed;Config;MinLen;MaxLen;Generation;Sort;Reshape;Build;Find;LevelsSizes;HitRate";

	void MultiConfigMasksBenchmark::recordParameters(Flavors::Configuration& config)
	{
		std::ofstream file{resultPath.c_str(), std::ios_base::app | std::ios_base::out};
		file << count << ";" << seed << ";" << config << ";" << minLen << ";" << maxLen << ";";
		file.close();
	}

	void MultiConfigMasksBenchmark::Run()
	{
		generateRawMasks();

		for(auto config : configs)
			runForConfig(config);
	}

	void MultiConfigMasksBenchmark::generateRawMasks()
	{
		timer.Start();
		rawMasks = Masks{Configuration::DefaultConfig32, count};
		rawMasks.FillRandom(seed, maxLen, minLen);
		measured.Generation = timer.Stop();

		timer.Start();
		rawMasks.Sort();
		measured.Sort = timer.Stop();
	}

	void MultiConfigMasksBenchmark::runForConfig(Flavors::Configuration& config)
	{
		recordParameters(config);

		timer.Start();
		auto masks = rawMasks.ReshapeMasks(config);
		measured.Reshape = timer.Stop();

		timer.Start();
		Tree tree{masks};
		measured.Build = timer.Stop();

		timer.Start();
		tree.FindMasks(masks, result.Get());
		measured.Find = timer.Stop();

		measured.appendToFile(resultPath);
		recordStatistics(tree);
	}
}