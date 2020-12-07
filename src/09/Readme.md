# 1.基于对象编程
**基于对象编程**：通过包含一个具体类，使得该具体类可以调用本类中的成员函数。  
对于TcpServer和EchoServer而言，就是EchoServer中包含一个TcpServer对象，并且在构造函数中，使用boost::bind()为TcpServer注册三个函数：OnConnection, OnMessage, OnClose，来达到回调效果。


# 2.C编程风格
**C编程风格**：定义三个全局函数，通过函数指针完成在TcpServer中的注册；也是通过函数指针来完成回调。