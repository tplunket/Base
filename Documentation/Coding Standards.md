# Coding Standards

This is the coding standard document for Next Audio Labs. These standards are mandatory and must be adhered to for any software project developed for Next.

Any topic not yet fleshed out but which should be added to this document in the future can be found in the "Topic to be added" page.

## Topics to be added

This page is a repository for coding standard topics that have yet to be fleshed out and added to the main document.

* Use lazy initializations for any ivars, when possible.
* Local variables should be used to store all if not most returned values from methods so that they can be more easily inspected during debugging.
* When those local variables are used in if() of for() statements, they should be initialized on the line right above those statements, if possible, with no blank line between the two.
* ZAssert() should be used to test against any object returned or created that could cause the method to silently fail because of a nil value (mainly because a message passed to a nil object returns nil or does nothing).
* If an object is created inside a constructor and it being nil doesn't affect the result of the constructor, ZAssert() is not needed because the caller can deal with the nil return value itself.
* ZAssert is also not needed in testing the return value of a local, private method as long as this method does its own testing before returning an object.
* Boolean tests do not need to specify == YES or == NO and should just use ! or nothing
* Pointer tests do not need to specify == nil or != nil and should just use ! or nothing
* Do not check-in code that is commented out. Comments are for additional information, not to disable code. Use #ifdef or delete the code instead.
* Last line of files should be a newline.
Warnings should always be treated as errors.
* @class should be used in headers instead of #import when possible.
* Document pragma 'mark' format and layout.
* Methods with multiple arguments list each argument on a separate line, both in the header and the implementation.
Private methods and private properties should be prefixed with p_.
* Methods only returning one value should be properties instead (readonly)
* Properties should always be accessed using the dot notation.
* Don't use #define for constants. Use extern NSString *const for strings or NS_ENUM(NSInteger, xxxx) for numerical values.

