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

#import "NxAMarkerProperty.h"
#import "NxAMessageLog.h"

#pragma mark Public Interface

@interface NxAMarkerProperty (TraktorParsing)

#pragma mark Instance Methods

/*
 * Add this marker to a Traktor track entry element.
 * \param entryElement Entry element to add this marker to.
 * \param log Log to use for errors and warnings.
 */
- (void)addToTraktorEntryElement:(NSXMLElement *)entryElement
                             log:(NxAMessageLog *)log;

#pragma mark Class Methods

/*!
 * Update Traktor marker elements.
 * \param markers Markers to update the Traktor track.
 * \param entryElement Entry element where to replace the markers.
 * \param log Log to use for errors and warnings.
 * \return YES if succeeded or NO if failed.
 */
+ (BOOL)updateTraktorMarkers:(NSArray *)markers
                entryElement:(NSXMLElement *)entryElement
                         log:(NxAMessageLog *)log;

@end
