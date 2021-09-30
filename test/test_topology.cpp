#include <gtest/gtest.h>
#include "test_topology.hpp"

void testTopology(TopologyConstPointer &topologyPtr)
{
    // Test topology getId
    EXPECT_EQ(topologyPtr->getId(), "top1");

    // Test topology getComponent
    EXPECT_THROW(topologyPtr->getComponent("res2"), std::out_of_range);
    auto componentWeakPtr = topologyPtr->getComponent("res1");
    if (auto componentPtr = componentWeakPtr.lock())
    {
        // Test component getId & getType
        EXPECT_EQ(componentPtr->getId(), "res1");
        EXPECT_EQ(componentPtr->getType(), "resistor");

        // Test component getAttribute
        EXPECT_THROW(componentPtr->getAttribute("resistance2"), std::out_of_range);
        auto &resistance = componentPtr->getAttribute("resistance");
        EXPECT_EQ(resistance.id, "resistance");
        EXPECT_EQ(resistance.def, 100);
        EXPECT_EQ(resistance.min, 10);
        EXPECT_EQ(resistance.max, 1000);

        // Test component getConnection
        EXPECT_THROW(componentPtr->getConnection("t3"), std::out_of_range);
        auto &t1 = componentPtr->getConnection("t1");
        EXPECT_EQ(t1.id, "t1");
        EXPECT_EQ(t1.nodeId, "vdd");
        auto &t2 = componentPtr->getConnection("t2");
        EXPECT_EQ(t2.id, "t2");
        EXPECT_EQ(t2.nodeId, "n1");

        // Test component isConnectedToNode
        EXPECT_EQ(componentPtr->isConnectedToNode("n3"), false);
        EXPECT_EQ(componentPtr->isConnectedToNode("vdd"), true);
        EXPECT_EQ(componentPtr->isConnectedToNode("n1"), true);
    }
}