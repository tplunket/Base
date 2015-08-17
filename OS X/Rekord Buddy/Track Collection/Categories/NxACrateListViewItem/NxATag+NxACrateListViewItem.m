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

#import "NxATag+NxACrateListViewItem.h"
#import "NxATag+NxACrateListViewItemSource.h"
#import "NxACrateListViewItem.h"
#import "NxACrateListViewTagClassItem.h"
#import "NxATrackBooleanTag.h"
#import "Track Collection/Tags/Internal/NxATag.h"

#pragma mark Implementation

@implementation NxATag (NxACrateListViewItem)

#pragma mark Class Methods

+ (NSArray *)crateListViewItemsForAllItemsInContext:(NSManagedObjectContext *)context
                                       withTagClass:(Class)tagClass
                                         andTagName:(NSString *)tagName
                                      existingItems:(NSArray *)existingItems
{
    NxAAssertNotNil(context);
    NxAAssertNotNil(tagClass);
    NxAAssertNotNil(tagName);
    NxAAssertNotNil(existingItems);

    NSFetchRequest *request = [[NSFetchRequest alloc] init];

    [request setEntity:[NSEntityDescription entityForName:[tagClass p_managedEntityName] inManagedObjectContext:context]];
    NSArray *tags = [context executeFetchRequest:request error:nil];
    if (!tags.count) {
        return nil;
    }

    NSSortDescriptor *sortDesc = [[NSSortDescriptor alloc] initWithKey:@"stringValue"
                                                             ascending:YES
                                                              selector:@selector(localizedStandardCompare:)];
    tags = [tags sortedArrayUsingDescriptors:@[ sortDesc ]];

    NSMutableArray *results = [NSMutableArray arrayWithCapacity:tags.count];

    if (tagName.length) {
        NSArray *tagsWithName = [NxATag tagsInTags:tags withName:tagName];
        for (NxATag *tag in tagsWithName) {
            [results addObject:tag.crateListViewItem];
        }
    }
    else {
        NSMutableDictionary *existingItemsByTitle = [NSMutableDictionary dictionaryWithCapacity:existingItems.count];
        for (id item in existingItems) {
            if ([item isKindOfClass:[NxACrateListViewTagClassItem class]]) {
                NxACrateListViewTagClassItem *tagClassItem = item;
                existingItemsByTitle[tagClassItem.displayTitle] = tagClassItem;
            }
        }

        NSMutableSet *tagNamesAlreadyAdded = [NSMutableSet set];

        for (NxATag *tag in tags) {
            NxAAssertTrue([tag isKindOfClass:[self class]]);

            if (tag.isNil || !tag.hasName) {
                NxACrateListViewItem *newItem = tag.crateListViewItem;
                [results addObject:newItem];
            }
            else {
                NSString *tagNameFound = tag.p_tagName;
                if ([tagNamesAlreadyAdded containsObject:tagNameFound]) {
                    continue;
                }

                [tagNamesAlreadyAdded addObject:tagNameFound];

                NxACrateListViewTagClassItem *existingItem = existingItemsByTitle[tagNameFound];
                if (existingItem) {
                    [results addObject:existingItem];
                }
                else {
                    NxACrateListViewTagClassItem *newItem = [NxACrateListViewTagClassItem itemWithTagClass:tagClass
                                                                                               displayedAs:tagNameFound
                                                                                              usingContext:context];
                    newItem.cellIdentifier = @"TagCell";
                    newItem.tagNameFilter = tagNameFound;
                    
                    [results addObject:newItem];
                }
            }
        }
    }

    return results;
}

@end
