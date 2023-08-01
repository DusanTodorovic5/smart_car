import 'package:flutter/material.dart';
import 'package:web_socket_channel/web_socket_channel.dart';

import '../pages/landing_page.dart';

class WebSocket {
  late WebSocketChannel channel;
  final Function onMessage;
  final String uri;
  WebSocket({required this.uri, required this.onMessage}) {
    channel = WebSocketChannel.connect(
      Uri.parse(uri),
    );
    channel.stream.listen((message) => onMessage(message));
  }

  void sendMessage(String message) {
    channel.sink.add(message);
  }
}
