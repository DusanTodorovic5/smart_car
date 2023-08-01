import 'package:application/classes/websocket.dart';
import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_joystick/flutter_joystick.dart';
import 'dart:async';

import 'package:video_player/video_player.dart';

class LandingPage extends StatefulWidget {
  LandingPage({super.key});

  final String title = "Landing page";

  @override
  State<LandingPage> createState() => _LandingPageState();
}

class _LandingPageState extends State<LandingPage> {
  bool automatic_lights = true;
  bool auto_mode = true;

  bool left_turn_signal = false;
  bool right_turn_signal = false;
  bool all_turn_signal = false;

  Color left_turn_signal_color = Colors.amber;
  Color right_turn_signal_color = Colors.amber;

  late Timer leftSignalTimer, rightSignalTimer, dirTimer;
  late WebSocket webSocket;

  double xDir = 0.0;
  double lastXDir = 0.0;

  double yDir = 0.0;
  double lastYDir = 0.0;

  late VideoPlayerController _controller;

  @override
  void initState() {
    super.initState();
    webSocket = WebSocket(
      uri: "ws://192.168.0.15:1234",
      onMessage: onMessage,
    );

    _controller = VideoPlayerController.networkUrl(
      Uri.parse(
        'http://192.168.0.15',
      ),
    )..initialize().then(
        (_) {
          // Ensure the first frame is shown after the video is initialized, even before the play button has been pressed.
          setState(() {});
        },
      );

    SystemChrome.setPreferredOrientations([
      DeviceOrientation.landscapeRight,
      DeviceOrientation.landscapeLeft,
    ]);

    leftSignalTimer = Timer.periodic(
      const Duration(milliseconds: 600),
      (timer) {
        if (left_turn_signal || all_turn_signal) {
          setState(() {
            if (left_turn_signal_color == Colors.amber) {
              left_turn_signal_color = Colors.white;
            } else {
              left_turn_signal_color = Colors.amber;
            }
          });
        }
      },
    );

    rightSignalTimer = Timer.periodic(
      const Duration(milliseconds: 600),
      (timer) {
        if (right_turn_signal || all_turn_signal) {
          setState(() {
            if (right_turn_signal_color == Colors.amber) {
              right_turn_signal_color = Colors.white;
            } else {
              right_turn_signal_color = Colors.amber;
            }
          });
        }
      },
    );

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
      appBar: createAppBar(),
      drawer: createDrawer(),
      body: Stack(
        children: [
          VideoPlayer(_controller),
          Center(
              child: Column(
            mainAxisAlignment: MainAxisAlignment.spaceBetween,
            crossAxisAlignment: CrossAxisAlignment.end,
            children: [
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: [
                  IconButton(
                    icon: Icon(
                      Icons.arrow_circle_left_outlined,
                      color: left_turn_signal || all_turn_signal
                          ? left_turn_signal_color
                          : Colors.black,
                    ),
                    onPressed: () {
                      setState(() {
                        if (left_turn_signal && !all_turn_signal) {
                          left_turn_signal = false;
                        } else {
                          left_turn_signal = true;
                          all_turn_signal = false;
                          right_turn_signal = false;
                        }
                      });
                      sendLights();
                    },
                  ),
                  Row(
                    mainAxisSize: MainAxisSize.min,
                    children: [
                      IconButton(
                        icon: Icon(
                          Icons.compare_arrows,
                          color: all_turn_signal ? Colors.amber : Colors.black,
                        ),
                        onPressed: () {
                          setState(() {
                            if (all_turn_signal) {
                              all_turn_signal = false;
                              left_turn_signal = false;
                              right_turn_signal = false;
                            } else {
                              all_turn_signal = true;
                              left_turn_signal = true;
                              right_turn_signal = true;
                            }
                          });
                          sendLights();
                        },
                      ),
                      IconButton(
                        icon: Icon(
                          automatic_lights ? Icons.lightbulb : Icons.ac_unit,
                        ),
                        onPressed: () {
                          setState(() {
                            automatic_lights = !automatic_lights;
                          });
                          sendLights();
                        },
                      ),
                      IconButton(
                        icon: Icon(
                          Icons.arrow_circle_right_outlined,
                          color: right_turn_signal || all_turn_signal
                              ? right_turn_signal_color
                              : Colors.black,
                        ),
                        onPressed: () {
                          setState(() {
                            if (right_turn_signal && !all_turn_signal) {
                              right_turn_signal = false;
                            } else {
                              left_turn_signal = false;
                              all_turn_signal = false;
                              right_turn_signal = true;
                            }
                          });
                          sendLights();
                        },
                      ),
                    ],
                  )
                ],
              ),
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: [
                  Transform.scale(
                    scale: 0.75,
                    child: Joystick(
                      mode: JoystickMode.horizontal,
                      listener: (details) {
                        print("X: ${details.x}");
                        xDir = details.x;
                      },
                    ),
                  ),
                  Transform.scale(
                    scale: 0.75,
                    child: Joystick(
                      mode: JoystickMode.vertical,
                      listener: (details) {
                        print("Y: ${details.y}");
                        yDir = details.y;
                      },
                    ),
                  ),
                ],
              ),
            ],
          )),
        ],
      ),
    );
  }

  AppBar createAppBar() => AppBar(
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
          // Important: Remove any padding from the ListView.
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
                // Update the state of the app.
                // ...
              },
            ),
            ListTile(
              title: const Row(
                children: [
                  Icon(Icons.gamepad),
                  SizedBox(
                    width: 5,
                  ),
                  Text('Controls'),
                ],
              ),
              onTap: () {
                // Update the state of the app.
                // ...
              },
            ),
            ListTile(
              title: const Row(
                children: [
                  Icon(Icons.connect_without_contact),
                  SizedBox(
                    width: 5,
                  ),
                  Text('Reconnect'),
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

  void onMessage(message) =>
      ScaffoldMessenger.of(context).showSnackBar(SnackBar(
        content: Text(message),
      ));

  void sendLights() {
    webSocket.sendMessage({
      "type": 5,
      "left_dir_light": left_turn_signal,
      "right_dir_light": right_turn_signal,
      "front_light": false,
      "auto_lights": automatic_lights
    }.toString());
  }

  void changeDirectionX() {
    webSocket.sendMessage(
      {"type": 4, "direction": xDir}.toString(),
    );
  }

  void changeDirectionY() {
    webSocket.sendMessage(
      {"type": 3, "intensity": yDir}.toString(),
    );
  }
}
