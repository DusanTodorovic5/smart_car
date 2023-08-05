import 'package:flutter/material.dart';
import 'package:flutter/services.dart';

class WifiDialog extends StatefulWidget {
  WifiDialog({Key? key}) : super(key: key);

  @override
  State<WifiDialog> createState() => _WifiDialogState();
}

class _WifiDialogState extends State<WifiDialog> {
  TextEditingController ssidController = TextEditingController();
  TextEditingController passwordController = TextEditingController();

  @override
  Widget build(BuildContext context) {
    return Center(
      child: Column(
        mainAxisSize: MainAxisSize.min,
        children: [
          TextField(
            decoration: const InputDecoration(
              border: UnderlineInputBorder(),
              hintText: 'Enter SSID',
            ),
            controller: ssidController,
          ),
          TextField(
            obscureText: true,
            enableSuggestions: false,
            autocorrect: false,
            decoration: const InputDecoration(
              border: UnderlineInputBorder(),
              hintText: 'Enter password',
            ),
            controller: passwordController,
          ),
          TextButton(
            onPressed: () {
              Navigator.of(context, rootNavigator: true).pop();
            },
            child: const Text("Connect to wifi!"),
          )
        ],
      ),
    );
  }
}
