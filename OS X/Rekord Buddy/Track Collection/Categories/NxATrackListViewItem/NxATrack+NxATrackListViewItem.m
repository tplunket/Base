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

#import "NxATrack+NxATrackListViewItem.h"
#import "NxAArtworkProperty+NxATrackListViewItem.h"
#import "NxATrackAlbum+NxATrackListViewItem.h"
#import "NxATrackListViewItem.h"
#import "Track Collection/Internal/NxATrack.h"
#import "NSArray+NxAUtility.h"
#import "Track Collection/Tags/NxATag.h"
#import "NxATagEntry.h"
#import "NxATrackArtistCredit.h"
#import "Track Collection/Tags/Crate Properties/NxATrackProperty.h"
#import "NxATrackRecordLabel.h"
#import "Track Collection/Tags/Track Properties/NxATrackAlbum.h"
#import "NxATrackRemixerCredit.h"
#import "NxATrackProducerCredit.h"
#import "NxATrackGenre.h"
#import "Track Collection/Tags/Properties/NxAIntegerProperty.h"
#import "NxADecimalProperty.h"
#import "NxATrackTitle.h"
#import "NxATrackComments.h"
#import "NxATrackMixName.h"
#import "NxATrackNumber.h"
#import "NxATrackRating.h"
#import "NxATrackBitRate.h"
#import "NxATrackSampleRate.h"
#import "NxATrackSize.h"
#import "NxATrackLength.h"
#import "NxATrackBitDepth.h"
#import "NxATrackPlayCount.h"
#import "NxATrackDateModified.h"
#import "NxATrackDateAdded.h"
#import "NxATrackDateReleased.h"
#import "Track Collection/Tags/Track Properties/NxATrackKey.h"
#import "NxATrackTag.h"
#import "NxAAudioFile.h"
#import "NxAWaveformViewController.h"
#import "NSDate+NxAUtility.h"
#import "NxAParsedStringValue.h"
#import "NxAParsedDateValue.h"
#import "NxAParsedIntegerValue.h"

#pragma mark Implementation

@implementation NxATrack (NxATrackListViewItem)

#pragma mark Instance Methods

/*!
 * Set a string property value for a tag.
 * \param tagClass Class of the tag holding the property.
 * \param value Value to set.
 */
- (void)p_trackListViewItemSetStringPropertyForClass:(Class)tagClass
                                               value:(NSString *)value
{
    NxAParsedStringValue *parsedValue = value ? [NxAParsedStringValue parsedValueWithString:value] : [NxAParsedStringValue parsedValueWithNoString];
    NSDate *now = [NSDate date];
    [self p_replaceTagClass:tagClass withParsedValues:@[ parsedValue ] modificationDate:now];
    [self markAsModifiedOn:now];
}

/*!
 * Set a number property value for a tag.
 * \param tagClass Class of the tag holding the property.
 * \param value Value to set.
 */
- (void)p_trackListViewItemSetIntegerPropertyForClass:(Class)tagClass
                                                value:(id)value
{
    NxAParsedIntegerValue *parsedValue = value ? [NxAParsedIntegerValue parsedValueWithInteger:[value integerValue]] : [NxAParsedIntegerValue parsedValueWithNoInteger];
    NSDate *now = [NSDate date];
    [self p_replaceTagClass:tagClass withParsedValues:@[ parsedValue ] modificationDate:now];
    [self markAsModifiedOn:now];
}

/*!
 * Set a date property value for a tag.
 * \param tagClass Class of the tag holding the property.
 * \param value Value to set.
 */
- (void)p_trackListViewItemSetDatePropertyForClass:(Class)tagClass
                                             value:(id)value
{
    NxAParsedDateValue *parsedValue = value ? [NxAParsedDateValue parsedValueWithDate:value] : [NxAParsedDateValue parsedValueWithNoDate];
    NSDate *now = [NSDate date];
    [self p_replaceTagClass:tagClass withParsedValues:@[ parsedValue ] modificationDate:now];
    [self markAsModifiedOn:now];
}

/*!
 * Return an array of property values for a tag.
 * \param tagClass Class of the tag holding the property.
 */
- (NSArray *)p_trackListViewItemStringArrayPropertyForClass:(Class)tagClass
{
    NSArray *values = [tagClass tagsIn:self];
    return values.count ? [NSArray arrayWithStringValuesFromArray:values] : nil;
}

/*!
 * Return an array of property descriptions for a tag.
 * \param tagClass Class of the tag holding the property.
 */
- (NSArray *)p_trackListViewItemDescriptionArrayPropertyForClass:(Class)tagClass
{
    NSArray *values = [tagClass tagsIn:self];
    return values.count ? [NSArray arrayWithDescriptionsFromArray:values] : nil;
}

/*!
 * Set an array of property values for a tag.
 * \param tagClass Class of the tag holding the array property.
 * \param values Values to set.
 */
- (void)p_trackListViewItemSetStringArrayPropertyForTagClass:(Class)tagClass
                                                      values:(NSArray *)values
{
    NSMutableArray *newValues = [NSMutableArray arrayWithCapacity:values.count];
    if (values.count) {
        for (NSString *value in values) {
            NxAParsedStringValue *newValue = [NxAParsedStringValue parsedValueWithString:value];
            [newValues addObject:newValue];
        }
    }
    else {
        [newValues addObject:[NxAParsedStringValue parsedValueWithNoString]];
    }

    NSDate *now = [NSDate date];
    [self p_replaceTagClass:tagClass withParsedValues:newValues modificationDate:now];
    [self markAsModifiedOn:now];
}

#pragma mark NxATrackListViewItem Protocol

- (BOOL)trackListViewItemIsGroupItem
{
    return NO;
}

- (NSString *)trackListViewItemCellIdentifier
{
    return @"TrackCellView";
}

- (NSArray *)trackListViewItemChildren
{
    return nil;
}

- (NSString *)trackListViewItemTitle
{
    return [[[NxATrackTitle tagsIn:self] firstObject] stringValue];
}

- (void)setTrackListViewItemTitle:(NSString *)value
{
    [self p_trackListViewItemSetStringPropertyForClass:[NxATrackTitle class] value:value];
}

- (NSArray *)trackListViewItemArtists
{
    return [self p_trackListViewItemStringArrayPropertyForClass:[NxATrackArtistCredit class]];
}

- (void)setTrackListViewItemArtists:(NSArray *)values
{
    [self p_trackListViewItemSetStringArrayPropertyForTagClass:[NxATrackArtistCredit class] values:values];
}

- (NSArray *)trackListViewItemRemixers
{
    return [self p_trackListViewItemStringArrayPropertyForClass:[NxATrackRemixerCredit class]];
}

- (void)setTrackListViewItemRemixers:(NSArray *)values
{
    [self p_trackListViewItemSetStringArrayPropertyForTagClass:[NxATrackRemixerCredit class] values:values];
}

- (NSArray *)trackListViewItemProducers
{
    return [self p_trackListViewItemStringArrayPropertyForClass:[NxATrackProducerCredit class]];
}

- (void)setTrackListViewItemProducers:(NSArray *)values
{
    [self p_trackListViewItemSetStringArrayPropertyForTagClass:[NxATrackProducerCredit class] values:values];
}

- (NSString *)trackListViewItemComments
{
    return [[[NxATrackComments tagsIn:self] firstObject] stringValue];
}

- (void)setTrackListViewItemComments:(NSString *)value
{
    [self p_trackListViewItemSetStringPropertyForClass:[NxATrackComments class] value:value];
}

- (NSImage *)trackListViewItemArtwork
{
    NxAArtworkProperty *artwork = [[NxAArtworkProperty tagsIn:self] firstObject];
    return artwork.trackListViewItemArtwork;
}

- (void)setTrackListViewItemArtwork:(NSImage *)value
{
    NSDate *now = [NSDate date];
    NSArray *newTags;

    if (value) {
        NxAArtworkProperty *newArtwork = [NxAArtworkProperty artworkPropertyWithImage:value
                                                          context:self.managedObjectContext];
        newTags = @[ newArtwork ];
    }
    else {
        newTags = @[ ];
    }

    [self p_replaceTagClass:[NxAArtworkProperty class] withTags:newTags modificationDate:now];
}

- (NSArray *)trackListViewItemTags
{
    return [self p_trackListViewItemDescriptionArrayPropertyForClass:[NxATrackTag class]];
}

- (void)setTrackListViewItemTags:(NSArray *)values
{
    [self p_trackListViewItemSetStringArrayPropertyForTagClass:[NxATrackTag class] values:values];
}

- (NSArray *)trackListViewItemKeys
{
    return [self p_trackListViewItemDescriptionArrayPropertyForClass:[NxATrackKey class]];
}

- (void)setTrackListViewItemKeys:(NSArray *)values
{
    [self p_trackListViewItemSetStringArrayPropertyForTagClass:[NxATrackKey class] values:values];
}

- (NSNumber *)trackListViewItemRating
{
    return [[[NxATrackRating tagsIn:self] firstObject] numberValue];
}

- (void)setTrackListViewItemRating:(NSNumber *)value
{
    [self p_trackListViewItemSetIntegerPropertyForClass:[NxATrackRating class] value:value];
}

- (NSArray *)trackListViewItemGenres
{
    return [self p_trackListViewItemStringArrayPropertyForClass:[NxATrackGenre class]];
}

- (void)setTrackListViewItemGenres:(NSArray *)values
{
    [self p_trackListViewItemSetStringArrayPropertyForTagClass:[NxATrackGenre class] values:values];
}

- (NSString *)trackListViewItemRecordLabel
{
    return [[[NxATrackRecordLabel tagsIn:self] firstObject] stringValue];
}

- (void)setTrackListViewItemRecordLabel:(NSArray *)values
{
    [self p_trackListViewItemSetStringPropertyForClass:[NxATrackRecordLabel class] value:values[0]];
}

- (NSDate *)trackListViewItemDateReleased
{
    NSArray *tags = [NxATrackDateReleased tagsIn:self];
    if (!tags.count) {
        return [NSDate dateWithTimeIntervalSince1970:0];
    }

    NxATrackDateReleased *dateReleased = tags.firstObject;
    NSString *stringValue = [dateReleased stringValueInLocalTimeZoneWithFormat:@"yyyy-MM-dd HH:mm:ss"];
    return [NSDate dateWithStringInLocalTimeZone:stringValue format:@"yyyy-MM-dd HH:mm:ss"];
}

- (void)setTrackListViewItemDateReleased:(NSDate *)value
{
    [self p_trackListViewItemSetDatePropertyForClass:[NxATrackDateReleased class] value:value];
}

- (NSDate *)trackListViewItemDateAdded
{
    NSArray *tags = [NxATrackDateAdded tagsIn:self];
    if (!tags.count) {
        return nil;
    }

    NxATrackDateAdded *dateAdded = tags.firstObject;
    NSString *stringValue = [dateAdded stringValueInLocalTimeZoneWithFormat:@"yyyy-MM-dd HH:mm:ss"];
    return [NSDate dateWithStringInLocalTimeZone:stringValue format:@"yyyy-MM-dd HH:mm:ss"];
}

- (void)setTrackListViewItemDateAdded:(NSDate *)value
{
    [self p_trackListViewItemSetDatePropertyForClass:[NxATrackDateAdded class] value:value];
}

- (NSDate *)trackListViewItemDateModified
{
    NSArray *tags = [NxATrackDateModified tagsIn:self];
    if (!tags.count) {
        return nil;
    }

    NxATrackDateModified *dateModified = tags.firstObject;
    NSString *stringValue = [dateModified stringValueInLocalTimeZoneWithFormat:@"yyyy-MM-dd HH:mm:ss"];
    return [NSDate dateWithStringInLocalTimeZone:stringValue format:@"yyyy-MM-dd HH:mm:ss"];
}

- (NSString *)trackListViewItemMixName
{
    return [[[NxATrackMixName tagsIn:self] firstObject] stringValue];
}

- (void)setTrackListViewItemMixName:(NSString *)value
{
    [self p_trackListViewItemSetStringPropertyForClass:[NxATrackMixName class] value:value];
}

- (NSNumber *)trackListViewItemPlayCount
{
    return [[[NxATrackPlayCount tagsIn:self] firstObject] numberValue];
}

- (void)setTrackListViewItemPlayCount:(NSNumber *)value
{
    [self p_trackListViewItemSetIntegerPropertyForClass:[NxATrackPlayCount class] value:value];
}

- (NSString *)trackListViewItemAlbum
{
    return [[[NxATrackAlbum tagsIn:self] firstObject] trackListViewItemAlbum];
}

- (NSNumber *)trackListViewItemTrackCount
{
    return [[[NxATrackAlbum tagsIn:self] firstObject] trackListViewItemTrackCount];
}

- (NSNumber *)trackListViewItemTrackNumber
{
    return [[[NxATrackNumber tagsIn:self] firstObject] numberValue];
}

- (void)setTrackListViewItemTrackNumber:(NSNumber *)value
{
    [self p_trackListViewItemSetIntegerPropertyForClass:[NxATrackNumber class] value:value];
}

- (NSArray *)trackListViewItemParentCrates
{
    NSSet *parentTrackProperties = self.p_parentTrackProperties;
    NSSet *parentCrates = [NxATrackProperty parentCratesForTrackProperties:parentTrackProperties];
    return parentCrates ? [NSArray arrayWithDescriptionsFromSet:parentCrates] : nil;
}

- (NSString *)trackListViewItemSampleRate
{
    return [[[NxATrackSampleRate tagsIn:self] firstObject] stringValue];
}

- (NSString *)trackListViewItemBitRate
{
    return [[[NxATrackBitRate tagsIn:self] firstObject] stringValue];
}

- (NSString *)trackListViewItemBitDepth
{
    return [[[NxATrackBitDepth tagsIn:self] firstObject] stringValue];
}

- (NSString *)trackListViewItemFileSize
{
    return [[[NxATrackSize tagsIn:self] firstObject] stringValue];
}

- (NSString *)trackListViewItemLength
{
    return [[[NxATrackLength tagsIn:self] firstObject] stringValue];
}

- (void)displayWaveformWithController:(NxAWaveformViewController *)controller
{
    [controller displayTrack:self];
}

- (void)trackListViewItemAddURIToArray:(NSMutableArray *)uris
{
    NSURL *trackURI = [[self objectID] URIRepresentation];
    [uris addObject:trackURI];
}

- (void)trackListViewItemAddURLToArray:(NSMutableArray *)urls
{
    NxAAudioFile *localFile = self.fileForCurrentPlatform;
    [localFile addURLToPasteboardArray:urls];
}

@end
