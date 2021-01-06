# 1.TcpClient的要点
* 1.包含一个Connector（对应Acceptor）用于建立连接。Connector中socket返回可写后，触发POLLOUT事件；Channel回调connector的```handleWrite```函数;此时需要及时取消对可写事件的关注，否则会busy loop；由于可写不一定代表连接成功，所以要进一步检查状态，未成功则及时重连
* 2.连接成功后，Connector回调设置的```TcpClient::newConnection()```，在其中创建一个TcpConnectionPtr，设置并保存这个连接。此时socket就由该连接接管了。
* 3.同样的，不能在回调时直接reset```ChannelPtr```，因为此时还在```Channel::handleEvent()```当中。

# 2.和TcpServer结构类似，而不太一样
* 1.Acceptor是通过通道监听listen_fd的可读事件；有新连接到来时，在handleRead中生成新的conn_fd，并且回调```TcpClient::newConnection()```，在其中创建一个TcpConnectionPtr，设置并保存这个连接。每次都会创建一个新的socket_fd。
* 2.Connector是创建一个conn_fd，与服务器连接，通过通道监听连接的状态。返回POLLOUT后检查连接状态，如果连接成功，则回调```TcpClient::newConnection()```，在其中创建一个TcpConnectionPtr，设置并保存这个连接。至始至终只有这一个socket_fd。
* 3.但是创建```TcpConnection```时，都是同样的，将用户指定的各回调函数设置到生成```TcpConnection```
中，并且调用```conn->connectEstablished()```开始监听该连接的fd。连接到来时，根据返回的类型，```Channel```回调```handle...()```函数，然后其中调用```TcpConnection```的不同回调函数。