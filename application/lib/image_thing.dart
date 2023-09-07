import 'package:image/image.dart';

class WidthHeightExtract {
  late int width;
  late int height;

  WidthHeightExtract({required imgData}) {
    final Image? image = decodeImage(imgData);

    this.width = image!.width;
    this.height = image.height;
  }
}
