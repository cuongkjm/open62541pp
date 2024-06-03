#ifndef PLUGINADAPTERUTIL_H
#define PLUGINADAPTERUTIL_H

#include "open62541pp/Server.h"  // getWrapper
#include "open62541pp/Session.h"

namespace opcua {

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
