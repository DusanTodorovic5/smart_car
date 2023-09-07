import 'dart:async';

import '../classes/self_driving_car_icons_icons.dart';
import '../classes/websocket.dart';
import 'package:flutter/material.dart';

class LightsWidget extends StatefulWidget {
  WebSocket webSocket;

  LightsWidget({required this.webSocket});

  @override
  _LightsWidgetState createState() => _LightsWidgetState();
}

class _LightsWidgetState extends State<LightsWidget> {
  bool automatic_lights = true;

  bool left_turn_signal = false;
  bool right_turn_signal = false;
  bool all_turn_signal = false;
  bool beam_lights = false;

  Color left_turn_signal_color = Colors.amber;
  Color right_turn_signal_color = Colors.amber;

  late Timer leftSignalTimer, rightSignalTimer;

  @override
  void initState() {
    super.initState();

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
  }

  @override
  Widget build(BuildContext context) {
    return Row(
      mainAxisAlignment: MainAxisAlignment.spaceBetween,
      children: [
        Row(
          children: [
            IconButton(
              icon: CircleAvatar(
                backgroundColor: const Color.fromARGB(99, 255, 255, 255),
                child: Icon(
                  Icons.arrow_circle_left_outlined,
                  color: left_turn_signal || all_turn_signal
                      ? left_turn_signal_color
                      : Colors.black,
                ),
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
            IconButton(
              icon: CircleAvatar(
                backgroundColor: const Color.fromARGB(99, 255, 255, 255),
                child: Icon(
                  beam_lights
                      ? SelfDrivingCarIcons.beamlights
                      : SelfDrivingCarIcons.beamlightsoff,
                  color: Colors.black,
                ),
              ),
              onPressed: () {
                setState(() {
                  beam_lights = !beam_lights;
                });
                sendLights();
              },
            ),
          ],
        ),
        Row(
          mainAxisSize: MainAxisSize.min,
          children: [
            IconButton(
              icon: CircleAvatar(
                backgroundColor: const Color.fromARGB(50, 255, 255, 255),
                child: Icon(
                  SelfDrivingCarIcons.hazard,
                  color: all_turn_signal ? Colors.amber : Colors.black,
                ),
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
              icon: CircleAvatar(
                backgroundColor: const Color.fromARGB(50, 255, 255, 255),
                child: Icon(
                  automatic_lights
                      ? SelfDrivingCarIcons.autolights
                      : SelfDrivingCarIcons.manuallights,
                ),
              ),
              onPressed: () {
                setState(() {
                  automatic_lights = !automatic_lights;
                });
                sendLights();
              },
            ),
            IconButton(
              icon: CircleAvatar(
                backgroundColor: const Color.fromARGB(50, 255, 255, 255),
                child: Icon(
                  Icons.arrow_circle_right_outlined,
                  color: right_turn_signal || all_turn_signal
                      ? right_turn_signal_color
                      : Colors.black,
                ),
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
    );
  }

  void sendLights() {
    widget.webSocket.sendMessage({
      "type": 3,
      "left_dir_light": left_turn_signal,
      "right_dir_light": right_turn_signal,
      "front_light": beam_lights,
      "auto_lights": automatic_lights,
    }.toString());
  }
}
