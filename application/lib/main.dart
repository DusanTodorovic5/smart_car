import 'package:flutter/material.dart';
import 'package:image_picker/image_picker.dart';
import 'dart:ffi';
import 'package:ffi/ffi.dart';
import 'dart:io';
import 'package:flutter/services.dart' show ByteData, Uint8List;
import 'image_thing.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatelessWidget {
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'OpenCV on Flutter',
      theme: ThemeData(primarySwatch: Colors.blue),
      home: MyHomePage(title: 'OpenCV C++ on dart:ffi'),
    );
  }
}

class MyHomePage extends StatefulWidget {
  MyHomePage({Key? key, required this.title}) : super(key: key);
  final String title;

  @override
  _MyHomePageState createState() => _MyHomePageState();
}

typedef ApplyGaussianBlurC = Int32 Function(
  Pointer<Uint8>,
  Int32,
  Pointer<Uint8>,
);

typedef ApplyGaussianBlurDart = int Function(
  Pointer<Uint8>,
  int,
  Pointer<Uint8>,
);

class _MyHomePageState extends State<MyHomePage> {
  final ImagePicker _picker = ImagePicker();
  final dylib = DynamicLibrary.open("libgaussian.so");

  int angle = 0;

  Uint8List slika = Uint8List(0);

  String _img = "none";
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
                final extractor = await decodeImageFromList(byteData);

                final gaussian = dylib.lookupFunction<ApplyGaussianBlurC,
                    ApplyGaussianBlurDart>('DetectLines');

                final frameData = malloc.allocate<Uint8>(uint8List.length);
                final pointerList = frameData.asTypedList(uint8List.length);
                pointerList.setAll(0, uint8List);

                final outPointer = malloc.allocate<Uint8>(uint8List.length);

                int x = gaussian(
                  frameData,
                  pointerList.length,
                  outPointer,
                );

                setState(() {
                  slika = outPointer.asTypedList(pointerList.length);
                  angle = x;
                });
              },
              child: Text("Pick Image from Gallery"),
            ),
            Expanded(
              child: Image.memory(
                slika,
                fit: BoxFit.contain,
              ),
            ),
          ],
        ),
      ),
    );
  }
}
