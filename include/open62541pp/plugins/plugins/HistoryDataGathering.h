#ifndef ABSTRACTHISTORYDATAGATHERING_H
#define ABSTRACTHISTORYDATAGATHERING_H

#include "open62541pp/plugins/PluginAdapter.h"
#include "open62541/plugin/historydata/history_data_gathering.h"
#include "open62541pp/types/NodeId.h"
#include <memory>
#include <map>

namespace opcua {

class HistorizingNodeIdSettings {
};

class AbstractHistoryDataGathering : public PluginAdapter<UA_HistoryDataGathering> {
public:
    // PluginAdapter interface
    void clear(UA_HistoryDataGathering &hdg) noexcept override;
    UA_HistoryDataGathering create() override;

    virtual StatusCode registerNodeId(const NodeId& nodeId, std::unique_ptr<HistorizingNodeIdSettings> settings);
    virtual StatusCode updateNodeIdSetting(const NodeId& nodeId, std::unique_ptr<HistorizingNodeIdSettings> settings);
    virtual const HistorizingNodeIdSettings* getHistorizingSetting(const NodeId& nodeId);

private:
    std::map<NodeId, std::unique_ptr<HistorizingNodeIdSettings>> mNodeIdToSettings;
};

}

#endif  // ABSTRACTHISTORYDATAGATHERING_H
