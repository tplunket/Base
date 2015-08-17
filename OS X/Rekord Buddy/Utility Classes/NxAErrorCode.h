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

#pragma mark Constants

/*!
 *  enum NxAErrorCode
 *  Error codes supported by the custom domain.
 */
typedef NS_ENUM(NSInteger, NxAErrorCode) {
    NxAInternalErrorCode,               /*!< Internal error not caused by the user or the OS. */
    NxACollectionParseErrorCode,        /*!< Error parsing a track collection. */
    NxAInvalidUserInputErrorCode,       /*!< Error caused by an invalid user input. */
    NxAInvalidArgumentErrorCode         /*!< Error caused by an invalid argument. */
};
