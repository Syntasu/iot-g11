using System;
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
        /// <returns></returns>
        public static SockErr Connect(string ip, int port)
        {
            //We are already connected, you need to reset the worker first.
            if(IsConnected)
            {
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

                if(!didNotTimeout)
                {
                    Reset();
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
                    Reset();
                    return SockErr.ConnectionRefused;
                }
            }
            catch (SocketException)
            {
                Reset();
                return SockErr.ConnectionFailed;
            }
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
                return SockErr.SendErr;
            }

            //Construct the payload
            string[] toSend = { command, arg1, arg2, arg3 };
            string sendStr = string.Join(";", toSend);
            byte[] payload = Encoding.ASCII.GetBytes(sendStr);

            //Send the messages, do error handling and sanity checking.
            if(IsConnected)
            {
                try
                {
                    socket.Send(payload);
                    return SockErr.None;
                }
                catch(SocketException)
                {
                    return SockErr.SendFailed;
                }
            }

            return SockErr.SendFailed;
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
                    string received = Encoding.ASCII.GetString(readBuffer, 0, receivedBytes);
                    OnSocketReceive?.Invoke(received);
                }
                catch (SocketException) { } //Woops! Reading failed.. not enough to tell to abort the connection... 
                                            //we don't want to crash the application either. So that's why we have an empty try catch
                          
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
                socketThread.Join();
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