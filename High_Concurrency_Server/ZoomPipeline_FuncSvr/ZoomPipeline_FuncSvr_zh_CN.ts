<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE TS>
<TS version="2.1" language="zh_CN">
<context>
    <name>DialogAddressInput</name>
    <message>
        <source>Dialog</source>
        <translation type="vanished">对话框</translation>
    </message>
    <message>
        <location filename="dialogaddressinput.ui" line="14"/>
        <source>Participate in CLUSTER</source>
        <translation>加入集群</translation>
    </message>
    <message>
        <location filename="dialogaddressinput.ui" line="22"/>
        <source>Addr</source>
        <translation>地址</translation>
    </message>
    <message>
        <location filename="dialogaddressinput.ui" line="36"/>
        <source>Port</source>
        <translation>端口</translation>
    </message>
</context>
<context>
    <name>ExampleServer::st_clientNodeAppLayer</name>
    <message>
        <location filename="smartlink/st_clientnode_app_imp.cpp" line="104"/>
        <source>Database Access Error :</source>
        <translation>数据库访问错误 :</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_app_imp.cpp" line="134"/>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="249"/>
        <source>Broken Message stMsg_UploadUserListReq, size not correct.</source>
        <translation>消息长度不匹配. stMsg_UploadUserListReq.</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="27"/>
        <source>try to get relations Failed! </source>
        <translation>试图获取权属关系失败!</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="59"/>
        <source>try to del old relations Failed! </source>
        <translation>试图删除旧的权属关系失败!</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="79"/>
        <source>try to insert new relations Failed! </source>
        <translation>试图插入新的权属关系失败!</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="110"/>
        <source>To-server Message Failed.</source>
        <translation>发往服务器的消息解译/处理失败.</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="122"/>
        <source>Client To Server Message Failed.</source>
        <translation>客户端发往主机的程序解译失败.</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="129"/>
        <source>warning, UUID 0xFFFFFFFF.ignore</source>
        <translation>警告, UUID为 0xFFFFFFF 的消息被忽略</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="135"/>
        <source>Bad UUID %1. Client Kicked out</source>
        <translation>非法的 UUID %1. 立刻踢出套接字</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="150"/>
        <source>Destin ID </source>
        <translation>目的节点ID </translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="150"/>
        <source> is not currently logged in.</source>
        <translation>尚未在本服务器登记。</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="200"/>
        <source>Broken Message stMsg_ClientLoginReq, size not correct.</source>
        <translation>消息长度不匹配. stMsg_ClientLoginReq.</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="207"/>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="287"/>
        <source>Message type not supported.</source>
        <translation>消息类型不被支持.</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="264"/>
        <source>Broken Message stMsg_DownloadUserListReq, size not correct.</source>
        <translation>消息长度不匹配. stMsg_DownloadUserListReq.</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_applayer.cpp" line="279"/>
        <source>Broken Message stMsg_ClientLogoutReq, size not correct.</source>
        <translation>消息长度不匹配. stMsg_ClientLogoutReq.</translation>
    </message>
</context>
<context>
    <name>ExampleServer::st_clientNode_baseTrans</name>
    <message>
        <location filename="smartlink/st_clientnode_basetrans.cpp" line="227"/>
        <source>Client Send a unknown start Header %1 %2. Close client immediately.</source>
        <translation>客户发出了一个未知的开始头 %1 %2. 立刻关闭连接.</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_basetrans.cpp" line="257"/>
        <location filename="smartlink/st_clientnode_basetrans.cpp" line="270"/>
        <source>Client ID is invalid! Close client immediatly.</source>
        <translation>节点的ID是不合标准的，立刻关闭该节点的套接字。</translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_basetrans.cpp" line="277"/>
        <source>Client ID Changed in Runtime! Close client immediatly.</source>
        <translation></translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_basetrans.cpp" line="293"/>
        <source>Client </source>
        <translation>节点 </translation>
    </message>
    <message>
        <location filename="smartlink/st_clientnode_basetrans.cpp" line="293"/>
        <source> is dead, kick out.</source>
        <translation>丢失心跳，终止该节点的连接。</translation>
    </message>
</context>
<context>
    <name>ExampleServer::st_client_table</name>
    <message>
        <location filename="smartlink/st_client_table.cpp" line="314"/>
        <source>Send Initial UUIDs to Remote Svr:</source>
        <translation>打包发送目前所有的UUID到远程服务器：</translation>
    </message>
    <message>
        <location filename="smartlink/st_client_table.cpp" line="362"/>
        <source>Recieved %1 bytes Msg from </source>
        <translation>收到了 %1 字节数据,来源</translation>
    </message>
    <message>
        <location filename="smartlink/st_client_table.cpp" line="387"/>
        <source>Recieved remote %1 client uuid(s) from svr </source>
        <translation>收到了远端服务器拥有的 %1 个客户端UUID. 远端服务器 </translation>
    </message>
    <message>
        <location filename="smartlink/st_client_table.cpp" line="415"/>
        <source>Removed remote %1 client uuid(s) from svr </source>
        <translation>移除了 %1 个属于远端服务器的 UUID. 远端服务器 </translation>
    </message>
</context>
<context>
    <name>MainDialog</name>
    <message>
        <location filename="../FunctionalClientTest/maindialog.ui" line="14"/>
        <source>MainDialog</source>
        <translation>功能测试客户端</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.ui" line="24"/>
        <source>Control</source>
        <translation>物理连接控制</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.ui" line="32"/>
        <source>IP</source>
        <translation>IP</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.ui" line="42"/>
        <source>Port</source>
        <translation>端口</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.ui" line="52"/>
        <source>Connect</source>
        <translation>连接</translation>
    </message>
    <message>
        <source>BoxToServer</source>
        <translation type="vanished">主机和服务器测试</translation>
    </message>
    <message>
        <source>login</source>
        <translation type="vanished">登入</translation>
    </message>
    <message>
        <source>Box Serial Number</source>
        <translation type="vanished">主机序列号</translation>
    </message>
    <message>
        <source>Box ID </source>
        <translation type="vanished">主机ID</translation>
    </message>
    <message>
        <source>Regisit</source>
        <translation type="vanished">注册</translation>
    </message>
    <message>
        <source>Log in</source>
        <translation type="vanished">登入</translation>
    </message>
    <message>
        <source>Time Correct</source>
        <translation type="vanished">时间校对</translation>
    </message>
    <message>
        <source>Correct</source>
        <translation type="vanished">校时</translation>
    </message>
    <message>
        <source>relations</source>
        <translation type="vanished">权属关系</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.ui" line="139"/>
        <source>Upload Relations ( , splitted)</source>
        <translation>上传下述关系(用逗号分隔各个客户端ID)</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.ui" line="159"/>
        <source>Upload</source>
        <translation>上传</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.ui" line="105"/>
        <source>download Relations</source>
        <translation>下载权属关系</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.ui" line="63"/>
        <source>user_id</source>
        <translation>用户ID</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.ui" line="125"/>
        <source>Download</source>
        <translation>下载</translation>
    </message>
    <message>
        <source>ClientToServer</source>
        <translation type="vanished">客户端和服务器测试</translation>
    </message>
    <message>
        <source>username</source>
        <translation type="vanished">用户名</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.ui" line="87"/>
        <source>Log out</source>
        <translation>注销</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.ui" line="94"/>
        <source>Login</source>
        <translation>登入</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.ui" line="73"/>
        <source>password</source>
        <translation>口令</translation>
    </message>
    <message>
        <source>userid</source>
        <translation type="vanished">用户ID</translation>
    </message>
    <message>
        <source>Download Hosts</source>
        <translation type="vanished">下载权属关系</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.ui" line="169"/>
        <source>Send Msg</source>
        <translation>发送测试数据</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.ui" line="175"/>
        <source>Msg</source>
        <translation>发送数据</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.ui" line="187"/>
        <source>Send To UUID:</source>
        <translation>发送到UUID:</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.ui" line="210"/>
        <source>Send</source>
        <translation>发送</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.ui" line="219"/>
        <source>Recieved:</source>
        <translation>收到的数据</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.ui" line="230"/>
        <source>Logs</source>
        <translation>记录</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="350"/>
        <source>Recieved Heart-beating msg sended %1 sec(s) ago.</source>
        <translation>收到了 %1 秒前发出的心跳测试.</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="419"/>
        <source>Client Send a unknown start Header %1 %2. Close client immediately.</source>
        <translation>客户发出了一个未知的开始头 %1 %2. 立刻关闭连接.</translation>
    </message>
    <message>
        <source>Res = %1, ID = %2</source>
        <translation type="vanished">结果 %1 ID %2</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="452"/>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="457"/>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="471"/>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="483"/>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="501"/>
        <source>Succeed!</source>
        <translation>成功！</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="452"/>
        <source>Log in succeed!</source>
        <translation>登入成功！</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="457"/>
        <source>But you can connect to another idle svr:%1:%2!</source>
        <translation>但是，您可以连接到一个更加空闲的服务器 %1:%2!</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="463"/>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="473"/>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="492"/>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="503"/>
        <source>Failed!</source>
        <translation>失败！</translation>
    </message>
    <message>
        <source>Log in Failed!</source>
        <translation type="vanished">登入失败！</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="464"/>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="474"/>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="493"/>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="504"/>
        <source>Res = %1</source>
        <translation>结果 %1 </translation>
    </message>
    <message>
        <source>Time Corrected!</source>
        <translation type="vanished">时间校对成功！</translation>
    </message>
    <message>
        <source>Time Correct Failed!</source>
        <translation type="vanished">时间校对失败！</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="463"/>
        <source>Log in failed!</source>
        <translation>登入失败！</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="471"/>
        <source>upload succeed!</source>
        <translation>上传成功！</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="473"/>
        <source>upload in Failed!</source>
        <translation>上传失败！</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="483"/>
        <source>download succeed!</source>
        <translation>下载成功!</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="492"/>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="503"/>
        <source>download in Failed!</source>
        <translation>下载失败!</translation>
    </message>
    <message>
        <location filename="../FunctionalClientTest/maindialog.cpp" line="501"/>
        <source>log out succeed!</source>
        <translation>注销成功!</translation>
    </message>
</context>
<context>
    <name>QTcpClientTest</name>
    <message>
        <location filename="../QTcpClientTest/qtcpclienttest.cpp" line="308"/>
        <source>Open UUID global Syn File</source>
        <translation>打开UUID全局同步文件</translation>
    </message>
</context>
<context>
    <name>QTcpClientTestClass</name>
    <message>
        <location filename="../QTcpClientTest/qtcpclienttest.ui" line="14"/>
        <source>QTcpClientTest</source>
        <translation>QTcpClientTest SSL客户端测试</translation>
    </message>
    <message>
        <location filename="../QTcpClientTest/qtcpclienttest.ui" line="21"/>
        <source>Message</source>
        <translation>信息</translation>
    </message>
    <message>
        <location filename="../QTcpClientTest/qtcpclienttest.ui" line="47"/>
        <source>&amp;File</source>
        <translation>文件(&amp;F)</translation>
    </message>
    <message>
        <location filename="../QTcpClientTest/qtcpclienttest.ui" line="57"/>
        <source>General</source>
        <translation>一般</translation>
    </message>
    <message>
        <location filename="../QTcpClientTest/qtcpclienttest.ui" line="73"/>
        <source>Settings</source>
        <translation>设置</translation>
    </message>
    <message>
        <location filename="../QTcpClientTest/qtcpclienttest.ui" line="85"/>
        <source>IP</source>
        <translation>IP</translation>
    </message>
    <message>
        <location filename="../QTcpClientTest/qtcpclienttest.ui" line="99"/>
        <source>Port</source>
        <translation>端口</translation>
    </message>
    <message>
        <location filename="../QTcpClientTest/qtcpclienttest.ui" line="111"/>
        <source>SSL(Need Certs)</source>
        <translation>SSL连接</translation>
    </message>
    <message>
        <location filename="../QTcpClientTest/qtcpclienttest.ui" line="118"/>
        <source>Global UUID file</source>
        <translation>全局UUID同步文件名</translation>
    </message>
    <message>
        <location filename="../QTcpClientTest/qtcpclienttest.ui" line="130"/>
        <source>...</source>
        <translation>...</translation>
    </message>
    <message>
        <location filename="../QTcpClientTest/qtcpclienttest.ui" line="139"/>
        <source>Simulation Clients Max</source>
        <translation>最大连接客户端数</translation>
    </message>
    <message>
        <location filename="../QTcpClientTest/qtcpclienttest.ui" line="178"/>
        <source>Payload mean size</source>
        <translation>发送信息平均载荷长度</translation>
    </message>
    <message>
        <location filename="../QTcpClientTest/qtcpclienttest.ui" line="230"/>
        <source>&amp;Connect</source>
        <translation>连接(&amp;C)</translation>
    </message>
    <message>
        <location filename="../QTcpClientTest/qtcpclienttest.ui" line="239"/>
        <source>E&amp;xit</source>
        <translation>退出(&amp;x)</translation>
    </message>
</context>
<context>
    <name>SmartLink::st_clientNode</name>
    <message>
        <source>Client ID is invalid! Close client immediatly.</source>
        <translation type="vanished">节点的ID是不合标准的，立刻关闭该节点的套接字。</translation>
    </message>
    <message>
        <source>To-server Message is not currently supported.</source>
        <translation type="vanished">发往服务器的消息尚未被支持。</translation>
    </message>
    <message>
        <source>Broadcast Message is not currently supported.</source>
        <translation type="vanished">广播消息尚未被支持。</translation>
    </message>
    <message>
        <source>Destin ID </source>
        <translation type="vanished">目的节点ID </translation>
    </message>
    <message>
        <source> is not currently logged in.</source>
        <translation type="vanished">尚未在本服务器登记。</translation>
    </message>
    <message>
        <source>Client </source>
        <translation type="vanished">节点 </translation>
    </message>
    <message>
        <source> is dead, kick out.</source>
        <translation type="vanished">丢失心跳，终止该节点的连接。</translation>
    </message>
</context>
<context>
    <name>SmartLink::st_clientNodeAppLayer</name>
    <message>
        <source>Database Access Error :</source>
        <translation type="vanished">数据库访问错误 :</translation>
    </message>
    <message>
        <source>Broken Message stMsg_UploadUserListReq, size not correct.</source>
        <translation type="vanished">消息长度不匹配. stMsg_UploadUserListReq.</translation>
    </message>
    <message>
        <source>try to save relations before login!</source>
        <translation type="vanished">试图在登入前进行权属关系保存!</translation>
    </message>
    <message>
        <source>try to get relations Failed! </source>
        <translation type="vanished">试图获取权属关系失败!</translation>
    </message>
    <message>
        <source>try to del old relations Failed! </source>
        <translation type="vanished">试图删除旧的权属关系失败!</translation>
    </message>
    <message>
        <source>try to insert new relations Failed! </source>
        <translation type="vanished">试图插入新的权属关系失败!</translation>
    </message>
    <message>
        <source>To-server Message Failed.</source>
        <translation type="vanished">发往服务器的消息解译/处理失败.</translation>
    </message>
    <message>
        <source>Box To Server Message Failed.</source>
        <translation type="vanished">主机发往服务器的消息解译失败.</translation>
    </message>
    <message>
        <source>Client To Server Message Failed.</source>
        <translation type="vanished">客户端发往主机的程序解译失败.</translation>
    </message>
    <message>
        <source>warning, UUID 0xFFFFFFFF.ignore</source>
        <translation type="vanished">警告, UUID为 0xFFFFFFF 的消息被忽略</translation>
    </message>
    <message>
        <source>Bad UUID %1. Client Kicked out</source>
        <translation type="vanished">非法的 UUID %1. 立刻踢出套接字</translation>
    </message>
    <message>
        <source>Broadcast Message is not currently supported.</source>
        <translation type="vanished">广播消息尚未被支持。</translation>
    </message>
    <message>
        <source>Destin ID </source>
        <translation type="vanished">目的节点ID </translation>
    </message>
    <message>
        <source> is not currently logged in.</source>
        <translation type="vanished">尚未在本服务器登记。</translation>
    </message>
    <message>
        <source>Application Layer Version too new.</source>
        <translation type="vanished">应用层协议版本太高。</translation>
    </message>
    <message>
        <source>Broken Message stMsg_HostRegistReq, size not correct.</source>
        <translation type="vanished">消息长度不匹配. stMsg_HostRegistReq.</translation>
    </message>
    <message>
        <source>Broken Message stMsg_HostLogonReq, size not correct.</source>
        <translation type="vanished">消息长度不匹配. stMsg_HostLogonReq.</translation>
    </message>
    <message>
        <source>Broken Message stMsg_ClientLoginReq, size not correct.</source>
        <translation type="vanished">消息长度不匹配. stMsg_ClientLoginReq.</translation>
    </message>
    <message>
        <source>Message type not supported.</source>
        <translation type="vanished">消息类型不被支持.</translation>
    </message>
    <message>
        <source>Broken Message stMsg_DownloadUserListReq, size not correct.</source>
        <translation type="vanished">消息长度不匹配. stMsg_DownloadUserListReq.</translation>
    </message>
    <message>
        <source>Broken Message stMsg_ClientLogoutReq, size not correct.</source>
        <translation type="vanished">消息长度不匹配. stMsg_ClientLogoutReq.</translation>
    </message>
    <message>
        <source>Broken Message stMsg_GetHostListReq, size not correct.</source>
        <translation type="vanished">消息长度不匹配. stMsg_GetHostListReq.</translation>
    </message>
</context>
<context>
    <name>SmartLink::st_clientNode_baseTrans</name>
    <message>
        <source>Client Send a unknown start Header %1 %2. Close client immediately.</source>
        <translation type="vanished">客户发出了一个未知的开始头 %1 %2. 立刻关闭连接.</translation>
    </message>
    <message>
        <source>Client ID is invalid! Close client immediatly.</source>
        <translation type="vanished">节点的ID是不合标准的，立刻关闭该节点的套接字。</translation>
    </message>
    <message>
        <source>Client </source>
        <translation type="vanished">节点 </translation>
    </message>
    <message>
        <source> is dead, kick out.</source>
        <translation type="vanished">丢失心跳，终止该节点的连接。</translation>
    </message>
</context>
<context>
    <name>SmartLink::st_client_table</name>
    <message>
        <source>Send Initial UUIDs to Remote Svr:</source>
        <translation type="vanished">打包发送目前所有的UUID到远程服务器：</translation>
    </message>
    <message>
        <source>Recieved remote user-data to uuid:%1,DATA:%2</source>
        <translation type="vanished">从远端服务器收到了来自UUID:%1 的数据: %2</translation>
    </message>
    <message>
        <source>Recieved %1 bytes Msg from </source>
        <translation type="vanished">收到了 %1 字节数据,来源</translation>
    </message>
    <message>
        <source>Recieved remote %1 client uuid(s) from svr </source>
        <translation type="vanished">收到了远端服务器拥有的 %1 个客户端UUID. 远端服务器 </translation>
    </message>
    <message>
        <source>Removed remote %1 client uuid(s) from svr </source>
        <translation type="vanished">移除了 %1 个属于远端服务器的 UUID. 远端服务器 </translation>
    </message>
</context>
<context>
    <name>ZPDatabase::DatabaseResource</name>
    <message>
        <location filename="database/databaseresource.cpp" line="27"/>
        <location filename="database/databaseresource.cpp" line="61"/>
        <source> Connection name </source>
        <translation>数据库连接名称  </translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="27"/>
        <location filename="database/databaseresource.cpp" line="61"/>
        <source> does not exist.</source>
        <translation>尚未定义。</translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="55"/>
        <location filename="database/databaseresource.cpp" line="114"/>
        <source> Connection removed </source>
        <translation>数据库连接已经移除 </translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="55"/>
        <location filename="database/databaseresource.cpp" line="114"/>
        <source> .</source>
        <translation>。</translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="128"/>
        <location filename="database/databaseresource.cpp" line="132"/>
        <location filename="database/databaseresource.cpp" line="169"/>
        <location filename="database/databaseresource.cpp" line="197"/>
        <location filename="database/databaseresource.cpp" line="202"/>
        <location filename="database/databaseresource.cpp" line="221"/>
        <location filename="database/databaseresource.cpp" line="225"/>
        <source> Connection  </source>
        <translation>数据库连接 </translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="128"/>
        <source> Established.</source>
        <translation>已经建立。</translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="132"/>
        <location filename="database/databaseresource.cpp" line="202"/>
        <location filename="database/databaseresource.cpp" line="225"/>
        <source> Can&apos;t be opened. MSG=</source>
        <translation>无法建立，错误消息=</translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="152"/>
        <location filename="database/databaseresource.cpp" line="184"/>
        <source> Connection </source>
        <translation>数据库连接</translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="152"/>
        <source> has not been added.</source>
        <translation>尚未建立.</translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="169"/>
        <source> confirm failed. MSG=</source>
        <translation>数据库连接确认失败，出错提示=</translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="184"/>
        <source> has not been opened.</source>
        <translation>尚未建立。</translation>
    </message>
    <message>
        <location filename="database/databaseresource.cpp" line="197"/>
        <location filename="database/databaseresource.cpp" line="221"/>
        <source> Re-Established.</source>
        <translation>已经恢复正常。</translation>
    </message>
</context>
<context>
    <name>ZPMainFrame</name>
    <message>
        <location filename="zpmainframe.ui" line="14"/>
        <source>ZPMainFrame</source>
        <translation>ZPMainFrame 伸缩流水线服务器框架</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="47"/>
        <source>Summary</source>
        <translation>概况</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="61"/>
        <source>Settings</source>
        <translation>设置</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="81"/>
        <source>Listeners</source>
        <translation>监听器</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="92"/>
        <location filename="zpmainframe.ui" line="353"/>
        <location filename="zpmainframe.cpp" line="126"/>
        <location filename="zpmainframe.cpp" line="134"/>
        <location filename="zpmainframe.cpp" line="153"/>
        <source>Name</source>
        <translation>名称</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="102"/>
        <location filename="zpmainframe.cpp" line="127"/>
        <source>Addr</source>
        <translation>地址</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="112"/>
        <location filename="zpmainframe.cpp" line="128"/>
        <location filename="zpmainframe.cpp" line="137"/>
        <source>Port</source>
        <translation></translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="122"/>
        <source>SSL Connection</source>
        <translation>使用SSL连接</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="146"/>
        <location filename="zpmainframe.ui" line="491"/>
        <source>Add</source>
        <translation>添加</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="153"/>
        <source>Del</source>
        <translation>删除</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="160"/>
        <location filename="zpmainframe.ui" line="313"/>
        <location filename="zpmainframe.ui" line="505"/>
        <source>Save</source>
        <translation>保存</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="178"/>
        <source>Task Pools</source>
        <translation>任务池</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="212"/>
        <location filename="zpmainframe.ui" line="238"/>
        <location filename="zpmainframe.ui" line="290"/>
        <source>%v</source>
        <translation>%v</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="219"/>
        <source>Working Task Threads</source>
        <translation>逻辑处理线程数目</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="267"/>
        <source>SSL connection Trans Threads</source>
        <translation>SSL 连接传输线程数目</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="274"/>
        <source>Plain connection Trans Threads</source>
        <translation>普通TCP连接传输线程数目</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="342"/>
        <location filename="zpmainframe.ui" line="948"/>
        <location filename="zpmainframe.cpp" line="138"/>
        <source>Database</source>
        <translation>数据库资源</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="363"/>
        <location filename="zpmainframe.cpp" line="135"/>
        <source>Type</source>
        <translation>类型</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="390"/>
        <source>Host Address</source>
        <translation>主机地址</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="400"/>
        <source>Host Port</source>
        <translation>主机端口</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="410"/>
        <source>Database Name</source>
        <translation>数据库实例</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="437"/>
        <source>User</source>
        <translation>用户名</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="447"/>
        <source>Password</source>
        <translation>口令</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="498"/>
        <source>Remove</source>
        <translation>移除</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="523"/>
        <location filename="zpmainframe.ui" line="962"/>
        <source>Cluster</source>
        <translation>集群终端</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="541"/>
        <source>Terminal Listen Address</source>
        <translation>终端本地监听地址</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="551"/>
        <source>Listen Port</source>
        <translation>本地端口</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="591"/>
        <source>TransThreads</source>
        <translation>传输线程数</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="617"/>
        <source>Working Threads</source>
        <translation>处理线程数</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="645"/>
        <source>Active Terminals</source>
        <translation>可用集群终端表</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="663"/>
        <source>LAN Address</source>
        <translation>服务器局域网地址</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="673"/>
        <source>LAN Port</source>
        <translation>服务器局域网端口</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="707"/>
        <source>Publish Address</source>
        <translation>本节点对外发布地址</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="717"/>
        <source>Publish Port</source>
        <translation>发布端口</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="531"/>
        <source>Publish Name</source>
        <translation>发布名称</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="696"/>
        <location filename="zpmainframe.ui" line="896"/>
        <source>&amp;Save</source>
        <translation>保存(&amp;S)</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="740"/>
        <source>Participate</source>
        <translation>加入集群</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="754"/>
        <source>Example Server</source>
        <translation>范例服务器</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="836"/>
        <source>Cluster balance max payload</source>
        <translation>本服务器承担客户端超过门限后触发集群均衡</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="976"/>
        <source>Smartlink</source>
        <translation>Smartlink 专有设置</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="762"/>
        <source>Heart-beating</source>
        <translation>心跳检测最大保活时延</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="801"/>
        <source>seconds</source>
        <translation>秒</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="812"/>
        <source>User Account Database Name</source>
        <translation>用户信息数据库连接资源名称</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="822"/>
        <source>Main event db name</source>
        <translation>关键消息缓存数据库连接资源名称</translation>
    </message>
    <message>
        <source>Large data root folder</source>
        <translation type="vanished">本地大文件存储根路径</translation>
    </message>
    <message>
        <source>...</source>
        <translation type="vanished">...</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="914"/>
        <source>Messages</source>
        <translation>运行消息</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="934"/>
        <source>Nodes</source>
        <translation>节点</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="1003"/>
        <source>&amp;Control</source>
        <translation>控制(&amp;C)</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="1012"/>
        <source>&amp;Help</source>
        <translation>帮助(&amp;H)</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="1043"/>
        <source>E&amp;xit</source>
        <translation>退出(&amp;x)</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="1056"/>
        <source>&amp;Start/Stop</source>
        <translation>开始/结束(&amp;S)</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="1059"/>
        <source>Start or stop the server</source>
        <translation>开启或者暂停服务</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="1068"/>
        <source>&amp;About</source>
        <translation>关于(&amp;A)</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="1077"/>
        <source>&amp;Reload config file</source>
        <translation>重新加载配置文件(&amp;R)</translation>
    </message>
    <message>
        <location filename="zpmainframe.ui" line="1080"/>
        <source>Reload Config file</source>
        <translation>重新加载配置文件并调整服务器参数</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="129"/>
        <source>SSL</source>
        <translation>SSL</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="136"/>
        <source>HostAddr</source>
        <translation>服务地址</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="139"/>
        <source>Username</source>
        <translation>用户名</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="140"/>
        <source>Options</source>
        <translation>附加选项</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="141"/>
        <source>TestSQL</source>
        <translation>用于测试连接的SQL</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="154"/>
        <source>LAN_Address</source>
        <translation>服务器局域网地址</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="155"/>
        <source>LAN_Port</source>
        <translation>服务器局域网端口</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="156"/>
        <source>Clients</source>
        <translation>拥有客户端</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="157"/>
        <source>Pub_Address</source>
        <translation>本节点对外发布地址</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="158"/>
        <source>Pub_Port</source>
        <translation>发布端口</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="244"/>
        <source>Current Listen Threads: %1
</source>
        <translation>监听端口: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="246"/>
        <source>	Listen Threads %1: %2
</source>
        <translation>	线程%1 监听 %2 
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="249"/>
        <source>Current Trans Threads: %1
</source>
        <translation>传输线程: %1
</translation>
    </message>
    <message>
        <source>	Trans Threads %1 hold %2 Client Sockets.
</source>
        <translation type="vanished">	传输线程 %1 负责 %2 个节点.
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="86"/>
        <source>Confire Exit</source>
        <translation>确认退出</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="87"/>
        <source>There are still some clients alive in the server. continue waiting?</source>
        <translation>有尚未主动退出的节点套接字，是否继续等待?</translation>
    </message>
    <message>
        <source>Address</source>
        <translation type="vanished">发布地址</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="168"/>
        <location filename="zpmainframe.cpp" line="179"/>
        <location filename="zpmainframe.cpp" line="190"/>
        <location filename="zpmainframe.cpp" line="216"/>
        <source>,Source=%1</source>
        <translation>,来源:%1</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="253"/>
        <location filename="zpmainframe.cpp" line="296"/>
        <source>	%1:%2</source>
        <translation></translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="264"/>
        <source>Current Task Threads: %1
</source>
        <translation>逻辑处理线程: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="265"/>
        <source>Current Task Payload: %1
</source>
        <translation>逻辑处理负荷: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="266"/>
        <source>Current Task Idle Threads: %1
</source>
        <translation>空闲逻辑处理线程:%1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="270"/>
        <source>Database Connections: %1
</source>
        <translation>数据库连接: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="274"/>
        <source>	%1 status = %2</source>
        <translation>	连接 %1 状态 %2</translation>
    </message>
    <message>
        <source>Cluster Group Paras:
</source>
        <translation type="vanished">集群终端参数:
</translation>
    </message>
    <message>
        <source>	Terminal %1 : %2, published Address: %3:%4
</source>
        <translation type="vanished">	终端 %1 : %2, 发布地址: %3:%4
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="281"/>
        <source>Cluster Paras:
</source>
        <translation>集群参数:
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="282"/>
        <source>	Terminal %1 : %2
		LAN Address: %3:%4
		 Publish: %5:%6
		balance max clients=%7
</source>
        <translation>	集群终端 %1:%2
		高速集群网地址 %3:%4
		发布公网地址  %5:%6
		集群均衡门限 %7
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="293"/>
        <source>Trans Threads: %1
</source>
        <translation>传输线程: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="301"/>
        <source>	Shift Threads: %1
</source>
        <translation>	处理线程: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="302"/>
        <source>	Shift Payload: %1
</source>
        <translation>	处理线程负荷: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="303"/>
        <source>	Shift Idle Threads: %1
</source>
        <translation>	处理线程空闲: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="306"/>
        <source>Smartlink Function Server Paras:
</source>
        <translation>Smartlink 功能服务器专有参数:
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="307"/>
        <source>	User Account Database is : %1
</source>
        <translation>	用户信息数据库: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="308"/>
        <source>	Event Database is : %1
</source>
        <translation>	重要事件数据库: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="309"/>
        <source>	Local folder for large file is : %1
</source>
        <translation>	磁盘仓库根路径: %1
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="310"/>
        <source>	Heart beating Threadhold is : %1
</source>
        <translation>	心跳保活门限 %1 秒
</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="340"/>
        <source> Rec %1B(%3 kbps)  Sent %2B(%4 kbps)</source>
        <translation>收 %1 字节(%3 kbps) 发 %2 字节(%4 kbps)</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="737"/>
        <source>Name Already Used.</source>
        <translation>名称已经存在.</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="737"/>
        <source>The listener name has been used.</source>
        <translation>监听器的名称已经被使用了。</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="742"/>
        <source>Invalid Paraments.</source>
        <translation>无效的参数.</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="742"/>
        <source>Address must be valid, Port between 1024 to 32767.</source>
        <translation>监听端口需要介于 1024 到 32767之间。</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="788"/>
        <source>Open Conf file</source>
        <translation>打开配置文件</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="789"/>
        <source>Ini files(*.ini)</source>
        <translation>Ini文件(*.ini)</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="803"/>
        <source>Name can&apos;t be empty.</source>
        <translation>名称不能为空.</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="803"/>
        <source>Database name can not be empty.</source>
        <translation>数据库资源名称不能为空。</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="808"/>
        <source>Name already exist.</source>
        <translation>名称已经被使用了。</translation>
    </message>
    <message>
        <location filename="zpmainframe.cpp" line="808"/>
        <source>Database name already exist.</source>
        <translation>名称已经被使用了。</translation>
    </message>
</context>
<context>
    <name>ZPNetwork::zp_netListenThread</name>
    <message>
        <location filename="network/zp_netlistenthread.cpp" line="40"/>
        <source>Can not start listen!</source>
        <translation>无法启动监听！</translation>
    </message>
    <message>
        <location filename="network/zp_netlistenthread.cpp" line="66"/>
        <source>Listen Closed!</source>
        <translation>监听结束！</translation>
    </message>
</context>
<context>
    <name>ZPNetwork::zp_netTransThread</name>
    <message>
        <location filename="network/zp_nettransthread.cpp" line="140"/>
        <source>Client Accepted.</source>
        <translation>接受节点的接入请求.</translation>
    </message>
    <message>
        <location filename="network/zp_nettransthread.cpp" line="207"/>
        <source>Client connected.</source>
        <translation>节点已成功连接.</translation>
    </message>
    <message>
        <location filename="network/zp_nettransthread.cpp" line="214"/>
        <source>Client Encrypted.</source>
        <translation>节点已成功建立加密连接.</translation>
    </message>
    <message>
        <location filename="network/zp_nettransthread.cpp" line="241"/>
        <location filename="network/zp_nettransthread.cpp" line="322"/>
        <source>Client Closed.</source>
        <translation>节点已成功关闭.</translation>
    </message>
</context>
<context>
    <name>ZPNetwork::zp_net_Engine</name>
    <message>
        <location filename="network/zp_net_threadpool.cpp" line="117"/>
        <source>This ID has been used.</source>
        <translation>这个ID已经被使用了。</translation>
    </message>
    <message>
        <location filename="network/zp_net_threadpool.cpp" line="157"/>
        <source>Non-zp_netListenThread type detected.</source>
        <translation>非标准线程被检测到，服务器可能受到攻击.</translation>
    </message>
    <message>
        <location filename="network/zp_net_threadpool.cpp" line="161"/>
        <source>Incomming client arriverd.</source>
        <translation>收到一个节点接入申请.</translation>
    </message>
    <message>
        <source>New Client Arriverd.</source>
        <translation type="vanished">新的节点接入。</translation>
    </message>
    <message>
        <location filename="network/zp_net_threadpool.cpp" line="191"/>
        <location filename="network/zp_net_threadpool.cpp" line="471"/>
        <source>Need Trans Thread Object for clients.</source>
        <translation>需要一些逻辑处理线程来处理具体的事件。</translation>
    </message>
</context>
<context>
    <name>ZP_Cluster::zp_ClusterNode</name>
    <message>
        <location filename="cluster/zp_clusternode.cpp" line="205"/>
        <source>Client Send a unknown start Header %1 %2. Close client immediately.</source>
        <translation>客户发出了一个未知的开始头 %1 %2. 立刻关闭连接.</translation>
    </message>
    <message>
        <location filename="cluster/zp_clusternode.cpp" line="250"/>
        <source>Info: New Svr already regisited. Ignored.</source>
        <translation>Info: 这个节点已经注册过了,忽略请求.</translation>
    </message>
    <message>
        <location filename="cluster/zp_clusternode.cpp" line="261"/>
        <source>Can not connect to it-self, Loopback connections is forbidden.</source>
        <translation>请不要连接到自己的服务器节点. 回环连接是不被支持的.</translation>
    </message>
    <message>
        <location filename="cluster/zp_clusternode.cpp" line="288"/>
        <source>Name %1 &lt;= %2, omitted.</source>
        <translation>服务器 %1 优先级低于 %2，不发起连接。</translation>
    </message>
    <message>
        <location filename="cluster/zp_clusternode.cpp" line="314"/>
        <source>Info:Unknown Msg Type got.</source>
        <translation>Info: 未知的消息类型。</translation>
    </message>
    <message>
        <location filename="cluster/zp_clusternode.cpp" line="335"/>
        <source>Client </source>
        <translation>节点 </translation>
    </message>
    <message>
        <location filename="cluster/zp_clusternode.cpp" line="335"/>
        <source> is dead, kick out.</source>
        <translation>丢失心跳，终止该节点的连接。</translation>
    </message>
</context>
<context>
    <name>ZP_Cluster::zp_ClusterTerm</name>
    <message>
        <location filename="cluster/zp_clusterterm.cpp" line="205"/>
        <source>Name is empty!</source>
        <translation>注册的服务器名称为空！</translation>
    </message>
</context>
</TS>
