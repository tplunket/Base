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

#include "Persistence/ObjectID.hpp"
#include "Base/String.hpp"

using namespace NxA;

#pragma mark Instance Methods

String::Pointer ObjectID::description(void) const
{
    return String::stringWithFormat("<ObjectID Type:%08lx, Index:%d>", this->type, this->index);
}

#pragma mark Comparison Operators

integer32 ObjectID::compareTo(const ObjectID& other) const
{
    if (type != other.type)
    {
        return (type < other.type) ? -1 : 1;
    }
    if (index != other.index)
    {
        return (index < other.index) ? -1 : 1;
    }
    return 0;
}
boolean ObjectID::operator<(const ObjectID& other) const
{
    return compareTo(other) < 0;
}
boolean ObjectID::operator>(const ObjectID& other) const
{
    return compareTo(other) > 0;
}
boolean ObjectID::operator<=(const ObjectID& other) const
{
    return compareTo(other) <= 0;
}
boolean ObjectID::operator>=(const ObjectID& other) const
{
    return compareTo(other) >= 0;
}
boolean ObjectID::operator==(const ObjectID& other) const
{
    return compareTo(other) == 0;
}
boolean ObjectID::operator!=(const ObjectID& other) const
{
    return compareTo(other) != 0;
}

