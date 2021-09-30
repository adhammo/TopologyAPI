#include <gtest/gtest.h>

#include "component.hpp"
#include "topology.hpp"
#include "topologyapi.hpp"
#include "test_topology.hpp"

TEST(TopologyAPITests, QueryTopologiesTest)
{
    // Test queryTopologies empty
    auto topologiesPtrList = TopologyAPI::queryTopologies();
    EXPECT_EQ(topologiesPtrList.size(), 0);

    // Read topology
    TopologyAPI::readTopology("top1.json");

    // Test queryTopologies one
    topologiesPtrList = TopologyAPI::queryTopologies();
    EXPECT_EQ(topologiesPtrList.size(), 1);
    if (auto topologyPtr = topologiesPtrList[0].lock())
    {
        // Test topology
        testTopology(topologyPtr);
    }

    // Test queryTopologies removed
    TopologyAPI::removeTopology("top1");
    topologiesPtrList = TopologyAPI::queryTopologies();
    EXPECT_EQ(topologiesPtrList.size(), 0);

    // Test queryTopologiesByNode empty
    topologiesPtrList = TopologyAPI::queryTopologiesByNode("vin");
    EXPECT_EQ(topologiesPtrList.size(), 0);

    // Read topology
    TopologyAPI::readTopology("top1.json");

    // Test queryTopologiesByNode one
    topologiesPtrList = TopologyAPI::queryTopologiesByNode("vin");
    EXPECT_EQ(topologiesPtrList.size(), 1);
    if (auto topologyPtr = topologiesPtrList[0].lock())
    {
        // Test topology
        testTopology(topologyPtr);
    }

    // Test queryTopologiesByNode no-found
    topologiesPtrList = TopologyAPI::queryTopologiesByNode("n2");
    EXPECT_EQ(topologiesPtrList.size(), 0);

    // Test queryTopologies removed
    TopologyAPI::removeTopology("top1");
    topologiesPtrList = TopologyAPI::queryTopologiesByNode("n2");
    EXPECT_EQ(topologiesPtrList.size(), 0);
}