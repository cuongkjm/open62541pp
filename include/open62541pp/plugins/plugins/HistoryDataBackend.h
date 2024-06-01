#ifndef ABSTRACTHISTORYDATABACKEND_H
#define ABSTRACTHISTORYDATABACKEND_H

#include "open62541pp/PluginAdapterUtil.h"
#include "open62541pp/Server.h"
#include "open62541pp/plugins/PluginAdapter.h"
#include "open62541/plugin/historydata/history_data_backend.h"
#include "open62541pp/types/Composed.h"
#include "open62541pp/types/DataValue.h"
#include <map>

namespace opcua {

class AbstractHistoryDataBackend : public PluginAdapter<UA_HistoryDataBackend> {
public:
    // PluginAdapter interface
    void clear(UA_HistoryDataBackend &hdb) noexcept override;
    UA_HistoryDataBackend create() override;

    virtual void serverSetHistoryData(const NodeId &nodeId, bool historizing, const DataValue &dataValue) = 0;
    virtual bool boundSupported(Server *server, const NodeId& session, const NodeId& nodeId) = 0;
    virtual bool timestampsToReturnSupported(
        Server* server, const NodeId& session,
        const NodeId& nodeId, TimestampsToReturn timestampsToReturn) = 0;
    virtual void getHistoryData(
        Server *server, const std::optional<Session> &session, const RequestHeader &requestHeader,
        const ReadRawModifiedDetails &historyReadDetails, int32_t timestampsToReturn,
        UA_Boolean releaseContinuationPoints, const HistoryReadValueId& nodesToRead,
        HistoryReadResponse &response, HistoryData &historyData) = 0;
};

class HistoryDataBackendMemory : public AbstractHistoryDataBackend {
public:
    // AbstractHistoryDataBackend interface
    void serverSetHistoryData(const NodeId &nodeId, bool historizing, const DataValue &dataValue) override;
    bool boundSupported(Server *server, const NodeId &session, const NodeId &nodeId) override;
    bool timestampsToReturnSupported(
        Server *server, const NodeId &session,
        const NodeId &nodeId, TimestampsToReturn timestampsToReturn) override;
    void getHistoryData(
        Server *server, const std::optional<Session> &session, const RequestHeader &requestHeader,
        const ReadRawModifiedDetails &historyReadDetails, int32_t timestampsToReturn,
        UA_Boolean releaseContinuationPoints, const HistoryReadValueId& nodesToRead,
        HistoryReadResponse &response, HistoryData &historyData) override;

private:
    std::map<DateTime, DataValue> mHistoryData;
};

}

#endif  // ABSTRACTHISTORYDATABACKEND_H
