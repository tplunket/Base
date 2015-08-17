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

#import "Track Edit View/NxATextField.h"
#import "NxAValidationAndCompletionQuerying.h"

#pragma mark Private Interface

@interface NxATextField ()

#pragma mark Properties

/*! Class of objects used by this text field. */
@property (assign, nonatomic) Class <NxAValidationAndCompletionQuerying> p_itemsClass;

@end
