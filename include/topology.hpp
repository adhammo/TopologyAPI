#ifndef TOPOLOGY_HPP
#define TOPOLOGY_HPP

#include <string>
#include <unordered_map>

#include "nlohmann/json.hpp"
#include "component.hpp"

class Topology;
typedef std::shared_ptr<Topology> TopologyPointer;
typedef std::shared_ptr<const Topology> TopologyConstPointer;
typedef std::weak_ptr<const Topology> TopologyWeakConstPointer;

/**
 * @brief Wrapper around topology data
 */
class Topology
{
private:
    template <typename T>
    using Dictionary = std::unordered_map<std::string, T>;

private:
    std::string m_id;                          /** @brief Topology id (string identifier) */
    Dictionary<ComponentPointer> m_components; /** @brief Topology components dictionary */

public:
    Topology() = default;
    Topology(const Topology &) = default;

public:
    /**
     * @brief Loads topology from a json string and returns it
     * 
     * @param topologyJson json string of topology
     * @return TopologyPointer pointer to loaded topology
     * @throw nlohmann::json::exceptions if topology json was invalid
     */
    static TopologyPointer loadTopologyFromJson(const nlohmann::json &topologyJson);

    /**
     * @brief Stores topology to a json string
     * 
     * @param topologyPtr pointer to topology to store
     * @param topologyJson json string to store topology
     */
    static void storeTopologyToJson(TopologyConstPointer topologyPtr, nlohmann::ordered_json &topologyJson);

public:
    /**
     * @brief Get topology id (string identifier)
     * 
     * @return const std::string& topology id
     */
    inline const std::string &getId() const { return m_id; }

    /**
     * @brief Get component (by id) from topology components
     * 
     * @param componentId requested component id
     * @return ComponentWeakConstPointer pointer to requested component
     * @throw std::out_of_range if component id was not found
     */
    inline ComponentWeakConstPointer getComponent(const std::string &componentId) const
    {
        // Try to get component; if didn't exist, throw error
        return (ComponentWeakConstPointer)m_components.at(componentId);
    }

public:
    /**
     * @brief Query all components in topology
     * 
     * @return std::vector<ComponentWeakConstPointer> list of pointers to all components
     */
    std::vector<ComponentWeakConstPointer> queryComponents() const;

    /**
     * @brief Query all components (by node id) in topology
     * 
     * @return std::vector<ComponentWeakConstPointer> list of pointers to all components (connected to node)
     */
    std::vector<ComponentWeakConstPointer> queryComponentsByNode(const std::string &nodeId) const;

public:
    /**
     * @brief Checks if topology is connected to node (by id)
     * 
     * @param nodeId node id to check connection for
     * @return true if topology is connected to node
     * @return false if topology isn't connected to node
     */
    bool isConnectedToNode(const std::string &nodeId) const;

private:
    /**
     * @brief Add or overwrite component (by id) in topology components
     * 
     * @param componentId added component id
     * @param componentPtr pointer to component
     */
    void setComponent(const std::string &componentId, ComponentPointer &componentPtr);
};

#endif // TOPOLOGY_HPP