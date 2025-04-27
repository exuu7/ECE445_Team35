
/*citations: https://www.youtube.com/watch?v=0edSNTjAaRg 
  https://stackoverflow.com/questions/77080095/problem-with-instanciation-on-flutterblueplus
  https://www.youtube.com/watch?v=an4NbIjcXYI
*/
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
  subscription.cancel();

}


Future scanDevices() async{

  // listen to scan results
// Note: `onScanResults` clears the results between scans. You should use
//  `scanResults` if you want the current scan results *or* the results from the previous scan.
var subscription = FlutterBluePlus.onScanResults.listen((results) {
    if (results.isNotEmpty) {
        ScanResult r = results.last; // the most recently found device
        print('${r.device.remoteId}: "${r.advertisementData.advName}" found!');
    }
  },
  onError: (e) => print(e),
);

// cleanup: cancel subscription when scanning stops
FlutterBluePlus.cancelWhenScanComplete(subscription);

// Wait for Bluetooth enabled & permission granted
// In your real app you should use `FlutterBluePlus.adapterState.listen` to handle all states
await FlutterBluePlus.adapterState.where((val) => val == BluetoothAdapterState.on).first;

// Start scanning w/ timeout
// Optional: use `stopScan()` as an alternative to timeout
// await FlutterBluePlus.startScan(
//   withServices:[Guid("180D")], // match any of the specified services
//   withNames:["Bluno"], // *or* any of the specified names
//   timeout: Duration(seconds:15));
FlutterBluePlus.startScan(timeout: Duration(seconds:15));
FlutterBluePlus.stopScan();
// wait for scanning to stop
await FlutterBluePlus.isScanning.where((val) => val == false).first;
}




  // FlutterBluePlus scanner = FlutterBluePlus.instance;
  // no longer used. Just use FlutterBluePlus.isScanning now according to the stackOverflow
  // Future scanDevices() async{
  //   if (await Permission.bluetooth.isRestricted){
  //     if (await Permission.bluetoothScan.request().isGranted){
  //       if (await Permission.bluetoothConnect.request().isGranted){

  //         FlutterBluePlus.startScan(timeout: const Duration(seconds: 5));

  //         FlutterBluePlus.stopScan();

  //       }
  //     }

  //   }
  // }


  // Stream<List<ScanResult>> get scanResult => FlutterBluePlus.scanResults;
}