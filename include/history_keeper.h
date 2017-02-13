#ifndef HISTORY_KEEPER_H_
#define HISTORY_KEEPER_H_

#include "common.h"
#include <unordered_map>

namespace skrobot {

typedef iVector* Hands;
typedef std::unordered_map<int, Hands> History;

class HistoryKeeperC {
    public:
        bool Record(int player, int *hand, int len);
        iVector* GetRecord (int player);
        HistoryKeeperC(int *players, int len);
    private:
        History history_;
};

}




#endif /* ifndef HISTORY_KEEPER_H_ */
