class IPAddress {
  static String address = "192.168.100.14";

  static String get websocket_ip => "ws://${address}:1234";
  static String get camera_ip => "http://${address}:";

  static void setAddress(String new_address) {
    address = new_address;
  }
}
