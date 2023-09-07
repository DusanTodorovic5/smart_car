import 'dart:typed_data';
import 'dart:ui';
import '../classes/ip_address.dart';
import 'package:flutter/material.dart';
import 'package:flutter/rendering.dart';
import 'package:webview_flutter/webview_flutter.dart';

class VideoPlayerWidget extends StatefulWidget {
  @override
  _VideoPlayerWidgetState createState() => _VideoPlayerWidgetState();

  var scr = GlobalKey();

  Future<Uint8List> requestScreenshot() async {
    final boundary =
        scr.currentContext!.findRenderObject() as RenderRepaintBoundary?;
    final image = await boundary?.toImage();
    final byteData = await image?.toByteData(format: ImageByteFormat.png);

    return byteData!.buffer.asUint8List();
  }
}

class _VideoPlayerWidgetState extends State<VideoPlayerWidget> {
  final controller = WebViewController()
    ..setJavaScriptMode(JavaScriptMode.unrestricted)
    ..setBackgroundColor(const Color(0x00000000))
    ..setNavigationDelegate(
      NavigationDelegate(
        onProgress: (int progress) {},
        onPageStarted: (String url) {},
        onPageFinished: (String url) {},
        onWebResourceError: (WebResourceError error) {},
        onNavigationRequest: (NavigationRequest request) {
          return NavigationDecision.navigate;
        },
      ),
    )
    ..loadRequest(Uri.parse(IPAddress.camera_ip));

  // Timer? frameCaptureTimer;

  @override
  void initState() {
    super.initState();
    // frameCaptureTimer = Timer.periodic(
    //   const Duration(seconds: 1),
    //   (timer) async {
    //     await takeScreenShot();
    //   },
    // );
  }

  @override
  Widget build(BuildContext context) {
    return RepaintBoundary(
      key: widget.scr,
      child: AspectRatio(
        aspectRatio: 800.0 / 600.0,
        child: WebViewWidget(controller: controller),
      ),
    );
  }

  @override
  void dispose() {
    // frameCaptureTimer?.cancel();
    super.dispose();
  }
}
