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

#pragma mark Public Interface

@interface NxAProgressViewController : NSViewController

#pragma mark Properties

/*! Main progress window. */
@property (strong, nonatomic) IBOutlet NSWindow *parentWindow;

/*! Status message currently displayed in the progress bar. */
@property (strong) NSString *status;

/*! Minimum value displayed by the progress meter. */
@property (assign) double min;

/*! Maximum value displayed by the progress meter. */
@property (assign) double max;

/*! Current value displayed by the progress meter. */
@property (assign) double value;

#pragma mark Instance Methods

/*! Show the progress window. */
- (void)show;

/*! Hide the progress window. */
- (void)hide;

@end
