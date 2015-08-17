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

/*! A custom type of table row view for the track list view. */
@interface NxATrackListRowView : NSTableRowView

#pragma mark Class Methods

/*!
 * Draw the row's background.
 * \param bounds Area to draw into.
 * \param dark YES if drawing a dark background, NO for light.
 */
+ (void)drawBackground:(NSRect)bounds
                  dark:(BOOL)dark;

/*!
 * Determine the background color for a given row.
 * \param rowIndex Index of the row.
 * \param outlineView View which contains the row view
 * \return YES if drawing a dark background, NO for light.
 */
+ (BOOL)rowHasDarkBackgroundAtIndex:(NSInteger)rowIndex
                        outlineView:(NSOutlineView *)outlineView;

@end
