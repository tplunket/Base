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

#pragma mark Public Interface

/*! View used to display an audio waveform. */
@interface NxAWaveformView : NSView

#pragma mark Properties

/*! Color used to draw the waveform. */
@property (strong, nonatomic) NSColor *waveformColor;

/*! Color used to clear the view. */
@property (strong, nonatomic) NSColor *clearColor;

/*! Time of the first sample displayed by the view, in seconds. */
@property (readonly, nonatomic) double startDisplayedTime;

/*! Time of the last sample displayed by the view, in seconds. */
@property (readonly, nonatomic) double endDisplayedTime;

#pragma mark Instance Methods

/*!
 * Set offsets by which the waveform is displayed in the view.
 * \param left Offset from the left side of the view.
 * \param right Offset from the right side of the view.
 * \param top Offset from the top of the view.
 * \param bottom Offset from the bottom of the view.
 */
- (void)offsetWaveDisplayBy:(NSUInteger)left
                      right:(NSUInteger)right
                        top:(NSUInteger)top
                     bottom:(NSUInteger)bottom;

/*!
 * Display audio samples in the waveform view.
 * \param samples Samples to display, stored as floats.
 * \param length Length of the sample data, in seconds.
 */
- (void)displaySamples:(NSData *)samples
                length:(double)length;

- (void)resetSamplesDisplayed;

/*!
 * Get the horizontal position of a given time in the view.
 * \param time Time in seconds of the desired position.
 * \return The x position, in view space, or a negative value if the time is not within the view.
 *         -1 means the time is before the start of the view.
 *         -2 means the time is after the end of the view.
 */
- (NSInteger)positionForTime:(double)time;

/*!
 * Pre-waveform draw call.
 * \details This method is called by the view after clearing it and before
 *          the waveform is drawn. This can be overidden by a subclass to
 *          draw any extra graphics that needs to be drawn behind the waveform.
 * \param bounds Bounds for the view to draw into.
 */
- (void)preDraw:(NSRect)bounds;

/*!
 * Post-waveform draw call.
 * \details This method is called by the view after the waveform is drawn.
 *          This can be overidden by a subclass to draw any extra graphics
 *          that need to be drawn in front the waveform.
 * \param bounds Bounds for the view to draw into.
 */
- (void)postDraw:(NSRect)bounds;

@end
