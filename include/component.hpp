#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <string>
#include <unordered_map>

#include "nlohmann/json.hpp"

using ordered_json = nlohmann::ordered_json;

/**
 * @brief Wrapper around component attribute data
 */
struct Attribute
{
    std::string id; /** @brief Attribute id (string identifier) */
    double def;     /** @brief Default value of attribute */
    double min;     /** @brief Minimum value of attribute */
    double max;     /** @brief Maximum value of attribute */
};

/**
 * @brief Wrapper around component connection data
 */
struct Connection
{
    std::string id;     /** @brief Connection id (string identifier) */
    std::string nodeId; /** @brief Node id (linked to connection) */
};

class Component;
typedef std::shared_ptr<Component> ComponentPointer;
typedef std::shared_ptr<const Component> ComponentConstPointer;
typedef std::weak_ptr<const Component> ComponentWeakConstPointer;

/**
 * @brief Wrapper around component data
 */
class Component
{
private:
    template <typename T>
    using Dictionary = std::unordered_map<std::string, T>;

private:
    std::string m_type;                 /** @brief Component type (eg. resistor, capacitor) */
    std::string m_id;                   /** @brief Component id (string identifier) */
    Dictionary<Attribute> m_attributes; /** @brief Component attributes dictionary */
    Dictionary<Connection> m_netlist;   /** @brief Component netlist (connections) dictionary */

public:
    Component() = default;
    Component(const Component &) = default;

public:
    /**
     * @brief Loads component from a json string and returns it
     * 
     * @param componentJson json string of component
     * @return ComponentPointer pointer to loaded component
     * @throw nlohmann::json::exceptions if component json was invalid
     */
    static ComponentPointer loadComponentFromJson(const nlohmann::json &componentJson);

    /**
     * @brief Stores component to a json string
     * 
     * @param componentPtr pointer to component to store
     * @param componentJson json string to store component
     */
    static void storeComponentToJson(ComponentConstPointer componentPtr, nlohmann::ordered_json &componentJson);

public:
    /**
     * @brief Get component id (string identifier)
     * 
     * @return const std::string& component id
     */
    inline const std::string &getId() const { return m_id; }

    /**
     * @brief Get component type (eg. resistor, capacitor)
     * 
     * @return const std::string& component type
     */
    inline const std::string &getType() const { return m_type; }

    /**
     * @brief Get attribute (by id) from component attributes
     * 
     * @param attributeId requested attribute id
     * @return const Attribute& requested attribute information
     * @throw std::out_of_range if attribute id was not found
     */
    inline const Attribute &getAttribute(const std::string &attributeId) const
    {
        // Try to get attribute; if didn't exist, throw error
        return m_attributes.at(attributeId);
    }

    /**
     * @brief Get connection (by id) from component netlist
     * 
     * @param connectionId requested connection id
     * @return const Connection& requested connection information
     * @throw std::out_of_range if connection id was not found
     */
    inline const Connection &getConnection(const std::string &connectionId) const
    {
        // Try to get connection; if didn't exist, throw error
        return m_netlist.at(connectionId);
    }

public:
    /**
     * @brief Checks if component is connected to node (by id)
     * 
     * @param nodeId node id to check connection for
     * @return true if component is connected to node
     * @return false if component isn't connected to node
     */
    bool isConnectedToNode(const std::string &nodeId) const;

private:
    /**
     * @brief Add or overwrite attribute (by id) in component attributes
     * 
     * @param attributeId added attribute id
     * @param attribute attribute information
     */
    void setAttribute(const std::string &attributeId, Attribute &attribute);

    /**
     * @brief Add or overwrite connection (by id) in component netlist
     * 
     * @param connectionId added connection id
     * @param connection connection information
     */
    void setConnection(const std::string &connectionId, Connection &connection);
};

#endif // COMPONENT_HPP