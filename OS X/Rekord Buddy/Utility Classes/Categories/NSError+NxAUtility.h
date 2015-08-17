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

#import "NxAErrorCode.h"

#pragma mark Constants

/*! Key for any additional error info stored in a returned NSError. */
extern NSString *const NxALocalizedAdditionalErrorInfoKey;

/*! Key for any URL pointing to additional error info stored in a returned NSError. */
extern NSString *const NxAAdditionalUrlInfoKey;

#pragma mark - Public Interface

/*! Custom category of the NSError class in order to easily build error objects. */
@interface NSError (NxAUtility)

#pragma mark Factory Methods

/*!
 * Creates an NSError for an internal error.
 * \attention Should only be used for error which the user can't recover from.
 * \param description A description of the error.
 * \return An NSError containing the description provided.
 */
+ (instancetype)errorWithInternalCause:(NSString *)description;

/*!
 * Creates an NSError using specific attributes.
 * \param code The error code, taken from the NxAErrorCode list.
 * \param description A description of the error.
 * \param suggestion A suggestion as to how the user can remedy the error.
 * \return An NSError containing the attributes provided.
 */
+ (instancetype)errorWithCode:(NxAErrorCode)code
                  description:(NSString *)description
           recoverySuggestion:(NSString *)suggestion;

/*!
 * Creates an NSError using specific attributes.
 * \param code The error code, taken from the NxAErrorCode list.
 * \param description A description of the error.
 * \param suggestion A suggestion as to how the user can remedy the error.
 * \param info Additional information about the error.
 * \return An NSError containing the attributes provided.
 */
+ (instancetype)errorWithCode:(NxAErrorCode)code
                  description:(NSString *)description
           recoverySuggestion:(NSString *)suggestion
               additionalInfo:(NSString *)info;

/*!
 * Creates an NSError using specific attributes.
 * \param code The error code, taken from the NxErrorCode list.
 * \param description A description of the error.
 * \param suggestion A suggestion as to how the user can remedy the error.
 * \param info Additional information about the error.
 * \param url A URL that links to even more information about the error.
 * \return An NSError containing the attributes provided.
 */
+ (instancetype)errorWithCode:(NxAErrorCode)code
                  description:(NSString *)description
           recoverySuggestion:(NSString *)suggestion
               additionalInfo:(NSString *)info
                additionalUrl:(NSString *)url;

#pragma mark Properties

/*! String which contains additional info on the error, localized if possible. */
@property (readonly, nonatomic) NSString *localizedAdditionalInfo;

/*! URL that links to even more information about the error. */
@property (readonly, nonatomic) NSString *additionalUrl;

/*! If this error relates to Core Data validation, this returns its expanded content.
 * \details This expands all validation errors into one error.
 */
@property (readonly, nonatomic) NSError *expandedCoreDataValidationError;

@end
