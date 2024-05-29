#include "open62541pp/plugins/plugins/HistoryDataBackend.h"

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

}
