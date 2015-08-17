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

/*! Encryption key used to encrypt the key file. */
#define NXA_APP_PROTECTION_ENCRYPTION_KEY_STRING        (@"falkdfjaldasdasldkfjslf")

/*! Encryption key used to encrypt the key file. */
#define NXA_APP_PROTECTION_KEY_FILENAME_STRING          (@"key.bin")

/*! Name of the app delegate method called if the copy protection check is successful. */
#define NXA_APP_PROTECTION_DELEGATE_METHOD_NAME         applicationHasDockMenu
#define NXA_APP_PROTECTION_DELEGATE_METHOD_NAME_STRING  (@"applicationHasDockMenu")

/*! Name of the object the copy protection check is injected into. */
#define NXA_APP_PROTECTION_INJECTED_OBJECT_NAME_STRING  (@"NxATrackProperty")

/*! Name of the method the copy protection check is injected into. */
#define NXA_APP_PROTECTION_INJECTED_METHOD_NAME         p_deleteTrackProperty
#define NXA_APP_PROTECTION_INJECTED_METHOD_NAME_STRING  (@"p_deleteTrackProperty")

/*! Injected method called by a succesful copy protection check. */
#if BETA_BUILD
#define NXA_APP_PROTECTION_INJECTED_METHOD              + (void)NXA_APP_PROTECTION_INJECTED_METHOD_NAME \
                                                        { \
                                                            NSApplication *app = NSApp; \
                                                            SEL selector = NSSelectorFromString(@"delegate"); \
                                                            IMP imp = [app methodForSelector:selector]; \
                                                            id (*func)(id, SEL) = (void *)imp; \
                                                            NSObject *delegate = func(app, selector); \
                                                            selector = NSSelectorFromString(NXA_APP_PROTECTION_DELEGATE_METHOD_NAME_STRING); \
                                                            imp = [delegate methodForSelector:selector]; \
                                                            func = (void *)imp; \
                                                            func(delegate, selector); \
                                                        }
#else
#define NXA_APP_PROTECTION_INJECTED_METHOD
#endif
