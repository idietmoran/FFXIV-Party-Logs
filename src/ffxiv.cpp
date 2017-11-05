#include "ffxiv.h"
#include "player/you.h"
#include <iostream>
#include <Windows.h>

// constructor
FFXIV::FFXIV()
{
    ffxiv_ = new Process("FINAL FANTASY XIV");

    exe_ = new Module("ffxiv.exe");
    exe_->getModule(ffxiv_);

    numberOfPartyMembers_ = new Offset({ 0x11404DC, 0x1C, 0x10, 0x104, 0x1C });

    getAllies();
    updateNames();
}

// deconstructor
FFXIV::~FFXIV()
{
    deleteAllies();
}

// gets ally list at offsets
void FFXIV::getAllies()
{
    allies_.reserve(8);
    allies_.push_back(new YOU(0x115E431));
    allies_.push_back(new Ally(0x117AA30));
    allies_.push_back(new Ally(0x117AC50));
    allies_.push_back(new Ally(0x117AE70));
    allies_.push_back(new Ally(0x117B090));
    allies_.push_back(new Ally(0x117B2B0));
    allies_.push_back(new Ally(0x117B4D0));
    allies_.push_back(new Ally(0x117B6F0));
    allies_.push_back(new Ally(0x117B910));
}

// delete all allies to prevent memory leak
void FFXIV::deleteAllies()
{
    for(auto &p : allies_)
    {
        delete p;
        p = nullptr;
    }
}


// updates the objects to the latest names
void FFXIV::updateNames()
{
    for(auto &p : allies_)
    {
        ReadProcessMemory(ffxiv_->getHandle(), (void*)(exe_->getAddress() + p->address_), &p->name_, 80, 0);
    }
    filterAllies();
}

// checks if name is already in filteredAllies_
bool FFXIV::checkArrayForDoubles(std::string name)
{
    for(auto &p : filteredAllies_)
    {
        if(p->name_ == name)
        {
            return true;
        }
    }
    return false;
}

/*
 * The way FFXIV handles party members, the container
 * of party members is never cleared, so therefore we need to
 * check the list for double party members so we don't have 
 * multiples of the same name appearing.
 */
void FFXIV::filterAllies()
{
    filteredAllies_.clear();
    filteredAllies_.reserve(8);
    filteredAllies_.push_back(allies_[0]);
    for(auto &p : allies_)
    {
        if(!checkArrayForDoubles(p->name_))
        {
            filteredAllies_.push_back(p);
        }
    }
}