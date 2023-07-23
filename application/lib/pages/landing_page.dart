import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:flutter_joystick/flutter_joystick.dart';

class LandingPage extends StatefulWidget {
  const LandingPage({super.key});

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

  @override
  void initState() {
    super.initState();
    SystemChrome.setPreferredOrientations([
      DeviceOrientation.landscapeRight,
      DeviceOrientation.landscapeLeft,
    ]);
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
      body: Center(
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
                  color: left_turn_signal ? Colors.amber : Colors.black,
                ),
                onPressed: () {
                  setState(() {
                    left_turn_signal = !left_turn_signal;
                  });
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
                        all_turn_signal = !all_turn_signal;
                      });
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
                    },
                  ),
                  IconButton(
                    icon: Icon(
                      Icons.arrow_circle_right_outlined,
                      color: right_turn_signal ? Colors.amber : Colors.black,
                    ),
                    onPressed: () {
                      setState(() {
                        right_turn_signal = !right_turn_signal;
                      });
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
                    print("X: ${details.x}    Y: ${details.y}");
                  },
                ),
              ),
              Transform.scale(
                scale: 0.75,
                child: Joystick(
                  mode: JoystickMode.vertical,
                  listener: (details) {
                    print("X: ${details.x}    Y: ${details.y}");
                  },
                ),
              ),
            ],
          ),
        ],
      )),
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
          ],
        ),
      );
}
