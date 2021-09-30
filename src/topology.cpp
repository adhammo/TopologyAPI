#include "topology.hpp"
#include <utility>

using json = nlohmann::json;
using ordered_json = nlohmann::ordered_json;

TopologyPointer Topology::loadTopologyFromJson(const json &topologyJson)
{
    // Create topology and assign id
    TopologyPointer topologyPtr = std::make_shared<Topology>();
    topologyPtr->m_id = topologyJson.at("id").get<std::string>();

    // Extract components and add them to topology
    auto &components = topologyJson.at("components");
    for (auto &componentJson : components)
    {
        auto componentPtr = Component::loadComponentFromJson(componentJson);
        topologyPtr->setComponent(componentPtr->getId(), componentPtr);
    }

    // Return pointer to extracted topology
    return topologyPtr;
}

void Topology::storeTopologyToJson(TopologyConstPointer topologyPtr, ordered_json &topologyJson)
{
    // Store topology id
    topologyJson["id"] = topologyPtr->m_id;

    // Store topology components
    for (auto &componentEntry : topologyPtr->m_components)
    {
        ordered_json componentJson;
        auto &componentPtr = componentEntry.second;
        Component::storeComponentToJson(componentPtr, componentJson);
        topologyJson["components"].push_back(std::move(componentJson));
    }
}

std::vector<ComponentWeakConstPointer> Topology::queryComponents() const
{
    // Get components from components map and return them as list of pointers
    std::vector<ComponentWeakConstPointer> componentsPtrList;
    for (auto &componentEntry : m_components)
    {
        auto &componentPtr = componentEntry.second;
        componentsPtrList.push_back((ComponentWeakConstPointer)componentPtr);
    }
    return componentsPtrList;
}

std::vector<ComponentWeakConstPointer> Topology::queryComponentsByNode(const std::string &nodeId) const
{
    // Get components (connected to node) from components map and return them as list of pointers
    std::vector<ComponentWeakConstPointer> componentsPtrList;
    for (auto &componentEntry : m_components)
    {
        auto &componentPtr = componentEntry.second;
        if (componentPtr->isConnectedToNode(nodeId))
            componentsPtrList.push_back((ComponentWeakConstPointer)componentPtr);
    }
    return componentsPtrList;
}

bool Topology::isConnectedToNode(const std::string &nodeId) const
{
    // Iterate over all connections to check for connection
    for (auto &componentEntry : m_components)
    {
        auto &componentPtr = componentEntry.second;
        if (componentPtr->isConnectedToNode(nodeId))
            return true;
    }
    return false;
}

void Topology::setComponent(const std::string &componentId, ComponentPointer &componentPtr)
{
    // Try to insert component; if already existed, overwrite it
    auto res = m_components.insert(std::make_pair(componentId, std::move(componentPtr)));
    ComponentPointer &topologyComponentPtr = res.first->second;
    // Overwrite toplogy component pointer
    if (!res.second)
        topologyComponentPtr = std::move(componentPtr);
}