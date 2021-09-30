#include "topologyapi.hpp"
#include <fstream>
#include <stdexcept>

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

template <typename T>
using Dictionary = TopologyAPI::Dictionary<T>;

Dictionary<TopologyPointer> TopologyAPI::topologies;

TopologyWeakConstPointer TopologyAPI::readTopology(const std::string &fileName)
{
    // Open topology json file for read
    std::ifstream topologyJsonFile(fileName);
    if (!topologyJsonFile.good())
        throw std::invalid_argument("[TopologyAPI] (readTopology): Couldn't find topology file.");

    // Read topology json file into json object
    json topologyJson;
    topologyJsonFile >> topologyJson;
    topologyJsonFile.close();

    // Load topology from json
    TopologyPointer topologyPtr = Topology::loadTopologyFromJson(topologyJson);

    // Add topology to topologies and return it
    return setTopology(topologyPtr->getId(), topologyPtr);
}

void TopologyAPI::writeTopology(const std::string &topologyId, const std::string &fileName)
{
    // Open topology json file for write
    std::ofstream topologyJsonFile(fileName);
    if (!topologyJsonFile.good())
        throw std::invalid_argument("[TopologyAPI] (writeTopology): Couldn't find topology file.");

    // Get topology by id and store in json
    ordered_json topologyJson;
    auto &topologyPtr = TopologyAPI::topologies.at(topologyId);
    Topology::storeTopologyToJson(topologyPtr, topologyJson);

    // Write topology json object to json file
    topologyJsonFile << topologyJson.dump(2);
    topologyJsonFile.flush();
    topologyJsonFile.close();
}

std::vector<TopologyWeakConstPointer> TopologyAPI::queryTopologies()
{
    // Get topologies from topologies map and return them as list of pointers
    std::vector<TopologyWeakConstPointer> topologyPtrList;
    for (auto &topologyEntry : topologies)
    {
        auto &topologyPtr = topologyEntry.second;
        topologyPtrList.push_back((TopologyWeakConstPointer)topologyPtr);
    }
    return topologyPtrList;
}

std::vector<TopologyWeakConstPointer> TopologyAPI::queryTopologiesByNode(const std::string &nodeId)
{
    // Get topologies (connected to node) from topologies map and return them as list of pointers
    std::vector<TopologyWeakConstPointer> topologyPtrList;
    for (auto &topologyEntry : topologies)
    {
        auto &topologyPtr = topologyEntry.second;
        if (topologyPtr->isConnectedToNode(nodeId))
            topologyPtrList.push_back((TopologyWeakConstPointer)topologyPtr);
    }
    return topologyPtrList;
}

TopologyWeakConstPointer TopologyAPI::setTopology(const std::string &topologyId, TopologyPointer &topologyPtr)
{
    // Try to insert toplogy; if already existed, overwrite it
    auto res = topologies.insert(std::make_pair(topologyId, std::move(topologyPtr)));
    TopologyPointer &apiTopologyPtr = res.first->second;
    // Overwrite toplogy
    if (!res.second)
        apiTopologyPtr = std::move(topologyPtr);
    return (TopologyWeakConstPointer)apiTopologyPtr;
}