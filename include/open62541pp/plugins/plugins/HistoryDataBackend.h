#ifndef ABSTRACTHISTORYDATABACKEND_H
#define ABSTRACTHISTORYDATABACKEND_H

#include "open62541pp/plugins/PluginAdapter.h"
#include "open62541/plugin/historydata/history_data_backend.h"
#include "open62541pp/types/DataValue.h"
#include <map>

namespace opcua {

class AbstractHistoryDataBackend : public PluginAdapter<UA_HistoryDataBackend> {
public:
    // PluginAdapter interface
    void clear(UA_HistoryDataBackend &hdb) noexcept override;
    UA_HistoryDataBackend create() override;

    virtual void serverSetHistoryData(const NodeId &nodeId, bool historizing, const DataValue &dataValue) = 0;
};

class HistoryDataBackendMemory : public AbstractHistoryDataBackend {
public:
    // AbstractHistoryDataBackend interface
    void serverSetHistoryData(const NodeId &nodeId, bool historizing, const DataValue &dataValue) override;

private:
    std::map<DateTime, DataValue> mHistoryData;
};

}

#endif  // ABSTRACTHISTORYDATABACKEND_H
