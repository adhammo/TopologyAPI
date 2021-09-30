#include <gtest/gtest.h>

#include "component.hpp"
#include "topology.hpp"
#include "topologyapi.hpp"
#include "test_topology.hpp"

TEST(TopologyAPITests, GetRemoveTopologyTest)
{
    // Read topology
    TopologyAPI::readTopology("top1.json");

    // Test getTopology
    EXPECT_THROW(TopologyAPI::getTopology("top2"), std::out_of_range);
    auto topologyWeakPtr = TopologyAPI::getTopology("top1");
    if (auto topologyPtr = topologyWeakPtr.lock())
    {
        // Test topology
        testTopology(topologyPtr);
    }

    // Test removeTopology fail
    EXPECT_NO_THROW(TopologyAPI::removeTopology("top2"));
    topologyWeakPtr = TopologyAPI::getTopology("top1");
    if (auto topologyPtr = topologyWeakPtr.lock())
    {
        // Test topology
        testTopology(topologyPtr);
    }

    // Test removeTopology success
    TopologyAPI::removeTopology("top1");
    EXPECT_THROW(TopologyAPI::getTopology("top1"), std::out_of_range);
}