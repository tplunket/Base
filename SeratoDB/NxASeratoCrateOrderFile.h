//
//  NxASeratoCrateOrderFile.h
//  SeratoDB
//
//  Created by Didier Malenfant on 2/4/15.
//  Copyright (c) 2015 Next Audio Labs, LLC. All rights reserved.
//
//  This file contains confidential and proprietary information of Serato
//  Inc. LLP ("Serato"). No use is permitted without express written
//  permission of Serato. If you are not a party to a Confidentiality/
//  Non-Disclosure Agreement with Serato, please immediately delete this
//  file as well as all copies in your possession. For further information,
//  please refer to the modified MIT license provided with this library,
//  or email licensing@serato.com.
//

#ifndef __SeratoDB__NxASeratoCrateOrderFile__
#define __SeratoDB__NxASeratoCrateOrderFile__

#include <SeratoDB/NxASeratoDBUtility.h>
#include <SeratoDB/NxASeratoCrate.h>

namespace NxA {
    #pragma mark Forward declarations
    class SeratoCrateOrderFile;

    #pragma mark Containers
    typedef std::auto_ptr<const SeratoCrateOrderFile> SeratoCrateOrderFileAutoPtr;

    #pragma mark Class Declaration
    class SeratoCrateOrderFile
    {
    private:
        #pragma mark Private Instance Variables
        StringAutoPtr p_crateOrderFilePath;

        SeratoCrateAutoPtr p_rootCrate;

        StringVectorAutoPtr p_unknownCrates;

        #pragma mark Private Instance Methods
        SeratoCrateVectorAutoPtr p_childrenCratesOfCrateNamedUsingNameList(const std::string& name,
                                                                           StringVector::iterator& it,
                                                                           const StringVector::iterator& end,
                                                                           const char* seratoFolderPath,
                                                                           const char* rootFolderPath);

    public:
        #pragma mark Constructors
        SeratoCrateOrderFile(const char* seratoFolderPath, const char* rootFolderPath);

        #pragma mark Instance Methods
        const SeratoCrate* rootCrate(void) const;

        void saveIfModified(void) const;
    };
}

#endif /* defined(__SeratoDB__NxASeratoCrateOrderFile__) */
