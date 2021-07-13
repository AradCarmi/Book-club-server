package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.impl.newsfeed.FetchNewsCommand;
import bgu.spl.net.impl.rci.Command;
import bgu.spl.net.impl.rci.ObjectEncoderDecoder;
import bgu.spl.net.impl.rci.RemoteCommandInvocationProtocol;
import bgu.spl.net.srv.*;
import bgu.spl.net.srv.Frames.ERROR;
import bgu.spl.net.srv.Frames.Frame;

import java.io.*;
import java.nio.ByteBuffer;
import java.util.Arrays;
import java.util.function.Supplier;

public class StompServer {

    public static void main(String[] args)
    {
        int port=Integer.parseInt(args[0]);
        String type=args[1];
        if (type.equals("tpc"))
        {
            Server.threadPerClient(port, StompProtocol::new, MessageEncoderDecoderImp::new).serve();
        }
        else
        {
            Server.reactor(Runtime.getRuntime().availableProcessors(),port,StompProtocol::new,MessageEncoderDecoderImp::new).serve();
        }

    }
}
