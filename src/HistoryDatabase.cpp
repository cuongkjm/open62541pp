#include "open62541pp/HistoryDatabase.h"
#include "open62541pp/PluginAdapterUtil.h"
#include <iostream>

namespace opcua {

static void setValueNative(
    UA_Server *server,
    void *hdbContext,
    const UA_NodeId *sessionId,
    void *sessionContext,
    const UA_NodeId *nodeId,
    UA_Boolean historizing,
    const UA_DataValue *value)
{
    auto *hdPtr = static_cast<AbstractHistoryDatabase*>(hdbContext);
    auto session = getSession(server, sessionId);
    return hdPtr->setValue(session.value(), asWrapperRef<NodeId>(nodeId), historizing, asWrapperRef<DataValue>(value));
}

static void setEventNative(
    UA_Server *server,
    void *hdbContext,
    const UA_NodeId *originId,
    const UA_NodeId *emitterId,
    const UA_EventFilter *historicalEventFilter,
    UA_EventFieldList *fieldList)
{
    auto hdPtr = static_cast<AbstractHistoryDatabase*>(hdbContext);
    hdPtr->setEvent(asWrapperRef<NodeId>(originId), asWrapperRef<NodeId>(emitterId),
                    asWrapperRef<EventFilter>(historicalEventFilter), asWrapperRef<EventFieldList>(fieldList));
}

static void readRawNative(
    UA_Server *server,
    void *hdbContext,
    const UA_NodeId *sessionId,
    [[maybe_unused]] void *sessionContext,
    const UA_RequestHeader *requestHeader,
    const UA_ReadRawModifiedDetails *historyReadDetails,
    UA_TimestampsToReturn timestampsToReturn,
    UA_Boolean releaseContinuationPoints,
    size_t nodesToReadSize,
    const UA_HistoryReadValueId *nodesToRead,
    UA_HistoryReadResponse *response,
    UA_HistoryData * const * const historyData)
{
    auto *hdPtr = static_cast<AbstractHistoryDatabase*>(hdbContext);
    auto session = getSession(server, sessionId);
    Span<const HistoryReadValueId> spanNodesToRead(asWrapper<HistoryReadValueId>(nodesToRead), nodesToReadSize);

    hdPtr->readRaw(session.value(), asWrapperRef<RequestHeader>(requestHeader),
                   asWrapperRef<ReadRawModifiedDetails>(historyReadDetails), timestampsToReturn, releaseContinuationPoints,
                   spanNodesToRead, asWrapper<HistoryReadResponse>(response), asWrapper<HistoryData>(*historyData));
}

void AbstractHistoryDatabase::clear(UA_HistoryDatabase &hd) noexcept
{
    if (hd.clear != nullptr) {
        hd.clear(&hd);
    }
    hd = UA_HistoryDatabase{};
}

UA_HistoryDatabase AbstractHistoryDatabase::create()
{
    UA_HistoryDatabase hd;
    hd.context = this;
    hd.setValue = &setValueNative;
    hd.setEvent = &setEventNative;
    hd.readRaw = &readRawNative;
    hd.readModified = nullptr;
    hd.readEvent = nullptr;
    hd.readProcessed = nullptr;
    hd.readAtTime = nullptr;
    hd.updateData = nullptr;
    hd.deleteRawModified = nullptr;
    return hd;
}

void HistoryDatabaseDefault::setValue(
    Session &session, const NodeId &nodeId, bool historizing,
    const DataValue &value)
{
    std::cout << __FUNCTION__ << " " << nodeId.toString() << " - historizing: " << historizing << std::endl;
}

void HistoryDatabaseDefault::setEvent(
    const NodeId &originId, const NodeId &emitterId,
    const EventFilter &historicalEventFilter, const EventFieldList &fieldList)
{
    std::cout << __FUNCTION__ << std::endl;
}

void HistoryDatabaseDefault::readRaw(
    Session &session, const RequestHeader &requestHeader,
    const ReadRawModifiedDetails &historyReadDetails, int32_t timestampsToReturn,
    UA_Boolean releaseContinuationPoints, opcua::Span<const HistoryReadValueId> nodesToRead,
    HistoryReadResponse *response, HistoryData * const historyData)
{
    std::cout << __FUNCTION__ << std::endl;
}

}
