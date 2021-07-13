package bgu.spl.net.srv;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;
import java.util.concurrent.atomic.AtomicInteger;

public class BlockingConnectionHandler<T> implements Runnable, ConnectionHandler<T> {

    private final MessagingProtocol<T> protocol;
    private final MessageEncoderDecoder<T> encdec;
    private final Socket sock;
    private BufferedInputStream in;
    private BufferedOutputStream out;
    private volatile boolean connected = true;
    private  static AtomicInteger connectionIdGenerator = new AtomicInteger(0);
    private Integer connectionId;
    private ConnectionsImpl connections;

    public BlockingConnectionHandler(Socket sock, MessageEncoderDecoder<T> reader, MessagingProtocol<T> protocol,ConnectionsImpl connections) {
        this.sock = sock;
        this.encdec = reader;
        this.protocol = protocol;
        this.connections=connections;
    }

    @Override
    public void run() {
        try (Socket sock = this.sock) { //just for automatic closing
            int read;
            in = new BufferedInputStream(sock.getInputStream());
            out = new BufferedOutputStream(sock.getOutputStream());
            connectionId = connectionIdGenerator.addAndGet(1);
            ((StompProtocol)protocol).start(connectionId,connections);
            while (!protocol.shouldTerminate() && connected && (read = in.read()) >= 0) {
                T nextMessage = encdec.decodeNextByte((byte) read);
                if (nextMessage != null) {
                    T response = protocol.process(nextMessage);
                    if((response != null) && ((String)response).startsWith("CONNECTED"))
                        connections.addNewClient(this, connectionId);
                    if (response != null) {
                        out.write(encdec.encode(response));
                        out.flush();
                        if(((String)response).startsWith("ERROR") | ((String)nextMessage).startsWith("DISCONNECT"))
                            close();
                    }
                }
            }

        } catch (IOException ex) {
            ex.printStackTrace();
        }

    }

    @Override
    public void close() throws IOException {
        connected = false;
        sock.close();
    }

    @Override
    public void send(T msg) {
        try
        {
            Socket sock = this.sock;
            if (msg != null) {
               out.write(encdec.encode(msg));
               out.flush();
        }
        }
        catch (Exception e)
        {

        }

    }
}
