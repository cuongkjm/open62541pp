#include "open62541pp/plugins/plugins/HistoryDataBackend.h"
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

}
