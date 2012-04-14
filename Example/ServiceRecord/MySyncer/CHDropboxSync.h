//
//  CHDropboxSync.h
//
//  Created by Chris Hulbert on 11/12/11.
//  Copyright (c) 2011 Splinter Software. All rights reserved.
//  MIT license - no warranties!

#import <Foundation/Foundation.h>
#import "DropboxSDK.h"

@class CHDropboxSync;

@protocol CHDropboxSyncDelegate <NSObject>

- (void) syncStarted:(CHDropboxSync*)sync;
- (void) sync:(CHDropboxSync*)sync remainingTasks:(NSInteger)remaining;
- (void) sync:(CHDropboxSync*)sync file:(NSString*)path progress:(CGFloat)progress;
- (void) syncComplete:(CHDropboxSync*)sync;
- (void) syncCancelled:(CHDropboxSync*)sync;
- (void) syncFailed:(CHDropboxSync*)sync;

@end

@interface CHDropboxSync : NSObject<DBRestClientDelegate, UIAlertViewDelegate>

@property(assign) id<CHDropboxSyncDelegate> delegate;
@property(assign) BOOL showAlerts;
@property(retain,nonatomic) NSString *rootDirectory;

- (void)doSync;
- (void)doSyncWithAlert;
- (void)cancel;

+ (void)clearLastSyncData;

@end
