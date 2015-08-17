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

#import "NxAWaveformView.h"

#pragma mark Private Interface

@interface NxAWaveformView ()

@property (assign, nonatomic) double startDisplayedTime;

@property (assign, nonatomic) double endDisplayedTime;

/*! Drawing offset from the left side of the view. */
@property (assign, nonatomic) NSUInteger p_left;

/*! Drawing offset from the right side of the view. */
@property (assign, nonatomic) NSUInteger p_right;

/*! Drawing offset from the top of the view. */
@property (assign, nonatomic) NSUInteger p_top;

/*! Drawing offset from the bottom of the view. */
@property (assign, nonatomic) NSUInteger p_bottom;

/*! Drawing boundaries for the waveform view. */
@property (assign, nonatomic) NSRect p_waveDisplayBounds;

/*! Sample data to display in the view as floats. */
@property (strong, nonatomic) NSData *p_samples;

/*! Length of the audio data in seconds. */
@property (assign, nonatomic) double p_length;

/*! First sample displayed by the view. */
@property (assign, nonatomic) double p_displaySampleStart;

/*! Last sample displayed by the view. */
@property (assign, nonatomic) double p_displaySampleEnd;

/*! Minimum value displayable as a first sample. */
@property (assign, nonatomic) double p_displaySampleStartMin;

/*! Maximum value displayable as a last sample. */
@property (assign, nonatomic) double p_displaySampleEndMax;

/*! Index of the sample the user clicked on in the view. */
@property (assign, nonatomic) double p_clickedSampleIndex;

/*! Indicate if the view is currently being dragged by the mouse in the X direction. */
@property (assign, nonatomic) BOOL p_isDraggedInX;

/*! Indicate if the view is currently being dragged by the mouse in the Y direction. */
@property (assign, nonatomic) BOOL p_isDraggedInY;

@end

#pragma mark - Implementation

@implementation NxAWaveformView

#pragma mark Instance Methods

/*!
 * Clear the entire view.
 * \param context Core Graphics context to render the waveform into.
 */
- (void)p_clearView:(CGContextRef)context
{
    [self.clearColor setFill];
    NSRectFill(self.bounds);
}

/*!
 * Draw the waveform.
 * \param context Core Graphics context to render the waveform into.
 */
- (void)p_drawWaveform:(CGContextRef)context
{
    NxAAssertTrue(self.p_displaySampleStart >= self.p_displaySampleStartMin);
    NxAAssertTrue(self.p_displaySampleEnd <= self.p_displaySampleEndMax);

    if (self.p_displaySampleStart > self.p_displaySampleEnd) {
        ALog(@"Illegal sample display range.");
        return;
    }

    double maxSampleIndex = (self.p_samples.length / sizeof(float)) - 1;
    double samplesInputCount = self.p_displaySampleEnd - self.p_displaySampleStart;
    double pointsOutputCount = NSWidth(self.p_waveDisplayBounds);
	double samplesPerPoint = samplesInputCount / pointsOutputCount;

    const float *srcSamples = self.p_samples.bytes;

    double ratio = samplesInputCount / pointsOutputCount;
    ratio -= 10.0;

    if (ratio < 0.0) {
        ratio = 0.0;
    }
    else if (ratio > 40.0) {
        ratio = 40.0;
    }

    ratio /= 40.0;

    CGFloat redHighlight = self.waveformColor.redComponent + (ratio * (1.0 - self.waveformColor.redComponent) / 3.0);
    CGFloat greenHighlight = self.waveformColor.greenComponent + (ratio * (1.0 - self.waveformColor.greenComponent) / 3.0);
    CGFloat blueHighlight = self.waveformColor.blueComponent + (ratio * (1.0 - self.waveformColor.blueComponent) / 3.0);
    CGFloat colors[32] = {
        self.clearColor.redComponent, self.clearColor.greenComponent, self.clearColor.blueComponent, 0.5,
        self.waveformColor.redComponent, self.waveformColor.greenComponent, self.waveformColor.blueComponent, 1.0,
        self.waveformColor.redComponent, self.waveformColor.greenComponent, self.waveformColor.blueComponent, 1.0,
        redHighlight, greenHighlight, blueHighlight, 1.0,
        redHighlight, greenHighlight, blueHighlight, 1.0,
        self.waveformColor.redComponent, self.waveformColor.greenComponent, self.waveformColor.blueComponent, 1.0,
        self.waveformColor.redComponent, self.waveformColor.greenComponent, self.waveformColor.blueComponent, 1.0,
        self.clearColor.redComponent, self.clearColor.greenComponent, self.clearColor.blueComponent, 0.5,
    };

    CGFloat locations[8] = {
        0.0,                // -- 0: Background color position
        0.01 * ratio,       // -- 1: Waveform color position
        0.2 * ratio,        // -- 2: Waveform color position
        0.0,                // -- 3: Hightlight color position (placed at average Max value)
        0.0,                // -- 4: Hightlight color position (placed
        0.0,                // -- 5: Waveform color position
        0.0,                // -- 6: Waveform color position
        1.0                 // -- 7: Background color position
    };
    locations[5] = 1.0 - locations[2];
    locations[6] = 1.0 - locations[1];

    CGColorSpaceRef baseSpace = CGColorSpaceCreateDeviceRGB();

    float lastValue = 0.0;

    for (NSUInteger pointIndex = 0; pointIndex < pointsOutputCount; ++pointIndex) {
        double sampleForPointIndex = self.p_displaySampleStart + (pointIndex * samplesPerPoint);

		float min = lastValue;
		float max = lastValue;
        float avgPositive = 0.0;
        float avgNegative = 0.0;
        NSInteger avgPositiveCount = 0;
        NSInteger avgNegativeCount = 0;

        for (NSInteger count = 0; count < samplesPerPoint; ++count) {
            NSUInteger lookupIndex = (NSUInteger)(sampleForPointIndex + (double)count);

            if ((lookupIndex < 0.0) || (lookupIndex > maxSampleIndex)) {
                min = max = 0.0;

                lastValue = 0.0;
            }
            else if(lookupIndex <= self.p_displaySampleEnd) {
                float value = srcSamples[lookupIndex];

                if (value >= 0.0) {
                    avgPositive += value;
                    ++avgPositiveCount;
                }
                else if (value < 0.0) {
                    avgNegative += value;
                    ++avgNegativeCount;
                }

                if(value > max) {
                    max = value;
                }

                if(value < min) {
                    min = value;
                }

                lastValue = value;
			}
            else {
                ALog(@"Out of bounds data access.");
            }
        }

        min = (min + 1.0) / 2.0;
        max = (max + 1.0) / 2.0;

        locations[3] = locations[2];
        locations[4] = locations[5];

        if (min != max) {
            avgPositive = ((avgPositive / (float)avgPositiveCount) + 1.0) / 2.0;
            avgNegative = ((avgNegative / (float)avgNegativeCount) + 1.0) / 2.0;

            float lengthRatio = (locations[5] - locations[2]) / (max - min);

            if (!avgPositiveCount || !avgNegativeCount) {
                if (!avgPositiveCount) {
                    avgPositive = avgNegative;
                }

                locations[4] = locations[5] - ((max - avgPositive) * lengthRatio);
                locations[3] = locations[4];
            }
            else {
                locations[3] = locations[2] + ((avgNegative - min) * lengthRatio);
                locations[4] = locations[5] - ((max - avgPositive) * lengthRatio);
            }

#if DEBUG
            for (NSInteger i = 1; i < 8; ++i) {
                NxAAssertTrue(locations[i-1] <= locations[i]);
            }
#endif
        }

        CGFloat xPos = pointIndex + NSMinX(self.p_waveDisplayBounds);

        CGContextSaveGState(context);

        CGGradientRef gradient = CGGradientCreateWithColorComponents(baseSpace, colors, locations, 8);

        CGFloat height = NSHeight(self.p_waveDisplayBounds);
        min = floorf(min * height) + NSMinY(self.p_waveDisplayBounds);
        max = floorf((max * height) + 1.0) + NSMinY(self.p_waveDisplayBounds);

        CGContextAddRect(context, CGRectMake(xPos, min, 1.0, max - min));
        CGContextClip(context);

        CGContextDrawLinearGradient(context,
                                    gradient,
                                    CGPointMake(xPos, min),
                                    CGPointMake(xPos, max),
                                    0);

        CGGradientRelease(gradient);

        CGContextRestoreGState(context);
    }

    CGColorSpaceRelease(baseSpace);
}

/*!
 * Get the time for a given sample.
 * \param sampleIndex Index of the sample.
 * \return The time of the sample in seconds.
 */
- (double)p_timeForSample:(double)sampleIndex
{
    return (sampleIndex * self.p_length * sizeof(float)) / (double)self.p_samples.length;
}

/*!
 * Get the index of a sample.
 * \param xPos Horizontal position of the sample in view space.
 * \return The index of the sample in the waveform data.
 */
- (NSUInteger)p_sampleAtPosition:(NSUInteger)xPos
{
    NxAAssertTrue(xPos < NSWidth(self.p_waveDisplayBounds));

    double positionRatio = (xPos - NSMinX(self.p_waveDisplayBounds)) / NSWidth(self.p_waveDisplayBounds);

    double totalSamples = self.p_displaySampleEnd - self.p_displaySampleStart;
    double sampleIndex = (totalSamples * positionRatio) + self.p_displaySampleStart;

    if (sampleIndex < 0) {
        sampleIndex  = 0;
    }
    else  {
        double maxSampleIndex = (self.p_samples.length / sizeof(float)) - 1;
        if (sampleIndex > maxSampleIndex) {
            sampleIndex = maxSampleIndex;
        }
    }

    return (NSUInteger)sampleIndex;
}

- (void)offsetWaveDisplayBy:(NSUInteger)left
                   right:(NSUInteger)right
                     top:(NSUInteger)top
                  bottom:(NSUInteger)bottom
{
    self.p_left = left;
    self.p_right = right;
    self.p_top = top;
    self.p_bottom = bottom;
}

- (void)displaySamples:(NSData *)samples
                length:(double)length
{
    NxAAssertNotNil(samples);
    NxAAssertTrue(length > 0.0);

    self.p_samples = samples;
    self.p_length = length;

    double maxSampleIndex = (self.p_samples.length / sizeof(float)) - 1;

    self.p_displaySampleStart = 0.0;
    self.p_displaySampleEnd = maxSampleIndex;

    self.p_displaySampleStartMin = -maxSampleIndex / 100.0;
    self.p_displaySampleEndMax = self.p_displaySampleEnd + (maxSampleIndex / 100.0);

    self.startDisplayedTime = 0.0;
    self.endDisplayedTime = [self p_timeForSample:self.p_displaySampleEnd];

    self.p_isDraggedInX = NO;
    self.p_isDraggedInY = NO;
    
    [self setNeedsDisplay:YES];
}

- (void)resetSamplesDisplayed
{
    self.p_samples = nil;
    self.p_length = 0.0;

    self.p_displaySampleStart = 0.0;
    self.p_displaySampleEnd = 0.0;

    self.p_displaySampleStartMin = 0.0;
    self.p_displaySampleEndMax = 0.0;

    self.startDisplayedTime = 0.0;
    self.endDisplayedTime = 0.0;

    self.p_isDraggedInX = NO;
    self.p_isDraggedInY = NO;
    
    [self setNeedsDisplay:YES];
}

- (NSInteger)positionForTime:(double)time
{
    double startTime = self.startDisplayedTime;
    if (startTime > time) {
        return -1;
    }

    double endTime = self.endDisplayedTime;
    if (endTime < time) {
        return -2;
    }

    double timeRatio = (time - startTime) / (endTime - startTime);

    NSInteger position = NSMinX(self.p_waveDisplayBounds) + (timeRatio * NSWidth(self.p_waveDisplayBounds));
    return position;
}

- (void)preDraw:(NSRect)bounds
{
    // -- By default this does nothing. A subclass can overide this.
}

- (void)postDraw:(NSRect)bounds
{
    // -- By default this does nothing. A subclass can overide this.
}

#pragma mark Overidden NSView Methods

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (!self) {
        return nil;
    }

    self.waveformColor = [NSColor colorWithSRGBRed:0.4 green:0.4 blue:0.4 alpha:1.0];
    self.clearColor = [NSColor colorWithSRGBRed:0.0 green:0.0 blue:0.0 alpha:1.0];

    return self;
}

- (BOOL)isOpaque
{
	return NO;
}

- (BOOL)acceptsFirstMouse:(NSEvent *)theEvent
{
	return YES;
}

- (void)mouseDragged:(NSEvent *)theEvent
{
    double displayedSampleCount = self.p_displaySampleEnd - self.p_displaySampleStart;

    if (!self.p_isDraggedInY) {
        if (self.p_isDraggedInX) {
            if (theEvent.deltaX == 0.0) {
                self.p_isDraggedInX = NO;
            }
        }
        else if (theEvent.deltaX != 0.0) {
            self.p_isDraggedInX = YES;
        }

        if (self.p_isDraggedInX) {
            double xOffset = theEvent.deltaX * displayedSampleCount / NSWidth(self.p_waveDisplayBounds);
            self.p_displaySampleStart -= xOffset;
            self.p_displaySampleEnd -= xOffset;

            if (self.p_displaySampleStart < self.p_displaySampleStartMin) {
                self.p_displaySampleStart = self.p_displaySampleStartMin;
                self.p_displaySampleEnd = self.p_displaySampleStart + displayedSampleCount;
            }

            if (self.p_displaySampleEnd > self.p_displaySampleEndMax) {
                self.p_displaySampleEnd = self.p_displaySampleEndMax;
                self.p_displaySampleStart = self.p_displaySampleEnd - displayedSampleCount;
            }

            [self setNeedsDisplay:YES];
        }
    }

    if (!self.p_isDraggedInX) {
        if (self.p_isDraggedInY) {
            if (theEvent.deltaY == 0.0) {
                self.p_isDraggedInY = NO;
            }
        }
        else if (theEvent.deltaY != 0.0) {
            self.p_isDraggedInY = YES;
        }

        if (self.p_isDraggedInY) {
            double scale = 1.0 - (float)(theEvent.deltaY) / 100.0;
            if (scale > 2.0) {
                scale = 2.0;
            }
            else if (scale < 0.5) {
                scale = 0.5;
            }

            self.p_displaySampleStart = ((self.p_displaySampleStart - self.p_clickedSampleIndex) * scale) + self.p_clickedSampleIndex;
            self.p_displaySampleEnd = ((self.p_displaySampleEnd - self.p_clickedSampleIndex) * scale) + self.p_clickedSampleIndex;

            if (self.p_displaySampleStart < self.p_displaySampleStartMin) {
                self.p_displaySampleStart = self.p_displaySampleStartMin;
            }

            if (self.p_displaySampleEnd > self.p_displaySampleEndMax) {
                self.p_displaySampleEnd = self.p_displaySampleEndMax;
            }

            [self setNeedsDisplay:YES];
        }
    }

    self.startDisplayedTime = [self p_timeForSample:self.p_displaySampleStart];
    self.endDisplayedTime = [self p_timeForSample:self.p_displaySampleEnd];
}

- (void)mouseUp:(NSEvent *)theEvent
{
    // -- Reset the sample clicked on by the user.
    self.p_clickedSampleIndex = 0.0;
    
    // -- Display the cursor and allow it to move again.
    CGAssociateMouseAndMouseCursorPosition(true);
    CGDisplayShowCursor(kCGDirectMainDisplay);
}

- (void)mouseDown:(NSEvent *)theEvent
{
    // -- Make a note of which sample the user clicked on.
    NSPoint clicked_point = [self convertPoint:theEvent.locationInWindow fromView:nil];
    self.p_clickedSampleIndex = (double)[self p_sampleAtPosition:clicked_point.x];

    // -- Hide the cursor and make it stop moving.
    CGDisplayHideCursor(kCGDirectMainDisplay);
    CGAssociateMouseAndMouseCursorPosition(false);
}

- (void)drawRect:(NSRect)dirtyRect
{
	[super drawRect:dirtyRect];

    NSGraphicsContext *nsGraphicsContext = [NSGraphicsContext currentContext];
    CGContextRef context = (CGContextRef)[nsGraphicsContext graphicsPort];

    self.p_waveDisplayBounds = NSMakeRect(NSMinX(self.bounds) + self.p_left,
                                          NSMinY(self.bounds) + self.p_bottom,
                                          NSWidth(self.bounds) - self.p_left - self.p_right,
                                          NSHeight(self.bounds) - self.p_top - self.p_bottom);

    [self p_clearView:context];

    [self preDraw:self.bounds];

    if (self.p_samples) {
        [self p_drawWaveform:context];
    }

    [self postDraw:self.bounds];
}

@end
