import 'package:flutter/material.dart';
import 'package:web_socket_channel/web_socket_channel.dart';

import '../pages/landing_page.dart';

class WebSocket {
  final channel = WebSocketChannel.connect(
    Uri.parse('ws://localhost:8765'),
  );
  final Function onMessage;

  WebSocket({required this.onMessage}) {
    channel.stream.listen((message) => onMessage(message));
    sendMessage("slavica");
  }

  void sendMessage(String message) {
    channel.sink.add(message);
  }
}
