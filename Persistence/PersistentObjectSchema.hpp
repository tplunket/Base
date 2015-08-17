//
//  Copyright (c) 2015 Next Audio Labs, LLC. All rights reserved.
//
//  This file contains confidential and proprietary information of Next
//  Audio Labs, LLC ("Next"). No use is permitted without express written
//  permission of Next. If you are not a party to a Confidentiality/
//  Non-Disclosure Agreement with Next, please immediately delete this
//  file as well as all copies in your possession. For further information,
//  please email info@nextaudiolabs.com.
//

#pragma once

#include "Base/Base.hpp"
#include "Base/Object.hpp"
#include "Persistence/PersistentObject.hpp"

namespace NxA {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(PersistentObjectSchema);

    #pragma mark Type Declarations
    typedef NxA::PersistentObject::Pointer (*CreateObjectFunc)(const NxA::ObjectID&);
    
    class PersistentObjectSchema : public Object {
        NXA_GENERATED_OPERATOR_EQUAL_DECLARATION_IN_NAMESPACE_FOR_CLASS(NxA, PersistentObjectSchema);
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA, PersistentObjectSchema);

    private:
        #pragma mark Internal methods
        static void registerSchema(PersistentObjectSchema::Pointer schema);

    public:

        using Map = Map<uinteger32, PersistentObjectSchema>;

        #pragma mark Factory Methods
        static PersistentObjectSchema::Pointer schema(uinteger32 classHash,
                                                      const character* className,
                                                      PersistentObject::Pointer obj,
                                                      CreateObjectFunc func);

        template<typename T> static void createSchema(void)
        {
            ObjectID tempID(T::hashOfClassName(), -1);
            auto newSchema = schema(T::hashOfClassName(),
                                    T::nameOfClass(),
                                    T::persistentObject(tempID),
                                    T::persistentObject);

            registerSchema(newSchema);
        }

        #pragma mark Static Methods
        static PersistentObjectSchema& getSchemaForType(uinteger32 type);
        static Map::Pointer getSchemaMap();

        #pragma mark Instance Methods
        uinteger32 identifier(void) const;
        String::Pointer tableName(void) const;
        String::Pointer createQuery(void) const;
        String::Pointer selectQuery(void) const;
        String::Pointer insertQuery(void) const;
        String::Pointer updateQuery(void) const;
        String::Pointer deleteQuery(void) const;
        uinteger32 fieldCount(void) const;
        PersistentObject::Pointer createObject(const ObjectID& id) const;
    };
}
