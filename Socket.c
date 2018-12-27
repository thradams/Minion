
#include "Socket.h"
#include <limits.h>

//https://www.thegeekstuff.com/2011/12/c-socket-programming/
//sudo apt-get install openssl
//sudo apt-get install libssl-dev
//sudo apt-get update

#ifdef _WIN32

int Socket_Recv(Socket socket, void *buf, size_t n, int flags)
{
    return recv(socket, buf, n, flags);
}

void SocketStaticInit()
{
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
}
void SocketStaticDestroy()
{
    WSACleanup();
}


const wchar_t* GetSocketErrorW(int error)

{

  switch (error)

  {

  case WSA_INVALID_HANDLE:
    return L"WSA_INVALID_HANDLE : Specified event object handle is invalid.";




  case WSA_NOT_ENOUGH_MEMORY:
    return L"WSA_NOT_ENOUGH_MEMORY : Insufficient memory available.";




  case WSA_INVALID_PARAMETER:
    return L"WSA_INVALID_PARAMETER : One or more parameters are invalid.";




  case WSA_OPERATION_ABORTED:
    return L"WSA_OPERATION_ABORTED : Overlapped operation aborted.";




  case WSA_IO_INCOMPLETE:
    return L"WSA_IO_INCOMPLETE : Overlapped I/O event object not in signaled state.";




  case WSA_IO_PENDING:
    return L"WSA_IO_PENDING : Overlapped operations will complete later. ";




  case WSAEINTR:
    return L"WSAEINTR : Interrupted function call.";




  case WSAEBADF:
    return L"WSAEBADF : File handle is not valid.";




  case WSAEACCES:
    return L"WSAEACCES : Permission denied.";




  case WSAEFAULT:
    return L"WSAEFAULT : Bad address.";




  case WSAEINVAL:
    return L"WSAEINVAL : Invalid argument.";




  case WSAEMFILE:
    return L"WSAEMFILE : Too many open files.";




  case WSAEWOULDBLOCK:
    return L"WSAEWOULDBLOCK : Resource temporarily unavailable.";




  case WSAEINPROGRESS:
    return L"WSAEINPROGRESS : Operation now in progress.";




  case WSAEALREADY:
    return L"WSAEALREADY : Operation already in progress.";




  case WSAENOTSOCK:
    return L"WSAENOTSOCK : Socket operation on nonsocket.";




  case WSAEDESTADDRREQ:
    return L"WSAEDESTADDRREQ : Destination address required.";




  case WSAEMSGSIZE:
    return L"WSAEMSGSIZE : Message too long.";




  case WSAEPROTOTYPE:
    return L"WSAEPROTOTYPE : Protocol wrong type for socket.";




  case WSAENOPROTOOPT:
    return L"WSAENOPROTOOPT : Bad protocol option.";




  case WSAEPROTONOSUPPORT:
    return L"WSAEPROTONOSUPPORT : Protocol not supported.";




  case WSAESOCKTNOSUPPORT:
    return L"WSAESOCKTNOSUPPORT : Socket type not supported.";




  case WSAEOPNOTSUPP:
    return L"WSAEOPNOTSUPP : Operation not supported.";




  case WSAEPFNOSUPPORT:
    return L"WSAEPFNOSUPPORT : Protocol family not supported.";




  case WSAEAFNOSUPPORT:
    return L"WSAEAFNOSUPPORT : Address family not supported by protocol family.";




  case WSAEADDRINUSE:
    return L"WSAEADDRINUSE : Address already in use.";




  case WSAEADDRNOTAVAIL:
    return L"WSAEADDRNOTAVAIL : Cannot assign requested address.";




  case WSAENETDOWN:
    return L"WSAENETDOWN : Network is down.";




  case WSAENETUNREACH:
    return L"WSAENETUNREACH : Network is unreachable.";




  case WSAENETRESET:
    return L"WSAENETRESET : Network dropped connection on reset.";




  case WSAECONNABORTED:
    return L"WSAECONNABORTED : Software caused connection abort.";




  case WSAECONNRESET:
    return L"WSAECONNRESET : Connection reset by peer.";




  case WSAENOBUFS:
    return L"WSAENOBUFS : No buffer space available.";




  case WSAEISCONN:
    return L"WSAEISCONN : Socket is already connected.";




  case WSAENOTCONN:
    return L"WSAENOTCONN : Socket is not connected.";




  case WSAESHUTDOWN:
    return L"WSAESHUTDOWN : Cannot send after socket shutdown.";




  case WSAETOOMANYREFS:
    return L"WSAETOOMANYREFS : Too many references.";




  case WSAETIMEDOUT:
    return L"WSAETIMEDOUT : Connection timed out.";




  case WSAECONNREFUSED:
    return L"WSAECONNREFUSED : Connection refused.";




  case WSAELOOP:
    return L"WSAELOOP : Cannot translate name.";




  case WSAENAMETOOLONG:
    return L"WSAENAMETOOLONG : Name too long.";




  case WSAEHOSTDOWN:
    return L"WSAEHOSTDOWN : Host is down.";




  case WSAEHOSTUNREACH:
    return L"WSAEHOSTUNREACH : No route to host.";




  case WSAENOTEMPTY:
    return L"WSAENOTEMPTY : Directory not empty.";




  case WSAEPROCLIM:
    return L"WSAEPROCLIM : Too many processes.";




  case WSAEUSERS:
    return L"WSAEUSERS : User quota exceeded.";




  case WSAEDQUOT:
    return L"WSAEDQUOT : Disk quota exceeded.";




  case WSAESTALE:
    return L"WSAESTALE : Stale file handle reference.";




  case WSAEREMOTE:
    return L"WSAEREMOTE : Item is remote.";




  case WSASYSNOTREADY:
    return L"WSASYSNOTREADY : Network subsystem is unavailable.";




  case WSANOTINITIALISED:
    return L"WSANOTINITIALISED : Successful WSAStartup not yet performed.";




  case WSAEDISCON:
    return L"WSAEDISCON : Graceful shutdown in progress.";




  case WSAENOMORE:
    return L"WSAENOMORE : No more results.";




  case WSAECANCELLED:
    return L"WSAECANCELLED : Call has been canceled.";




  case WSAEINVALIDPROCTABLE:
    return L"WSAEINVALIDPROCTABLE : Procedure call table is invalid.";




  case WSAEINVALIDPROVIDER:
    return L"WSAEINVALIDPROVIDER : Service provider is invalid.";




  case WSAEPROVIDERFAILEDINIT:
    return L"WSAEPROVIDERFAILEDINIT : Service provider failed to initialize.";




  case WSASYSCALLFAILURE:
    return L"WSASYSCALLFAILURE : System call failure.";




  case WSASERVICE_NOT_FOUND:
    return L"WSASERVICE_NOT_FOUND : Service not found.";




  case WSATYPE_NOT_FOUND:
    return L"WSATYPE_NOT_FOUND : Class type not found.";




  case WSA_E_NO_MORE:
    return L"WSA_E_NO_MORE : No more results.";




  case WSA_E_CANCELLED:
    return L"WSA_E_CANCELLED : Call was canceled.";




  case WSAEREFUSED:
    return L"WSAEREFUSED : Database query was refused.";




  case WSAHOST_NOT_FOUND:
    return L"WSAHOST_NOT_FOUND : Host not found.";




  case WSATRY_AGAIN:
    return L"WSATRY_AGAIN : Nonauthoritative host not found.";




  case WSANO_RECOVERY:
    return L"WSANO_RECOVERY : This is a nonrecoverable error.";




  case WSANO_DATA:
    return L"WSANO_DATA : Valid name, no data record of requested type.";




  case WSA_QOS_RECEIVERS:
    return L"WSA_QOS_RECEIVERS : QoS receivers.";




  case WSA_QOS_SENDERS:
    return L"WSA_QOS_SENDERS : QoS senders.";




  case WSA_QOS_NO_SENDERS:
    return L"WSA_QOS_NO_SENDERS : No QoS senders.";




  case WSA_QOS_NO_RECEIVERS:
    return L"WSA_QOS_NO_RECEIVERS : QoS no receivers.";




  case WSA_QOS_REQUEST_CONFIRMED:
    return L"WSA_QOS_REQUEST_CONFIRMED : QoS request confirmed.";




  case WSA_QOS_ADMISSION_FAILURE:
    return L"WSA_QOS_ADMISSION_FAILURE : QoS admission error.";




  case WSA_QOS_POLICY_FAILURE:
    return L"WSA_QOS_POLICY_FAILURE : QoS policy failure.";




  case WSA_QOS_BAD_STYLE:
    return L"WSA_QOS_BAD_STYLE : QoS bad style.";




  case WSA_QOS_BAD_OBJECT:
    return L"WSA_QOS_BAD_OBJECT : QoS bad object.";




  case WSA_QOS_TRAFFIC_CTRL_ERROR:
    return L"WSA_QOS_TRAFFIC_CTRL_ERROR : QoS traffic control error.";




  case WSA_QOS_GENERIC_ERROR:
    return L"WSA_QOS_GENERIC_ERROR : QoS generic error.";




  case WSA_QOS_ESERVICETYPE:
    return L"WSA_QOS_ESERVICETYPE : QoS service type error.";




  case WSA_QOS_EFLOWSPEC:
    return L"WSA_QOS_EFLOWSPEC : QoS flowspec error.";




  case WSA_QOS_EPROVSPECBUF:
    return L"WSA_QOS_EPROVSPECBUF : Invalid QoS provider buffer.";




  case WSA_QOS_EFILTERSTYLE:
    return L"WSA_QOS_EFILTERSTYLE : Invalid QoS filter style.";




  case WSA_QOS_EFILTERTYPE:
    return L"WSA_QOS_EFILTERTYPE : Invalid QoS filter type.";




  case WSA_QOS_EFILTERCOUNT:
    return L"WSA_QOS_EFILTERCOUNT : Incorrect QoS filter count.";




  case WSA_QOS_EOBJLENGTH:
    return L"WSA_QOS_EOBJLENGTH : Invalid QoS object length.";




  case WSA_QOS_EFLOWCOUNT:
    return L"WSA_QOS_EFLOWCOUNT : Incorrect QoS flow count.";




  case WSA_QOS_EUNKOWNPSOBJ:
    return L"WSA_QOS_EUNKOWNPSOBJ : Unrecognized QoS object.";




  case WSA_QOS_EPOLICYOBJ:
    return L"WSA_QOS_EPOLICYOBJ : Invalid QoS policy object.";




  case WSA_QOS_EFLOWDESC:
    return L"WSA_QOS_EFLOWDESC : Invalid QoS flow descriptor.";




  case WSA_QOS_EPSFLOWSPEC:
    return L"WSA_QOS_EPSFLOWSPEC : Invalid QoS provider-specific flowspec.";




  case WSA_QOS_EPSFILTERSPEC:
    return L"WSA_QOS_EPSFILTERSPEC : Invalid QoS provider-specific filterspec.";




  case WSA_QOS_ESDMODEOBJ:
    return L"WSA_QOS_ESDMODEOBJ : Invalid QoS shape discard mode object.";




  case WSA_QOS_ESHAPERATEOBJ:
    return L"WSA_QOS_ESHAPERATEOBJ : Invalid QoS shaping rate object.";




  case WSA_QOS_RESERVED_PETYPE:
    return L"WSA_QOS_RESERVED_PETYPE : Reserved policy QoS element type.";




  default:

    //        ASSERT(false);
    break;
  }




  return L"";

}

const char* GetSocketErrorA(int error)
{
  switch (error)
  {
  case WSA_INVALID_HANDLE:
    return "WSA_INVALID_HANDLE : Specified event object handle is invalid.";

  case WSA_NOT_ENOUGH_MEMORY:
    return "WSA_NOT_ENOUGH_MEMORY : Insufficient memory available.";

  case WSA_INVALID_PARAMETER:
    return "WSA_INVALID_PARAMETER : One or more parameters are invalid.";

  case WSA_OPERATION_ABORTED:
    return "WSA_OPERATION_ABORTED : Overlapped operation aborted.";

  case WSA_IO_INCOMPLETE:
    return "WSA_IO_INCOMPLETE : Overlapped I/O event object not in signaled state.";

  case WSA_IO_PENDING:
    return "WSA_IO_PENDING : Overlapped operations will complete later. ";

  case WSAEINTR:
    return "WSAEINTR : Interrupted function call.";

  case WSAEBADF:
    return "WSAEBADF : File handle is not valid.";

  case WSAEACCES:
    return "WSAEACCES : Permission denied.";

  case WSAEFAULT:
    return "WSAEFAULT : Bad address.";

  case WSAEINVAL:
    return "WSAEINVAL : Invalid argument.";

  case WSAEMFILE:
    return "WSAEMFILE : Too many open files.";

  case WSAEWOULDBLOCK:
    return "WSAEWOULDBLOCK : Resource temporarily unavailable.";

  case WSAEINPROGRESS:
    return "WSAEINPROGRESS : Operation now in progress.";

  case WSAEALREADY:
    return "WSAEALREADY : Operation already in progress.";

  case WSAENOTSOCK:
    return "WSAENOTSOCK : Socket operation on nonsocket.";

  case WSAEDESTADDRREQ:
    return "WSAEDESTADDRREQ : Destination address required.";

  case WSAEMSGSIZE:
    return "WSAEMSGSIZE : Message too long.";

  case WSAEPROTOTYPE:
    return "WSAEPROTOTYPE : Protocol wrong type for socket.";

  case WSAENOPROTOOPT:
    return "WSAENOPROTOOPT : Bad protocol option.";

  case WSAEPROTONOSUPPORT:
    return "WSAEPROTONOSUPPORT : Protocol not supported.";

  case WSAESOCKTNOSUPPORT:
    return "WSAESOCKTNOSUPPORT : Socket type not supported.";

  case WSAEOPNOTSUPP:
    return "WSAEOPNOTSUPP : Operation not supported.";

  case WSAEPFNOSUPPORT:
    return "WSAEPFNOSUPPORT : Protocol family not supported.";

  case WSAEAFNOSUPPORT:
    return "WSAEAFNOSUPPORT : Address family not supported by protocol family.";

  case WSAEADDRINUSE:
    return "WSAEADDRINUSE : Address already in use.";

  case WSAEADDRNOTAVAIL:
    return "WSAEADDRNOTAVAIL : Cannot assign requested address.";

  case WSAENETDOWN:
    return "WSAENETDOWN : Network is down.";

  case WSAENETUNREACH:
    return "WSAENETUNREACH : Network is unreachable.";

  case WSAENETRESET:
    return "WSAENETRESET : Network dropped connection on reset.";

  case WSAECONNABORTED:
    return "WSAECONNABORTED : Software caused connection abort.";

  case WSAECONNRESET:
    return "WSAECONNRESET : Connection reset by peer.";

  case WSAENOBUFS:
    return "WSAENOBUFS : No buffer space available.";

  case WSAEISCONN:
    return "WSAEISCONN : Socket is already connected.";

  case WSAENOTCONN:
    return "WSAENOTCONN : Socket is not connected.";

  case WSAESHUTDOWN:
    return "WSAESHUTDOWN : Cannot send after socket shutdown.";

  case WSAETOOMANYREFS:
    return "WSAETOOMANYREFS : Too many references.";

  case WSAETIMEDOUT:
    return "WSAETIMEDOUT : Connection timed out.";

  case WSAECONNREFUSED:
    return "WSAECONNREFUSED : Connection refused.";

  case WSAELOOP:
    return "WSAELOOP : Cannot translate name.";

  case WSAENAMETOOLONG:
    return "WSAENAMETOOLONG : Name too long.";

  case WSAEHOSTDOWN:
    return "WSAEHOSTDOWN : Host is down.";

  case WSAEHOSTUNREACH:
    return "WSAEHOSTUNREACH : No route to host.";

  case WSAENOTEMPTY:
    return "WSAENOTEMPTY : Directory not empty.";

  case WSAEPROCLIM:
    return "WSAEPROCLIM : Too many processes.";

  case WSAEUSERS:
    return "WSAEUSERS : User quota exceeded.";

  case WSAEDQUOT:
    return "WSAEDQUOT : Disk quota exceeded.";

  case WSAESTALE:
    return "WSAESTALE : Stale file handle reference.";

  case WSAEREMOTE:
    return "WSAEREMOTE : Item is remote.";

  case WSASYSNOTREADY:
    return "WSASYSNOTREADY : Network subsystem is unavailable.";

  case WSANOTINITIALISED:
    return "WSANOTINITIALISED : Successful WSAStartup not yet performed.";

  case WSAEDISCON:
    return "WSAEDISCON : Graceful shutdown in progress.";

  case WSAENOMORE:
    return "WSAENOMORE : No more results.";

  case WSAECANCELLED:
    return "WSAECANCELLED : Call has been canceled.";

  case WSAEINVALIDPROCTABLE:
    return "WSAEINVALIDPROCTABLE : Procedure call table is invalid.";

  case WSAEINVALIDPROVIDER:
    return "WSAEINVALIDPROVIDER : Service provider is invalid.";

  case WSAEPROVIDERFAILEDINIT:
    return "WSAEPROVIDERFAILEDINIT : Service provider failed to initialize.";

  case WSASYSCALLFAILURE:
    return "WSASYSCALLFAILURE : System call failure.";

  case WSASERVICE_NOT_FOUND:
    return "WSASERVICE_NOT_FOUND : Service not found.";

  case WSATYPE_NOT_FOUND:
    return "WSATYPE_NOT_FOUND : Class type not found.";

  case WSA_E_NO_MORE:
    return "WSA_E_NO_MORE : No more results.";

  case WSA_E_CANCELLED:
    return "WSA_E_CANCELLED : Call was canceled.";

  case WSAEREFUSED:
    return "WSAEREFUSED : Database query was refused.";

  case WSAHOST_NOT_FOUND:
    return "WSAHOST_NOT_FOUND : Host not found.";

  case WSATRY_AGAIN:
    return "WSATRY_AGAIN : Nonauthoritative host not found.";

  case WSANO_RECOVERY:
    return "WSANO_RECOVERY : This is a nonrecoverable error.";

  case WSANO_DATA:
    return "WSANO_DATA : Valid name, no data record of requested type.";

  case WSA_QOS_RECEIVERS:
    return "WSA_QOS_RECEIVERS : QoS receivers.";

  case WSA_QOS_SENDERS:
    return "WSA_QOS_SENDERS : QoS senders.";

  case WSA_QOS_NO_SENDERS:
    return "WSA_QOS_NO_SENDERS : No QoS senders.";

  case WSA_QOS_NO_RECEIVERS:
    return "WSA_QOS_NO_RECEIVERS : QoS no receivers.";

  case WSA_QOS_REQUEST_CONFIRMED:
    return "WSA_QOS_REQUEST_CONFIRMED : QoS request confirmed.";

  case WSA_QOS_ADMISSION_FAILURE:
    return "WSA_QOS_ADMISSION_FAILURE : QoS admission error.";

  case WSA_QOS_POLICY_FAILURE:
    return "WSA_QOS_POLICY_FAILURE : QoS policy failure.";

  case WSA_QOS_BAD_STYLE:
    return "WSA_QOS_BAD_STYLE : QoS bad style.";

  case WSA_QOS_BAD_OBJECT:
    return "WSA_QOS_BAD_OBJECT : QoS bad object.";

  case WSA_QOS_TRAFFIC_CTRL_ERROR:
    return "WSA_QOS_TRAFFIC_CTRL_ERROR : QoS traffic control error.";

  case WSA_QOS_GENERIC_ERROR:
    return "WSA_QOS_GENERIC_ERROR : QoS generic error.";

  case WSA_QOS_ESERVICETYPE:
    return "WSA_QOS_ESERVICETYPE : QoS service type error.";

  case WSA_QOS_EFLOWSPEC:
    return "WSA_QOS_EFLOWSPEC : QoS flowspec error.";

  case WSA_QOS_EPROVSPECBUF:
    return "WSA_QOS_EPROVSPECBUF : Invalid QoS provider buffer.";

  case WSA_QOS_EFILTERSTYLE:
    return "WSA_QOS_EFILTERSTYLE : Invalid QoS filter style.";

  case WSA_QOS_EFILTERTYPE:
    return "WSA_QOS_EFILTERTYPE : Invalid QoS filter type.";

  case WSA_QOS_EFILTERCOUNT:
    return "WSA_QOS_EFILTERCOUNT : Incorrect QoS filter count.";

  case WSA_QOS_EOBJLENGTH:
    return "WSA_QOS_EOBJLENGTH : Invalid QoS object length.";

  case WSA_QOS_EFLOWCOUNT:
    return "WSA_QOS_EFLOWCOUNT : Incorrect QoS flow count.";

  case WSA_QOS_EUNKOWNPSOBJ:
    return "WSA_QOS_EUNKOWNPSOBJ : Unrecognized QoS object.";

  case WSA_QOS_EPOLICYOBJ:
    return "WSA_QOS_EPOLICYOBJ : Invalid QoS policy object.";

  case WSA_QOS_EFLOWDESC:
    return "WSA_QOS_EFLOWDESC : Invalid QoS flow descriptor.";

  case WSA_QOS_EPSFLOWSPEC:
    return "WSA_QOS_EPSFLOWSPEC : Invalid QoS provider-specific flowspec.";

  case WSA_QOS_EPSFILTERSPEC:
    return "WSA_QOS_EPSFILTERSPEC : Invalid QoS provider-specific filterspec.";

  case WSA_QOS_ESDMODEOBJ:
    return "WSA_QOS_ESDMODEOBJ : Invalid QoS shape discard mode object.";

  case WSA_QOS_ESHAPERATEOBJ:
    return "WSA_QOS_ESHAPERATEOBJ : Invalid QoS shaping rate object.";


  case WSA_QOS_RESERVED_PETYPE:
    return "WSA_QOS_RESERVED_PETYPE : Reserved policy QoS element type.";

  default:
    //ASSERT(false);
    break;
  }

  return "";
}
#else

void SocketStaticInit()
{
}
void SocketStaticDestroy()
{
}

int Socket_Recv(Socket socket, void *buf, size_t n, int flags)
{
    return recv(socket, buf, n, flags);
}
#endif

void Socket_Close(Socket socket)
{
#ifdef _WIN32
  /*int i =*/ closesocket(socket);
#else
    /*int i =*/ close(socket);
#endif
}



size_t Socket_PushBytes(Socket socket,
  const char* bytes,
  size_t len)
{
  //*socketerror = 0; //out

  size_t sent = 0;

#ifdef _DEBUG
  int i = 0; //só para ver quando ele nao enviar tudo
#endif

  while (sent < (long long)len)
  {
    // How many bytes we send in this iteration
    int k = len - sent > INT_MAX ? INT_MAX : (int)(len - sent);

    int bytesent = (int)send(socket, bytes + sent, k, 0);
    if (bytesent == SOCKET_ERROR)
    {
      //*socketerror = WSAGetLastError();
      return 0;
    }
    sent += bytesent;

    //ASSERT(i++ == 0);
  }
  return sent;
}


int Socket_SetTimeout(Socket sock, int milliseconds)
{
#ifdef _WIN32
  DWORD t = milliseconds;
#else
  struct timeval t;
  t.tv_sec = milliseconds / 1000;
  t.tv_usec = (milliseconds * 1000) % 1000000;
#endif
  return setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&t, sizeof(t)) ||
    setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (const char*)&t, sizeof(t));
}

int Socket_Bind(Socket socket,
  const struct sockaddr * name,
  int namelen
)
{
  int i = bind(socket, name, namelen);
  if (i == SOCKET_ERROR)
  {
    //socketerror = WSAGetLastError();
  }
  return i;
}


int Socket_Listen(Socket socket, int backlog)
{
  //socketerror = 0;//out

  int i = listen(socket, backlog);
  if (i == SOCKET_ERROR)
  {
    //socketerror = WSAGetLastError();
    //ASSERT(false);
  }
  return i;
}

Socket Socket_Accept(Socket socket, struct sockaddr * addr, int *addrlen)
{
  //socketerror = 0;//out

  Socket s = accept(socket, addr, addrlen);

  if (s == INVALID_SOCKET)
  {
    //socketerror = WSAGetLastError();
    //ASSERT(false);
  }
  return s;
}


Socket Socket_Create(int family,
  int socktype,
  int protocol
)
{
  return  socket(family,
    socktype,
    protocol);
}



bool Socket_IsReadyToReceive(Socket sock, int intervalMs)
{
  //*socketError = 0;//out

  fd_set fds;
  FD_ZERO(&fds);

#ifdef _WIN32
#pragma warning( push )
#pragma warning( disable : 4127 )
  FD_SET(sock, &fds);
#pragma warning( pop )
#else
  FD_SET(sock, &fds);
#endif

  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = intervalMs;

  int count = select((int)(sock + 1), &fds, NULL, NULL, &tv);
  if (count == SOCKET_ERROR)
  {
    //    *socketError = WSAGetLastError();
        //        std::this_thread::sleep_for(std::chrono::seconds(1));
        //  ASSERT(false);
    return false;
  }
  if (count > 0)
  {
    if (FD_ISSET(sock, &fds))
    {
      return true;
    }
    else
    {
      return false;
    }
  }
  return false;
}


void Socket_CloseGracefully(Socket socket)
{
  char buf[100];
  struct linger lg;
  int n;

  lg.l_onoff = 1;
  lg.l_linger = 1;
  setsockopt(socket, SOL_SOCKET, SO_LINGER, (const char*)&lg, sizeof(lg));
  int SHUT_WR = 1;

  shutdown(socket, SHUT_WR);
  SetNonBlockingMode(socket);

  do
  {
    n = (int)recv(socket, buf, sizeof(buf), 0);
  } while (n > 0);

  Socket_Close(socket);
  
  socket = INVALID_SOCKET;
}
 int SetNonBlockingMode(Socket sock)
{
    unsigned long on = 1;
#ifdef _WIN32
    return ioctlsocket(sock, FIONBIO, &on);
#else
    return ioctl(sock, FIONBIO, &on);
#endif
}

