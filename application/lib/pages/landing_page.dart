import 'package:smart_car_controller/pages/image_processing_test.dart';

import '../classes/ip_address.dart';
import '../classes/self_driving_car_icons_icons.dart';
import '../classes/websocket.dart';
import '../pages/bluetooth_manager.dart';
import '../widgets/lights_widget.dart';
import '../widgets/video_player.dart';
import '../widgets/wifi_connect.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_joystick/flutter_joystick.dart';
import 'dart:async';

import 'package:shared_preferences/shared_preferences.dart';

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

  late VideoPlayerWidget? player;

  @override
  void initState() {
    super.initState();

    webSocket = WebSocket(
      uri: IPAddress.websocket_ip,
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
        return const Icon(SelfDrivingCarIcons.autodrive);
      }
      return const Icon(SelfDrivingCarIcons.manualdrive);
    },
  );

  @override
  Widget build(BuildContext context) {
    player = VideoPlayerWidget();
    return Scaffold(
      extendBodyBehindAppBar: true,
      appBar: createAppBar(),
      drawer: createDrawer(),
      body: Stack(
        fit: StackFit.expand,
        children: [
          player!,
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
                Padding(
                  padding: const EdgeInsets.all(25.0),
                  child: Row(
                    mainAxisAlignment: MainAxisAlignment.spaceBetween,
                    children: [
                      Transform.scale(
                        scale: 0.75,
                        child: Joystick(
                          mode: JoystickMode.horizontal,
                          listener: (details) {
                            // print("### X: ${details.x}");
                            xDir = details.x;
                          },
                        ),
                      ),
                      Transform.scale(
                        scale: 0.75,
                        child: Joystick(
                          mode: JoystickMode.vertical,
                          listener: (details) {
                            // print("### Y: ${details.y}");
                            yDir = details.y;
                          },
                        ),
                      ),
                    ],
                  ),
                ),
              ],
            )),
          ),
        ],
      ),
    );
  }

  AppBar createAppBar() => AppBar(
        iconTheme: const IconThemeData(color: Colors.grey),
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
            Padding(
              padding: const EdgeInsets.all(8.0),
              child: Text(
                IPAddress.address,
                style: const TextStyle(
                  fontSize: 25,
                ),
              ),
            ),
            const SizedBox(
              height: 10,
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
                  Icon(Icons.bluetooth),
                  SizedBox(
                    width: 5,
                  ),
                  Text('Configure ESP'),
                ],
              ),
              onTap: () {
                Navigator.of(context).push(
                  MaterialPageRoute(
                    builder: (context) {
                      return MainPage();
                    },
                  ),
                );
              },
            ),
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
              onTap: showReconnectDialog,
            ),
            ListTile(
              title: const Row(
                children: [
                  Icon(Icons.add_a_photo_sharp),
                  SizedBox(
                    width: 5,
                  ),
                  Text('Test Image Processing'),
                ],
              ),
              onTap: () => player?.requestScreenshot().then(
                (image) {
                  Navigator.push(
                    context,
                    MaterialPageRoute(
                      builder: (context) => ImageProcessingTestPage(
                        imageParam: image,
                      ),
                    ),
                  );
                },
              ),
            ),
          ],
        ),
      );

  void onMessage(message) => print("### ${message}");
  // ScaffoldMessenger.of(context).showSnackBar(SnackBar(
  //   content: Text(message),
  // ));

  void showReconnectDialog() {
    TextEditingController _controller = TextEditingController();
    showDialog<String>(
      context: context,
      builder: (BuildContext context) => AlertDialog(
        title: const Text('AlertDialog Title'),
        content: TextField(
          controller: _controller,
          decoration: const InputDecoration.collapsed(
            hintText: "Type in ip address",
          ),
        ),
        actions: <Widget>[
          TextButton(
            onPressed: () => Navigator.pop(context),
            child: const Text('Cancel'),
          ),
          TextButton(
            onPressed: () {
              if (_controller.text.length > 4) {
                IPAddress.setAddress(_controller.text);
                SharedPreferences.getInstance().then(
                  (value) => value.setString("ip", IPAddress.address),
                );

                webSocket.channel.sink.close();

                setState(() {
                  webSocket = WebSocket(
                    uri: IPAddress.websocket_ip,
                    onMessage: onMessage,
                  );
                });
              }

              Navigator.pop(context);
            },
            child: const Text('OK'),
          ),
        ],
      ),
    );
  }

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

    if (newValue > -5 && newValue < 5) {
      return 0;
    }

    if (newValue < 0) {
      newValue -= 40;
      if (newValue < -100) {
        newValue = -100;
      }
      newValue = 255 + newValue;
    } else {
      newValue += 40;
      if (newValue > 100) {
        newValue = 100;
      }
    }

    return newValue;
  }
}
