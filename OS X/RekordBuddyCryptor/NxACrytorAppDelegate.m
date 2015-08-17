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

#import "RNCryptor/RNEncryptor.h"
#import "RNCryptor/RNDecryptor.h"

#import "NxACrytorAppDelegate.h"
#import "NxAAppProtection.h"

@implementation NxACrytorAppDelegate

- (void)applicationDidFinishLaunching:(NSNotification *)aNotification
{
    NSArray *srcData = @[ NXA_APP_PROTECTION_INJECTED_OBJECT_NAME_STRING,
                          NXA_APP_PROTECTION_INJECTED_METHOD_NAME_STRING ];
    NSData *data = [NSKeyedArchiver archivedDataWithRootObject:srcData];
    NSError *error;
    NSData *encryptedData = [RNEncryptor encryptData:data
                                        withSettings:kRNCryptorAES256Settings
                                            password:NXA_APP_PROTECTION_ENCRYPTION_KEY_STRING
                                               error:&error];

    NSString *path = [[[[NSBundle mainBundle] bundlePath] stringByDeletingLastPathComponent] stringByAppendingPathComponent:NXA_APP_PROTECTION_KEY_FILENAME_STRING];

    if (!encryptedData || ![encryptedData writeToFile:path atomically:YES]) {
        NSAlert *alert = [NSAlert alertWithMessageText:@"Can't create beta key."
                                         defaultButton:@"Ok"
                                       alternateButton:nil
                                           otherButton:nil
                             informativeTextWithFormat:@"Something went wrong."];
        
        [alert runModal];
        exit(-1);
    }

    exit(0);
}

@end
