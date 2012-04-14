Additions
---

This library has been modified to allow for additional notifications during the syncing progress.

A CHDropboxSyncDelegate protocol has been added with the following methods:

	- (void) syncStarted:(CHDropboxSync*)sync

	//Called for each new task started by CHDropboxSync
	- (void) sync:(CHDropboxSync*)sync remainingTasks:(NSInteger)remaining;

	//Called with the progress for each file down/upload
	- (void) sync:(CHDropboxSync*)sync file:(NSString*)path progress:(CGFloat)progress;

	- (void) syncComplete:(CHDropboxSync*)sync;
	- (void) syncCancelled:(CHDropboxSync*)sync;
	- (void) syncFailed:(CHDropboxSync*)sync;

The original CHDropboxSync class has been expanded with the following methods:

	//Whether to show alerts during the syncing process (defaults to YES)
	@property(assign) BOOL showAlerts;
	
	//A root directory to use (defaults to documents directory)
	@property(retain,nonatomic) NSString *rootDirectory;

	//The original doSync method now no longer 
	// asks the user before beginning the sync
	- (void)doSync;

	//This method is the same as the original library's doSync method
	- (void)doSyncWithAlert;

	//Cancel the process mid-sync
	// (this tells Dropbox to cancel any partially loaded files)
	- (void)cancel;

	//If you are unlinking and linking multiple accounts, you can call
	// this method to remove information about the last sync, as it will be
	// invalid when a new account is linked.
	+ (void)clearLastSyncData;

CHDropboxSync
===

This is a small library designed to make it extremely simple for you to sync your iOS app's 'Documents' folder's files and subfolders to a folder on Dropbox.

Recently Dropbox brought out 'sandbox' api keys, which mean that your app only gets access to a single folder such as Dropbox/Apps/MyAppName. The idea is that this library is to be used with that setup.

Unfortunately the normal Dropbox API only gives primitive methods for getting metadata, uploading and downloading specific files - it doesn't give you the simple 'sync this folder now please' functionality that you're probably used to in the desktop variant. So this library hopes to deal with that.

It's been extracted from an app I've been writing to help keep track of your cars' oil changes. You can see the app on http://apps.splinter.com.au. If you find this library useful, please buy my app, it may also be useful! Unless you live in appsterdam, sorry but I don't have any apps for keeping track of the oil changes on your bicycle ;)

Where to get it
---
[github.com/chrishulbert/CHDropboxSync](https://github.com/chrishulbert/CHDropboxSync)


How to use
---
The idea is that in one view controller of your app, you'll have a 'sync' button. In this controller, you'll want a retain property to keep the syncer object alive. And this controller will be a delegate of the syncer, so it'll know when to dealloc the syncer by nilling out the property.

Eg, in your view controller:

	#import "CHDropboxSync.h"
	@property(retain) CHDropboxSync* syncer;
	@synthesize syncer;
	
	// Do the dropbox sync. This takes care of all user alerts
	- (void)mySyncButtonWasTapped {
		self.syncer = $new(CHDropboxSync);
		self.syncer.delegate = self;
		[self.syncer doSync];
	}
	
	// Delegate callback from the syncer. You can dealloc it now.
	- (void)syncComplete {
	    self.syncer = nil;
	}
	
	- (void)dealloc {
	    self.syncer = nil; // Probably a good idea to do this
	    ...
	    [super dealloc];
	}
	
Note that it is up to you to handle the normal Dropbox linking process.

Sync strategy
---
You may choose to write your own syncer, eg you may wish it to be a background process instead of modal which i've chosen to. Or you may be curious about the two-way sync strategy. It's very simple algorithm, so I wouldn't recommend it for life-critical apps, but it does work well for my app:

* After each synchronisation, make a note of all files/folders in the local documents folder, and their last modified dates, and store it for use in the next sync. This is called the previous sync status.
* When syncing, it compares files and folders locally vs remotely.
* If a file exists in both places but the last modified dates are different, it syncs to use the newer one
* If a file/folder only exists in one place, it uses the previous sync status to determine if the file was deleted or added since the last sync.
* If the item was deleted in one place since the last sync, it will be deleted in the other place.
* If the item was added since the last sync, it will be uploaded/downloaded to the other place.
* The code is intentionally clear (not concise) so you should be able to read it to get how this works.

Dependencies
---

* ConciseKit
* DropboxSDK v1.0+

Other options
---

* There is also jessegrosjean's [DropboxSync](https://github.com/jessegrosjean/DropboxSync), however it does not work with the latest Dropbox API. You should check it out if you're not happy with CHDropboxSync.

License
---
MIT license - no warranties!
