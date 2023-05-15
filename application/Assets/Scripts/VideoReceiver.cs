using UnityEngine;
using UnityEngine.UI;
using System;
using System.Net.WebSockets;
using System.Threading;
using System.Threading.Tasks;
using System.Collections;
using System.Collections.Generic;
using System.Net;

public class VideoReceiver : MonoBehaviour
{
    private const string SERVER_IP = "192.168.0.30"; // Replace with the IP address of your ESP32 camera
    private const int SERVER_PORT = 1400;

    private ClientWebSocket websocket;
    private CancellationTokenSource cancelToken;

    private Thread receiveThread;
    private Texture2D receivedTexture;
    private int imageSize = 1280 * 720 * 2;
    private byte[] imageData;
    private int bytesReceived = 0;
    public Image image;

    async void Start()
    {
        try
        {
            imageData = new byte[imageSize];
            receivedTexture = new Texture2D(1280, 720);
            websocket = new ClientWebSocket();
            Uri serverUri = new Uri($"ws://{SERVER_IP}:{SERVER_PORT}"); // Replace with the URI of your ESP32 camera

            cancelToken = new CancellationTokenSource();

            await websocket.ConnectAsync(serverUri, cancelToken.Token);

            receiveThread = new Thread(new ThreadStart(ReceiveData));
            receiveThread.Start();

        }
        catch (Exception e)
        {
            Debug.LogException(e);
        }
    }

    async void ReceiveData()
    {
        byte[] buffer = new byte[1280*720];
        WebSocketReceiveResult result;

        try
        {
            while (websocket.State == WebSocketState.Open)
            {
                result = await websocket.ReceiveAsync(new ArraySegment<byte>(buffer), CancellationToken.None);

                if (result.MessageType == WebSocketMessageType.Binary)
                {
                    // Find the start of the JPEG image data
                    int startIndex = FindJpegStart(buffer, result.Count);
                    if (startIndex >= 0)
                    {
                        // Decode the JPEG image data

                        // Update the received texture on the main thread
                        UnityMainThreadDispatcher.Instance().Enqueue(() => {
                            Texture2D texture = new Texture2D(1280, 720, TextureFormat.RGB24, false);
                            texture.LoadImage(buffer);
                            receivedTexture = texture;
                        });
                    }
                }
            }
        }
        catch (Exception e)
        {
            Debug.LogException(e);
        }
    }

    void Update()
    {
        image.sprite = Sprite.Create(receivedTexture, new Rect(0, 0, receivedTexture.width, receivedTexture.height), Vector2.zero);
    }

    async void OnApplicationQuit()
    {
        try
        {
            cancelToken.Cancel();

            if (websocket.State == WebSocketState.Open)
            {
                await websocket.CloseAsync(WebSocketCloseStatus.NormalClosure, "Closing", CancellationToken.None);
            }
        }
        catch (Exception e)
        {
            Debug.LogException(e);
        }
    }

    private int FindJpegStart(byte[] data, int length)
    {
        // Find the first occurrence of the JPEG start marker (0xFFD8)
        for (int i = 0; i < length - 1; i++)
        {
            if (data[i] == 0xFF && data[i+1] == 0xD8)
            {
                return i;
            }
        }

        return -1;
    }
}
