#include <gtest/gtest.h>

#include "component.hpp"
#include "topology.hpp"
#include "topologyapi.hpp"
#include "test_topology.hpp"

TEST(TopologyAPITests, ReadWriteTopologyTest)
{
    // Set random seed
    srand((unsigned int)time(nullptr));

    // Generate random name for topologyFileName
    std::string wroteTopFilename = "top" + std::to_string(rand()) + ".json";

    // Test readTopology
    auto topologyWeakPtr = TopologyAPI::readTopology("top1.json");
    if (auto topologyPtr = topologyWeakPtr.lock())
    {
        // Test topology
        testTopology(topologyPtr);

        // Write topology to randomly generated file
        TopologyAPI::writeTopology(topologyPtr->getId(), wroteTopFilename);
    }

    // Test writeTopology through reading prev wrote topology
    topologyWeakPtr = TopologyAPI::readTopology(wroteTopFilename);
    if (auto topologyPtr = topologyWeakPtr.lock())
    {
        // Test topology
        testTopology(topologyPtr);
    }
}