import java.io.IOException;
import java.io.InputStream;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.*;
import java.io.*;

public class Server {

	PrintWriter out;

	Server() {}

	void run() throws IOException
	{
		ServerSocket server = new ServerSocket(3456);
		System.out.println("Creo el Server Socket");

		Socket clientSocket = new Socket("192.168.1.41", 2000);
		System.out.println("Me conecto al Arduino");

		out = new PrintWriter(clientSocket.getOutputStream(), true);

		while (true) {

			Socket socket = server.accept();

			System.out.println("Client accepted!");

			Thread t = new ClientThread(socket, out);
			t.start();

		}
	}


	public static void main(String[] args) throws IOException {
		Server server = new Server();
        server.run();
	}

}

class ClientThread extends Thread{

	private Socket socket;
	private InputStream in;
	private int c;
	private PrintWriter out;

	public ClientThread(Socket socket, PrintWriter output) throws IOException{
		this.socket = socket;
		in = socket.getInputStream();
		out = output;
	}

	public void run(){
		try {
			while ((c = in.read())!=-1){
				System.out.println("Received: "+c);
				out.print((char)c);
				out.flush();
			}
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}

}