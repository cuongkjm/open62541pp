#ifndef HISTORIZINGNODEIDSETTINGS_H
#define HISTORIZINGNODEIDSETTINGS_H

#include <memory>
#include "open62541pp/plugins/PluginAdapter.h"
#include "open62541/plugin/historydata/history_data_gathering.h"
#include "open62541pp/plugins/plugins/HistoryDataBackend.h"

namespace opcua {

class HistorizingNodeIdSettings : public PluginAdapter<UA_HistorizingNodeIdSettings> {
public:
    // PluginAdapter interface
    void clear(UA_HistorizingNodeIdSettings &settings) noexcept override;
    UA_HistorizingNodeIdSettings create() override;

    HistorizingNodeIdSettings(std::unique_ptr<AbstractHistoryDataBackend> historyDataBackend);

    void setHistorizingBackend(std::unique_ptr<AbstractHistoryDataBackend> historyDataBackend);
    AbstractHistoryDataBackend* getHistorizingBackend() const;

    void setMaxHistoryDataResponseSize(size_t maxHistoryDataResponseSize);
    size_t getMaxHistoryDataResponseSize() const;

    using HistorizingUpdateStrategy = UA_HistorizingUpdateStrategy;
    void setHistorizingUpdateStrategy(HistorizingUpdateStrategy historizingUpdateStrategy);
    HistorizingUpdateStrategy getHistorizingUpdateStrategy() const;

    void setPollingInterval(size_t pollingInterval);
    size_t getPollingInterval() const;

private:
    std::unique_ptr<AbstractHistoryDataBackend> mHistoryDataBackend;
    UA_HistorizingNodeIdSettings mNativeSettings;
};

}

#endif  // HISTORIZINGNODEIDSETTINGS_H
