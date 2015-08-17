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

#import "NxAWarning.h"
#import "NxAWarningContextObject.h"

#pragma mark Public Interface

/*! List of unique warnings and lists of similar warnings for different tracks. */
@interface NxAWarningList : NSObject

#pragma mark Properties

/*! Number of warnings currently stored in this list. */
@property (readonly, nonatomic) NSUInteger count;

#pragma mark Instance Methods

/*! Reset the list of warning and delete any warning it had stored. */
- (void)reset;

/*!
 * Add a warning to the list.
 * \details If the same warning is added more than one, only one gets added to the list.
 * \param warning The warning to add to the list.
 */
- (void)addWarning:(NxAWarning *)warning;

/*!
 * Add a warning to the list a given object.
 * \details This is used to group all warnings which are the same but relate to different objects.
 *          Warnings are then printed in a list with the prefix "Found xx <name>(s) <warning>".
 *          Because of this, warnings should be written as, for example, "which have an invalid size.".
 *          This is then folled by a list of objects this warning applies formatted as
 *          <description> (<info>) for each line of the list.
 * \param warning The warning message, used as <warning>.
 * \param object The object this warning relates to.
 *          The object's description: will be used as <description>.
 *          The object's name: will be used as <name>.
 * \param perObjectInfo Additional info printed for each object, used as <info> if not an empty string.
 */
- (void)addWarning:(NxAWarning *)warning
         forObject:(id <NxAWarningContextObject>)object
     perObjectInfo:(NSString *)perObjectInfo;

/*!
 * Complete text of all the warnings stored in this list.
 * \return An NSAttributedString containing the formatted text of all warnings.
 */
- (NSAttributedString *)warningText;

@end
