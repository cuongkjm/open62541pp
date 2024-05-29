#ifndef ABSTRACTHISTORYDATABACKEND_H
#define ABSTRACTHISTORYDATABACKEND_H

#include "open62541pp/plugins/PluginAdapter.h"
#include "open62541/plugin/historydata/history_data_backend.h"

namespace opcua {

class AbstractHistoryDataBackend : public PluginAdapter<UA_HistoryDataBackend> {
public:

    // PluginAdapter interface
    void clear(UA_HistoryDataBackend &hdb) noexcept override;
    UA_HistoryDataBackend create() override;
};

}

#endif  // ABSTRACTHISTORYDATABACKEND_H
