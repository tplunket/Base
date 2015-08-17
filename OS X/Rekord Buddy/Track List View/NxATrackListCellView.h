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

/*! A custom type of table cell view for the track list view. */
@interface NxATrackListCellView : NSTableCellView

#pragma mark Properties

/*! Type of background color to use when drawing the view, NO=dark YES:light */
@property (assign, nonatomic) BOOL darkBackground;

@end
