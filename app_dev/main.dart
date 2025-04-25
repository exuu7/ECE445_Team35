import 'package:flutter/material.dart';
import 'package:test_notifs/notification.dart';

import 'package:url_launcher/url_launcher.dart';
import 'package:url_launcher/url_launcher_string.dart';

import 'package:flutter_local_notifications/flutter_local_notifications.dart'; 


/* citations: https://www.youtube.com/watch?v=rT7-p5t_35c
https://www.youtube.com/watch?v=--PQXg_mx9I&t=802s 
https://www.youtube.com/watch?v=26TTYlwc6FM 

Flutter push notifications documentation and set up code: https://pub.dev/packages/flutter_local_notifications
*/
void main() {
  WidgetsFlutterBinding.ensureInitialized();
  // pushNotifs().initNotification();
  local_notifs.init();
  runApp(const MyApp());
}


class MyApp extends StatelessWidget {
  const MyApp({super.key});
  
  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Flutter Demo',
      theme: ThemeData(
        // This is the theme of your application.
        //
        // TRY THIS: Try running your application with "flutter run". You'll see
        // the application has a purple toolbar. Then, without quitting the app,
        // try changing the seedColor in the colorScheme below to Colors.green
        // and then invoke "hot reload" (save your changes or press the "hot
        // reload" button in a Flutter-supported IDE, or press "r" if you used
        // the command line to start the app).
        //
        // Notice that the counter didn't reset back to zero; the application
        // state is not lost during the reload. To reset the state, use hot
        // restart instead.
        //
        // This works for code too, not just values: Most code changes can be
        // tested with just a hot reload.
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
      ),
      home: MyHamburgerClass(),
    );
  }
}

class MyHamburgerClass extends StatefulWidget {
  @override
  State<MyHamburgerClass> createState() => _MyHamburgerClassState();
}

class _MyHamburgerClassState extends State<MyHamburgerClass> {

  var selectedIndex = 0;
  
  @override
  Widget build(BuildContext context) {

    Widget page;
    switch (selectedIndex) {
      case 0:
        page = HomePage();
        break;
      case 1:
        page = InfoPage();
        break;
      default:
        throw UnimplementedError('no widget for $selectedIndex');
    }
    
    // icons: https://fonts.google.com/icons?selected=Material+Icons:settings:&icon.size=24&icon.color=%23e3e3e3 
    return LayoutBuilder(
      builder: (context, constraints) {
        return Scaffold(
          body: Row(
            children: [
              SafeArea(
                child: NavigationRail(
                  extended: constraints.maxWidth >= 600,
                  destinations: [
                    NavigationRailDestination(
                      icon: Icon(Icons.analytics),
                      label: Text('Status'),
                    ),
                    NavigationRailDestination(
                      icon: Icon(Icons.info),
                      label: Text('Info'),
                    ),
                  ],
                  selectedIndex: selectedIndex,
                  onDestinationSelected: (value) {
                    setState(() {
                      selectedIndex = value;
                    });
                  },
                ),
              ),
              Expanded(
                child: Container(
                  color: Theme.of(context).colorScheme.primaryContainer,
                  child: page,
                ),
              ),
            ],
          ),
        );
      }
    );
  }
}

// class MyHomePage extends StatelessWidget {
//   const MyHomePage({super.key, required this.title});

//   // This widget is the home page of your application. It is stateful, meaning
//   // that it has a State object (defined below) that contains fields that affect
//   // how it looks.

//   // This class is the configuration for the state. It holds the values (in this
//   // case the title) provided by the parent (in this case the App widget) and
//   // used by the build method of the State. Fields in a Widget subclass are
//   // always marked "final".

//   final String title;

//   // @override
//   // State<MyHomePage> createState() => _MyHomePageState();
// }

class HomePage extends StatelessWidget {
  // int _counter = 0;

  // void _incrementCounter() { 
  //   setState(() {
  //     // This call to setState tells the Flutter framework that something has
  //     // changed in this State, which causes it to rerun the build method below
  //     // so that the display can reflect the updated values. If we changed
  //     // _counter without calling setState(), then the build method would not be
  //     // called again, and so nothing would appear to happen.
  //     _counter++;
  //   });
  // }

  @override
  Widget build(BuildContext context) {
    // This method is rerun every time setState is called, for instance as done
    // by the _incrementCounter method above.
    //
    // The Flutter framework has been optimized to make rerunning build methods
    // fast, so that you can just rebuild anything that needs updating rather
    // than having to individually change instances of widgets.

    // temporary for now
    int tempVal = 89;
    int coVal = 11;
    int batVal = 50;

    var uri = Uri.http('192.168.4.1', '');
    
    if (tempVal >= 85){
      local_notifs.giveNotif(
                          title: "CarGuard temperature warning",
                          body: "The temperature inside your car is too high! Opening window. Please return to your vehicle immediately.",
                          payload: "temp",
                          );
    }
    if (coVal >= 9){
      local_notifs.giveNotif(
                          title: "CarGuard CO warning",
                          body: "The CO levels in your vehicle is too high. Please evacuate for safety and check your vehicle's exhaust system.",
                          payload: "CO",
                          );
    }
    if (batVal >= 100){
      local_notifs.giveNotif(
                          title: "CarGuard battery warning",
                          body: "The temperature of the battery is too high. Please replace the battery.",
                          payload: "CO",
                          );
    }
    // reference: https://www.youtube.com/watch?v=AQT05gjCBtY
    // Color _setColor(String color){
    //   if(color == "red"){
    //     return const Color.fromARGB(255, 242, 6, 6);
    //   }
    //   else if (color == "orange"){
    //     return const Color.fromARGB(255, 237, 142, 8);
    //   }
    //   else{
    //     return const Color.fromARGB(255, 49, 124, 26);
    //   }
    // }
  

    return LayoutBuilder(
      builder: (context, constraints) {
        return Scaffold(
          appBar: AppBar(
            // TRY THIS: Try changing the color here to a specific color (to
            // Colors.amber, perhaps?) and trigger a hot reload to see the AppBar
            // change color while the other colors stay the same.
            backgroundColor: Theme.of(context).colorScheme.inversePrimary,
            // Here we take the value from the MyHomePage object that was created by
            // the App.build method, and use it to set our appbar title.
            title: Text("CarGuard"),
          ),
          body: Center(
            // Center is a layout widget. It takes a single child and positions it
            // in the middle of the parent.
              child:
                Column(
                  // Column is also a layout widget. It takes a list of children and
                  // arranges them vertically. By default, it sizes itself to fit its
                  // children horizontally, and tries to be as tall as its parent.
                  //
                  // Column has various properties to control how it sizes itself and
                  // how it positions its children. Here we use mainAxisAlignment to
                  // center the children vertically; the main axis here is the vertical
                  // axis because Columns are vertical (the cross axis would be
                  // horizontal).
                  //
                  // TRY THIS: Invoke "debug painting" (choose the "Toggle Debug Paint"
                  // action in the IDE, or press "p" in the console), to see the
                  // wireframe for each widget.
                  
                  mainAxisAlignment: MainAxisAlignment.start,
                  children: <Widget>[
                    // used flexible instead of expanded bc flexible allows the widget to control the size 
                    // whereas expanded takes as much space as it wants
                    
                    


                    Flexible(
                      flex: 1,
                      child:SizedBox(height: 100)
                    ),
                    
                    Flexible(
                      flex: 3,
                      child: Container( // box for text
                        // create a border
                        decoration: BoxDecoration(
                          border: Border.all(color: Colors.black, width: 2),
                          borderRadius: BorderRadius.all(Radius.circular(10)), 
                        ),
                        margin: const EdgeInsets.all(10.0),
                        //color: const Color.fromARGB(255, 235, 110, 210),
                        height: 35.0,
                        width: 200.0,
                        child:Center(
                          child:Row(
                            mainAxisSize: MainAxisSize.min,
                            children: [
                              Icon(Icons.videocam),
                              Text("Livestream Footage", textAlign: TextAlign.center,),
                            ],
                          ),
                        ),
                      ), // end of box for livestream
                    ),

                    Flexible(
                      flex: 3,
                      child: Container( // box for livestream
                        // create a border
                        decoration: BoxDecoration(
                          border: Border.all(color: Colors.black, width: 2),
                          borderRadius: BorderRadius.all(Radius.circular(10)), 
                        ),
                        margin: const EdgeInsets.all(10.0),
                        //color: const Color.fromARGB(255, 235, 110, 210),
                        height: 500.0,
                        width: 800.0,
                        child:Center(child: Text("Camera Footage", textAlign:TextAlign.center)), 
                      ), // end of box for livestream
                    ),


                    Flexible(
                      flex: 1,
                      child: InkWell(
                        child: Text("Test"),
                        onTap: () => launchUrl(uri),
                      ) 
                    ),


                    Flexible(
                      flex: 1,
                      child: ElevatedButton(
                        
                        onPressed: () {local_notifs.giveNotif(
                          title: "test title",
                          body: "testing",
                          payload: "adfa",
                          );
                        },
                        child: const Text("send notification"),
                        )
                    ),

                    Flexible(
                      flex: 1,
                      child:SizedBox(height: 200)
                    ),
                    
                    // Temp reading
                    Flexible(
                      flex: 1,
                      child: Container(
                        // create border around shape
                        decoration: BoxDecoration(
                          border: Border.all(color: Colors.black, width: 2),
                          borderRadius: BorderRadius.all(Radius.circular(20)), 
                        ),
                        margin: EdgeInsets.only(left: 50,right: 50),
                        // create rounded rectangle shape
                        child: ClipRRect(                
                          borderRadius: BorderRadius.circular(17.0),
                          child: Container(
                            color: tempVal > 85 ? Colors.redAccent : tempVal > 80 ? Colors.orange : Colors.green,
                            height: 50,
                            width: 700,                   
                            child:Center(
                              child:Row(
                                mainAxisSize: MainAxisSize.min,
                                children: [
                                  Icon(Icons.car_crash),
                                  SizedBox(width: 4), 
                                  Text("Car Temperature: $tempVal°F", textAlign: TextAlign.center,),
                                ],
                              ),
                            ),
                          ),
                        ),
                      ), 
                    ), 

                    // space between temp and CO
                    Flexible(
                      flex: 1,
                      child:SizedBox(height: 10)
                    ),
                      
                    // CO reading
                    Flexible(
                      flex: 1,
                      child: Container(
                        // create border around shape
                        decoration: BoxDecoration(
                          border: Border.all(color: Colors.black, width: 2),
                          borderRadius: BorderRadius.all(Radius.circular(20)), 
                        ),
                        margin: EdgeInsets.only(left: 50,right: 50),
                        // create rounded rectangle shape
                        child: ClipRRect(                
                          borderRadius: BorderRadius.circular(17.0),
                          child: Container(
                            color: coVal >= 9 ? Colors.redAccent : coVal >= 7 ? Colors.orange : Colors.green,
                            height: 50,
                            width: 700,                   
                            child:Center(
                              child:Row(
                                mainAxisSize: MainAxisSize.min,
                                children: [
                                  Icon(Icons.device_thermostat),
                                  Text("CO Levels: $coVal ppm", textAlign: TextAlign.center,),
                                ],
                              ),
                            ),
                          ),
                        ),
                      ), 
                    ), 
              
                    // space between CO and car battery temp
                    Flexible(
                      flex: 1,
                      child:SizedBox(height: 10)
                    ),
                      
                    // Battery Temperature Readings
                    Flexible(
                      flex: 1,
                      child: Container(
                      // create border around shape
                      decoration: BoxDecoration(
                        border: Border.all(color: Colors.black, width: 2),
                        borderRadius: BorderRadius.all(Radius.circular(20)), 
                      ),
                      margin: EdgeInsets.only(left: 50,right: 50),
                      // create rounded rectangle shape
                      child: ClipRRect(                
                        borderRadius: BorderRadius.circular(17.0),
                        child: Container(
                          color: batVal >= 100 ? Colors.redAccent : batVal >= 90 ? Colors.orange : Colors.green,
                          height: 50,
                          width: 700,                   
                          child:Center(
                            child:Row(
                              mainAxisSize: MainAxisSize.min,
                              children: [
                                Icon(Icons.battery_4_bar),
                                Text("Battery Temperature: $batVal F", textAlign: TextAlign.center,),
                              ],
                            ),
                          ),
                        ),
                      ),
                    ),   
                  )
                ], // end of children <widget>
              ), // end of column for first child
        ), // body ends here
        
          // floatingActionButton: FloatingActionButton(
          //   onPressed: _incrementCounter,
          //   tooltip: 'Increment',
          //   child: const Icon(Icons.add),
          // ), // This trailing comma makes auto-formatting nicer for build methods.
        );
      }
    );
  }
} // end of home page

class InfoPage extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    // var appState = context.watch<MyAppState>();
    
    return LayoutBuilder(
      builder: (context, constraints) {
        return Scaffold(
          
          appBar: AppBar(
            backgroundColor: Theme.of(context).colorScheme.inversePrimary,
            // Here we take the value from the MyHomePage object that was created by
            // the App.build method, and use it to set our appbar title.
            // title: Text(widget.title),
            title: Text("CarGuard"),
          ),
          body: Center(
            child:
                Column(
                  // Column is also a layout widget. It takes a list of children and
                  // arranges them vertically. By default, it sizes itself to fit its
                  // children horizontally, and tries to be as tall as its parent.
                  //
                  // Column has various properties to control how it sizes itself and
                  // how it positions its children. Here we use mainAxisAlignment to
                  // center the children vertically; the main axis here is the vertical
                  // axis because Columns are vertical (the cross axis would be
                  // horizontal).
                  //
                  // TRY THIS: Invoke "debug painting" (choose the "Toggle Debug Paint"
                  // action in the IDE, or press "p" in the console), to see the
                  // wireframe for each widget.
                  
                  mainAxisAlignment: MainAxisAlignment.center,
                  children: <Widget>[
                  // Center is a layout widget. It takes a single child and positions it
                  // in the middle of the parent.
                  
                  // child: Center(
                  //   child: 
                  //     Text("The green color means that the levels are safe.\n The orange color means that the level is ok but not optimal.\n The red means the levels are too high"),
                  //     ),
                  Flexible(
                      // temp reading
                      flex: 1,
                      child: Container(
                        //padding: EdgeInsets.only(left: 30,right: 30),
                        // create border around shape
                        decoration: BoxDecoration(
                            border: Border.all(color: Colors.black, width: 2),
                            borderRadius: BorderRadius.all(Radius.circular(20)), 
                          ),
                          margin: EdgeInsets.only(left: 50,right: 50),
                        child: ClipRRect(
                          borderRadius: BorderRadius.circular(17.0),
                          child: Container(
                            color: Colors.green,
                            height: 50,
                            width: 1000,                         
                            child:Center(child: Text( "This color means levels are safe", textAlign:TextAlign.center)),
                          ),
                        ),
                      ),
                    ),  

                    //space between temp and CO
                    Flexible(
                      flex: 1,
                      child:SizedBox(height: 10)
                    ),
                      
                    // CO sensor
                    Flexible(
                      flex: 1,
                      child:Container(
                        // padding: EdgeInsets.only(left: 30,right: 30),
                        // create border around shape
                        decoration: BoxDecoration(
                            border: Border.all(color: Colors.black, width: 2),
                            borderRadius: BorderRadius.all(Radius.circular(20)), 
                          ),
                          margin: EdgeInsets.only(left: 50,right: 50),
                        child: ClipRRect(
                          borderRadius: BorderRadius.circular(17),
                          child: Container(
                            color:  Colors.orange,
                            height: 50,
                            width: 1000,
                            child: Center(child: Text ("This color means that the level is ok but not optimal", textAlign: TextAlign.center,))
                          )
                        ),
                      ),
                    ),
              
                      // space between CO and car battery temp
                    Flexible(
                      flex: 1,
                      child:SizedBox(height: 10)
                    ),
                      
                    // Battery Temperature Readings
                    Flexible(
                      flex: 1,
                      child:Container(
                        //padding: EdgeInsets.only(left: 30,right: 30),
                        // create border around shape
                        decoration: BoxDecoration(
                          border: Border.all(color: Colors.black, width: 2),
                          borderRadius: BorderRadius.all(Radius.circular(20)), 
                        ),
                        margin: EdgeInsets.only(left: 50,right: 50),
                        child: ClipRRect(
                          borderRadius: BorderRadius.circular(17.0),
                          child: Container(
                            color: Colors.redAccent,
                            // color: const Color.fromARGB(255, 93, 169, 94),
                            height: 50,
                            width: 1000,
                            child:Center(child: Text("This color means the levels are too high", textAlign:TextAlign.center)),
                          ),
                        ),
                      ),
                    )
                  ]
            ), // end of column
          )
        ); // end of scaffold
      }
    );
  }
} // end of info page
