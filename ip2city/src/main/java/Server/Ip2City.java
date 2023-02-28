package Server;
import com.github.jarod.qqwry.IPZone;
import com.github.jarod.qqwry.QQWry;

import java.io.IOException;
import java.io.PrintWriter;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;

public class Ip2City {
    private static final int PORT = 12345;

    public static void main(String[] args) throws IOException {
        ServerSocket serverSocket = new ServerSocket(PORT);
        System.out.println("Server started. Listening on port " + PORT);

        while (true) {
            try (Socket clientSocket = serverSocket.accept()) {
                InetAddress clientAddress = clientSocket.getInetAddress();
                System.out.println("New connection from " + clientAddress.getHostAddress());

                // 解析客户端IP地址对应的城市信息
                String city = getCityFromIP(clientAddress.getHostAddress());

                // 将城市信息发送给客户端
                PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true);
                out.println(city);
            } catch (IOException e) {
                System.out.println("Error handling client request: " + e.getMessage());
            }
        }
    }

    private static String getCityFromIP(String ip) throws IOException {
        QQWry wry = new QQWry();
        IPZone zone = wry.findIP("223.104.204.22");
        System.out.println(((IPZone) zone).getMainInfo());
        System.out.println(zone.getSubInfo());
        System.out.println("------");
        return zone.getMainInfo() + " " + zone.getSubInfo();
    }
}

