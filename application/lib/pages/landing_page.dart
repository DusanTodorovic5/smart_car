import 'package:application/classes/websocket.dart';
import 'package:application/widgets/lights_widget.dart';
import 'package:application/widgets/video_player.dart';
import 'package:application/widgets/wifi_connect.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_joystick/flutter_joystick.dart';
import 'dart:async';

class LandingPage extends StatefulWidget {
  LandingPage({super.key});

  final String title = "Landing page";

  @override
  State<LandingPage> createState() => _LandingPageState();
}

class _LandingPageState extends State<LandingPage> {
  late WebSocket webSocket;
  bool auto_mode = true;

  double xDir = 0.0;
  double lastXDir = 0.0;

  double yDir = 0.0;
  double lastYDir = 0.0;

  late Timer dirTimer;

  @override
  void initState() {
    super.initState();
    webSocket = WebSocket(
      uri: "ws://192.168.0.15:1234",
      onMessage: onMessage,
    );

    SystemChrome.setPreferredOrientations([
      DeviceOrientation.landscapeRight,
      DeviceOrientation.landscapeLeft,
    ]);

    dirTimer = Timer.periodic(
      const Duration(milliseconds: 60),
      (timer) {
        if (xDir != lastXDir) {
          print("xDir = ${xDir}");
          lastXDir = xDir;
          changeDirectionX();
        }

        if (yDir != lastYDir) {
          print("yDir = ${yDir}");
          lastYDir = yDir;
          changeDirectionY();
        }
      },
    );
  }

  final MaterialStateProperty<Icon?> thumbIcon =
      MaterialStateProperty.resolveWith<Icon?>(
    (Set<MaterialState> states) {
      if (states.contains(MaterialState.selected)) {
        return const Icon(Icons.wifi_protected_setup);
      }
      return const Icon(Icons.drive_eta);
    },
  );

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      extendBodyBehindAppBar: true,
      appBar: createAppBar(),
      drawer: createDrawer(),
      body: Stack(
        fit: StackFit.expand,
        children: [
          VideoPlayerWidget(),
          Positioned(
            top: AppBar().preferredSize.height +
                MediaQuery.of(context).viewPadding.top,
            bottom: 0,
            left: 0,
            right: 0,
            child: Center(
                child: Column(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              crossAxisAlignment: CrossAxisAlignment.end,
              children: [
                LightsWidget(webSocket: webSocket),
                Row(
                  mainAxisAlignment: MainAxisAlignment.spaceBetween,
                  children: [
                    Transform.scale(
                      scale: 0.75,
                      child: Joystick(
                        mode: JoystickMode.horizontal,
                        listener: (details) {
                          print("### X: ${details.x}");
                          xDir = details.x;
                        },
                      ),
                    ),
                    Transform.scale(
                      scale: 0.75,
                      child: Joystick(
                        mode: JoystickMode.vertical,
                        listener: (details) {
                          print("### Y: ${details.y}");
                          yDir = details.y;
                        },
                      ),
                    ),
                  ],
                ),
              ],
            )),
          ),
        ],
      ),
    );
  }

  AppBar createAppBar() => AppBar(
        iconTheme: const IconThemeData(color: Colors.white),
        backgroundColor: Colors.transparent,
        actions: [
          Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: <Widget>[
              Switch(
                activeTrackColor: Colors.greenAccent,
                thumbIcon: thumbIcon,
                value: auto_mode,
                onChanged: (bool value) {
                  setState(() {
                    auto_mode = value;
                  });
                },
              ),
            ],
          ),
          const SizedBox(
            width: 10,
          ),
        ],
      );

  Drawer createDrawer() => Drawer(
        child: ListView(
          padding: EdgeInsets.zero,
          children: [
            SizedBox(
              height: AppBar().preferredSize.height,
            ),
            ListTile(
              title: const Row(
                children: [
                  Icon(Icons.wifi),
                  SizedBox(
                    width: 5,
                  ),
                  Text('Conect to wifi'),
                ],
              ),
              onTap: () {
                showDialog(
                    context: context,
                    barrierDismissible: true,
                    builder: (BuildContext context) {
                      return WifiDialog();
                    });
              },
            ),
            // ListTile(
            //   title: const Row(
            //     children: [
            //       Icon(Icons.gamepad),
            //       SizedBox(
            //         width: 5,
            //       ),
            //       Text('Controls'),
            //     ],
            //   ),
            //   onTap: () {
            //     // Update the state of the app.
            //     // ...
            //   },
            // ),
            ListTile(
              title: const Row(
                children: [
                  Icon(Icons.connect_without_contact),
                  SizedBox(
                    width: 5,
                  ),
                  Text('Reconnect to ESP'),
                ],
              ),
              onTap: () {
                webSocket.channel.sink.close();
                webSocket = WebSocket(
                  uri: "ws://192.168.0.15:1234",
                  onMessage: onMessage,
                );
              },
            ),
          ],
        ),
      );

  void onMessage(message) => print("### ${message}");
  // ScaffoldMessenger.of(context).showSnackBar(SnackBar(
  //   content: Text(message),
  // ));

  void changeDirectionX() {
    print("### xDir = ${xDir}");
    webSocket.sendMessage(
      {
        "type": 2,
        "direction": convertToSTPercentage(xDir),
      }.toString(),
    );
  }

  void changeDirectionY() {
    print("### yDir = ${yDir}");
    webSocket.sendMessage(
      {
        "type": 1,
        "voltage": convertToSTPercentage(yDir),
      }.toString(),
    );
  }

  int convertToSTPercentage(double percentage) {
    int newValue = (percentage * 100).toInt();

    if (newValue < 0) {
      newValue = 255 + newValue;
    }

    return newValue;
  }
}
