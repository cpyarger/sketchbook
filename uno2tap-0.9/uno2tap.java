
/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

import gnu.io.CommPortIdentifier; 
import gnu.io.SerialPort;
import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.Enumeration;
import java.util.Scanner;


public class uno2tap {

	String devname=null;
	private SerialPort serial_port;
	private InputStream serial_input;
	private OutputStream serial_output;
	boolean connected=false;

	private static final int TIME_OUT=2000;
	private static final int DATA_RATE=115200;

	File tapfile;
	String tapname=null;
	byte[] tap_buf;
	static final int buf_size=48;
	int tap_size;
	int tap_pos;
	boolean tap_loaded=false;

	boolean playing=false;
	boolean recording=false;
	boolean cassette_motor_on=false;

	static final int CMD_NOP='Z';
	static final int CMD_PING='P';
	static final int CMD_PLAY='R';
	static final int CMD_RECORD='W';
	static final int CMD_STOP='r';
	static final int CMD_SENSE_ON='S';
	static final int CMD_SENSE_OFF='s';

	public void open_serial() {
		// default serial ports to try
		String PORT_NAMES[] = { 
				"/dev/ttyACM0",
				"/dev/ttyUSB0", // Linux
				"/dev/tty.usbserial-A9007UX1", // Mac OS X
				"COM3", // Windows
		};

		CommPortIdentifier portId=null;
		Enumeration portEnum=CommPortIdentifier.getPortIdentifiers();

		while(portEnum.hasMoreElements()) {
			CommPortIdentifier currPortId=(CommPortIdentifier)portEnum.nextElement();
			if(devname!=null) {
				if(currPortId.getName().equals(devname)) {
					portId=currPortId;
					break;
				}
			} else {
				for(String portName : PORT_NAMES) {
					if(currPortId.getName().equals(portName)) {
						portId=currPortId;
						break;
					}
				}
			}
		}

		if (portId==null) {
			System.out.println("Could not find serial port.");
			return;
		}

		try {
			// open serial port, and use class name for the appName.
			serial_port=(SerialPort)portId.open(this.getClass().getName(), TIME_OUT);

			// set port parameters
			serial_port.setSerialPortParams(DATA_RATE,
					SerialPort.DATABITS_8,
					SerialPort.STOPBITS_1,
						SerialPort.PARITY_NONE);

			// open the streams
			serial_input=serial_port.getInputStream();
			serial_output=serial_port.getOutputStream();

			connected=true;
			System.out.println("Connected to serial device");
			send_tape_status();
		} catch (Exception e) {
			System.err.println(e.toString());
		}
	}

	public synchronized void close_serial() {
		if (serial_port!=null) serial_port.close();
	}

	private void print_status() {
		System.out.println("");

		System.out.println("Using device "+devname);
		if(connected) System.out.println("Connected");
		else System.out.println("Disconnected");

		if(cassette_motor_on) System.out.println("Cassette motor on");
		else System.out.println("Cassette motor off");

		if(playing) System.out.println("Playing");
		else System.out.println("Not playing");

		if(recording) System.out.println("Recording");
		else System.out.println("Not recording");

		double time=pos2time(tap_pos);
		System.out.println("Tap file position: "+tap_pos);
		System.out.println("Tape time: "+time+" seconds");

		System.out.println("");
	}

	byte[] serial_buf=new byte[1];

	// return playing time elapsed for given tap position (including header)
	private double pos2time(int pos) {
		double time=0.0;
		int xpos;
		for(xpos=0x14; xpos<pos; xpos++) {
			if(xpos>=tap_size) break;
			int tap_data=(int)tap_buf[xpos];
			if(tap_data!=0) {
				time+=(tap_data*8)/985248.0;
			} else {
				int d1=tap_buf[xpos+1];
				int d2=tap_buf[xpos+2];
				int d3=tap_buf[xpos+3];
				time+=((d3*256*256)+(d2*256)+d1)/985248.0;
				xpos+=3;
			}
		}
		return time;
	}

	// return tap position (including header) for given time
	private int time2pos(double time) {
		double xtime=0.0;
		int xpos;
		for(xpos=0x14; xpos<tap_size; xpos++) {
			if(xtime>=time) break;
			int tap_data=(int)tap_buf[xpos];
			if(tap_data!=0) {
				xtime+=(tap_data*8)/985248.0;
			} else {
				int d1=tap_buf[xpos+1];
				int d2=tap_buf[xpos+2];
				int d3=tap_buf[xpos+3];
				xtime+=((d3*256*256)+(d2*256)+d1)/985248.0;
				xpos+=3;
			}
		}
		return xpos;
	}

	private void process_input() throws Exception {
		while(serial_input.available()>0) {
			if(recording) {
				tap_data_receive();
				return;
			}

			serial_input.read(serial_buf, 0, 1);

			switch(serial_buf[0]) {
				// request for tap data
				case 'N':
					tap_data_send();
					break;
				// motor control on
				case 'M':
					System.out.println("Cassette motor on");
					cassette_motor_on=true;
					break;
				// motor control off
				case 'm':
					System.out.println("Cassette motor off");
					cassette_motor_on=false;
					break;
				// ping reply
				case 'P':
					System.out.println("Ping response received");
					break;
				// error
				case 'E':
					System.out.println("Error");
					break;
				default:
					System.out.println(serial_buf[0]);
					break;
			}
		}
	}

	private void tap_data_send() {
		if(tap_pos>=tap_size) {
			System.out.println("Tape end reached");
			try {
				Thread.sleep(100);
			} catch (Exception e) {
				e.printStackTrace();
			}
			press_stop();
			press_rewind();
			return;
		}

		int tap_remaining=tap_size-tap_pos;
		int write_size;
		if(tap_remaining>=buf_size-1) write_size=buf_size-1;
		else write_size=tap_remaining;
		try {
			serial_output.write(CMD_NOP);
			serial_output.write(tap_buf, tap_pos, write_size);
			tap_pos+=write_size;
			while(write_size<buf_size-1) {
				serial_output.write(0x20);
				write_size++;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void send_command(int cmd) {
		try {
			serial_output.write(cmd);
			if(playing) {
				for(int i=0; i<buf_size-1; i++) serial_output.write(0);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void tap_data_receive() {
		byte[] serial_buf=new byte[1];
		byte tap_data;
		if(tap_pos>=tap_size) {
			System.out.println("Tape end reached");
			press_stop();
			return;
		}
		try {
			while(serial_input.available()==0);
			serial_input.read(serial_buf, 0, 1);
			tap_data=serial_buf[0];
			tap_buf[tap_pos++]=tap_data;
			if(tap_data==0) {
				for(int i=0; i<3; i++) {
					while(serial_input.available()==0);
					serial_input.read(serial_buf, 0, 1);
					tap_data=serial_buf[0];
					if(tap_pos+2>=tap_size) {
						System.out.println("Tape end reached");
						press_stop();
						return;
					}
					tap_buf[tap_pos++]=tap_data;
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void stop_recording() {
		if(!recording) return;
		send_command(CMD_STOP);
		send_command(CMD_SENSE_OFF);
		recording=false;

		tap_buf[0x10]=(byte)(tap_size&0xff);
		tap_buf[0x11]=(byte)((tap_size&0xff00)>>8);
		tap_buf[0x12]=(byte)((tap_size&0xff0000)>>16);
		tap_buf[0x13]=(byte)((tap_size&0xff000000)>>24);

		try {
			FileOutputStream fs=new FileOutputStream(tapfile);
			fs.write(tap_buf, 0, tap_size);
			fs.close();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void stop_playing() {
		if(!playing) return;
		send_command(CMD_STOP);
		send_command(CMD_SENSE_OFF);
		playing=false;
	}

	private void press_stop() {
		stop_playing();
		stop_recording();
		System.out.println("Tape stopped");
	}

	private void set_position(int pos) {
		tap_pos=pos;
	}

	private void press_rewind() {
		tap_pos=0x14;
		System.out.println("Tape rewound");
	}

	private void press_eject() {
		press_stop();
		tap_loaded=false;
		System.out.println("Tape ejected");
	}

	private void press_play() {
		if(playing) return;
		stop_recording();
		if(!tap_loaded) {
			System.out.println("No tap loaded");
			return;
		}
		send_command(CMD_SENSE_ON);
		send_command(CMD_PLAY);
		playing=true;
		System.out.println("Tape playing");
	}

	private void press_record() {
		stop_playing();
		send_command(CMD_RECORD);
		send_command(CMD_SENSE_ON);
		recording=true;
	}

	private void send_tape_status() {
			if(playing) send_command(CMD_SENSE_ON);
			else send_command(CMD_SENSE_OFF);
	}

	int last_status;

	private void run() {
		Scanner keyboard=new Scanner(System.in);
		InputStreamReader consoleInputStream=new InputStreamReader(System.in);
		BufferedReader consoleBufferedReader=new BufferedReader(consoleInputStream);

		boolean running=true;
		while(running) {
			if(connected) {
				try {
					process_input();
				} catch (Exception e) {
					e.printStackTrace();
				}
			} else {
				open_serial();
			}

			try {
				if(consoleBufferedReader.ready()) {
					String cmd=keyboard.nextLine();

					if(cmd.equals("help")) {
						print_console_help();
					} else if(cmd.equals("eject")) {
						press_eject();
					} else if(cmd.equals("ping")) {
						send_command(CMD_PING);
					} else if(cmd.equals("play")) {
						press_play();
					} else if(cmd.equals("quit")) {
						press_stop();
						running=false;
					} else if(cmd.equals("record")) {
						press_record();
					} else if(cmd.equals("rewind")) {
						press_rewind();
					} else if(cmd.equals("status")) {
						print_status();
					} else if(cmd.equals("stop")) {
						press_stop();
					} else if(cmd.startsWith("pos ")) {
						String stime=cmd.substring(4);
						double time=Double.valueOf(stime);
						set_position(time2pos(time));
					} else {
						System.out.println("Unknown command: "+cmd);
						System.out.println("Type 'help' for help");
					}
				}
			} catch (Exception e) {
				e.printStackTrace();
			}

			if(!playing && !recording) {
				if(connected) {
					last_status++;
					if(last_status>5) {
						send_tape_status();
						last_status=0;
					}
				}
				try {
					Thread.sleep(100);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
	}

	private void print_console_help() {
		System.out.println("");
		System.out.println("Commands:");
		System.out.println("");
		System.out.println("play\t\tPress play on tape");
		System.out.println("record\t\tPress record and play on tape");
		System.out.println("rewind\t\tRewind tape to start");
		System.out.println("stop\t\tStop the tape");
		System.out.println("pos <seconds>\tMove cassette to time at <seconds>");
		System.out.println("");
		System.out.println("status\t\tShow tape status");
		System.out.println("");
		System.out.println("quit\t\tQuit");
		System.out.println("");
	}

	private void create_tap() {
		System.out.println("Creating tap file "+tapname);
		try {
			tapfile.createNewFile();
			tap_size=1024*1024;
			tap_buf=new byte[tap_size];
			String header="C64-TAPE-RAW";
			System.arraycopy(header.getBytes(), 0, tap_buf, 0, header.length());
			tap_buf[0xc]=0x01;
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void read_tap() {
		System.out.println("Opening tap file "+tapname);
		tap_size=(int)tapfile.length();
		tap_buf=new byte[(int)tap_size];
		try {
			InputStream input=new BufferedInputStream(new FileInputStream(tapfile));
			int totalBytesRead=0;
			while(totalBytesRead<tap_size){
          			int bytesRemaining=tap_size-totalBytesRead;
				int bytesRead=input.read(tap_buf, totalBytesRead, bytesRemaining);
				if(bytesRead>0) {
					totalBytesRead=totalBytesRead+bytesRead;
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void open_tap() {
		tapfile=new File(tapname);

		if(tapfile.exists()) {
			read_tap();
		} else {
			create_tap();
		}
		tap_pos=0x14;
		tap_loaded=true;
	}

	private static void print_help() {
		System.out.println("uno2tap [-dev <serial device name>] [-tap <tap image file name>]");
	}

	private void read_args(String[] args) {
		String last_arg="";
		for(String arg : args) {
			if(arg.equals("-h")
				|| arg.contains("-help")) {
				print_help();
				System.exit(0);
			}
			if(last_arg.equals("-dev")) devname=arg;
			if(last_arg.equals("-tap")) {
				tapname=arg;
				open_tap();
			}
			last_arg=arg;
		}
	}

	public static void main(String[] args) throws Exception {
		uno2tap main = new uno2tap();
		main.read_args(args);
		main.print_console_help();
		main.run();
		main.close_serial();
	}
}

