#include "PPClient.h"
#include "../../base/Log/Logger.h"
#include "../../base/NetThreadPool.h"
#include "../../base/ClientSocket.h"
#include "ServerTask.h"
#include "../../base/Timer.h"


PPClient::PPClient()
{
    m_pLog = NULL;
}

PPClient::~PPClient()
{
}

SharedPtr<StreamSocket>   PPClient::_OnNewConnection(SOCKET connfd)
{
    DBG(m_pLog) << "New tcp task " << connfd;

    SocketAddr  localAddr,  peerAddr;
    Socket::GetLocalAddr(connfd, localAddr);
    Socket::GetPeerAddr(connfd,  peerAddr);

    SharedPtr<ServerTask>    pNewTask(new ServerTask());
    if (!pNewTask->Init(connfd, localAddr.GetPort(), peerAddr.GetPort()))
    {
        pNewTask.Reset();
    }
    else
    {
        pNewTask->SendMsg(); // FOR PING_PONG TEST
    }

    return  pNewTask;
}

bool PPClient::_Init()
{
    m_pLog = LogManager::Instance().CreateLog(Logger::logALL, Logger::logALL);

    SharedPtr<ClientSocket>     pClient(new ClientSocket);
    if (!pClient->Connect("127.0.0.1", 8888))
    {
        ERR(m_pLog) << "can not connect server 8888";
        return false;
    }

    return  true;
}

bool PPClient::_RunLogic()
{
    g_now.Now();

    if (!Server::_RunLogic())
        Thread::MSleep(1);

    return  true;
}


int main()
{
    UniquePtr<PPClient>     pSvr(new PPClient);
    pSvr->MainLoop();

    return 0;
}
