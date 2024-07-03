#pragma once
#include "ResourceEntry.h"
#include "GPIOResource.h"

class ResourceMap {
    public: 
        ResourceMap() = default;
        ~ResourceMap() = default;   
    
    protected:
        RESOURCE_MAP(
            GPIOResource, ""
        );

        // Above macro expands to the following:
        // static constexpr const ResourceEntry ResourceInfo[]{
        //     {
        //         "GPIOResource", 
        //         GPIOResource::Init, 
        //         GPIOResource::Handler, 
        //         GPIOResource::Task, 
        //         GPIOResource::CmdMap, 
        //         (sizeof(GPIOResource::CmdMap) / sizeof(GPIOResource::CmdMap[0])), 
        //         ""
        //     },
        // };
};
