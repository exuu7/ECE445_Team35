
/*citations: https://www.youtube.com/watch?v=0edSNTjAaRg 
  https://stackoverflow.com/questions/77080095/problem-with-instanciation-on-flutterblueplus
  https://www.youtube.com/watch?v=an4NbIjcXYI
*/
import 'dart:async';
import 'dart:io';
import 'package:flutter/foundation.dart';
import 'package:get/get.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:permission_handler/permission_handler.dart';

class bluetooth_cont extends GetxController{

  static Future<void> btPermission() async{
    if (await FlutterBluePlus.isSupported == false) {
    print("Bluetooth not supported by this device");
    return;
  }

// handle bluetooth on & off
// note: for iOS the initial state is typically BluetoothAdapterState.unknown
// note: if you have permissions issues you will get stuck at BluetoothAdapterState.unauthorized
  var subscription = FlutterBluePlus.adapterState.listen((BluetoothAdapterState state) {
      print(state);
      if (state == BluetoothAdapterState.on) {
        print("got the state");
          // usually start scanning, connecting, etc
      } else {
          // show an error to the user, etc
      }
  });

  // turn on bluetooth ourself if we can
  // for iOS, the user controls bluetooth enable/disable
  if (!kIsWeb && Platform.isAndroid) {
      await FlutterBluePlus.turnOn();
  }

  // cancel to prevent duplicate listeners
  // subscription.cancel();
  print("getting permission complete");

  }
  static Future<void> connectDevice(BluetoothDevice device) async{
    print("within connect: $device");
    
   // listen for disconnection
    var subscription = device.connectionState.listen((BluetoothConnectionState state) async {
        if (state == BluetoothConnectionState.disconnected) {
            // 1. typically, start a periodic timer that tries to 
            //    reconnect, or just call connect() again right now
            // 2. you must always re-discover services after disconnection!
            print("${device.disconnectReason?.code} ${device.disconnectReason?.description}");
        }
    });

    // cleanup: cancel subscription when disconnected
    //   - [delayed] This option is only meant for `connectionState` subscriptions.  
    //     When `true`, we cancel after a small delay. This ensures the `connectionState` 
    //     listener receives the `disconnected` event.
    //   - [next] if true, the the stream will be canceled only on the *next* disconnection,
    //     not the current disconnection. This is useful if you setup your subscriptions
    //     before you connect.
    device.cancelWhenDisconnected(subscription, delayed:true, next:true);

    // Connect to the device
    await device.connect(timeout: Duration(seconds: 15));
    print("connected to device");

    // Disconnect from device
    // await device.disconnect();

    // cancel to prevent duplicate listeners
    // subscription.cancel(); 
  }
}
