#include "component.hpp"
#include <utility>

using json = nlohmann::json;

ComponentPointer Component::loadComponentFromJson(const json &componentJson)
{
    // Create component and assign type and id
    ComponentPointer componentPtr = std::make_shared<Component>();
    componentPtr->m_type = componentJson.at("type").get<std::string>();
    componentPtr->m_id = componentJson.at("id").get<std::string>();

    // Extract netlist and add connections to component
    auto &netlist = componentJson.at("netlist");
    for (auto &connectionJson : netlist.items())
    {
        Connection connection;
        connection.id = connectionJson.key();
        connection.nodeId = connectionJson.value().get<std::string>();
        componentPtr->setConnection(connection.id, connection);
    }

    // Remove other json values command
    json removeExtras = R"([
            { "op": "remove", "path": "/type"},
            { "op": "remove", "path": "/id"},
            { "op": "remove", "path": "/netlist"}
        ])"_json;

    // Extract attributes and add them to component
    auto attributes = componentJson.patch(removeExtras);
    for (auto &attributeJson : attributes.items())
    {
        Attribute attribute;
        attribute.id = attributeJson.key();
        attribute.def = attributeJson.value().at("default").get<double>();
        attribute.min = attributeJson.value().at("min").get<double>();
        attribute.max = attributeJson.value().at("max").get<double>();
        componentPtr->setAttribute(attribute.id, attribute);
    }

    // Return pointer to extracted component
    return componentPtr;
}

void Component::storeComponentToJson(ComponentConstPointer componentPtr, ordered_json &componentJson)
{
    // Store component id and type
    componentJson["id"] = componentPtr->m_id;
    componentJson["type"] = componentPtr->m_type;

    // Store component attributes
    for (auto &attributeEntry : componentPtr->m_attributes)
    {
        auto &attribute = attributeEntry.second;
        componentJson[attribute.id]["default"] = attribute.def;
        componentJson[attribute.id]["min"] = attribute.min;
        componentJson[attribute.id]["max"] = attribute.max;
    }

    // Store component connections (netlist)
    for (auto &connectionEntry : componentPtr->m_netlist)
    {
        auto &connection = connectionEntry.second;
        componentJson["netlist"][connection.id] = connection.nodeId;
    }
}

bool Component::isConnectedToNode(const std::string &nodeId) const
{
    // Iterate over all connections to find node id
    for (auto &connectionEntry : m_netlist)
    {
        auto &connection = connectionEntry.second;
        if (connection.nodeId == nodeId)
            return true;
    }
    return false;
}

void Component::setAttribute(const std::string &attributeId, Attribute &attribute)
{
    // Try to insert attribute; if already existed, overwrite it
    auto res = m_attributes.insert(std::make_pair(attributeId, std::move(attribute)));
    auto &compAttribute = res.first->second;
    // Overwrite component attribute
    if (!res.second)
    {
        compAttribute.id = std::move(attribute.id);
        compAttribute.def = attribute.def;
        compAttribute.min = attribute.min;
        compAttribute.max = attribute.max;
    }
}

void Component::setConnection(const std::string &connectionId, Connection &connection)
{
    // Try to insert connection; if already existed, overwrite it
    auto res = m_netlist.insert(std::make_pair(connectionId, std::move(connection)));
    auto &compConnection = res.first->second;
    // Overwrite component connection
    if (!res.second)
    {
        compConnection.id = std::move(connection.id);
        compConnection.nodeId = std::move(connection.nodeId);
    }
}
