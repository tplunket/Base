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

#import "Track Collection/Tags/Track Properties/Internal/NxATrackKey.h"
#import "Track Collection/NxASortingCriteria.h"
#import "NxAGeneralPreferencesViewController.h"
#import "Track Collection/Tags/Internal/NxATag.h"

#pragma mark Implementation

@implementation NxATrackKey

#pragma mark Overridden NxATag Factory Methods

+ (instancetype)tagWithName:(NSString *)name
                    context:(NSManagedObjectContext *)context
{
    // -- We don't support setting keys with just names.
    ALog(@"Illegal call.");

    return nil;
}

+ (instancetype)tagWithValue:(id)value
                     context:(NSManagedObjectContext *)context
{
    NSString *name = nil;
    NSNumber *keyValue = nil;

    if (value) {
        NxAAssertTrue([value isKindOfClass:[NSString class]]);

        NSArray *components = [value componentsSeparatedByString:@":"];
        NSString *stringValue;

        if (components.count == 1) {
            stringValue = components[0];
        }
        else {
            name = components[0];
            stringValue = components[1];
        }

        keyValue = [NxATrackKey keyValueForString:stringValue];
        NxAAssertNotNil(keyValue);
    }

    NxATrackKey *newKey;
    if (name) {
        newKey = [super tagWithName:name
                              value:keyValue
                            context:context];
    }
    else {
        newKey = [super tagWithValue:keyValue
                             context:context];
    }

    NxAAssertNotNil(newKey);
    return newKey;
}

#pragma mark Overridden NxATag Class Methods

+ (NSString *)p_managedEntityName
{
    return @"TrackKey";
}

+ (BOOL)isATagTypeSortedByPropertyName
{
    return YES;
}

#pragma mark Class Methods

/*!
 * Translation to musical keys for the regular musical notation.
 * \return A dictionary to translate key values to key names.
 */
+ (NSDictionary *)p_musicalNotationTranslation
{
    static NSDictionary *notationNames = nil;
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        notationNames = @{ @(NxAMusicalKeyC):@"C",
                           @(NxAMusicalKeyCsDb):@"Db",
                           @(NxAMusicalKeyD):@"D",
                           @(NxAMusicalKeyDsEb):@"Eb",
                           @(NxAMusicalKeyE):@"E",
                           @(NxAMusicalKeyF):@"F",
                           @(NxAMusicalKeyFsGb):@"Gb",
                           @(NxAMusicalKeyG):@"G",
                           @(NxAMusicalKeyGsAb):@"Ab",
                           @(NxAMusicalKeyA):@"A",
                           @(NxAMusicalKeyAsBb):@"Bb",
                           @(NxAMusicalKeyB):@"B",
                           @(NxAMusicalKeyCm):@"Cm",
                           @(NxAMusicalKeyCsmDbm):@"Dbm",
                           @(NxAMusicalKeyDm):@"Dm",
                           @(NxAMusicalKeyDsmEbm):@"Ebm",
                           @(NxAMusicalKeyEm):@"Em",
                           @(NxAMusicalKeyFm):@"Fm",
                           @(NxAMusicalKeyFsmGbm):@"Gbm",
                           @(NxAMusicalKeyGm):@"Gm",
                           @(NxAMusicalKeyGsmAbm):@"Abm",
                           @(NxAMusicalKeyAm):@"Am",
                           @(NxAMusicalKeyAsmBbm):@"Bbm",
                           @(NxAMusicalKeyBm):@"Bm"
                           };
    });

    return notationNames;
}

/*!
 * Translation to musical keys for the regular musical notation with sharps instead of flats.
 * \return A dictionary to translate key values to key names.
 */
+ (NSDictionary *)p_musicalNotationAllSharpsTranslation
{
    static NSDictionary *notationNames = nil;
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        notationNames = @{ @(NxAMusicalKeyC):@"C",
                           @(NxAMusicalKeyCsDb):@"C#",
                           @(NxAMusicalKeyD):@"D",
                           @(NxAMusicalKeyDsEb):@"D#",
                           @(NxAMusicalKeyE):@"E",
                           @(NxAMusicalKeyF):@"F",
                           @(NxAMusicalKeyFsGb):@"F#",
                           @(NxAMusicalKeyG):@"G",
                           @(NxAMusicalKeyGsAb):@"G#",
                           @(NxAMusicalKeyA):@"A",
                           @(NxAMusicalKeyAsBb):@"A#",
                           @(NxAMusicalKeyB):@"B",
                           @(NxAMusicalKeyCm):@"Cm",
                           @(NxAMusicalKeyCsmDbm):@"C#m",
                           @(NxAMusicalKeyDm):@"Dm",
                           @(NxAMusicalKeyDsmEbm):@"D#m",
                           @(NxAMusicalKeyEm):@"Em",
                           @(NxAMusicalKeyFm):@"Fm",
                           @(NxAMusicalKeyFsmGbm):@"F#m",
                           @(NxAMusicalKeyGm):@"Gm",
                           @(NxAMusicalKeyGsmAbm):@"G#m",
                           @(NxAMusicalKeyAm):@"Am",
                           @(NxAMusicalKeyAsmBbm):@"A#m",
                           @(NxAMusicalKeyBm):@"Bm"
                           };
    });

    return notationNames;
}

/*!
 * Translation to musical keys for an alternative regular musical notation.
 * \return A dictionary to translate key values to key names.
 */
+ (NSDictionary *)p_otherMusicalNotationTranslation
{
    static NSDictionary *notationNames = nil;
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        notationNames = @{ @(NxAMusicalKeyC):@"Cmaj",
                           @(NxAMusicalKeyCsDb):@"Dbmaj",
                           @(NxAMusicalKeyD):@"Dmaj",
                           @(NxAMusicalKeyDsEb):@"Ebmaj",
                           @(NxAMusicalKeyE):@"Emaj",
                           @(NxAMusicalKeyF):@"Fmaj",
                           @(NxAMusicalKeyFsGb):@"Gbmaj",
                           @(NxAMusicalKeyG):@"Gmaj",
                           @(NxAMusicalKeyGsAb):@"Abmaj",
                           @(NxAMusicalKeyA):@"Amaj",
                           @(NxAMusicalKeyAsBb):@"Bbmaj",
                           @(NxAMusicalKeyB):@"Bmaj",
                           @(NxAMusicalKeyCm):@"Cmin",
                           @(NxAMusicalKeyCsmDbm):@"Dbmin",
                           @(NxAMusicalKeyDm):@"Dmin",
                           @(NxAMusicalKeyDsmEbm):@"Ebmin",
                           @(NxAMusicalKeyEm):@"Emin",
                           @(NxAMusicalKeyFm):@"Fmin",
                           @(NxAMusicalKeyFsmGbm):@"Gbmin",
                           @(NxAMusicalKeyGm):@"Gmin",
                           @(NxAMusicalKeyGsmAbm):@"Abmin",
                           @(NxAMusicalKeyAm):@"Amin",
                           @(NxAMusicalKeyAsmBbm):@"Bbmin",
                           @(NxAMusicalKeyBm):@"Bmin"
                           };
    });

    return notationNames;
}

/*!
 * Translation to musical keys for an alternative regular musical notation with sharps instead of flats.
 * \return A dictionary to translate key values to key names.
 */
+ (NSDictionary *)p_otherMusicalNotationAllSharpsTranslation
{
    static NSDictionary *notationNames = nil;
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        notationNames = @{ @(NxAMusicalKeyCsDb):@"C#maj",
                           @(NxAMusicalKeyDsEb):@"D#maj",
                           @(NxAMusicalKeyFsGb):@"F#maj",
                           @(NxAMusicalKeyGsAb):@"G#maj",
                           @(NxAMusicalKeyAsBb):@"A#maj",
                           @(NxAMusicalKeyCsmDbm):@"C#min",
                           @(NxAMusicalKeyDsmEbm):@"D#min",
                           @(NxAMusicalKeyFsmGbm):@"F#min",
                           @(NxAMusicalKeyGsmAbm):@"G#min",
                           @(NxAMusicalKeyAsmBbm):@"A#min"
                           };
    });

    return notationNames;
}

/*!
 * Translation to musical keys for the open key musical notation.
 * \return A dictionary to translate key values to key names.
 */
+ (NSDictionary *)p_openKeyNotationTranslation
{
    static NSDictionary *notationNames = nil;
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        notationNames = @{ @(NxAMusicalKeyC):@"1d",
                           @(NxAMusicalKeyCsDb):@"8d",
                           @(NxAMusicalKeyD):@"3d",
                           @(NxAMusicalKeyDsEb):@"10d",
                           @(NxAMusicalKeyE):@"5d",
                           @(NxAMusicalKeyF):@"12d",
                           @(NxAMusicalKeyFsGb):@"7d",
                           @(NxAMusicalKeyG):@"2d",
                           @(NxAMusicalKeyGsAb):@"9d",
                           @(NxAMusicalKeyA):@"4d",
                           @(NxAMusicalKeyAsBb):@"11d",
                           @(NxAMusicalKeyB):@"6d",
                           @(NxAMusicalKeyCm):@"10m",
                           @(NxAMusicalKeyCsmDbm):@"5m",
                           @(NxAMusicalKeyDm):@"12m",
                           @(NxAMusicalKeyDsmEbm):@"7m",
                           @(NxAMusicalKeyEm):@"2m",
                           @(NxAMusicalKeyFm):@"9m",
                           @(NxAMusicalKeyFsmGbm):@"4m",
                           @(NxAMusicalKeyGm):@"11m",
                           @(NxAMusicalKeyGsmAbm):@"6m",
                           @(NxAMusicalKeyAm):@"1m",
                           @(NxAMusicalKeyAsmBbm):@"8m",
                           @(NxAMusicalKeyBm):@"3m"
                           };
    });

    return notationNames;
}

/*!
 * Translation to musical keys for the open key musical notation with leading zeros.
 * \return A dictionary to translate key values to key names.
 */
+ (NSDictionary *)p_openKeyNotationLeadingZerosTranslation
{
    static NSDictionary *notationNames = nil;
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        notationNames = @{ @(NxAMusicalKeyC):@"01d",
                           @(NxAMusicalKeyCsDb):@"08d",
                           @(NxAMusicalKeyD):@"03d",
                           @(NxAMusicalKeyDsEb):@"10d",
                           @(NxAMusicalKeyE):@"05d",
                           @(NxAMusicalKeyF):@"12d",
                           @(NxAMusicalKeyFsGb):@"07d",
                           @(NxAMusicalKeyG):@"02d",
                           @(NxAMusicalKeyGsAb):@"09d",
                           @(NxAMusicalKeyA):@"04d",
                           @(NxAMusicalKeyAsBb):@"11d",
                           @(NxAMusicalKeyB):@"06d",
                           @(NxAMusicalKeyCm):@"10m",
                           @(NxAMusicalKeyCsmDbm):@"05m",
                           @(NxAMusicalKeyDm):@"12m",
                           @(NxAMusicalKeyDsmEbm):@"07m",
                           @(NxAMusicalKeyEm):@"02m",
                           @(NxAMusicalKeyFm):@"09m",
                           @(NxAMusicalKeyFsmGbm):@"04m",
                           @(NxAMusicalKeyGm):@"11m",
                           @(NxAMusicalKeyGsmAbm):@"06m",
                           @(NxAMusicalKeyAm):@"01m",
                           @(NxAMusicalKeyAsmBbm):@"08m",
                           @(NxAMusicalKeyBm):@"03m"
                           };
    });

    return notationNames;
}

/*!
 * Translation to musical keys for the camelot musical notation.
 * \return A dictionary to translate key values to key names.
 */
+ (NSDictionary *)p_camelotNotationTranslation
{
    static NSDictionary *notationNames = nil;
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        notationNames = @{ @(NxAMusicalKeyC):@"8B",
                           @(NxAMusicalKeyCsDb):@"3B",
                           @(NxAMusicalKeyD):@"10B",
                           @(NxAMusicalKeyDsEb):@"5B",
                           @(NxAMusicalKeyE):@"12B",
                           @(NxAMusicalKeyF):@"7B",
                           @(NxAMusicalKeyFsGb):@"2B",
                           @(NxAMusicalKeyG):@"9B",
                           @(NxAMusicalKeyGsAb):@"4B",
                           @(NxAMusicalKeyA):@"11B",
                           @(NxAMusicalKeyAsBb):@"6B",
                           @(NxAMusicalKeyB):@"1B",
                           @(NxAMusicalKeyCm):@"5A",
                           @(NxAMusicalKeyCsmDbm):@"12A",
                           @(NxAMusicalKeyDm):@"7A",
                           @(NxAMusicalKeyDsmEbm):@"2A",
                           @(NxAMusicalKeyEm):@"9A",
                           @(NxAMusicalKeyFm):@"4A",
                           @(NxAMusicalKeyFsmGbm):@"11A",
                           @(NxAMusicalKeyGm):@"6A",
                           @(NxAMusicalKeyGsmAbm):@"1A",
                           @(NxAMusicalKeyAm):@"8A",
                           @(NxAMusicalKeyAsmBbm):@"3A",
                           @(NxAMusicalKeyBm):@"10A"
                           };
    });

    return notationNames;
}

/*!
 * Translation to musical keys for the camelot musical notation with leading zeros.
 * \return A dictionary to translate key values to key names.
 */
+ (NSDictionary *)p_camelotNotationLeadingZerosTranslation
{
    static NSDictionary *notationNames = nil;
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        notationNames = @{ @(NxAMusicalKeyC):@"08B",
                           @(NxAMusicalKeyCsDb):@"03B",
                           @(NxAMusicalKeyD):@"10B",
                           @(NxAMusicalKeyDsEb):@"05B",
                           @(NxAMusicalKeyE):@"12B",
                           @(NxAMusicalKeyF):@"07B",
                           @(NxAMusicalKeyFsGb):@"02B",
                           @(NxAMusicalKeyG):@"09B",
                           @(NxAMusicalKeyGsAb):@"04B",
                           @(NxAMusicalKeyA):@"11B",
                           @(NxAMusicalKeyAsBb):@"06B",
                           @(NxAMusicalKeyB):@"01B",
                           @(NxAMusicalKeyCm):@"05A",
                           @(NxAMusicalKeyCsmDbm):@"12A",
                           @(NxAMusicalKeyDm):@"07A",
                           @(NxAMusicalKeyDsmEbm):@"02A",
                           @(NxAMusicalKeyEm):@"09A",
                           @(NxAMusicalKeyFm):@"04A",
                           @(NxAMusicalKeyFsmGbm):@"11A",
                           @(NxAMusicalKeyGm):@"06A",
                           @(NxAMusicalKeyGsmAbm):@"01A",
                           @(NxAMusicalKeyAm):@"08A",
                           @(NxAMusicalKeyAsmBbm):@"03A",
                           @(NxAMusicalKeyBm):@"10A"
                           };
    });

    return notationNames;
}

+ (NSDictionary *)p_keyPreferenceNames
{
    static NSDictionary *keyPrefNames = nil;
    static dispatch_once_t onceToken = 0;
    dispatch_once(&onceToken, ^{
        NSMutableDictionary *keyTable = [NSMutableDictionary dictionary];
        NSArray *srcKeyTables = @[ [NxATrackKey p_musicalNotationTranslation],
                                   [NxATrackKey p_musicalNotationAllSharpsTranslation],
                                   [NxATrackKey p_otherMusicalNotationTranslation],
                                   [NxATrackKey p_otherMusicalNotationAllSharpsTranslation],
                                   [NxATrackKey p_openKeyNotationTranslation],
                                   [NxATrackKey p_openKeyNotationLeadingZerosTranslation],
                                   [NxATrackKey p_camelotNotationTranslation],
                                   [NxATrackKey p_camelotNotationLeadingZerosTranslation]
                                   ];

        for (NSDictionary *fromKeys in srcKeyTables) {
            for (NSString *keyPrefName in fromKeys.allKeys) {
                NSString *keyName = fromKeys[keyPrefName];
                keyTable[keyName.lowercaseString] = keyPrefName;
            }
        }

        keyPrefNames = keyTable;
    });

    return keyPrefNames;
}

+ (NSNumber *)keyValueForString:(NSString *)value
{
    NxAAssertNotNil(value);
    
    NSDictionary *keyNames = [NxATrackKey p_keyPreferenceNames];

    NSNumber *keyValue = keyNames[value.lowercaseString];
    return keyValue;
}

+ (BOOL)stringIsAValidKeyValue:(NSString *)value
{
    NxAAssertNotNil(value);
    return [NxATrackKey keyValueForString:value] != nil;
}

+ (NSString *)stringValueForKeyValue:(NSNumber *)value
                       usingNotation:(NxAMusicalNotation)notation
{
    NSDictionary *keyTranslation = nil;
    switch (notation) {
        case NxAMusicalNotationOpenKey: {
            keyTranslation = [NxATrackKey p_openKeyNotationTranslation];
            break;
        }
        case NxAMusicalNotationCamelot: {
            keyTranslation = [NxATrackKey p_camelotNotationTranslation];
            break;
        }
        case NxAMusicalNotationStandardAllSharps: {
            keyTranslation = [NxATrackKey p_musicalNotationAllSharpsTranslation];
            break;
        }
        default:
            keyTranslation = [NxATrackKey p_musicalNotationTranslation];
            break;
    }

    return keyTranslation[value];
}

#pragma mark Overridden NxATag Accessors

- (NSString *)propertyDescription
{
    if (self.hasName) {
        return self.name;
    }
    else {
        return @"Key";
    }
}

- (NSString *)stringValue
{
    NSInteger notation = [[NSUserDefaults standardUserDefaults] integerForKey:NxAMusicalNotationPreferenceKey];
    return [self stringValueForNotation:notation];
}

#pragma mark Instance Methods

- (NSString *)stringValueForNotation:(NxAMusicalNotation)notation
{
    return [NxATrackKey stringValueForKeyValue:self.p_value usingNotation:notation];
}

#pragma mark Overridden NSObject Methods

- (NSString *)description
{
    NSString *value = self.stringValue;

    if (self.hasName) {
        return [NSString stringWithFormat:@"%@:%@", self.name, value];
    }
    else {
        return value;
    }
}

@end
