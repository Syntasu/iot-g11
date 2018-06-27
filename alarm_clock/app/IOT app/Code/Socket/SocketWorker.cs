using System;
using System.Diagnostics;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;

namespace IOT_app.Code
{
    public delegate void SocketConnect();
    public delegate void SocketDisconnect();
    public delegate void SocketReceive(string message);

    public class SocketWorker
    {
        //All the events that can be fired by the socket woker
        public static event SocketConnect OnSocketConnect;
        public static event SocketDisconnect OnSocketDisconnect;
        public static event SocketReceive OnSocketReceive;

        //Some variables indicating the current state of the socket worker.
        public static bool IsConnected { get; set; } = false;
        public static IPAddress ConnectedTo { get; set; } = null;
        public static int ConnectedPort { get; set; } = 0;

        //Internal variable used by the socket worker.
        private static Socket socket;
        private static Thread socketThread;

        /// <summary>
        ///     Connect to the arduino.
        /// </summary>
        /// <param name="ip">The IP address to connect to.</param>
        /// <param name="port">The port to connect to.</param>
        /// <returns>Any potential errors, SockErr.None means everything went OK.</returns>
        public static SockErr Connect(string ip, int port)
        {
            //We are already connected, you need to reset (or disconnect) the worker first in order to reconnect
            if(IsConnected)
            {
                Debug.WriteLine("Warning: trying to connect when we already have a connection.");
                return SockErr.ConnectionDuplicate;
            }

            //Try connecting...
            try
            {
                //Construct and connect the socket.
                IPAddress ipAddr = IPAddress.Parse(ip);
                socket = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

                IAsyncResult result = socket.BeginConnect(new IPEndPoint(ipAddr, port), null, null);
                bool didNotTimeout = result.AsyncWaitHandle.WaitOne(500, true);

                //If a timeout occured, reset the SocketWorker and make it known an timeout occured.
                if(!didNotTimeout)
                {
                    Reset();
                    Debug.WriteLine("Socket connection timed out.");
                    return SockErr.ConnectionTimeout;
                }

                //Erorr handling if we did or did not successfully connect.
                if (socket.Connected)
                {
                    socket.EndConnect(result);

                    IsConnected = true;
                    ConnectedTo = ipAddr;
                    ConnectedPort = port;

                    OnSocketConnect?.Invoke();

                    //Boot up the read thread.
                    socketThread = new Thread(new ThreadStart(Read));
                    socketThread.Start();

                    return SockErr.None;
                }
                else
                {
                    //We did get some sort of connection, but the
                    //devices actively refused out connection... :thinking:
                    Reset();
                    Debug.WriteLine("Warning: Trying to connect, but is silently failed");
                    return SockErr.ConnectionRefused;
                }
            }
            catch (SocketException e)
            {
                Reset();
                Debug.WriteLine("Socket connection exception: " + e.Message);
                return SockErr.ConnectionFailed;
            }
        }

        /// <summary>
        ///     Disconnect
        /// </summary>
        public static void Disconnect()
        {
            Reset();
        }

        /// <summary>
        ///     Send an command to the arduino.
        /// </summary>
        /// <param name="command">The command we are sending.</param>
        /// <param name="arg1">The first argument.</param>
        /// <param name="arg2">The second argumnet.</param>
        /// <param name="arg3">The third argument. </param>
        /// <returns>A SockErr enum, wether there has been an error whilest sending.</returns>
        public static SockErr Send(string command, string arg1 = "", string arg2 = "", string arg3 = "")
        {
            //We cannot send data without an command. 
            if(string.IsNullOrEmpty(command))
            {
                Debug.WriteLine("Warning: Trying to send command without specifying a command.");
                return SockErr.SendErr;
            }

            string data = command + ";";

            //Append the arguments to the end of the command string.
            if(!string.IsNullOrEmpty(arg1)) data += arg1 + ";";
            if(!string.IsNullOrEmpty(arg2)) data += arg2 + ";";
            if(!string.IsNullOrEmpty(arg3)) data += arg3 + ";";

            byte[] payload = Encoding.ASCII.GetBytes(data);

            //Send the messages, do error handling and sanity checking.
            if(IsConnected)
            {
                try
                {
                    socket.Send(payload);
                    return SockErr.None;
                }
                catch(SocketException e)
                {
                    Debug.WriteLine("Socket send exception" + e.Message);
                    return SockErr.SendFailed;
                }
            }
            else
            {
                Debug.WriteLine("Warning: Trying to send when the connection was not established.");
                return SockErr.SendFailed;
            }

        }

        /// <summary>
        ///     Read whatever is being sent by the arduino.
        /// </summary>
        private static void Read()
        {
            byte[] readBuffer = new byte[512];

            //As long as we are connected and socket is not null, keep reading.
            //Prevent the read thread from dying off.
            while(IsConnected && socket != null)
            {
                try
                {
                    //Read incoming.
                    int receivedBytes = socket.Receive(readBuffer);

                    //Catch read overflows, for now we ignore the message.
                    if(receivedBytes >= readBuffer.Length)
                    {
                        Debug.WriteLine("Warning: Read overflow, considering increasing the read buffer size or send less data.");
                        return;
                    }

                    string received = Encoding.ASCII.GetString(readBuffer, 0, receivedBytes);
                    OnSocketReceive?.Invoke(received);
                }
                //Woops! Reading failed.. not enough to say to abort the connection... 
                //we don't want to crash the application either. Probably an error occured when reading the socket.
                catch (SocketException e)
                {
                    Debug.WriteLine("Socket read exception: " + e.Message);
                    return;
                } 
            }
        }

        /// <summary>
        ///     Properly dispose all the connections and variables.
        /// </summary>
        private static void Reset()
        {
            //Reset the variables.
            if (ConnectedTo != null) ConnectedTo = null;
            if (ConnectedPort != 0) ConnectedPort = 0;
            if (IsConnected) IsConnected = false;

            //Kill off the thread
            if(socketThread != null)
            { 
                socketThread.Abort();
                socketThread = null;
            }

            //Kill sockets.
            if(socket != null && socket.Connected)
            {
                socket.Shutdown(SocketShutdown.Both);
                socket.Close();
                socket = null;
            }

            //Tell everyone we disconnected.
            OnSocketDisconnect?.Invoke();
        }
    }
}