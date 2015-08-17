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

#import "NxAParsedValue.h"
#import "Track Collection/Tags/Marker Properties/NxACueMarkerProperty.h"

#pragma mark Constants

typedef NS_ENUM(NSUInteger, NxAParsedMarkerValueType) {
    NxAParsedMarkerValueNilType,
    NxAParsedMarkerValueCueType,
    NxAParsedMarkerValueGridType,
    NxAParsedMarkerValueLoopType,
};

#pragma mark - Public Interface

/*! Parsed value that can be used to create a track marker. */
@interface NxAParsedMarkerValue : NxAParsedValue

#pragma mark Factory Methods

+ (instancetype)parsedMarkerValueWithNoMarker;

+ (instancetype)parsedMarkerValueWithName:(NSString *)name
                        positionInSeconds:(NSDecimalNumber *)positionInSeconds
                             hotCueNumber:(NSNumber *)hotCueNumber;

+ (instancetype)parsedLoopMarkerValueWithName:(NSString *)name
                            positionInSeconds:(NSDecimalNumber *)positionInSeconds
                              lengthInSeconds:(NSDecimalNumber *)lengthInSeconds
                                 hotCueNumber:(NSNumber *)hotCueNumber;

+ (instancetype)parsedGridMarkerValueAtPositionInSeconds:(NSDecimalNumber *)positionInSeconds
                                          beatsPerMinute:(NSDecimalNumber *)beatsPerMinute;

#pragma mark Properties

@property (readonly, nonatomic) NxAParsedMarkerValueType type;

@property (strong, nonatomic) NSString *name;

@property (strong, nonatomic) NSDecimalNumber *positionInSeconds;

@property (strong, nonatomic) NSDecimalNumber *lengthInSeconds;

@property (strong, nonatomic) NSNumber *hotCueNumber;

@property (strong, nonatomic) NSDecimalNumber *beatsPerMinute;

@property (readonly, nonatomic) NSSet *customFlags;

#pragma mark Instance Methods

- (void)addCustomFlag:(NSString *)flag;

- (void)removeCustomFlag:(NSString *)flag;

- (BOOL)hasCustomFlag:(NSString *)flag;

@end
