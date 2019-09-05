#ifndef RewardsForActions_h
#define RewardsForActions_h
struct RewardsForActions
{
    int ff; //forward forward
    int fb;
    int bf;
    int bb; //backward backward
};
#endif

//Including a hashmap type is too expensive, so this will have to do
//Not sure if a linear search will be performance issue

#ifndef MemoryOfRewardsForActions_h
#define MemoryOfRewardsForActions_h
struct MemoryOfRewardsForActions
{
    String states[8]; //key
    RewardsForActions rewardsForActions[8]; //value
};
#endif
