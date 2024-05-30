#ifndef HISTORYDATABASE_H
#define HISTORYDATABASE_H

#include "open62541/plugin/historydatabase.h"
#include "open62541pp/Session.h"
#include "open62541pp/plugins/PluginAdapter.h"
#include "open62541pp/plugins/plugins/HistoryDataGathering.h"
#include "open62541pp/types/Composed.h"
#include "open62541pp/types/DataValue.h"
#include "open62541pp/PluginAdapterUtil.h"

namespace opcua {

class AbstractHistoryDatabase : public PluginAdapter<UA_HistoryDatabase>
{
public:
    virtual void setValue(Session& session, const NodeId& nodeId, bool historizing, const DataValue& value) = 0;
    virtual void setEvent(const NodeId& originId, const NodeId& emitterId,
                          const EventFilter& historicalEventFilter, const EventFieldList& fieldList) = 0;
    virtual void readRaw(Session& session, const RequestHeader& requestHeader,
                         const ReadRawModifiedDetails& historyReadDetails, int32_t timestampsToReturn,
                         UA_Boolean releaseContinuationPoints, Span<const HistoryReadValueId> nodesToRead,
                         HistoryReadResponse* response, HistoryData* const historyData) = 0;

    // PluginAdapter interface
    void clear(UA_HistoryDatabase &hd) noexcept override;
    UA_HistoryDatabase create() override;
};

class HistoryDatabaseDefault : public AbstractHistoryDatabase
{
public:
    HistoryDatabaseDefault(AbstractHistoryDataGathering* gather);
    // HistoryDatabase interface
    void setValue(Session &session, const NodeId &nodeId, bool historizing, const DataValue &value) override;
    void setEvent(const NodeId &originId, const NodeId &emitterId,
                  const EventFilter &historicalEventFilter, const EventFieldList &fieldList) override;
    void readRaw(Session &session, const RequestHeader &requestHeader,
                 const ReadRawModifiedDetails &historyReadDetails, int32_t timestampsToReturn,
                 UA_Boolean releaseContinuationPoints, opcua::Span<const HistoryReadValueId> nodesToRead,
                 HistoryReadResponse *response, HistoryData * const historyData) override;

private:
    AbstractHistoryDataGathering* mGathering;
};

}

#endif // HISTORYDATABASE_H
