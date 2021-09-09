# Bowers & Wilkins: DSP Interview Challenge 2021
---
## Introduction
Welcome to the Bowers & Wilkins DSP Engineer Challenge.
The object of this exercise is to test the day-to-day skills required by Audio DSP Engineers.

```
The exercise consists of THREE parts:
    1. Part 1: Hardware setup.
    2. Part 2: Audio algorithm design & implementation (Biquad Filter).
    3. Part 3: Creative engineering, and communication skills.
```

**IMPORTANT:**
It should take a few hours to complete the exercise thoroughly.
We THANK YOU for your time and effort. But ask that you do NOT spend a lifetime on the project.

If you get stuck or cannot get part of the system working, intially try to research/debug the problem yourself.
But if things are not working as expected please email: keith.ovenden@soundunited.com who will be happy to try to point you in the right direction.
In the meantime - please skip to the next part of the task.

While the project fits together as a whole, being unable to get a single element working shouldn't mean that you are unable to continue.
We are looking at ALL aspects of your work, and there are plenty of ways to impress - even if the hardware isn't passing audio!
---
In true 'Bake-Off' Technical Challenge style...
Some instructions are quite thorough (particularly with regard to setting up the hardware). Please read them **carefully** and follow accordingly.
Other instructions are much more ambiguous. It is upto YOU to work out what is required and fill in the details.

**Note: The example 'Volume Control' CLI program is written in C, however, you are NOT restricted to writing in C.**

---
## Part 1 - System setup
### 1.1: Initial Setup
In this section you will configure the Raspberry Pi to play audio files via the HiFiBerry DSP expander board.

```
    1. Attach HifiBerry to Pi.
    2. Connect peripherals to Pi (PSU, keyboard, mouse, monitor)
    3. Boot Pi and connect to internet (WIFI network)
    4. Update the Pi - Terminal:
            'sudo apt-get update'
            'sudo apt-get upgrade'
        Note: Default update from RaspberryPi OS requires to remove python-colorzero package:
            'sudo apt autoremove'
            'sudo apt-get upgrade'
        Reboot.

    5. Ensure Python is installed:
            'sudo apt-get install python'
    6. Ensure GIT is installed:
            'sudo apt-get install git'
    7. Ensure CMAKE is installed:
            'sudo apt-get install -y git cmake'
        
    8. Update /boot/config.txt - Enable SPI & I2S, replace audio driver with HifiBerry.
        Note: Config.txt is write-protected by default.
        Quick way to update it:
            Copy /boot/config.txt file to desktop
            Edit file:
                Uncomment:      dtparam=i2s=on
                Uncomment:      dtparam=spi=on
                Comment Out:    dtparam=audio=on
                Add:            dtoverlay=hifiberry-dacplusdsp
            save.
        In Terminal:
            sudo -i
            cd /home/pi/Desktop
            scp config.txt /boot/
        Verify changes have been applied.
        Reboot.
    
    9. Verify HifiBerryDSP board is connected
            'aplay -l'
            
            **** List of PLAYBACK Hardware Devices ****
            card 0: sndrpihifiberry [snd_rpi_hifiberrydacplusdsp_sou], device 0: Hifiberry DAC+DSP SoundCard HiFi dacplusdsp-codec-0 [Hifiberry DAC+DSP SoundCard HiFi dacplusdsp-codec-0]
              Subdevices: 1/1
              Subdevice #0: subdevice #0

    10. Copy a wav file onto Pi.
    11. Use 'aplay' to play the wav file - ensure audio plays out of the HifiBerry (Phono/SPDIF).
        WARNING: PLAYBACK WILL BE LOUD!!
    
    12. Update HifiBerry Toolkit - 'sudo pip3 install --upgrade hifiberrydsp'
    
    13. Get, build and install HifiBerry TCP-IP Server (allows SigmaStudio to connect to the DSP):
            https://github.com/bang-olufsen/create
            IMPORTANT: Use You MUST use the 'BEOCreate1' branch.

            cd SigmaDaemon
            cmake .
            make
            sudo cp sigmaTcpDaemon /usr/local/bin/beocreate-tcp
            sudo chown root /usr/local/bin/beocreate-tcp
            sudo chmod u+s /usr/local/bin/beocreate-tcp
            sudo chmod go+rx /usr/local/bin/beocreate-tcp
            cd ../SigmaClientTool
            make .
            make
            sudo cp SigmaClientTool /usr/local/bin/beocreate-client
            sudo chown root /usr/local/bin/beocreate-tcp
            sudo chmod go+rx /usr/local/bin/beocreate-tcp
            
            cat <<EOF >beocreate-tcp.service
            [Unit]
            Description=Beocreate DSP TCP deamon
            After=network.target
            
            [Service]
            Type=simple
            ExecStart=/usr/local/bin/beocreate-tcp
            StandardOutput=null
            Restart=on-failure
            
            [Install]
            WantedBy=multi-user.target
            Alias=beocreate-tcp.service
            EOF
            
            sudo mv beocreate-tcp.service /etc/systemd/system
            sudo systemctl daemon-reload

    14. Start the TCP-IP Server:
            sudo systemctl start beocreate-tcp
            
    IMPORTANT: If You reboot the Pi - You MUST restart the TCP-IP process.
                
    15. Verify TCP-IP Stack Is Running:
            sudo systemctl status beocreate-tcp
```

### 1.2: SigmaStudio Setup & Connection to RaspberryPi
In this section you will configure the DSP programming utility and ensure the HifiBerry audio path is configured.
**NOTE:** If you do not have access to a Windows PC, see Appendix for workaround instructions.
```
    1. Install SigmaStudio 4.6 (On your Windows PC)
        <https://www.analog.com/en/design-center/evaluation-hardware-and-software/software/ss_sigst_02.html>
    2. Connect ethernet cable between PC and Pi.
    3. Set PC & Pi network connections to both use a static IP addresses.
    4. Ensure PC can 'ping' the Pi.
    5. Ensure the Pi can 'ping' the PC.
    6. Open "dacdsp-v11.dspproj" project in SigmaStudio.
    7. On the "Hardware Configuration" Tab - Right-click, show "TCP-IP settings".
    8. Enter the IP address of the Pi (Port = 8086), and click 'Open Connection'
       Note:    There is no obvious SUCCESS message if communications is established.
                However there IS an error message if it fails.
    9. Click 'Link, Compile, Connect' to build the project.
    10. On the Hardware tab - Right-click on the ADAU1451.
                Self-Boot-Memory -> Write Latest Compilation Through DSP.
        This should program the DSP project into the HifiBerry DSP.
    11. "Export System Files" (You'll need these later).
```
**IMPORTANT: When setting the Ethernet connection on a Windows PC, if you also have WIFI active, you will possibly need to disable the 'Automatic Metric' setting in your advanced IP4 parameters, and instead use an Interface Metric = 1.
This makes your Windows PC direct traffic to your Ethernet port rather than attempting to talk over the wifi**

### 1.3: Command-Line-Interface Setup
In this section you will configure the CLI program to run on the Pi to control the audio volume.
```
    1. Copy the CLI_Tools folder to the Pi.
    2. Open a terminal in the CLI_Tools folder.
    3. Use the command 'make' to trigger a build of the Volume.c source file.
    4. Note: The build will fail due to a number of DELIBERATE errors.
    5. Debug volume.c.
              3 x Syntax errors
            + 1 x Logic error (it should be pretty obvious to find).
    6. Rebuild the project to produce "dsp-volume" executable.
    7. Use aplay to playabck an audio file.
    8. Run the volume control tool ("./dsp-volume") and ensure volume adjustment takes effect.
```

---
## Part 2 - Audio algorithm design & implementation (Biquad Filter)
In this section you should design and implement a Biquad audio filter.

If you're feeling ambitious, you could update the DSP Schematic to add a new filter block into the signal path.
However it is STRONGLY recommended you initially target ONE of the filters contained within the 'Tone Controls' section.

**Remember: While the example 'Volume Control' CLI program is written in C, you are NOT restricted to writing in C.**

```
    1. Create a program to accept the following inputs:
            Frequency, Q and Gain
    2. Generate the relevant coefficients for a standard biquad filter.
    3. Convert the filter coefficients to a suitable format, and transmit to the DSP.
```
**Hint: Cross-reference the Volume-Control example against the generated DSP headers to understand how to transmit your coefficients to the DSP**

---
## Part 3 - 'Creative' engineering, and communication skills
Now is your chance to demonstrate your creative flair, plus your communication skills.
```
    1. Expand the project ANY way you see fit.
    2. Create a 15 min presentation detailing your work.

    Your presentation could include information about (but not limited to) some of the following topics.    
        - The hardware
        - The audio path
        - The control data path
        - Biquad filter design
        - Expansion ideas (theoretical or implemented)
        - Design considerations & potential problems
```
---
## Appendix: No Access To Windows PC
You will not be able to use SigmaStudio to update the DSP path, or be able to tweak the audio controls live via the GUI to ensure correct operation. However this should not prevent you from achieving a successful result.

You can bypass the SigmaStudio setup process and use the files provided to proceed with your filter implementation.
Please see the folder "NoWindows" in your git checkout.

```
"NoWindows" Folder Contents:
    1. jpg images of the DSP signal-path you would have seen in SigmaStudio (Top Level, and ToneControl pages)
    2. The files a SigmaStudio export would have created for you.
```

---
## Help! Everything's Gone Wrong!!
If you think everything has gone badly wrong, and you REALLY want to start again - you can re-format the uSD card with a fresh copy of Raspberry Pi OS. This performs a factory-reset on the Raspberry Pi, and gets the system back to the state it should have been delivered to you.
**WARNING: This process can take quite a long time, so should only be used if you are REALLY stuck.**
```
    1. On Windows/Mac download the Raspberry Pi Imager software (https://www.raspberrypi.org/software/)
    2. Use the imager software to format the uSD card
        - "Raspberry Pi OS With Desktop" (NOT 'Lite' or 'Inc Recommended SW').
    3. Boot the Raspberry Pi - wait for 1st time setup.
    4. Connect the Pi to the internet.
    5. Open a terminal and run: 'sudo apt-get update'
    6. Run: 'sudo apt-get update'
        - This updates the installed Linux programs/drivers.
    7. Your Pi should be 'clean' and ready for a fresh attempt at the challenge.
```

---
## References / Useful Links
https://github.com/bwgroupltd/BW_DSP_Challenge
https://www.analog.com/en/design-center/evaluation-hardware-and-software/software/ss_sigst_02.html
https://www.hifiberry.com/docs/data-sheets/datasheet-dac-dsp/
https://www.hifiberry.com/beocreate/beocreate-doc/beocreate-installing-the-sigmatcpserver/
https://support.hifiberry.com/hc/en-us/community/posts/115004375269-SigmaStudio-connection-how-to-establish-or-what-is-wrong
https://github.com/hifiberry/hifiberry-dsp/blob/master/doc/adaptexistingprofiles.md