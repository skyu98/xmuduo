# 1.HttpServer
HttpServer的结构，其实就是对TcpServer再包装了一下。
* 1.我们知道，TcpServer中，它的两个回调函数```connectionCallback_```和```messageCallback_```，由用户指定，然后它在创建连接时绑定给conn；HttpServer中，由于我们确定发来的数据是满足Http协议的，所以我们直接指定```messageCallback_```为一个确定的可以解析协议的函数。

* 2.每个连接在生成时，会回调设置的```connectionCallback_```；这里是回调HttpServer提供的回调函数，在其中，会给每个连接绑定一个```context```对象（boost::any)。

* 3.每次有消息到来，我们用```context```解析该请求，并且用```HttpRequest```对象来保存；用户能够获取的，是这个已经解析好的```HttpRequest```对象。

* 4.对于HttpServer，用户设置的函数只有一个：
```cpp
typedef boost::function<void (const HttpRequest&,
                                HttpResponse*)> HttpCallback;
```
此函数中，给用户传入一个```HttpRequest```对象，```HttpResponse*```对象作为传入传出参数，用户对于该请求的回应则保存到该参数中传出。

* 5.```messageCallback_```中，解析好后，则会调用用户设置的```HttpCallback```函数，得到用户的回应，然后发送回去。