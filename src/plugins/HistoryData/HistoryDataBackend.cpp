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

bool HistoryDataBackendMemory::boundSupported(
    [[maybe_unused]]Server *server, [[maybe_unused]]const NodeId &session, [[maybe_unused]]const NodeId &nodeId)
{
    return true;
}

bool HistoryDataBackendMemory::timestampsToReturnSupported(
    [[maybe_unused]] Server *server, [[maybe_unused]] const NodeId &session,
    [[maybe_unused]] const NodeId &nodeId, [[maybe_unused]] TimestampsToReturn timestampsToReturn)
{
    return true;
}

void HistoryDataBackendMemory::getHistoryData(
    Server *server, const std::optional<Session> &session,
    const RequestHeader &requestHeader, const ReadRawModifiedDetails &historyReadDetails,
    int32_t timestampsToReturn,
    UA_Boolean releaseContinuationPoints,
    const HistoryReadValueId &nodeToRead, HistoryReadResponse &response, std::vector<HistoryData> &historyData)
{
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "cuongkjmlog %s - %s",
                historyReadDetails.getStartTime().format("yyyy-mm-ddThh:mm:ssZ").c_str(),
                historyReadDetails.getEndTime().format("yyyy-mm-ddThh:mm:ssZ").c_str());
}

}
