// reference: https://pub.dev/packages/flutter_local_notifications#-ios-setup
import 'package:flutter_local_notifications/flutter_local_notifications.dart'; 

class local_notifs{
 
  static final FlutterLocalNotificationsPlugin flutterLocalNotificationsPlugin =
    FlutterLocalNotificationsPlugin();
  
  static Future<void> onDidReceiveNotification(NotificationResponse notificationResponse) async{}
  // initialization
  static Future<void> init() async {
  // initialise the plugin. app_icon needs to be a added as a drawable resource to the Android head project
    const AndroidInitializationSettings initializationSettingsAndroid =
        AndroidInitializationSettings('@mipmap/ic_launcher');
    final DarwinInitializationSettings initializationSettingsDarwin =
        DarwinInitializationSettings(
          
          // requestSoundPermission: false,
          // requestBadgePermission: false,
          // requestAlertPermission: false,
          notificationCategories: [
              DarwinNotificationCategory(
                'demoCategory',
                actions: <DarwinNotificationAction>[
                    DarwinNotificationAction.plain('id_1', 'Action 1'),
                    DarwinNotificationAction.plain('id_2','Action 2',
                    options: <DarwinNotificationActionOption>{
                        DarwinNotificationActionOption.destructive,
                    },
                    ),
                    DarwinNotificationAction.plain('id_3','Action 3',
                    options: <DarwinNotificationActionOption>{
                        DarwinNotificationActionOption.foreground,
                    },
                    ),
                ],
                options: <DarwinNotificationCategoryOption>{
                    DarwinNotificationCategoryOption.hiddenPreviewShowTitle,
                },
            )
        ],

        );
    
    final LinuxInitializationSettings initializationSettingsLinux =
        LinuxInitializationSettings(
            defaultActionName: 'Open notification');
        @pragma('vm:entry-point')
        void notificationTapBackground(NotificationResponse notificationResponse) {
          // handle action
        }
    // final WindowsInitializationSettings initializationSettingsWindows =
    //     WindowsInitializationSettings(
    //         appName: 'Flutter Local Notifications Example',
    //         appUserModelId: 'Com.Dexterous.FlutterLocalNotificationsExample',
    //         // Search online for GUID generators to make your own
    //         guid: 'd49b0314-ee7a-4626-bf79-97cdb8a991bb');
    final InitializationSettings initializationSettings = InitializationSettings(
        android: initializationSettingsAndroid,
        iOS: initializationSettingsDarwin,
        macOS: initializationSettingsDarwin,
        linux: initializationSettingsLinux);
        // windows: initializationSettingsWindows);
    await flutterLocalNotificationsPlugin.initialize(initializationSettings,
        onDidReceiveNotificationResponse:
        onDidReceiveNotification);
    await flutterLocalNotificationsPlugin.resolvePlatformSpecificImplementation<AndroidFlutterLocalNotificationsPlugin>()?.requestNotificationsPermission();
    await flutterLocalNotificationsPlugin.initialize(
    initializationSettings,
    onDidReceiveNotificationResponse: (NotificationResponse notificationResponse) async {
        // ...
    },
    onDidReceiveBackgroundNotificationResponse: notificationTapBackground,
    );
  }

  // simple notif
  static Future giveNotif ({
     required String title,
     required String body,
     required String payload,
  }) async {
    const AndroidNotificationDetails androidNotificationDetails =
    AndroidNotificationDetails('CarGuard_id', 'your channel name',
        channelDescription: 'your channel description',
        importance: Importance.max,
        priority: Priority.high,
        ticker: 'ticker');
  const NotificationDetails notificationDetails =
      NotificationDetails(android: androidNotificationDetails);
  // so notification are not overwritter in the banner
  final id = DateTime.now().millisecondsSinceEpoch ~/ 1000;
  await flutterLocalNotificationsPlugin.show(
      id, title, body, notificationDetails,
      payload: payload);
  print('sent!');
  }
}

// class pushNotifs {
//   final notificationsPlugin = FlutterLocalNotificationsPlugin();

//   bool _isInitialized = false;

//   bool get isInitialized => _isInitialized;

//   Future<void> initNotification() async{
//     if (_isInitialized) return;

//     //android
//     const initAndroid = AndroidInitializationSettings('@mipmap/ic_launcher');

//     //iOS
//     const initiOS = DarwinInitializationSettings(
//       requestAlertPermission: true,
//       requestBadgePermission: true,
//       requestSoundPermission: true,
//     );


//     //settings
//     const initSetting = InitializationSettings(
//         android: initAndroid,
//         iOS: initiOS,
//     );

//     await notificationsPlugin.initialize(initSetting);


//   }

//   NotificationDetails notificationDetails(){
//     return const NotificationDetails(
//     android: AndroidNotificationDetails('CarGuard_id', 'CarGuard',
//       channelDescription: 'Notifications from CarGuard', importance: Importance.max,
//       priority: Priority.high, playSound: true),
//     iOS: DarwinNotificationDetails(),

//     );
//   }

//   Future<void> showNotification({
//     int id = 0,
//     String? title,
//     String? body,
//   }) async{
//     return notificationsPlugin.show(id, title, body, const NotificationDetails(),);
      
//     }
// }
