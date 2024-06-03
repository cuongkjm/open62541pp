#include "open62541pp/plugins/plugins/HistoryDataBackend.h"
#include "open62541/plugin/log_stdout.h"
#include <iostream>

namespace opcua {

void AbstractHistoryDataBackend::clear(UA_HistoryDataBackend &hdb) noexcept
{
    if (hdb.deleteMembers != nullptr) {
        hdb.deleteMembers(&hdb);
    }
    hdb = UA_HistoryDataBackend{};
}

UA_HistoryDataBackend AbstractHistoryDataBackend::create()
{
    UA_HistoryDataBackend hdb;
    hdb.context = this;

    return hdb;
}

void HistoryDataBackendMemory::serverSetHistoryData(
    [[maybe_unused]]const NodeId &nodeId, bool historizing, const DataValue &dataValue)
{
    if (!historizing) {
        return;
    }
    DateTime timestamp = 0;
    if (dataValue.hasSourceTimestamp()) {
        timestamp = dataValue.getSourceTimestamp();
    }
    else if (dataValue.hasServerTimestamp()) {
        timestamp = dataValue.getServerTimestamp();
    }
    else {
        timestamp = DateTime::now();
    }
    mHistoryData[timestamp] = dataValue;
}

void HistoryDataBackendMemory::getHistoryData(
    [[maybe_unused]] Server *server, [[maybe_unused]] const std::optional<Session> &session,
    [[maybe_unused]] const RequestHeader &requestHeader, const ReadRawModifiedDetails &historyReadDetails,
    [[maybe_unused]] int32_t timestampsToReturn,
    [[maybe_unused]] UA_Boolean releaseContinuationPoints,
    [[maybe_unused]] const HistoryReadValueId &nodeToRead, [[maybe_unused]] HistoryReadResponse &response,
    HistoryData &historyData)
{
    std::vector<DataValue> vDataValue;
    for (const auto &kv : mHistoryData) {
        if (kv.first >= historyReadDetails.getStartTime() &&
            kv.first <= historyReadDetails.getEndTime()
            && vDataValue.size() < historyReadDetails.getNumValuesPerNode()) {
            vDataValue.push_back(kv.second);
        }
    }
    historyData = HistoryData(vDataValue);
}

}
