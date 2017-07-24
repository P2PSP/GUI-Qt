#include "peer.h"

namespace p2psp
{

    peer::peer(QWidget *parent) : QWidget(parent), io_service_(),
        acceptor_(io_service_),
        source_socket_(io_service_),
        player_socket_(io_service_)
    {
        emit logEvent(LOG_TRACE,"Log initialized");
    }

    void peer::ReceiveSourceEndpoint()
    {

        boost::array<char, 6> buffer;
        read(splitter_socket_, ::buffer(buffer));

        char *raw_data = buffer.data();

        in_addr ip_raw = *(in_addr *)(raw_data);
        source.addr = ip::address::from_string(inet_ntoa(ip_raw));
        source.port = ntohs(*(short *)(raw_data + 4));
        QString message="Source_endpoint = ("+QString::fromStdString(source.addr.to_string())+","+QString::fromStdString(std::to_string(source.port))+")";
        emit logEvent(LOG_TRACE,message);

    }

    ip::address peer::GetSourceAddr()
    {
        return source.addr;
    }

    PORT_TYPE peer::GetSourcePort()
    {


        return source.port;


    }

    void peer::SetGETMessage()
    {


        std::stringstream ss;
        ss << "GET /" << channel_ << " HTTP/1.1\r";
        GET_message_ = ss.str();
        emit logEvent(LOG_TRACE,"GET_message = "+QString::fromStdString(GET_message_));
        ss.str("");

    }

    void peer::ReceiveChannel()
    {


        unsigned short channel_size;
        {
            std::vector<char> message(2);
            read(splitter_socket_, boost::asio::buffer(message/*,2*/));
            channel_size = ntohs(*(short *)(message.data()));
        }
        emit logEvent(LOG_TRACE,"channel_size = "+QString::number(channel_size));
        {
            std::vector<char> messagex(channel_size);
            boost::asio::read(splitter_socket_, boost::asio::buffer(messagex/*, channel_size*/));

            channel_ = std::string(messagex.data(), channel_size);
        }
        emit logEvent(LOG_TRACE,"channel = "+QString::fromStdString(channel_));
        SetGETMessage();
    }

    std::string peer::GetChannel()
    {
        return channel_;
    }

    void peer::ReceiveHeaderSize()
    {
        boost::array<char, 2> buffer;
        read(splitter_socket_, ::buffer(buffer));

        header_size_ = ntohs(*(short *)(buffer.c_array()));

        emit logEvent(LOG_TRACE,"Header_size (in bytes) = "+QString::number(header_size_));
    }

    HEADER_SIZE_TYPE peer::GetHeaderSize()
    {
        return header_size_;
    }

    void peer::ConnectToTheSource() /*throw(boost::system::system_error) */
    {
        ip::tcp::endpoint source_ep(source.addr, source.port);
        boost::system::error_code ec;
        source_socket_.connect(source_ep, ec);

        if (ec) {
            emit logEvent(LOG_ERROR,QString::fromStdString(ec.message()));
            emit logEvent(LOG_ERROR,QString::fromStdString(source_socket_.local_endpoint().address().to_string())+
                            "\b: unable to connect to the source ("+
                            QString::fromStdString(source.addr.to_string())+
                            ","+
                            QString::number(source.port)+
                            ")");
            source_socket_.close();
            exit(-1);
        }
        emit logEvent(LOG_TRACE,"Connected to the sourve at ("+
                        QString::fromStdString(source.addr.to_string())+
                        ","+
                        QString::number(source.port)+
                        ") from "+
                        QString::fromStdString(source_socket_.local_endpoint().address().to_string()));
    }

    void peer::RequestHeader()
    {
        source_socket_.send(boost::asio::buffer(GET_message_));
    }

    void peer::RelayHeader()
    {
        boost::array<char, 128> buf;
        //boost::system::error_code error;
        for (int header_load_counter_ = 0; header_load_counter_ < GetHeaderSize();) {

            //size_t len = socket.read_some(boost::asio::buffer(buf), error);
            size_t len = source_socket_.read_some(boost::asio::buffer(buf));
            header_load_counter_ += len;

            if (len <= 0) break;

            player_socket_.send(boost::asio::buffer(buf, len));
            //boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
        }
    }

    void peer::WaitForThePlayer()
    {

        ip::tcp::endpoint endpoint(ip::tcp::v4(), player_port_);

        acceptor_.open(endpoint.protocol());
        acceptor_.set_option(ip::tcp::acceptor::reuse_address(true));
        acceptor_.bind(endpoint);
        acceptor_.listen();
        emit logEvent(LOG_INFO,"Waiting for the player at ("+
                        QString::fromStdString(endpoint.address().to_string())+
                        ","+
                        QString::fromStdString(std::to_string(endpoint.port()))+
                        ")"
                        );
        acceptor_.accept(player_socket_);
        emit logEvent(LOG_TRACE,"Player connected. Player is ("+
                        QString::fromStdString(player_socket_.remote_endpoint().address().to_string())+
                        ","+
                        QString::number(player_socket_.remote_endpoint().port())+
                        ")"
                        );
    }

    bool peer::PlayChunk(int chunk)
    {
        try {
            write(player_socket_, buffer(chunk_ptr[chunk % buffer_size_].data));
            return true;
        } catch (std::exception e) {
            emit logEvent(LOG_TRACE,"Player disconnected");
            //std::cout << e.what() << std::endl;
            //player_alive_ = false;
            return false;
            //return true;
        }
    }

    void peer::SetPlayerPort(uint16_t player_port)
    {
        player_port_ = player_port;
    }

    uint16_t peer::GetPlayerPort()
    {
        return  player_port_;
    }

    uint16_t peer::GetDefaultPlayerPort()
    {
        return 9999;
    }

}
