import 'package:flutter/material.dart';
import 'package:image_picker/image_picker.dart';
import 'dart:ffi';
import 'dart:io';
import 'package:flutter/services.dart' show Uint8List;

import '../classes/opencv_facade.dart';

class ImageProcessingTestPage extends StatefulWidget {
  ImageProcessingTestPage({Key? key, imageParam}) : super(key: key);
  @override
  _ImageProcessingTestPageState createState() =>
      _ImageProcessingTestPageState();

  Uint8List? imageParam;
}

typedef DetectLinesCVC = Int32 Function(
  Pointer<Uint8>,
  Int32,
  Pointer<Uint8>,
);

typedef DetectLinesCVDart = int Function(
  Pointer<Uint8>,
  int,
  Pointer<Uint8>,
);

class _ImageProcessingTestPageState extends State<ImageProcessingTestPage> {
  final ImagePicker _picker = ImagePicker();
  final dylib = DynamicLibrary.open("libdetect_lines.so");

  @override
  void initState() {
    // TODO: implement initState
    super.initState();

    if (widget.imageParam != null) {
      final facade = OpenCVFacade();

      final imgWithAngle = facade.getAngleWithImage(
        widget.imageParam!,
      );

      angle = imgWithAngle.angle;
      image = imgWithAngle.image;
    }
  }

  int angle = 0;

  Uint8List image = Uint8List(0);

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text("Angle: $angle")),
      body: Center(
        child: Column(
          mainAxisAlignment: MainAxisAlignment.center,
          children: [
            ElevatedButton(
              onPressed: () async {
                final imagePicked =
                    await _picker.pickImage(source: ImageSource.gallery);

                File imageFile = File(imagePicked!.path);
                final byteData = await imageFile.readAsBytes();
                final Uint8List uint8List = byteData.buffer.asUint8List();

                final facade = OpenCVFacade();

                final imgWithAngle = facade.getAngleWithImage(
                  uint8List,
                );

                setState(() {
                  image = imgWithAngle.image;
                  angle = imgWithAngle.angle;
                });
              },
              child: Text("Pick Image from Gallery"),
            ),
            Expanded(
              child: Image.memory(
                image,
                fit: BoxFit.contain,
              ),
            ),
          ],
        ),
      ),
    );
  }
}
