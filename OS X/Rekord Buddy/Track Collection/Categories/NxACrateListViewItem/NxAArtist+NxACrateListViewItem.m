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

#import "NxACrateListViewItem.h"
#import "NxAArtist+NxACrateListViewItem.h"
#import "NxAArtist+NxACrateListViewItemSource.h"
#import "Track Collection/Internal/NxAArtist.h"

#pragma mark Implementation

@implementation NxAArtist (NxACrateListViewItem)

#pragma mark Class Methods

+ (NSArray *)crateListViewItemsForAllItemsInContext:(NSManagedObjectContext *)context
                                        withTagName:(NSString *)tagName
                                      existingItems:(NSArray *)existingItems
{
    NxAAssertTrue(tagName.length == 0);

    NSFetchRequest *request = [[NSFetchRequest alloc] init];

    NSEntityDescription *entity = [NSEntityDescription entityForName:@"Artist" inManagedObjectContext:context];
    [request setEntity:entity];

    NSArray *results = [context executeFetchRequest:request error:nil];
    if (!results.count) {
        return nil;
    }

    NSSortDescriptor *sortDesc = [[NSSortDescriptor alloc] initWithKey:@"p_name"
                                                             ascending:YES
                                                              selector:@selector(localizedStandardCompare:)];
    results = [results sortedArrayUsingDescriptors:@[ sortDesc ]];

    NSMutableArray *resultItems = [NSMutableArray arrayWithCapacity:results.count];
    for (NxAArtist *artist in results) {
        NxACrateListViewItem *newItem = artist.crateListViewItem;
        [resultItems addObject:newItem];
    }

    return resultItems;
}

@end
