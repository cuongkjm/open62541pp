#include "open62541pp/plugins/plugins/HistorizingNodeIdSettings.h"

namespace opcua {

void HistorizingNodeIdSettings::clear([[maybe_unused]]UA_HistorizingNodeIdSettings &settings) noexcept
{
}

UA_HistorizingNodeIdSettings HistorizingNodeIdSettings::create()
{
    mNativeSettings.historizingBackend = mHistoryDataBackend->create();
    return mNativeSettings;
}

HistorizingNodeIdSettings::HistorizingNodeIdSettings(std::unique_ptr<AbstractHistoryDataBackend> historyDataBackend)
    : mHistoryDataBackend(std::move(historyDataBackend))
{
    mNativeSettings.userContext = this;
}

void HistorizingNodeIdSettings::setHistorizingBackend(std::unique_ptr<AbstractHistoryDataBackend> historyDataBackend)
{
    mHistoryDataBackend = std::move(historyDataBackend);
}

AbstractHistoryDataBackend *HistorizingNodeIdSettings::getHistorizingBackend() const
{
    return mHistoryDataBackend.get();
}

void HistorizingNodeIdSettings::setMaxHistoryDataResponseSize(size_t maxHistoryDataResponseSize)
{
    mNativeSettings.maxHistoryDataResponseSize = maxHistoryDataResponseSize;
}

size_t HistorizingNodeIdSettings::getMaxHistoryDataResponseSize() const
{
    return mNativeSettings.maxHistoryDataResponseSize;
}

void HistorizingNodeIdSettings::setHistorizingUpdateStrategy(HistorizingUpdateStrategy historizingUpdateStrategy)
{
    mNativeSettings.historizingUpdateStrategy = historizingUpdateStrategy;
}

HistorizingNodeIdSettings::HistorizingUpdateStrategy HistorizingNodeIdSettings::getHistorizingUpdateStrategy() const
{
    return mNativeSettings.historizingUpdateStrategy;
}

void HistorizingNodeIdSettings::setPollingInterval(size_t pollingInterval)
{
    mNativeSettings.pollingInterval = pollingInterval;
}

size_t HistorizingNodeIdSettings::getPollingInterval() const
{
    return mNativeSettings.pollingInterval;
}

}
