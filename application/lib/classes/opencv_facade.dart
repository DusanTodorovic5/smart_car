import 'dart:ffi';
import 'package:ffi/ffi.dart';
import 'package:flutter/services.dart' show Uint8List;

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

class ImageWithAngle {
  Uint8List image;
  int angle;

  ImageWithAngle({required this.image, required this.angle});
}

class OpenCVFacade {
  final dylib = DynamicLibrary.open("libdetect_lines.so");

  int getAngle(Uint8List image) {
    final detectLines =
        dylib.lookupFunction<DetectLinesCVC, DetectLinesCVDart>('DetectLines');

    final frameData = malloc.allocate<Uint8>(image.length);
    final pointerList = frameData.asTypedList(image.length);
    pointerList.setAll(0, image);

    final outPointer = malloc.allocate<Uint8>(image.length);

    int x = detectLines(
      frameData,
      pointerList.length,
      outPointer,
    );

    return x;
  }

  ImageWithAngle getAngleWithImage(Uint8List image) {
    final detectLines =
        dylib.lookupFunction<DetectLinesCVC, DetectLinesCVDart>('DetectLines');

    final frameData = malloc.allocate<Uint8>(image.length);
    final pointerList = frameData.asTypedList(image.length);
    pointerList.setAll(0, image);

    final outPointer = malloc.allocate<Uint8>(image.length);

    int x = detectLines(
      frameData,
      pointerList.length,
      outPointer,
    );

    return ImageWithAngle(
      angle: x,
      image: outPointer.asTypedList(pointerList.length),
    );
  }
}
