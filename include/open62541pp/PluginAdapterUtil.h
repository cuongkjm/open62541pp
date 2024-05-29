#ifndef PLUGINADAPTERUTIL_H
#define PLUGINADAPTERUTIL_H

#include "open62541pp/Server.h"  // getWrapper
#include "open62541pp/Session.h"
#include "open62541pp/types/Variant.h"
#include "open62541pp/types/Composed.h"

namespace opcua {

class EventFieldList : public TypeWrapper<UA_EventFieldList, UA_TYPES_EVENTFIELDLIST>
{
public:
    using TypeWrapper::TypeWrapper;
    EventFieldList(UA_UInt32 clientHandle, Span<const Variant> eventFields) {
        handle()->clientHandle = clientHandle;
        handle()->eventFieldsSize = eventFields.size();
        handle()->eventFields = detail::toNativeArray(eventFields);
    }

    UAPP_GETTER(UA_UInt32, getClientHandle, clientHandle)
    UAPP_GETTER_SPAN_WRAPPER(Variant, getEventFields, eventFields, eventFieldsSize)
};

class ReadRawModifiedDetails : public TypeWrapper<UA_ReadRawModifiedDetails, UA_TYPES_READRAWMODIFIEDDETAILS>
{
public:
    using TypeWrapper::TypeWrapper;

    UAPP_GETTER(UA_Boolean, getIsReadModified, isReadModified)
    UAPP_GETTER_WRAPPER(DateTime, getStartTime, startTime)
    UAPP_GETTER_WRAPPER(DateTime, getEndTime, endTime)
    UAPP_GETTER(UA_UInt32, getNumValuesPerNode, numValuesPerNode)
    UAPP_GETTER(UA_Boolean, getReturnBounds, returnBounds)
};

class HistoryReadValueId : public TypeWrapper<UA_HistoryReadValueId, UA_TYPES_HISTORYREADVALUEID>
{
public:
    using TypeWrapper::TypeWrapper;
    HistoryReadValueId(NodeId nodeId, std::string_view indexRange, 
                       QualifiedName dataEncoding, ByteString continuationPoint) {
        handle()->nodeId = detail::toNative(std::move(nodeId));
        handle()->indexRange = detail::toNative(indexRange);
        handle()->dataEncoding = detail::toNative(std::move(dataEncoding));
        handle()->continuationPoint = detail::toNative(std::move(continuationPoint));
    }

    UAPP_GETTER_WRAPPER(NodeId, getNodeId, nodeId)
    UAPP_GETTER_WRAPPER(String, getIndexRange, indexRange)
    UAPP_GETTER_WRAPPER(QualifiedName, getDataEncoding, dataEncoding)
    UAPP_GETTER_WRAPPER(ByteString, getContinuationPoint, continuationPoint)
};

class HistoryReadResult : public TypeWrapper<UA_HistoryReadResult, UA_TYPES_HISTORYREADRESULT>
{
    using TypeWrapper::TypeWrapper;

    UAPP_GETTER_WRAPPER(StatusCode, getStatusCode, statusCode)
    UAPP_GETTER_WRAPPER(ByteString, getContinuationPoint, continuationPoint)
    UAPP_GETTER_WRAPPER(ExtensionObject, getHistoryData, historyData)
};

class HistoryReadResponse : public TypeWrapper<UA_HistoryReadResponse, UA_TYPES_HISTORYREADRESPONSE>
{
public:
    using TypeWrapper::TypeWrapper;

    UAPP_GETTER_WRAPPER(ResponseHeader, getResponseHeader, responseHeader)
    UAPP_GETTER_SPAN_WRAPPER(HistoryReadResult, getResults, results, resultsSize)
    UAPP_GETTER_SPAN_WRAPPER(DiagnosticInfo, getDiagnosticInfos, diagnosticInfos, diagnosticInfosSize)
};

class HistoryData : public TypeWrapper<UA_HistoryData, UA_TYPES_HISTORYDATA>
{
public:
    using TypeWrapper::TypeWrapper;

    HistoryData(Span<const DataValue> dataValues) {
        handle()->dataValuesSize = dataValues.size();
        handle()->dataValues = detail::toNativeArray(dataValues);
    }

    UAPP_GETTER_SPAN_WRAPPER(DataValue, getDataValues, dataValues, dataValuesSize)
};

template <typename WrapperType, typename NativeType = typename WrapperType::NativeType>
inline static const WrapperType& asWrapperRef(const NativeType* nativePtr) {
    static const WrapperType empty;
    return nativePtr == nullptr ? empty : asWrapper<WrapperType>(*nativePtr);
}

inline static std::optional<Session> getSession(
    UA_Server* server, const UA_NodeId* sessionId
    ) noexcept {
    auto* wrapper = detail::getWrapper(server);
    if (wrapper == nullptr) {
        return std::nullopt;
    }
    return Session(*wrapper, asWrapperRef<NodeId>(sessionId));
}

static void logException(
    UA_Server* server, std::string_view callbackName, std::string_view exceptionMessage
    ) {
    const auto message =
        std::string("Exception in access control callback ")
            .append(callbackName)
            .append(": ")
            .append(exceptionMessage);
    log(server, LogLevel::Warning, LogCategory::Server, message);
}

template <typename F, typename ReturnType = std::invoke_result_t<F>>
inline static auto invokeAccessCallback(
    UA_Server* server, std::string_view callbackName, ReturnType returnOnException, F&& fn
    ) noexcept {
    try {
        return std::invoke(std::forward<F>(fn));
    } catch (const std::exception& e) {
        logException(server, callbackName, e.what());
        return returnOnException;
    }
}

}

#endif // PLUGINADAPTERUTIL_H
