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

#include "Persistence/PersistentContext.hpp"
#include "Persistence/Internal/PersistentObject.hpp"
#include "Base/Test.hpp"

using namespace testing;
using namespace NxA;

#pragma mark Test Helper Class

namespace NxA {
    NXA_GENERATED_FORWARD_DECLARATIONS_FOR_CLASS(TestObj);

    class TestObj final : public PersistentObject {
        NXA_GENERATED_OPERATOR_EQUAL_DECLARATION_IN_NAMESPACE_FOR_CLASS(NxA, TestObj);
        NXA_GENERATED_DECLARATIONS_IN_NAMESPACE_FOR_CLASS(NxA, TestObj);
    public:
        NxA::Pointer<NxA::String> description(void) const {
            return String::string();
        }

        NxA::String::Pointer name(void) const;
        integer32 value(void) const;

        void setName(NxA::String::Pointer name);
        void setValue(integer32 value);

        static PersistentObject::Pointer persistentObject(const ObjectID& id)
        {
            auto obj = makeShared();
            obj->setObjectID(id);
            return NxA::PersistentObject::Pointer::dynamicCastFrom(obj);
        }
    };

    namespace Internal {
        struct TestObj : public PersistentObject {
            NXA_GENERATED_INTERNAL_DECLARATIONS_FOR(NxA, TestObj);

            NxA::String::Pointer name;
            integer32 value;

            virtual void serialize(NxA::Serializer& s);
        };

        TestObj::TestObj() : name(NxA::String::string())
        {
        }
        void TestObj::serialize(NxA::Serializer& s)
        {
            s.serialize(NXA_SERIALIZED_MEMBER(name));
            s.serialize(NXA_SERIALIZED_MEMBER(value));
        }
    }

    NxA::String::Pointer TestObj::name() const {
        return internal->name;
    }
    integer32 TestObj::value() const {
        return internal->value;
    }

    void TestObj::setName(NxA::String::Pointer name) {
        internal->name = name;
    }
    void TestObj::setValue(integer32 value) {
        internal->value = value;
    }
}

NXA_GENERATED_IMPLEMENTATION_IN_NAMESPACE_FOR_CLASS_WITH_PARENT(NxA, TestObj, PersistentObject);

#pragma mark Unit Tests

NXA_CONTAINS_TEST_SUITE_NAMED(Persistence_PersistentContext_Tests);

TEST(Persistence_PersistentContext, ClassName_PersistentContext_Creation)
{
    // -- Given.
    auto store = PersistentStore::persistentStore("/tmp/test.db");
    auto context = PersistentContext::persistentContext(store);

    // -- When.
    // -- Then.
    ASSERT_EQ(store, context->store());
}

TEST(Persistence_PersistentContext, PersistentContext_TestObj_ThrowsRecordNotFoundError)
{
    PersistentObjectSchema::createSchema<TestObj>();
    
    // -- Given.
    auto store = PersistentStore::persistentStore("/tmp/test.db");
    auto context = PersistentContext::persistentContext(store);
    
    // -- When.
    ObjectID id(TestObj::hashOfClassName(), 99);

    // -- Then.
    EXPECT_THROW(context->loadObjectWithID(id), RecordNotFoundError);
}

TEST(Persistence_PersistentContext, PersistentContext_TestObj_SaveAndLoad)
{
    PersistentObjectSchema::createSchema<TestObj>();

    // -- Given.
    ObjectID id;
    NxA::String::Pointer name = String::stringWith("MyObject");
    integer32 value = 42;
    {
        auto store = PersistentStore::persistentStore("/tmp/test.db");
        auto context = PersistentContext::persistentContext(store);

        auto obj = context->createObject<TestObj>();
        obj->setName(name);
        obj->setValue(value);
        id = obj->objectID();

        context->saveContext();
    }

    // -- When.
    auto store = PersistentStore::persistentStore("/tmp/test.db");
    auto context = PersistentContext::persistentContext(store);

    auto obj = TestObj::Pointer::dynamicCastFrom(context->loadObjectWithID(id));

    // -- Then.
    ASSERT_EQ(obj->objectID(), id);
    //ASSERT_EQ(obj->name(), name);
    //ASSERT_EQ(obj->value(), value);
}

