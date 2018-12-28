
#include "Socket.h"
#include <limits.h>

//https://www.thegeekstuff.com/2011/12/c-socket-programming/
//sudo apt-get install openssl
//sudo apt-get install libssl-dev
//sudo apt-get update



#ifdef _WIN32

int Socket_Recv(Socket socket, void *buf, size_t n, int flags, struct Error* error)
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
#include <errno.h>

const char* GetSocketErrorA(int error)
{
    switch (error)
    {
    case EPERM:      return " Operation not permitted ";
    case  ENOENT:      return " No such file or directory ";
    case  ESRCH:return " No such process ";
    case  EINTR:      return " Interrupted system call ";
    case  EIO:     return " I/O error ";
    case  ENXIO:      return " No such device or address ";
    case  E2BIG:      return " Arg list too long ";
    case  ENOEXEC:      return " Exec format error ";
    case  EBADF:      return " Bad file number ";
    case  ECHILD:      return " No child processes ";
    case  EAGAIN:      return " Try again ";
    case  ENOMEM:      return " Out of memory ";
    case  EACCES:      return " Permission denied ";
    case  EFAULT:      return " Bad address ";
    case  ENOTBLK:     return " Block device required ";
    case  EBUSY:      return " Device or resource busy ";
    case  EEXIST:     return " File exists ";
    case  EXDEV:      return " Cross-device link ";
    case  ENODEV:      return " No such device ";
    case  ENOTDIR:      return " Not a directory ";
    case  EISDIR:      return " Is a directory ";
    case  EINVAL:      return " Invalid argument ";
    case  ENFILE:      return " File table overflow ";
    case  EMFILE:      return " Too many open files ";
    case  ENOTTY:      return " Not a typewriter ";
    case  ETXTBSY:      return " Text file busy ";
    case  EFBIG:      return " File too large ";
    case  ENOSPC:      return " No space left on device ";
    case  ESPIPE:      return " Illegal seek ";
    case  EROFS:      return " Read-only file system ";
    case  EMLINK:      return " Too many links ";
    case  EPIPE:      return " Broken pipe ";
    case  EDOM:      return " Math argument out of domain of func ";
    case  ERANGE:      return " Math result not representable ";
    case  EDEADLK:      return " Resource deadlock would occur ";
    case  ENAMETOOLONG:      return " File name too long ";
    case  ENOLCK:      return " No record locks available ";
    case  ENOSYS:      return " Function not implemented ";
    case  ENOTEMPTY:      return " Directory not empty ";
    case  ELOOP:      return " Too many symbolic links encountered ";
        //case  EWOULDBLOCK:
        //case EAGAIN:  return " Operation would block ";
    case  ENOMSG:      return " No message of desired type ";
    case  EIDRM:      return " Identifier removed ";
    case  ECHRNG:      return " Channel number out of range ";
    case  EL2NSYNC:      return " Level 2 not synchronized ";
    case  EL3HLT:      return " Level 3 halted ";
    case  EL3RST:      return " Level 3 reset ";
    case  ELNRNG:      return " Link number out of range ";
    case  EUNATCH:      return " Protocol driver not attached ";
    case  ENOCSI:      return " No CSI structure available ";
    case  EL2HLT:      return " Level 2 halted ";
    case  EBADE:      return " Invalid exchange ";
    case  EBADR:      return " Invalid request descriptor ";
    case  EXFULL:      return " Exchange full ";
    case  ENOANO:      return " No anode ";
    case  EBADRQC:      return " Invalid request code ";
    case  EBADSLT:      return " Invalid slot ";

        //case  EDEADLOCK:
        //case EDEADLK:

    case  EBFONT:      return " Bad font file format ";
    case  ENOSTR:      return " Device not a stream ";
    case  ENODATA:      return " No data available ";
    case  ETIME:      return " Timer expired ";
    case  ENOSR:      return " Out of streams resources ";
    case  ENONET:      return " Machine is not on the network ";
    case  ENOPKG:      return " Package not installed ";
    case  EREMOTE:      return " Object is remote ";
    case  ENOLINK:      return " Link has been severed ";
    case  EADV:      return " Advertise error ";
    case  ESRMNT:      return " Srmount error ";
    case  ECOMM:      return " Communication error on send ";
    case  EPROTO:      return " Protocol error ";
    case  EMULTIHOP:      return " Multihop attempted ";
    case  EDOTDOT:      return " RFS specific error ";
    case  EBADMSG:      return " Not a data message ";
    case  EOVERFLOW:      return " Value too large for defined data type ";
    case  ENOTUNIQ:      return " Name not unique on network ";
    case  EBADFD:      return " File descriptor in bad state ";
    case  EREMCHG:      return " Remote address changed ";
    case  ELIBACC:      return " Can not access a needed shared library ";
    case  ELIBBAD:      return " Accessing a corrupted shared library ";
    case  ELIBSCN:      return " .lib section in a.out corrupted ";
    case  ELIBMAX:      return " Attempting to link in too many shared libraries ";
    case  ELIBEXEC:      return " Cannot exec a shared library directly ";
    case  EILSEQ:      return " Illegal byte sequence ";
    case  ERESTART:      return " Interrupted system call should be restarted ";
    case  ESTRPIPE:      return " Streams pipe error ";
    case  EUSERS:      return " Too many users ";
    case  ENOTSOCK:      return " Socket operation on non-socket ";
    case  EDESTADDRREQ:      return " Destination address required ";
    case  EMSGSIZE:      return " Message too long ";
    case  EPROTOTYPE:      return " Protocol wrong type for socket ";
    case  ENOPROTOOPT:      return " Protocol not available ";
    case  EPROTONOSUPPORT:      return " Protocol not supported ";
    case  ESOCKTNOSUPPORT:      return " Socket type not supported ";
    case  EOPNOTSUPP:      return " Operation not supported on transport endpoint ";
    case  EPFNOSUPPORT:      return " Protocol family not supported ";
    case  EAFNOSUPPORT:      return " Address family not supported by protocol ";
    case  EADDRINUSE:      return " Address already in use ";
    case  EADDRNOTAVAIL:      return " Cannot assign requested address ";
    case  ENETDOWN:     return " Network is down ";
    case  ENETUNREACH:     return " Network is unreachable ";
    case  ENETRESET:     return " Network dropped connection because of reset ";
    case  ECONNABORTED:     return " Software caused connection abort ";
    case  ECONNRESET:     return " Connection reset by peer ";
    case  ENOBUFS:     return " No buffer space available ";
    case  EISCONN:     return " Transport endpoint is already connected ";
    case  ENOTCONN:     return " Transport endpoint is not connected ";
    case  ESHUTDOWN:     return " Cannot send after transport endpoint shutdown ";
    case  ETOOMANYREFS:     return " Too many references: cannot splice ";
    case  ETIMEDOUT:     return " Connection timed out ";
    case  ECONNREFUSED:     return " Connection refused ";
    case  EHOSTDOWN:     return " Host is down ";
    case  EHOSTUNREACH:     return " No route to host ";
    case  EALREADY:     return " Operation already in progress ";
    case  EINPROGRESS:     return " Operation now in progress ";
    case  ESTALE:     return " Stale NFS file handle ";
    case  EUCLEAN:     return " Structure needs cleaning ";
    case  ENOTNAM:     return " Not a XENIX named type file ";
    case  ENAVAIL:     return " No XENIX semaphores available ";
    case  EISNAM:     return " Is a named type file ";
    case  EREMOTEIO:     return " Remote I/O error ";
    case  EDQUOT:     return " Quota exceeded ";

    case  ENOMEDIUM:     return " No medium found ";
    case  EMEDIUMTYPE:     return " Wrong medium type";
    default:
        break;

    }

    return "Unknown";
}


void SocketStaticInit()
{
}
void SocketStaticDestroy()
{
}

int Socket_Recv(Socket socket, void *buf, size_t n, int flags, struct Error* error)
{
    return recv(socket, buf, n, flags);
}
#endif

void SysGetLastError(struct Error* error)
{
#ifdef _WIN32
    int errorCode = WSAGetLastError();
    Error_Set(error, "%s", GetSocketErrorA(errorCode));

#else
    int errorCode = errno;
    //Error_Set(error, "%s", GetSocketErrorA(errorCode));
    //strerror_r(errnum, error->Msg, sizeof(error->Msg));
    Error_Set(error, "%s", GetSocketErrorA(errorCode));
#endif
}

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
    size_t len, struct Error* error)
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


int Socket_SetTimeout(Socket sock, int milliseconds, struct Error* error)
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

bool Socket_Bind(Socket socket,
    const struct sockaddr * name,
    int namelen,
    struct Error* error
)
{
    int i = bind(socket, name, namelen);
    if (i == SOCKET_ERROR)
    {
        SysGetLastError(error);
    }
    return i != SOCKET_ERROR;
}


bool Socket_Listen(Socket socket, int backlog, struct Error* error)
{
    int i = listen(socket, backlog);
    if (i == SOCKET_ERROR)
    {
        SysGetLastError(error);
    }
    return i != SOCKET_ERROR;
}

Socket Socket_Accept(Socket socket, struct sockaddr * addr, int *addrlen, struct Error* error)
{
    Socket s = accept(socket, addr, addrlen);

    if (s == INVALID_SOCKET)
    {
        SysGetLastError(error);
    }
    return s;
}


Socket Socket_Create(int family,
    int socktype,
    int protocol,
    struct Error* error
)
{
    Socket s = socket(family,
        socktype,
        protocol);
    if (s == NULL)
    {
        SysGetLastError(error);
    }
    return s;
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

