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

#import "NxARekordBuddyWaveformView.h"
#import "Track Collection/Tags/Marker Properties/NxACueMarkerProperty.h"
#import "NxAWaveformViewMarker.h"

#pragma mark Private Interface

@interface NxARekordBuddyWaveformView ()

/*! Array of markers. */
@property (strong, nonatomic) NSMutableArray *p_markers;

/*! Array of grid markers. */
@property (strong, nonatomic) NSMutableArray *p_gridMarkers;

@end

#pragma mark - Implementation

@implementation NxARekordBuddyWaveformView

#pragma mark Accessors

- (NSMutableArray *)p_markers
{
    if (!_p_markers) {
        _p_markers = [NSMutableArray arrayWithCapacity:8];
    }

    return _p_markers;
}

- (NSMutableArray *)p_gridMarkers
{
    if (!_p_gridMarkers) {
        _p_gridMarkers = [NSMutableArray arrayWithCapacity:8];
    }

    return _p_gridMarkers;
}

#pragma mark Instance Methods

- (void)resetMarkers
{
    [self.p_markers removeAllObjects];
    [self.p_gridMarkers removeAllObjects];
}

- (void)addMarker:(NxAWaveformViewMarker *)marker
{
    if (marker.type == NxAWaveformViewGridMarkerType) {
        [self.p_gridMarkers addObject:marker];

        NSSortDescriptor *sortDesc = [[NSSortDescriptor alloc] initWithKey:@"startPositionInSeconds"
                                                                 ascending:YES
                                                                  selector:@selector(compare:)];
        [self.p_gridMarkers sortUsingDescriptors:@[ sortDesc ]];
    }
    else {
        [self.p_markers addObject:marker];
    }
}

- (void)preDraw:(NSRect)bounds
{
    // -- Draw the beat grid.
    for (NSInteger index = 0; index < self.p_gridMarkers.count; ++index) {
        NxAWaveformViewMarker *marker = self.p_gridMarkers[index];

        if (marker.startPositionInSeconds > self.endDisplayedTime) {
            break;
        }

        double endGrid;
        if ((index + 1) >= self.p_gridMarkers.count) {
            endGrid = self.endDisplayedTime;
        }
        else {
            NxAWaveformViewMarker *nextMarker = self.p_gridMarkers[index + 1];
            endGrid = nextMarker.startPositionInSeconds;
        }

        if (endGrid < self.startDisplayedTime) {
            continue;
        }

        double gridStep = 60.0 / marker.beatsPerMinute;

        NSInteger pixelSpacing = [self positionForTime:self.startDisplayedTime + gridStep];
        if ((pixelSpacing != -2) && (pixelSpacing < 10)) {
            continue;
        }

        CGFloat alpha = ((pixelSpacing == -2) || (pixelSpacing > 20)) ? 1.0 : ((CGFloat)pixelSpacing - 10.0) / 10.0;

        double gridTick = marker.startPositionInSeconds + gridStep;
        NSUInteger gridMarkIndex = 1;
        while (gridTick < endGrid) {
            if (gridTick >= self.startDisplayedTime) {
                NSInteger xPos = [self positionForTime:gridTick];

                if (xPos >= 0) {
                    NSBezierPath *beatGridPath = [[NSBezierPath alloc] init];

                    beatGridPath.lineWidth = 2.0;

                    [beatGridPath moveToPoint:NSMakePoint(xPos, 0)];
                    [beatGridPath lineToPoint:NSMakePoint(xPos, NSHeight(bounds))];

                    NSColor *beatGridColor = (gridMarkIndex % 4) ? [NSColor colorWithSRGBRed:0.5 green:0.5 blue:0.5 alpha:alpha] :
                                                                   [NSColor colorWithSRGBRed:1.0 green:0.0 blue:0.0 alpha:alpha];
                    [beatGridColor setStroke];
                    [beatGridPath stroke];
                }
            }

            gridTick += gridStep;
            ++gridMarkIndex;
        }
    }
}

- (void)postDraw:(NSRect)bounds
{
    NSBezierPath *cueMarkersPath = [[NSBezierPath alloc] init];
    NSBezierPath *loopMarkersPath = [[NSBezierPath alloc] init];

    NSColor *loopsColor = [NSColor colorWithSRGBRed:0.0 green:1.0 blue:0.0 alpha:0.5];
    [loopsColor setFill];

    // -- Draw the cue points and loops.
    for (NxAWaveformViewMarker *marker in self.p_markers) {
        NSInteger xPos = [self positionForTime:marker.startPositionInSeconds];

        if (marker.type == NxAWaveformViewCueMarkerType) {
            if (xPos >= 0) {
                [cueMarkersPath moveToPoint:NSMakePoint(xPos, 0)];
                [cueMarkersPath lineToPoint:NSMakePoint(xPos, NSHeight(bounds))];
            }
        }
        else if (marker.type == NxAWaveformViewLoopMarkerType) {
            if (xPos == -2) {
                continue;
            }
            else if (xPos == -1) {
                xPos = 0;
            }
            else {
                [loopMarkersPath moveToPoint:NSMakePoint(xPos, 0)];
                [loopMarkersPath lineToPoint:NSMakePoint(xPos, NSHeight(bounds))];
            }

            NSInteger xEndPos = [self positionForTime:(marker.startPositionInSeconds + marker.lengthInSeconds)];

            if (xEndPos == -1) {
                continue;
            }
            else if (xEndPos == -2) {
                xEndPos = NSMaxX(bounds);
            }
            else {
                [loopMarkersPath moveToPoint:NSMakePoint(xEndPos, 0)];
                [loopMarkersPath lineToPoint:NSMakePoint(xEndPos, NSHeight(bounds))];
            }

            [NSBezierPath fillRect:NSMakeRect(xPos,
                                              0.0,
                                              xEndPos - xPos,
                                              NSHeight(bounds))];
        }
    }

    loopsColor = [NSColor colorWithSRGBRed:0.0 green:1.0 blue:0.0 alpha:1.0];
    [loopsColor setStroke];
    loopMarkersPath.lineWidth = 2.0;
    [loopMarkersPath stroke];

    NSColor *cuePointsColor = [NSColor colorWithSRGBRed:0.2 green:0.2 blue:0.8 alpha:1.0];
    [cuePointsColor setStroke];
    cueMarkersPath.lineWidth = 2.0;
    [cueMarkersPath stroke];

    NSBezierPath *gridMarkersPath = [[NSBezierPath alloc] init];

    // -- Draw the grid markers.
    for (NxAWaveformViewMarker *marker in self.p_gridMarkers) {
        double position = marker.startPositionInSeconds;
        NSInteger xPos = [self positionForTime:position];

        if (xPos >= 0) {
            [gridMarkersPath moveToPoint:NSMakePoint(xPos, 0)];
            [gridMarkersPath lineToPoint:NSMakePoint(xPos, NSHeight(bounds))];
        }
    }

    NSColor *gridMarkersColor = [NSColor colorWithSRGBRed:1.0 green:1.0 blue:1.0 alpha:1.0];
    [gridMarkersColor setStroke];
    gridMarkersPath.lineWidth = 2.0;
    [gridMarkersPath stroke];
}

#pragma mark Overidden NxAWaveformView methods

- (void)displaySamples:(NSData *)samples
                length:(double)length
{
    [self offsetWaveDisplayBy:0 right:0 top:10 bottom:10];
    [super displaySamples:samples length:length];
}

#pragma mark Overidden NSView Methods

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (!self) {
        return nil;
    }

    self.waveformColor = [NSColor colorWithSRGBRed:0.4 green:0.4 blue:0.6 alpha:1.0];
    self.clearColor = [NSColor colorWithSRGBRed:0.1 green:0.1 blue:0.3 alpha:1.0];

    return self;
}

@end
