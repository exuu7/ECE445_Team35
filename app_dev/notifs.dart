// reference: https://www.youtube.com/watch?v=uKz8tWbMuUw&t=65s
import 'package:flutter_local_notifications/flutter_local_notifications.dart';

class Notifs {
  final notificationsPlugin = FlutterLocalNotificationsPlugin();
  bool _isInit = false;
  bool get isInit => _isInit;

  // initialize
  Future<void> initNotification() async {
    if(_isInit) return;
    const initSettingsAndroid = AndroidInitializationSettings('@mipmap/ic_launcher');

    const initSettingsIOS = DarwinInitializationSettings(
      requestAlertPermission: true,
      requestBadgePermission: true,
      requestSoundPermission: true,
    );

    const initSettings = InitializationSettings(
      android: initSettingsAndroid,
      iOS: initSettingsIOS,
    );

    await notificationsPlugin.initialize(initSettings);
  }

  // notification details
  NotificationDetails notifDetails() {
    return const NotificationDetails(
      android: AndroidNotificationDetails(
        'CarGuard_id',
        'CarGuard',
        channelDescription: 'CarGuard Notifications',
        importance: Importance.max,
        priority: Priority.high,
      ),
      iOS: DarwinNotificationDetails(),
    );
  }

  // show notifs
  Future<void> showNotifs({
    int id = 0,
    String? title,
    String? body,
  }) async {
    return notificationsPlugin.show(id, title, body, const NotificationDetails(),);
  }
}
