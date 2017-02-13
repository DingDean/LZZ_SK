#include "history_keeper.h"

namespace skrobot {

HistoryKeeperC::HistoryKeeperC(int *players, int len) 
{
    for (int i=0;i<len;i++)
    {
        iVector history;
        history_.insert({{players[i], &history}});
    }
}

bool HistoryKeeperC::Record (int player, int *hand, int len)
{
    history_[player]->push_back(len);
    for (int i=0;i<len;i++)
        history_[player]->push_back(hand[i]);
}

iVector* HistoryKeeperC::GetRecord (int player)
{
    History::const_iterator record = history_.find(player);
    return record->second;
}

}

