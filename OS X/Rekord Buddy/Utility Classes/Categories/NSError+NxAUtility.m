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

#import "NSError+NxAUtility.h"

#pragma mark Constants

NSString *const NxALocalizedAdditionalErrorInfoKey = @"NxAAdditionalErrorInfo";

NSString *const NxAAdditionalUrlInfoKey = @"NxAAdditionalUrlInfo";

#pragma mark - Implementation

@implementation NSError (NxAUtility)

#pragma mark Factory Methods

/*!
 * Creates an NSError using specific attributes.
 * \param code The error code, taken from the NxAErrorCode list.
 * \param dict An NSDictionary filled with the error's attributes.
 * \return An NSError containing the attributes provided.
 */
+ (instancetype)p_errorwithCode:(NxAErrorCode)code
                     dictionary:(NSDictionary *)dict
{
    NSDictionary *appInfo = [[NSBundle mainBundle] infoDictionary];
    NxAAssertNotNil(appInfo);
    
    // -- The error domain is always the bundle identifier.
    NSString *userErrorDomain = appInfo[@"CFBundleIdentifier"];

    NSError *error = [NSError errorWithDomain:userErrorDomain code:code userInfo:dict];
    NxAAssertNotNil(error);
    
    return error;
}

+ (instancetype)errorWithInternalCause:(NSString *)description
{
    NxAAssertNotNil(description);
    
    return [NSError errorWithCode:NxAInternalErrorCode
                      description:NSLocalizedString(@"Unexpected Internal Error.",
                                                    @"Name of an internal error that happened unexpectedly.")
               recoverySuggestion:[NSString stringWithFormat:NSLocalizedString(@"Internal error: '%@'.\nPlease email support@nextaudiolabs.com to tell us about this error and how to reproduce it.",
                                                                               @"Tells the user to email support about the error."), description]];
}

+ (instancetype)errorWithCode:(NxAErrorCode)code
                  description:(NSString *)description
           recoverySuggestion:(NSString *)suggestion
{
    NxAAssertNotNil(description);
    NxAAssertNotNil(suggestion);
    
    NSDictionary *dict = @{ NSLocalizedDescriptionKey : description,
                            NSLocalizedRecoverySuggestionErrorKey : suggestion
                            };
    NxAAssertNotNil(dict);
    
    NSError *error = [NSError p_errorwithCode:code dictionary:dict];
    return error;
}

+ (instancetype)errorWithCode:(NxAErrorCode)code
                  description:(NSString *)description
           recoverySuggestion:(NSString *)suggestion
               additionalInfo:(NSString *)info
{
    NxAAssertNotNil(description);
    NxAAssertNotNil(info);

    NSDictionary *dict = suggestion ? @{ NSLocalizedDescriptionKey : description,
                                         NSLocalizedRecoverySuggestionErrorKey : suggestion,
                                         NxALocalizedAdditionalErrorInfoKey : info
                                         } : @{ NSLocalizedDescriptionKey : description,
                                                NxALocalizedAdditionalErrorInfoKey : info
                                                };
    NxAAssertNotNil(dict);
    
    NSError *error = [NSError p_errorwithCode:code dictionary:dict];
    return error;
}

+ (instancetype)errorWithCode:(NxAErrorCode)code
                  description:(NSString *)description
           recoverySuggestion:(NSString *)suggestion
               additionalInfo:(NSString *)info
                additionalUrl:(NSString *)url
{
    NxAAssertNotNil(description);
    NxAAssertNotNil(suggestion);
    NxAAssertNotNil(info);
    NxAAssertNotNil(url);

    NSDictionary *dict = @{ NSLocalizedDescriptionKey : description,
                            NSLocalizedRecoverySuggestionErrorKey : suggestion,
                            NxALocalizedAdditionalErrorInfoKey : info,
                            NxAAdditionalUrlInfoKey : url
                            };
    NxAAssertNotNil(dict);
    
    NSError *error = [NSError p_errorwithCode:code dictionary:dict];
    return error;
}

#pragma mark Accessors

- (NSString *)localizedAdditionalInfo
{
    return self.userInfo[NxALocalizedAdditionalErrorInfoKey];
}

- (NSString *)additionalUrl
{
    return self.userInfo[NxAAdditionalUrlInfoKey];
}

- (NSError *)expandedCoreDataValidationError
{
    if (![self.domain isEqualToString:NSCocoaErrorDomain]) {
        // -- If this is not a core data error, we just pass it along.
        return self;
    }

    NSInteger errorCode = self.code;
    if ((errorCode < NSValidationErrorMinimum) || (errorCode > NSValidationErrorMaximum)) {
        // -- If this is not a validation error, we just pass it along.
        return self;
    }

    if (errorCode != NSValidationMultipleErrorsError) {
        // -- If there is only 1 error, we just pass it along
        return self;
    }

    NSMutableString *errorString = [NSMutableString stringWithFormat:@"Failed to save to data store: %@", self.localizedDescription];

    // -- We have multiple errors in an array in the userInfo dictionary for key NSDetailedErrorsKey
    NSArray *detailedErrors = self.userInfo[NSDetailedErrorsKey];
    if (detailedErrors.count > 0) {
        for (NSError *detailedError in detailedErrors) {
            [errorString appendFormat:@"\nDetailedError: %@", detailedError.userInfo];
        }
    }
    else {
        [errorString appendFormat:@"\n%@", self.userInfo];
    }

    // -- We create a new userInfo with the same info as the original error.
    NSMutableDictionary *newUserInfo = [NSMutableDictionary dictionaryWithDictionary:self.userInfo];
    newUserInfo[NSLocalizedDescriptionKey] = errorString;

    // -- Finally, we create a new error with the new userInfo and return it
    NSError *newError = [NSError errorWithDomain:self.domain code:self.code userInfo:newUserInfo];
    return newError;
}

@end
