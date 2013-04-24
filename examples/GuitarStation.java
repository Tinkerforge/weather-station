import com.tinkerforge.BrickletLCD20x4;
import com.tinkerforge.IPConnection;
import javax.sound.midi.*;
import java.util.*;

class GameHandle extends Thread{
	private static final String host = "localhost";
	private static final int port = 4223;

	private BrickletLCD20x4 lcd = null;
	private IPConnection ipcon = null;
	private Synthesizer synthesizer = null;
	private MidiChannel[] channels = null;


	private static final int ACOUSTIC_GUITAR_NYLON = 24;
	private static final int ACOUSTIC_GUITAR_STEEL = 25;
	private static final int ELECTRIC_GUITAR_JAZZ = 26;
	private static final int ELECTRIC_GUITAR_CLEAN = 27;
	private static final int ELECTRIC_GUITAR_MUTED = 28;
	private static final int OVERDRIVEN_GUITAR = 29;
	private static final int DISTORTION_GUITAR = 30;
	private static final int GUITAR_HARMONICS = 31;
	private static final int CHOIR = 52;
	private static final int TRUMPET = 56;
	private static final int OBOE = 68;
	private static final int PAN_FLUTE = 75;
	private static final int BLOWN_BOTTLE = 76;
	private static final int SYNTH_PAD = 88;
	private static final int SYNTH_POLY = 90;

	// Here you can select other instruments
	private static final int INSTRUMENT = SYNTH_PAD;

	private static final int MAX_BAR_LENGTH = 15;
	private static final double BAR_PROPABILITY = 0.95;
	private static final int SPEED = 300;

	// Maps a normal UTF-16 encoded string to the LCD charset
	static String createLCDString(String utf16)
	{
		String ks0066u = "";
		char c;

		for (int i = 0; i < utf16.length(); i++) {
			int codePoint = utf16.codePointAt(i);

			switch (codePoint) {
				case ' ': c = (char)' '; break;
				default:
					c = (char)0xff; break; // BLACK SQUARE
			}
			ks0066u += c;
		}

		return ks0066u;
	}


	public GameHandle () {
		this.ipcon = new IPConnection();

		// Connect to Brickd
		System.out.println("Waiting for Brickd...");
		while(true) {
			try {
				ipcon.connect(host, port);
				break;
			}
			catch(Exception e) {}
		}

		ipcon.addEnumerateListener(new IPConnection.EnumerateListener() {
            public void enumerate(String uid, String connectedUid, char position,
                                  short[] hardwareVersion, short[] firmwareVersion,
                                  int deviceIdentifier, short enumerationType) {

                if(enumerationType == IPConnection.ENUMERATION_TYPE_CONNECTED || 
                  enumerationType == IPConnection.ENUMERATION_TYPE_AVAILABLE) {

					if(deviceIdentifier == BrickletLCD20x4.DEVICE_IDENTIFIER) {
						System.out.println("Found LCD");
						try {
							lcd = new BrickletLCD20x4(uid, ipcon);
							lcd.clearDisplay();
							lcd.backlightOn();
        					synthesizer = MidiSystem.getSynthesizer();
							synthesizer.open();
							Instrument[] instr = synthesizer.getDefaultSoundbank().getInstruments();
							synthesizer.loadInstrument(instr[INSTRUMENT]);
							channels = synthesizer.getChannels();
							channels[0].programChange(INSTRUMENT);

							lcd.writeLine((short)0, (short)0, createLCDString("  XXX   "));
							lcd.addButtonPressedListener(new BrickletLCD20x4.ButtonPressedListener() {
            					public void buttonPressed(short button) {
					                System.out.println("Pressed: " + button);
									channels[0].noteOn(40+10*button, 100);
            					}
        					});
							lcd.addButtonReleasedListener(new BrickletLCD20x4.ButtonReleasedListener() {
            					public void buttonReleased(short button) {
                					System.out.println("Released: " + button);
									channels[0].noteOff(40+10*button);
            					}
        					});

						} 
						catch(Exception e)
						{
							System.out.println("LCD Initialization Failed");
						}
					}
                }

            }
        });

		// Try to enumerate
		System.out.println("Enummerate...");
		while(true) {
			try {
				ipcon.enumerate();
				break;
			}
			catch(Exception e) {}
		}


	}

	public void run() {
        char mask[][] = new char[4][20];
		int barlist[] = {0,0,0,0};

		System.out.println("Start thread");

        // initial drawing empty mask
        for(int i=0; i < 4; i++) {
			try {
	            lcd.writeLine((short)i, (short)0, createLCDString(new String(mask[i])));
			}
			catch (Exception e) {}
		}

        while(true) {
			try {
	            Thread.sleep(SPEED);
			} catch(Exception e) {}

            // move mask
            for(int i=0; i < 4; i++) {
            	for(int j=0; j < 19; j++) {
                    mask[i][19-j] = mask[i][19-j-1];
				}
			}

            // decrement counters
            for(int i=0; i < 4; i++) {
                if(barlist[i] > 0) {
                    barlist[i] = barlist[i] - 1;
				}
			}

            // create new bars
            for(int i=0; i < 4; i++) {
                if(barlist[i] == 0) {
					Random rand = new Random();

                    if(rand.nextDouble() >= BAR_PROPABILITY) {
                        barlist[i] = (int)(MAX_BAR_LENGTH*rand.nextDouble());
					}
				}
			}

            // create first new line based on bars
            for(int i=0; i < 4; i++) {
                if(barlist[i] > 0) {
                    mask[i][0] = 'X';
				}
                else {
                    mask[i][0] = ' ';
				}
			}

            // print mask
            for(int i=0; i < 4; i++) {
				try {
	            	lcd.writeLine((short)i, (short)0, createLCDString(new String(mask[i])));
				}
				catch (Exception e) {}
			}
		}
	}


}


public class GuitarStation {
	// Note: To make the example code cleaner we do not handle exceptions. Exceptions you
	//       might normally want to catch are described in the documentation
	public static void main(String args[]) throws Exception {
		(new GameHandle()).start();
		System.console().readLine("Press key to exit\n");
		// Try to disconnect
		System.out.println("Disconnect...");
	}
}
