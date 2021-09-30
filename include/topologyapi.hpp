#ifndef TOPOLOGYAPI_HPP
#define TOPOLOGYAPI_HPP

#include <string>
#include <unordered_map>
#include <utility>
#include <memory>

#include "nlohmann/json.hpp"
#include "topology.hpp"

class TopologyAPI
{
private:
    template <typename T>
    using Dictionary = std::unordered_map<std::string, T>;

public:
    TopologyAPI() = delete;

private:
    static Dictionary<TopologyPointer> topologies;

public:
    /**
     * @brief Read topology from json file and save it to memory
     * 
     * @param fileName topology json file name
     * @return TopologyWeakConstPointer pointer to loaded topology
     * @throw std::invalid_argument if file name was invalid
     * @throw nlohmann::json::exceptions if json was invalid
     */
    static TopologyWeakConstPointer readTopology(const std::string &fileName);

    /**
     * @brief Write topology from memory to json file
     * 
     * @param topologyId topology id to write to file
     * @param fileName topology json file name
     * @throw std::invalid_argument if file name was invalid
     */
    static void writeTopology(const std::string &topologyId, const std::string &fileName);

public:
    /**
     * @brief Get topology from memory
     * 
     * @param topologyId topology id to retrieve
     * @return TopologyWeakConstPointer pointer to retrieved topology
     * @throw std::out_of_range if topology id was not found
     */
    inline static TopologyWeakConstPointer getTopology(const std::string &topologyId)
    {
        // Try to get topology; if didn't exist, throw error
        return TopologyAPI::topologies.at(topologyId);
    }

    /**
     * @brief Remove topology from memory
     * 
     * @param topologyId topology id to remove
     * @return size_t number of topologies removed
     */
    inline static size_t removeTopology(const std::string &topologyId)
    {
        // Try to remove topology
        return TopologyAPI::topologies.erase(topologyId);
    }

public:
    /**
     * @brief Query all topologies in memory
     * 
     * @return std::vector<TopologyWeakConstPointer> list of pointers to all topologies
     */
    static std::vector<TopologyWeakConstPointer> queryTopologies();

    /**
     * @brief Query all topologies (by node id) in memory
     * 
     * @return std::vector<ComponentWeakConstPointer> list of pointers to all topologies (connected to node)
     */
    static std::vector<TopologyWeakConstPointer> queryTopologiesByNode(const std::string &nodeId);

private:
    /**
     * @brief Add or overwrite topology (by id) in memory
     * 
     * @param topologyId added topology id
     * @param topologyPtr pointer to topology
     * @return TopologyWeakConstPointer pointer to created/overwrote topology
     */
    static TopologyWeakConstPointer setTopology(const std::string &topologyId, TopologyPointer &topologyPtr);
};

#endif // TOPOLOGYAPI_HPP