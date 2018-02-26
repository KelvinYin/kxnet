#include <kxnet/codec/Codec.h>
#include <kxnet/codec/ReadStream.h>
#include <kxnet/codec/WriteStream.h>

#include <kxnet/base/Logging.h>
#include <kxnet/base/Mutex.h>
#include <kxnet/net/EventLoopThread.h>
#include <kxnet/net/TcpClient.h>

#include <iostream>
#include <stdio.h>
#include <unistd.h>

using namespace kxnet;
using namespace kxnet::net;
using namespace std;

class ChatClient : noncopyable
{
public:
    ChatClient(EventLoop* loop, const InetAddress& serverAddr)
        : client_(loop, serverAddr, "ChatClient"),
        codec_(std::bind(&ChatClient::onStringMessage, this, _1, _2, _3))
    {
        client_.setConnectionCallback(
                std::bind(&ChatClient::onConnection, this, _1));
        client_.setMessageCallback(
                std::bind(&LengthHeaderCodec::onMessage, &codec_, _1, _2, _3));
        client_.enableRetry();
    }

    void connect()
    {
        client_.connect();
    }

    void disconnect()
    {
        client_.disconnect();
    }

    void write(const StringPiece& message)
    {
        LOG_INFO << "client write";
        MutexLockGuard lock(mutex_);
        if (connection_)
        {
            LOG_INFO << "message length = " << message.size();
            codec_.send(connection_, message);
        }
    }

private:
    void onConnection(const TcpConnectionPtr& conn)
    {
        LOG_INFO << conn->localAddress().toIpPort() << " -> "
            << conn->peerAddress().toIpPort() << " is "
            << (conn->connected() ? "UP" : "DOWN");

        MutexLockGuard lock(mutex_);
        if (conn->connected())
        {
            connection_ = conn;
        }
        else
        {
            connection_.reset();
        }
    }

    void onStringMessage(const TcpConnectionPtr&,
            const string& message,
            Timestamp)
    {
        BinaryReadStream readStream(message.data(), message.length());
        int seq;
        readStream.Read(seq);
        std::cout << "<<< " << seq << std::endl;
        
        std::string sUser;
        size_t len;
        readStream.Read(&sUser, 0, len);
        std::cout << "<<< " << sUser << std::endl;
    }

    TcpClient client_;
    LengthHeaderCodec codec_;
    MutexLock mutex_;
    TcpConnectionPtr connection_;
};

int main(int argc, char* argv[])
{
    LOG_INFO << "pid = " << getpid();
    if (argc > 2)
    {
        EventLoopThread loopThread;
        uint16_t port = static_cast<uint16_t>(atoi(argv[2]));
        InetAddress serverAddr(argv[1], port);

        std::string sendString;
        int32_t seq = 1;
        std::string sUser;

        ChatClient client(loopThread.startLoop(), serverAddr);
        client.connect();
        while (true)
        {
            sendString.clear();
            sUser.clear();
            sleep(1);
            BinaryWriteStream writeStream(&sendString);
            std::cout << "user: ";
            std::cin >> sUser;

            writeStream.Write(seq);
            writeStream.Write(sUser.c_str(), sUser.length());
            writeStream.Flush();
            client.write(sendString);
            seq++;
        }
        client.disconnect();
        CurrentThread::sleepUsec(1000*1000);  // wait for disconnect, see ace/logging/client.cc
    }
    else
    {
        printf("Usage: %s host_ip port\n", argv[0]);
    }
}

