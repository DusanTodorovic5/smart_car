import 'package:flutter/material.dart';
import 'package:shared_preferences/shared_preferences.dart';

import 'classes/ip_address.dart';
import 'pages/landing_page.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  final prefs = await SharedPreferences.getInstance();

  if (prefs.getString("ip") != null) {
    IPAddress.setAddress(prefs.getString("ip")!);
  } else {
    IPAddress.setAddress("192.168.100.14");
  }

  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Smart car controller',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(seedColor: Colors.deepPurple),
        useMaterial3: true,
      ),
      home: LandingPage(),
    );
  }
}
