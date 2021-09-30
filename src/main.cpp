#include <iostream>
#include <iomanip>

#include "component.hpp"
#include "topology.hpp"
#include "topologyapi.hpp"

int main(int argc, char const *argv[])
{
    std::cout << "############### Welcome ###############\n";
    while (true)
    {
        std::cout << "---------------------------------------\n\n";
        std::cout << "1. Read    2. Write     3. Query\n";
        std::cout << "4. Get     5. Remove    6. Exit\n";
        std::cout << "Which operation to execute? ";

        unsigned int op;
        std::cin >> op;
        std::cout << "\n";

        switch (op)
        {
        case 1:
        {
            std::string topologyFileName;
            std::cout << "Enter topology json file name: ";
            std::cin >> topologyFileName;
            auto topologyWeakPtr = TopologyAPI::readTopology(topologyFileName);
            if (auto topologyPtr = topologyWeakPtr.lock())
                std::cout << "Read Topology; Topology Id: " << topologyPtr->getId() << "\n\n";
            break;
        }
        case 2:
        {
            std::string topologyId;
            std::cout << "Enter topology id: ";
            std::cin >> topologyId;
            std::string topologyFileName;
            std::cout << "Enter topology json file name: ";
            std::cin >> topologyFileName;
            TopologyAPI::writeTopology(topologyId, topologyFileName);
            std::cout << "Wrote Topology\n\n";
            break;
        }
        case 3:
        {
            auto topologiesPtrList = TopologyAPI::queryTopologies();
            if (topologiesPtrList.size() != 0)
            {
                std::cout << "Topologies:\n";
                std::cout << "===========\n";
                for (auto &topologyWeakPtr : topologiesPtrList)
                {
                    if (auto topologyPtr = topologyWeakPtr.lock())
                        std::cout << topologyPtr->getId() << "\n";
                }
                std::cout << "\n";
            }
            else
                std::cout << "No Topologies\n\n";

            std::string nodeId;
            std::cout << "Enter node id (queryTopologiesByNode): ";
            std::cin >> nodeId;
            topologiesPtrList = TopologyAPI::queryTopologiesByNode(nodeId);
            if (topologiesPtrList.size() != 0)
            {
                std::cout << "Topologies:\n";
                std::cout << "===========\n";
                for (auto &topologyWeakPtr : topologiesPtrList)
                {
                    if (auto topologyPtr = topologyWeakPtr.lock())
                        std::cout << topologyPtr->getId() << "\n";
                }
                std::cout << "\n";
            }
            else
                std::cout << "No Topologies\n\n";
            break;
        }
        case 4:
        {
            std::string topologyId;
            std::cout << "Enter topology id: ";
            std::cin >> topologyId;
            auto topologyWeakPtr = TopologyAPI::getTopology(topologyId);
            if (auto topologyPtr = topologyWeakPtr.lock())
            {
                std::cout << "Topology Id: " << topologyPtr->getId() << "\n";
                std::cout << "Topology Components:\n";
                std::cout << "====================\n";
                auto componentsPtrList = topologyPtr->queryComponents();
                for (auto &componentWeakPtr : componentsPtrList)
                {
                    if (auto componentPtr = componentWeakPtr.lock())
                    {
                        std::cout << std::left << "Component Id: " << std::setw(10) << componentPtr->getId()
                                  << "Type: " << std::setw(10) << componentPtr->getType() << "\n";
                    }
                }
            }
            std::cout << "\n";
            break;
        }
        case 5:
        {
            std::string topologyId;
            std::cout << "Enter topology id: ";
            std::cin >> topologyId;
            TopologyAPI::removeTopology(topologyId);
            std::cout << "Removed Topology\n\n";
            break;
        }
        default:
            return 0;
        }
    }
    return 0;
}
