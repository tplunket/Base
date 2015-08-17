//
//  Copyright (c) 2015 Next Audio Labs, LLC. All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//  this software and associated documentation files (the "Software"), to deal in the
//  Software without restriction, including without limitation the rights to use, copy,
//  modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
//  and to permit persons to whom the Software is furnished to do so, subject to the
//  following conditions:
//
//  The above copyright notice and this permission notice shall be included in all
//  copies or substantial portions of the Software.
//

#include "Persistence/PersistentObject.hpp"
#include "Persistence/PersistentObjectSchema.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;

NXA_CONTAINS_TEST_SUITE_NAMED(Persistence_PersistentObject_Tests);

TEST(Persistence_PersistentObject, ClassName_PersistentObject_ObjectIDComparison)
{
    // -- Given.
    auto objID = ObjectID(5, 10);
    auto test = PersistentObject::persistentObject(objID);

    // -- When.
    auto testObjID = test->objectID();

    // -- Then.
    ASSERT_EQ(objID, testObjID);
    ASSERT_EQ(objID.type, testObjID.type);
    ASSERT_EQ(objID.index, testObjID.index);
}

TEST(Persistence_PersistentObject, ClassName_PersistentObject_SaveToContext)
{
    auto store = PersistentStore::persistentStore("/tmp/test.db");
    auto context = PersistentContext::persistentContext(store);

    PersistentObjectSchema::createSchema<PersistentObject>();

    auto obj = context->createObject<PersistentObject>();
    auto obj2 = context->createObject<PersistentObject>();

    ASSERT_TRUE(obj2->objectID().type == obj->objectID().type);
    ASSERT_TRUE(obj2->objectID().index > obj->objectID().index);
    ASSERT_TRUE(obj2->objectID() > obj->objectID());
}

