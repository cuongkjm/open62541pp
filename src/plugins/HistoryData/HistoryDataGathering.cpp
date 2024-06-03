#include "open62541pp/plugins/plugins/HistoryDataGathering.h"

namespace opcua {

void AbstractHistoryDataGathering::clear(UA_HistoryDataGathering &hdg) noexcept
{
    if (hdg.deleteMembers != nullptr) {
        hdg.deleteMembers(&hdg);
    }
    hdg = UA_HistoryDataGathering{};
}

UA_HistoryDataGathering AbstractHistoryDataGathering::create()
{
    UA_HistoryDataGathering hdg;
    hdg.context = this;
    hdg.registerNodeId = nullptr;
    hdg.stopPoll = nullptr;
    hdg.startPoll = nullptr;
    hdg.updateNodeIdSetting = nullptr;
    hdg.getHistorizingSetting = nullptr;
    hdg.setValue = nullptr;
    return hdg;
}

StatusCode AbstractHistoryDataGathering::registerNodeId(
    const NodeId &nodeId, std::unique_ptr<HistorizingNodeIdSettings> settings)
{
    if (mNodeIdToSettings.find(nodeId) != mNodeIdToSettings.end()) {
        return UA_STATUSCODE_BADNODEIDEXISTS;
    }
    mNodeIdToSettings[nodeId] = std::move(settings);
    return UA_STATUSCODE_GOOD;
}

const HistorizingNodeIdSettings *AbstractHistoryDataGathering::getHistorizingSetting(const NodeId &nodeId)
{
    if (mNodeIdToSettings.find(nodeId) == mNodeIdToSettings.end()) {
        return nullptr;
    }
    return mNodeIdToSettings[nodeId].get();
}

void HistoryDataGatheringDefault::setValue(
    [[maybe_unused]]Server *server, [[maybe_unused]]const std::optional<Session> &session,
    const NodeId &nodeId, bool historizing,
    const DataValue &value)
{
    const auto* settings = getHistorizingSetting(nodeId);
    if (settings == nullptr) {
        return;
    }
    if (settings->getHistorizingUpdateStrategy() == UA_HISTORIZINGUPDATESTRATEGY_VALUESET) {
        settings->getHistorizingBackend()->serverSetHistoryData(nodeId, historizing, value);
    }
}

}
