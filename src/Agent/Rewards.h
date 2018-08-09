#ifndef RewardsForActionsAfterObservingAState_h
#define RewardsForActionsAfterObservingAState_h
struct RewardsForActionsAfterObservingAState
{
    int ff; //forward forward
    int fb;
    int bf;
    int bb; //backward backward
};
#endif

//Including a hashmap type is too expensive, so this will have to do
//Not sure if a linear search will be performance issue

#ifndef MemoryOfRewardsForStateActionPairs_h
#define MemoryOfRewardsForStateActionPairs_h
struct MemoryOfRewardsForStateActionPairs
{
    String key[8];
    RewardsForActionsAfterObservingAState value[8];
};
#endif
