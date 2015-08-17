//
//  Copyright (c) 2013-2015 Next Audio Labs, LLC. All rights reserved.
//
//  This file contains confidential and proprietary information of Next
//  Audio Labs, LLC ("Next"). No use is permitted without express written
//  permission of Next. If you are not a party to a Confidentiality/
//  Non-Disclosure Agreement with Next, please immediately delete this
//  file as well as all copies in your possession. For further information,
//  please email info@nextaudiolabs.com.
//

#import "Track Edit View/Internal/NxATextField.h"

#pragma mark Implementation

@implementation NxATextField

#pragma mark NSNibAwaking Protocol

- (void)awakeFromNib
{
    [super awakeFromNib];

    self.delegate = self;
}

#pragma mark NSTextFieldDelegate protocol

- (BOOL)control:(NSControl *)control
  isValidObject:(id)object
{
    if ([object isKindOfClass:[NSString class]]) {
        return [self.p_itemsClass isAValidStringDescription:object];
    }
    else if ([object isKindOfClass:[NSNumber class]]) {
        return [self.p_itemsClass isAValidNumberDescription:object];
    }
    else {
        return NO;
    }
}

@end
